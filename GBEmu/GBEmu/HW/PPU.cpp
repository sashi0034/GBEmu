#include "stdafx.h"
#include "PPU.h"

#include "HWEnv.h"
#include "MemoryAddress.h"

namespace GBEmu::HW
{
	using namespace MemoryAddress;

	constexpr int dotIndexFreq_70224 = 70224;
	constexpr int scanLineFreq_456 = 456;
	constexpr int scanLineSize_144 = 144;

	// Object Attribute Memory
	struct OAMData
	{
		uint8 Y; // Y-coordinate + 16
		uint8 X; // X-coordinate + 8
		uint8 TileIndex;
		uint8 Flags;

		uint8 ActualY() const {return Y - 16; }
		uint8 ActualX() const {return X - 8; }
		bool FlagPriority() const {return (Flags >> 7) & 0b1; };
		bool FlagYFlip() const {return (Flags >> 6) & 0b1; };
		bool FlagXFlip() const {return (Flags >> 5) & 0b1; };
		bool Palette() const {return (Flags >> 4) & 0b1; };
	};

	PPU::PPU()
	{
		m_dotIndex = dotIndexFreq_70224 - 52 + 4;
	}

	PPUResult PPU::StepCycle(HWEnv& env)
	{
		auto&& lcd = env.GetMemory().GetLCD();

		m_dotIndex = (m_dotIndex + 1) % dotIndexFreq_70224;
		if (lcd.IsLCDDisplayEnable() == false) m_dotIndex = 0;

		updateLY(env, lcd, m_dotIndex);
		const auto modeBefore = m_mode;
		const auto modeAfter = updateMode(env, lcd, m_dotIndex);
		m_mode = modeAfter;

		if (m_mode == PPUMode::OAMSearch && (m_dotIndex % scanLineFreq_456) == 79)
		{
			m_oamBuffer = scanOAM(env, lcd);
		}

		return PPUResult{};
	}

	void PPU::updateLY(HWEnv& env, LCD& lcd, int dotIndex)
	{
		const uint8 ly = dotIndex / scanLineFreq_456;
		lcd.SetLY(env, ly);
		lcd.UpdateLYCoincidenceFlag(env);
	}

	PPUMode PPU::updateMode(HWEnv& env, LCD& lcd, int dotIndex)
	{
		const int x = dotIndex % scanLineFreq_456;
		const PPUMode mode =
			lcd.LY() >= scanLineSize_144 ? PPUMode::VBlank :
			(x < 80) ? PPUMode::OAMSearch :
			(x < 369) ? PPUMode::PixelTransfer :
			PPUMode::HBlank;
		lcd.SetMode(env, mode);

		return mode;
	}

	Array<OAMData> PPU::scanOAM(HWEnv& env, LCD& lcd)
	{
		auto&& memory = env.GetMemory();

		const uint8 ly = lcd.LY();
		const int objHeight = lcd.OBJHeight(); // 16 or 8

		Array<OAMData> result{};
		for (uint16 addr = OAMStart_0xFE00; addr <= OAMEnd_0xFE9F; addr += 4)
		{
			// https://github.com/pokemium/gb-docs-ja/tree/main/video/oam
			// DMGモードだとX座標が小さいほど優先度が高くなり、CGBモードだとアドレスの小さい方が優先されるらしいが
			// ここではあえてCGBモードで進めてみる

			OAMData oam{};

			oam.Y = memory.Read(addr + 0);
			const bool isVisibleY = oam.ActualY() <=ly && ly < oam.ActualY() + objHeight;
			if (isVisibleY == false) continue;

			oam.X = memory.Read(addr + 1);
			const bool isVisibleX = oam.ActualX() > -8;
			if (isVisibleX == false) continue;

			oam.TileIndex = memory.Read(addr + 2);
			oam.Flags = memory.Read(addr + 3);

			if (objHeight == 16)
			{
				// スプライトのサイズが8x16なら、8x8のタイルの連続タイルごとにスプライトが形成される
				bool isTop =
					(oam.FlagYFlip() == false && ly < oam.ActualY() + 8) ||
					(oam.FlagYFlip() && (ly >= oam.ActualY() + 8));
				oam.TileIndex = isTop
					? oam.TileIndex & 0xFE
					: oam.TileIndex | 0x01;
			}

			result.push_back(oam);

			// 11個以上のOBJは無視
			if (result.size() > 10) break;
		}
		return result;
	}
}
