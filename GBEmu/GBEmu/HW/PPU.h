﻿#pragma once
#include "HWParams.h"
#include "LCD.h"

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

	struct OAMData;

	class PPU
	{
	public:
		PPU();

		PPUResult StepCycle(HWEnv& env);

		void Draw(const Point pos, double scale) const;
	private:
		int m_dotCycle{};
		PPUMode m_mode = PPUMode::OAMSearch;
		Array<OAMData> m_oamBuffer{};

		Image m_bitmap{HWParam::DisplayResolution, ColorF{1.0}};
		int m_fetcherX{};
		bool m_canSTATInterruptBefore{};

		void checkInterrupt(HWEnv& env, LCD& lcd, bool isModeChanged);

		static void updateLY(HWEnv& env, LCD& lcd, int dotCycle);
		static PPUMode updateMode(HWEnv& env, LCD& lcd, int dotCycle);
		static Array<OAMData> scanOAM(HWEnv& env, LCD& lcd);
		static void scanLineX(HWEnv& env, LCD& lcd, int fetcherX, const Array<OAMData>& oamBuffer, Image& bitmap);
		static Color fetchPixelByMergeOAM(LCD& lcd, int fetcherX, const Array<OAMData>& oamBuffer, uint8 ly, uint8 tileDataColor);
	};
}
