﻿#include "stdafx.h"
#include "PPURender.h"

#include "HWAsset.h"
#include "Memory.h"
#include "MemoryAddress.h"
#include "PPU.h"

namespace GBEmu::HW::PPURender
{
	constexpr int displayWidth_160 = HWParam::DisplayResolution.x;
	constexpr int displayHeight_144 = HWParam::DisplayResolution.y;
	constexpr int windowPriorityBufferSize_5 = PPU::WindowPriorityBufferSize_5;

	using namespace MemoryAddress;

	static const std::array<ColorF, 4> displayColorPaletteF{
		ColorF{ 221 / 255.0, 255 / 255.0, 212 / 255.0 },
		ColorF{ 144 / 255.0, 238 / 255.0, 144 / 255.0 },
		ColorF{ 29 / 255.0, 114 / 255.0, 61 / 255.0 },
		ColorF{ 0 / 255.0, 51 / 255.0, 0 / 255.0 },
	};

	constexpr int hlslPacking_4 = 4;

	struct TileBgAndWindowDmgCb
	{
		Float4 palette[4];
		uint32 windowPriorityBuffer[windowPriorityBufferSize_5 * hlslPacking_4];
	};

	struct TileObjDmgCb
	{
		Float4 palette[4];
		bool isMasking;
	};

	void RenderBGAndWindow(const PPURenderBGAndWindowArgs& arg)
	{
		// パレット
		ConstantBuffer<TileBgAndWindowDmgCb> tileCb{};

		// BG兼 ウィンドウパレットを設定
		for (int i=0; i<4; ++i) tileCb->palette[i] = displayColorPaletteF[arg.LCD.BGPaletteData(i)].toFloat4();

		// WindowEnable情報を設定
		for (int i=0; i<arg.WindowEnableBuffer.size(); ++i)
			tileCb->windowPriorityBuffer[i * hlslPacking_4] = arg.WindowEnableBuffer[i];

		// 転送
		Graphics2D::SetPSConstantBuffer(1, tileCb);

		// BG描画
		RenderBGCompletely(arg);

		// ウィンドウ描画
		RenderWindowCompletely(arg);
	}

	// Priorityフラグが付いたOBJ用のマスクを作成
	void RenderObjMaskFromBGAndWindow(const PPURenderBGAndWindowArgs& arg, const RenderTexture& objMaskBuffer)
	{
		ConstantBuffer<TileBgAndWindowDmgCb> tileCb{};

		const ScopedRenderTarget2D target{ objMaskBuffer };
		(void)objMaskBuffer.clear(Palette::Black);

		// マスク用パレットを設定
		tileCb->palette[0] = Float4(1, 1, 1, 1);
		tileCb->palette[1] = Float4(0, 0, 0, 0);
		tileCb->palette[2] = Float4(0, 0, 0, 0);
		tileCb->palette[3] = Float4(0, 0, 0, 0);

		// WindowEnable情報を設定
		for (int i=0; i<arg.WindowEnableBuffer.size(); ++i) tileCb->windowPriorityBuffer[i] = arg.WindowEnableBuffer[i];

		// 転送
		Graphics2D::SetPSConstantBuffer(1, tileCb);

		// BG描画
		RenderBGCompletely(arg);

		// ウィンドウ描画
		RenderWindowCompletely(arg);
	}

	void RenderBGCompletely(const PPURenderBGAndWindowArgs& arg)
	{
		const ScopedCustomShader2D shader{ HWAsset::Instance().PsTileBgDMG };

		const uint8 scy = arg.LCD.SCY();
		const uint8 scx = arg.LCD.SCX();

		const int scxModulo = scx % 8;
		const int scyModulo = scy % 8;

		int tileDataBaseIndex = 0;
		int bgBaseIndex = 0;
		for (int y= -8 + scyModulo; y < displayHeight_144 + scyModulo; y += 8)
		{
			// 描画中に割り込みでベースアドレスが変わっているかもしれないので、差分経歴を確認して反映
			if (arg.BGAndWindowTileDataDiff.History().size() > tileDataBaseIndex + 1 &&
				arg.BGAndWindowTileDataDiff.History()[tileDataBaseIndex + 1].LY <= y) tileDataBaseIndex++;
			const uint16 tileDataBaseAddr = arg.BGAndWindowTileDataDiff.History()[tileDataBaseIndex].Address;

			if (arg.BGTileMapDisplayDiff.History().size() > bgBaseIndex + 1 &&
				arg.BGTileMapDisplayDiff.History()[bgBaseIndex + 1].LY <= y) bgBaseIndex++;
			const uint16 bgBaseAddr = arg.BGTileMapDisplayDiff.History()[bgBaseIndex].Address;

			// 水平方向描画
			for (int x = -8 + scxModulo; x< displayWidth_160 + scxModulo; x += 8)
			{
				const uint8 scrolledX = static_cast<uint8>(x + scx);
				const uint8 scrolledY = static_cast<uint8>(y + scy);

				const uint8 tileX = scrolledX / 8;
				const uint8 tileY = scrolledY / 8;

				const uint16 tileIdAddr = bgBaseAddr + ((tileX + tileY * 32) & 0x03FF);
				const uint8 tileId = arg.Memory.Read(tileIdAddr);
				(void)arg.VRAM.GetTileData(tileDataBaseAddr, tileId).draw(x, y);
			}
		}
	}

