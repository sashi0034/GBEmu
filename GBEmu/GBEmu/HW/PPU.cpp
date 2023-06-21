#include "stdafx.h"
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

	// Array<OAMData> PPU::scanOAM(HWEnv& env, LCD& lcd)
	// {
	// 	auto&& memory = env.GetMemory();
	//
	// 	const uint8 ly = lcd.LY();
	// 	const int objHeight = lcd.OBJHeight(); // 16 or 8
	//
	// 	Array<OAMData> result{};
	// 	for (uint16 addr = OAMStart_0xFE00; addr <= OAMEnd_0xFE9F; addr += 4)
	// 	{
	// 		// https://github.com/pokemium/gb-docs-ja/tree/main/video/oam
	// 		// DMGモードだとX座標が小さいほど優先度が高くなり、CGBモードだとアドレスの小さい方が優先されるらしいが
	// 		// ここではあえてCGBモードで進めてみる
	//
	// 		OAMData oam{};
	//
	// 		oam.Y = memory.Read(addr + 0);
	// 		const bool isVisibleY = oam.ActualY() <=ly && ly < oam.ActualY() + objHeight;
	// 		if (isVisibleY == false) continue;
	//
	// 		oam.X = memory.Read(addr + 1);
	// 		const bool isVisibleX = oam.ActualX() > -8;
	// 		if (isVisibleX == false) continue;
	//
	// 		oam.TileIndex = memory.Read(addr + 2);
	// 		oam.Flags = memory.Read(addr + 3);
	//
	// 		if (objHeight == 16)
	// 		{
	// 			// スプライトのサイズが8x16なら、8x8のタイルの連続タイルごとにスプライトが形成される
	// 			const bool isTop =
	// 				(oam.FlagYFlip() == false && ly < oam.ActualY() + 8) ||
	// 				(oam.FlagYFlip() && (ly >= oam.ActualY() + 8));
	// 			oam.TileIndex = isTop
	// 				? oam.TileIndex & 0xFE
	// 				: oam.TileIndex | 0x01;
	// 		}
	//
	// 		result.push_back(oam);
	//
	// 		// 10個を超えたら他のOBJは無視
	// 		if (result.size() >= oamCapacity_10) break;
	// 	}
	// 	return result;
	// }

	// inline uint16 getTileDataAddress(uint16 baseAddr, uint8 tileId, uint8 row, bool isFlipY = false)
	// {
	// 	// 1枚のタイルは16バイト (0x10)
	// 	const int tileOffset = baseAddr == TileDataTableStart_0x8000
	// 		// 0x8000アドレッシングでは、0x8000をベースポインタとし、符号なしのアドレッシング
	// 		? tileId * 0x10
	// 		// 0x8800アドレッシングでは、0x9000をベースポインタとして使用し、符号付きアドレッシング ()
	// 		: static_cast<int8>(tileId) * 0x10;
	// 	const int rowOffset = isFlipY ? 2 * (7 - row) : 2 * row;
	// 	return baseAddr + tileOffset + rowOffset;
	// }
	//
	// inline uint8 getTileDataColor(uint16 tileData, int pixelIndex, bool isFlipX = false)
	// {
	// 	const auto bitShift = isFlipX ? pixelIndex : 7 - pixelIndex;
	// 	return ((tileData >> bitShift) & 0b1) | (((tileData >> (bitShift + 8)) & 0b1) << 1);
	// }

	// void PPU::scanLineX(HWEnv& env, LCD& lcd, int fetcherX, const Array<OAMData>& oamBuffer, Image& bitmap)
	// {
	// 	auto&& memory = env.GetMemory();
	//
	// 	const uint8 ly = lcd.LY();
	// 	const uint8 scy = lcd.SCY();
	// 	const uint8 scx = lcd.SCX();
	// 	const uint8 wy = lcd.WY();
	// 	const uint8 wx = lcd.WX();
	// 	const bool isWindowDisplayEnable = lcd.IsWindowDisplayEnable();
	//
	// 	// WX=7, WY=0のときBGを完全に覆うようになる (ハードウェアのバグ(?)により7未満は未定義動作)
	// 	const int windowFetcherX = fetcherX - (wx - 7);
	// 	const int windowFetcherY = ly - wy;
	// 	const bool isWindowFetch = isWindowDisplayEnable && windowFetcherX >= 0 && windowFetcherY >= 0;
	//
	// 	const auto [tileIdAddrBase, tileIdAddrOffsetX, tileIdAddrOffsetY] =
	// 		isWindowFetch
	// 		// ウィンドウフェッチ中
	// 		? std::tuple<uint16, uint16, uint16>{
	// 			lcd.WindowTileMapDisplayAddress(),
	// 			windowFetcherX / 8,
	// 			32 * (windowFetcherY / 8)}
	// 		// 通常はBG
	// 		: std::tuple<uint16, uint16, uint16>{
	// 			lcd.BGTileMapDisplayAddress(),
	// 			((fetcherX / 8) + (scx / 8)) & 0x1f,
	// 			32 * (((ly + scy) & 0xff) / 8)};
	// 	// タイルID
	// 	const uint16 tileIdAddr = tileIdAddrBase + ((tileIdAddrOffsetX + tileIdAddrOffsetY) & 0x03FF);
	//
	// 	// タイルデータ
	// 	const uint16 tileDataAddr = getTileDataAddress(
	// 		lcd.BGAndWindowTileDataAddress(),
	// 		memory.Read(tileIdAddr),
	// 		isWindowFetch ? (windowFetcherY % 8) : (ly + scy) % 8);
	// 	const uint8 tileDataColor = getTileDataColor(memory.Read16(tileDataAddr), isWindowFetch ? windowFetcherX % 8 : fetcherX % 8);
	//
	// 	// BGまたはウィンドウの色をフェッチできたので、OAMもマージしてディスプレイ上の色をフェッチ
	// 	const Color displayColor = fetchPixelByMergeOAM(memory, lcd, fetcherX, oamBuffer, ly, tileDataColor);
	//
	// 	// 色情報転送
	// 	bitmap[ly][fetcherX] = displayColor;
	// }

	// Color PPU::fetchPixelByMergeOAM(
	// 	Memory& memory, LCD& lcd,
	// 	int fetcherX, const Array<OAMData>& oamBuffer, uint8 ly, uint8 bgWindowTileDataColor)
	// {
	// 	Color displayColor = displayColorPalette[lcd.BGPaletteData(bgWindowTileDataColor)];
	// 	if (lcd.IsOBJDisplayEnable() == false) return displayColor;
	//
	// 	// OAM中のスプライトをフェッチ
	// 	for (auto&& oam : oamBuffer)
	// 	{
	// 		// 描画範囲X
	// 		const bool isBetweenX = oam.ActualX() <= fetcherX && fetcherX < oam.ActualX() + 8;
	// 		if (isBetweenX == false) continue;
	//
	// 		// スプライト左から何ドット目か
	// 		const int oamX = fetcherX - oam.ActualX();
	//
	// 		// スプライトのタイルデータアドレス
	// 		const uint16 oamTileDataAddr =
	// 			getTileDataAddress(TileDataTableStart_0x8000, oam.TileIndex, (ly - oam.ActualY()) % 8, oam.FlagYFlip());
	// 		const uint8 oamTileDataColor = getTileDataColor(memory.Read16(oamTileDataAddr), oamX % 8, oam.FlagXFlip());
	//
	// 		// TODO: これだとまだ条件不十分?
	// 		if (oamTileDataColor != 0 && (oam.FlagPriority() == false || bgWindowTileDataColor == 0 || lcd.IsBGAndWindowEnable() == false))
	// 		{
	// 			// マージ
	// 			displayColor = displayColorPalette[lcd.ObjectPaletteData(oam.Palette(), oamTileDataColor)];
	// 		}
	// 	}
	// 	return displayColor;
	// }
}
