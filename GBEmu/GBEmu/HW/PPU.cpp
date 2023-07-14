#include "stdafx.h"
#include "PPU.h"

#include "HWAsset.h"
#include "HWEnv.h"
#include "MemoryAddress.h"
#include "PPURender.h"

namespace GBEmu::HW
{
	using namespace MemoryAddress;

	constexpr int dotCycleFreq_70224 = HWParam::PPUCyclePeriod_70224;
	constexpr int scanLineFreq_456 = 456;
	constexpr int scanLineSize_144 = 144;
	constexpr int displayWidth_160 = HWParam::DisplayResolution.x;
	constexpr int displayHeight_144 = HWParam::DisplayResolution.y;

	constexpr int oamCapacity_10 = 10;
	constexpr int oamSearchDuration_80 = 80;
	constexpr int pixelTransferDuration_289 = 289;

	PPU::PPU()
	{
		m_dotCycle = dotCycleFreq_70224 - 52 + 4;
		// m_oamBuffer.reserve(oamCapacity_10);
	}

	PPUResult PPU::StepCycle(HWEnv& env)
	{
		m_dotCycle = (m_dotCycle + 1) % dotCycleFreq_70224;

		updateLY(m_lcd, m_dotCycle);

		const auto modeBefore = m_mode;
		m_mode = m_nextMode;
		m_nextMode = judgePPUMode((m_dotCycle + 1) % dotCycleFreq_70224);

		const bool isModeChanged = modeBefore != m_mode;
		if (isModeChanged) m_lcd.SetMode(m_mode);

		// モード分岐
		if (m_mode == PPUMode::PixelTransfer && m_nextMode == PPUMode::HBlank)
		{
			// WindowEnableFlagを記憶
			updateBGAndWindowFlagBuffer(m_lcd);

			// アドレス差分を記憶
			const auto ly = m_lcd.LY();
			m_bgAndWindowTileDataDiff.TrackAddressLY(m_lcd.BGAndWindowTileDataAddress(), ly);
			m_bgTileMapDisplayDiff.TrackAddressLY(m_lcd.BGTileMapDisplayAddress(), ly);
			m_windowTileMapDisplayDiff.TrackAddressLY(m_lcd.WindowTileMapDisplayAddress(), ly);
		}
		else if (m_mode == PPUMode::HBlank && m_nextMode == PPUMode::VBlank)
		{
			// 画面描画
			renderAtVBlank(env, env.GetMemory(), m_lcd);

			// いろいろクリア
			m_bgAndWindowTileDataDiff.Clear();
			m_bgTileMapDisplayDiff.Clear();
			m_windowTileMapDisplayDiff.Clear();
		}

		// 割り込みチェック
		checkInterrupt(env, m_lcd, isModeChanged);

		return PPUResult{
			isModeChanged && m_mode == PPUMode::VBlank
		};
	}

	void PPU::updateBGAndWindowFlagBuffer(LCD& lcd)
	{
		const int ly = lcd.LY(); // 0~143
		if (ly < 0 || 144 <= ly) return;

		auto&& buffer = m_lcdcFlagBuffer[ly / 32];

		// 0x00
		if (lcd.IsWindowDisplayEnable() && ly >= lcd.WY())
		{
			buffer.WindowDisplayEnable |= 1 << (ly % 32);
		}
		else
		{
			buffer.WindowDisplayEnable &= ~(1 << (ly % 32));
		}

		// 0x20
		if (lcd.IsBGAndWindowEnable())
		{
			buffer.BGAndWindowEnable |= 1 << (ly % 32);
		}
		else
		{
			buffer.BGAndWindowEnable &= ~(1 << (ly % 32));
		}

		// 0x40
		if (lcd.IsOBJDisplayEnable())
		{
			buffer.OBJDisplayEnable |= 1 << (ly % 32);
		}
		else
		{
			buffer.OBJDisplayEnable &= ~(1 << (ly % 32));
		}
	}

	struct DisplaySpecialCb
	{
		float PixelScale{};
	};

	Size PPU::DrawAt(const Point& pos, double scale) const
	{
		const ScopedRenderStates2D sampler{ SamplerState::ClampNearest };
		const ScopedCustomShader2D shader{ HWAsset::Instance().PsDisplaySpecial };

		ConstantBuffer<DisplaySpecialCb> cb{};
		cb->PixelScale = static_cast<float>(scale);
		Graphics2D::SetPSConstantBuffer(1, cb);

		(void)m_renderBuffer.scaled(scale).drawAt(pos);

		return (m_renderBuffer.size() * scale).asPoint();
	}

	void PPU::checkInterrupt(HWEnv& env, LCD& lcd, bool isModeChanged)
	{
		auto&& memory = env.GetMemory();
		auto&& interrupt = memory.Interrupt();

		// VBlank割り込みチェック
		if (isModeChanged && m_mode == PPUMode::VBlank) { interrupt.SetFlag(InterruptFlags::VBlank); }

		// STAT割り込みチェック
		const bool canSTATInterrupt =
			(lcd.IsLYCoincidenceInterruptEnable() && lcd.LYCoincidenceFlag()) ||
			(isModeChanged && m_mode == PPUMode::OAMSearch && lcd.IsOAMInterruptEnable()) ||
			(isModeChanged && m_mode == PPUMode::HBlank && lcd.IsHBlankInterruptEnable()) ||
			(isModeChanged && m_mode == PPUMode::VBlank && lcd.IsVBlankInterruptEnable());

		if (canSTATInterrupt && m_canSTATInterruptBefore == false) { interrupt.SetFlag(InterruptFlags::STAT); }

		m_canSTATInterruptBefore = canSTATInterrupt;
	}

	void PPU::renderAtVBlank(HWEnv& env, Memory& memory, const LCD& lcd) const
	{
		memory.GetVRAM().CheckRefreshAtlas();

		const ScopedRenderTarget2D target{ m_renderBuffer };

		auto&& vram = memory.GetVRAM();

		(void)m_renderBuffer.clear(Palette::Black);

		if (lcd.IsLCDDisplayEnable() == false) return;

		const auto renderBGAndWindowArgs = PPURenderBGAndWindowArgs{
			env, memory, lcd, vram,
			m_lcdcFlagBuffer,
			m_bgAndWindowTileDataDiff,
			m_bgTileMapDisplayDiff,
			m_windowTileMapDisplayDiff
		};

		// BGとウィンドウ描画
		PPURender::RenderBGAndWindow(renderBGAndWindowArgs);

		// OBJマスク作成
		PPURender::RenderObjMaskFromBGAndWindow(renderBGAndWindowArgs, m_objMaskBuffer);

		// OBJ描画
		PPURender::RenderOBJCompletely({env, memory, lcd, vram, m_lcdcFlagBuffer, m_objMaskBuffer});
	}

	void PPU::updateLY(LCD& lcd, int dotCycle)
	{
		if (dotCycle % scanLineFreq_456 != 0) return;

		const uint8 ly = dotCycle / scanLineFreq_456;

		lcd.SetLY(ly);
	}

	PPUMode PPU::judgePPUMode(int dotCycle)
	{
		const int scan = dotCycle % scanLineFreq_456;
		const PPUMode mode =
			(dotCycle / scanLineFreq_456) >= scanLineSize_144 ? PPUMode::VBlank :
			(scan < oamSearchDuration_80) ? PPUMode::OAMSearch :
			(scan < oamSearchDuration_80 + pixelTransferDuration_289) ? PPUMode::PixelTransfer : // less than 369
			PPUMode::HBlank; // duration: scanLineFreq - pixelTransferDuration = 456 - 369
		return mode;
	}
}