	void RenderWindowCompletely(const PPURenderBGAndWindowArgs& arg)
	{
		const ScopedCustomShader2D shader{ HWAsset::Instance().PsTileWindowDMG };

		const uint8 wy = arg.LCD.WY();
		const uint8 wx = arg.LCD.WX();

		int tileDataBaseIndex = 0;
		int windowBaseIndex = 0;
		for (int y= wy; y < displayHeight_144; y += 8)
		{
			// 描画中に割り込みでベースアドレスが変わっているかもしれないので、差分経歴を確認して反映
			if (arg.BGAndWindowTileDataDiff.History().size() > tileDataBaseIndex + 1 &&
				arg.BGAndWindowTileDataDiff.History()[tileDataBaseIndex + 1].LY <= y) tileDataBaseIndex++;
			const uint16 tileDataBaseAddr = arg.BGAndWindowTileDataDiff.History()[tileDataBaseIndex].Address;

			if (arg.WindowTileMapDisplayDiff.History().size() > windowBaseIndex + 1 &&
				arg.WindowTileMapDisplayDiff.History()[windowBaseIndex + 1].LY <= y) windowBaseIndex++;
			const uint16 windowBaseAddr = arg.WindowTileMapDisplayDiff.History()[windowBaseIndex].Address;

			// 水平方向描画
			for (int x = wx - 7; x< displayWidth_160 + 8; x += 8)
			{
				const uint8 tileX = static_cast<uint8>(x - (wx - 7)) / 8;
				const uint8 tileY = static_cast<uint8>(y - wy) / 8;

				const uint16 tileIdAddr = windowBaseAddr + ((tileX + tileY * 32) & 0x03FF);
				const uint8 tileId = arg.Memory.Read(tileIdAddr);
				(void)arg.VRAM.GetTileData(tileDataBaseAddr, tileId).draw(x, y);
			}
		}
	}

	Array<OAMData> correctOAM(Memory& memory, const LCD& lcd)
	{
		const int objHeight = lcd.OBJHeight(); // 16 or 8

		Array<OAMData> result{};
		for (uint16 addr = OAMStart_0xFE00; addr <= OAMEnd_0xFE9F; addr += 4)
		{
			OAMData oam{};

			oam.Y = memory.Read(addr + 0);
			oam.X = memory.Read(addr + 1);

			oam.TileIndex = memory.Read(addr + 2);
			oam.Flags = memory.Read(addr + 3);

			if (objHeight == 16)
			{
				// スプライトのサイズが8x16なら、8x8のタイルの連続タイルごとにスプライトが形成される
				if (oam.FlagYFlip())
				{
					// 8x16 (反転あり)
					oam.TileIndex = oam.TileIndex | 0x01;
					result.push_back(oam);
					oam.TileIndex = oam.TileIndex & 0xFE;
					oam.Y += 8;
					result.push_back(oam);
				}
				else
				{
					// 8x16 (反転なし)
					oam.TileIndex = oam.TileIndex & 0xFE;
					result.push_back(oam);
					oam.TileIndex = oam.TileIndex | 0x01;
					oam.Y += 8;
					result.push_back(oam);
				}
			}
			else
			{
				// 8x8のタイル
				result.push_back(oam);
			}
		}
		return result;
	}

	void RenderOBJCompletely(const RenderOBJArgs& arg)
	{
		const ScopedCustomShader2D shader{ HWAsset::Instance().PsTileObjDMG };
		Graphics2D::SetPSTexture(1, arg.objMask);

		ConstantBuffer<TileObjDmgCb> tileCb{};
		tileCb->palette[0] = Float4(0, 0, 0, 0);

		const Array<OAMData> oamList = correctOAM(arg.memory, arg.lcd);

		// 末尾から描画していく
		for (int i=oamList.size()-1; i>=0; --i)
		{
			auto&& oam = oamList[i];

			tileCb->isMasking = oam.FlagPriorityBGAndWindow();

			auto texture =
				oam.FlagXFlip() ? arg.vram.GetTileData(TileDataTableStart_0x8000, oam.TileIndex).mirrored() :
					oam.FlagYFlip() ? arg.vram.GetTileData(TileDataTableStart_0x8000, oam.TileIndex).flipped() :
					arg.vram.GetTileData(TileDataTableStart_0x8000, oam.TileIndex);

			// パレット設定
			for (int color = 1; color < 4; ++color)
				tileCb->palette[color] =
					displayColorPaletteF[arg.lcd.ObjectPaletteData(oam.Palette(), color)].toFloat4();

			Graphics2D::SetPSConstantBuffer(1, tileCb);

			// 描画
			(void)texture.draw(oam.ActualX(), oam.ActualY());
		}
	}
}