﻿#include "stdafx.h"
#include "PPU.h"

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
		auto&& lcd = env.GetMemory().GetLCD();

		m_dotCycle = (m_dotCycle + 1) % dotCycleFreq_70224;

		updateLY(lcd, m_dotCycle);

		const auto modeBefore = m_mode;
		m_mode = m_nextMode;
		m_nextMode = judgePPUMode((m_dotCycle + 1) % dotCycleFreq_70224);

		const bool isModeChanged = modeBefore != m_mode;
		if (isModeChanged) lcd.SetMode(m_mode);

		// モード分岐
		if (m_mode == PPUMode::PixelTransfer && m_nextMode == PPUMode::HBlank)
		{
			// WindowEnableFlagを記憶
			updateBGAndWindowFlagBuffer(lcd);

			// アドレス差分を記憶
			const auto ly = lcd.LY();
			m_bgAndWindowTileDataDiff.TrackAddressLY(lcd.BGAndWindowTileDataAddress(), ly);
			m_bgTileMapDisplayDiff.TrackAddressLY(lcd.BGTileMapDisplayAddress(), ly);
			m_windowTileMapDisplayDiff.TrackAddressLY(lcd.WindowTileMapDisplayAddress(), ly);
		}
		else if (m_mode == PPUMode::HBlank && m_nextMode == PPUMode::VBlank)
		{
			// 画面描画
			renderAtVBlank(env.GetMemory(), lcd);

			// いろいろクリア
			m_bgAndWindowTileDataDiff.Clear();
			m_bgTileMapDisplayDiff.Clear();
			m_windowTileMapDisplayDiff.Clear();
		}

		// 割り込みチェック
		checkInterrupt(env, lcd, isModeChanged);

		return PPUResult{
			isModeChanged && m_mode == PPUMode::VBlank
		};
	}

	void PPU::updateBGAndWindowFlagBuffer(LCD& lcd)
	{
		const int ly = lcd.LY(); // 0~143
		if (ly < 0 || 144 <= ly) return;

		auto&& buffer = m_bgAndWindowFlagBuffer[ly / 32];

		// 0x00
		if (lcd.IsWindowDisplayEnable() && ly >= lcd.WY())
		{
			buffer.WindowPriority |= 1 << (ly % 32);
		}
		else
		{
			buffer.WindowPriority &= ~(1 << (ly % 32));
		}

		// 0x20
		if (lcd.IsBGAndWindowEnable())
		{
			buffer.Enable |= 1 << (ly % 32);
		}
		else
		{
			buffer.Enable &= ~(1 << (ly % 32));
		}
	}

	Size PPU::DrawAt(const Point& pos, double scale) const
	{
		const ScopedRenderStates2D sampler{ SamplerState::ClampNearest };

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
			(isModeChanged && lcd.IsOAMInterruptEnable()) ||
			(isModeChanged && lcd.IsHBlankInterruptEnable()) ||
			(isModeChanged && lcd.IsVBlankInterruptEnable());

		if (canSTATInterrupt && m_canSTATInterruptBefore == false) { interrupt.SetFlag(InterruptFlags::STAT); }

		m_canSTATInterruptBefore = canSTATInterrupt;
	}

	void PPU::renderAtVBlank(Memory& memory, const LCD& lcd) const
	{
		// TODO: バッファにLCD経歴を格納して、IsWindowDisplayEnableが付いてる行だけ描画したい

		memory.GetVRAM().CheckRefreshAtlas();

		const ScopedRenderTarget2D target{ m_renderBuffer };

		auto&& vram = memory.GetVRAM();

		(void)m_renderBuffer.clear(Palette::Black);

		const auto renderBGAndWindowArgs = PPURenderBGAndWindowArgs{
			memory, lcd, vram,
			m_bgAndWindowFlagBuffer,
			m_bgAndWindowTileDataDiff,
			m_bgTileMapDisplayDiff,
			m_windowTileMapDisplayDiff
		};

		// BGとウィンドウ描画
		PPURender::RenderBGAndWindow(renderBGAndWindowArgs);

		// OBJマスク作成
		PPURender::RenderObjMaskFromBGAndWindow(renderBGAndWindowArgs, m_objMaskBuffer);

		// OBJ描画
		PPURender::RenderOBJCompletely({memory, lcd, vram, m_objMaskBuffer});
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
