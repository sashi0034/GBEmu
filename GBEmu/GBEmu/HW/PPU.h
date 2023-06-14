#pragma once
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

	};

	struct OAMData;

	class PPU
	{
	public:
		PPU();

		PPUResult StepCycle(HWEnv& env);
	private:
		RenderTexture m_bitmap{HWParam::DisplayResolution, ColorF{1.0}};

		int m_dotIndex{};
		PPUMode m_mode = PPUMode::OAMSearch;
		Array<OAMData> m_oamBuffer{};

		static void updateLY(HWEnv& env, LCD& lcd, int dotIndex);
		static PPUMode updateMode(HWEnv& env, LCD& lcd, int dotIndex);
		static Array<OAMData> scanOAM(HWEnv& env, LCD& lcd);
	};
}
