#pragma once
#include "HWParams.h"
#include "LCD.h"
#include "PPUAddressLYDiff.h"
#include "PPURender.h"

namespace GBEmu::HW
{
	class HWEnv;

	enum class PPUMode : uint8
	{
		OAMSearch = 2,
		PixelTransfer = 3,
		HBlank = 0,
		VBlank = 1,
	};

	struct PPUResult
	{
		bool IsEnteredVBlank;
	};

	// Object Attribute Memory
	struct OAMData
	{
		uint8 Y; // Y-coordinate + 16
		uint8 X; // X-coordinate + 8
		uint8 TileIndex;
		uint8 Flags;

		int ActualY() const { return static_cast<int>(Y) - 16; }
		int ActualX() const { return static_cast<int>(X) - 8; }
		bool FlagPriorityBGAndWindow() const { return (Flags >> 7) & 0b1; };
		bool FlagYFlip() const { return (Flags >> 6) & 0b1; };
		bool FlagXFlip() const { return (Flags >> 5) & 0b1; };
		bool Palette() const { return (Flags >> 4) & 0b1; };
	};

	class PPU
	{
	public:
		PPU();
		void updateBGAndWindowFlagBuffer(LCD& lcd);

		PPUResult StepCycle(HWEnv& env);

		Size DrawAt(const Point& pos, double scale) const;

		LCD& GetLCD() { return m_lcd; };
		const LCD& GetLCD() const { return m_lcd; };

	private:
		int m_dotCycle{};
		PPUMode m_nextMode = PPUMode::OAMSearch;
		PPUMode m_mode = PPUMode::OAMSearch;

		RenderTexture m_renderBuffer{HWParam::DisplayResolution, ColorF{1.0}};
		RenderTexture m_objMaskBuffer{HWParam::DisplayResolution, TextureFormat::R16G16_Float};

		std::array<LCDCFlag128, LCDCFlagBufferSize_5> m_lcdcFlagBuffer{};

		PPUAddressLYDiff m_bgAndWindowTileDataDiff{};
		PPUAddressLYDiff m_bgTileMapDisplayDiff{};
		PPUAddressLYDiff m_windowTileMapDisplayDiff{};

		LCD m_lcd{};

		bool m_canSTATInterruptBefore{};

		void checkInterrupt(HWEnv& env, LCD& lcd, bool isModeChanged);

		void renderAtVBlank(HWEnv& env, Memory& memory, const LCD& lcd) const;

		static void updateLY(LCD& lcd, int dotCycle);
		static PPUMode judgePPUMode(int dotCycle);
	};
}
