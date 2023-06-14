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
		int m_dotIndex{};
		PPUMode m_mode = PPUMode::OAMSearch;
		Array<OAMData> m_oamBuffer{};

		// Y優先のビットマップ
		Image m_bitmap{HWParam::DisplayResolution.yx(), ColorF{1.0}};
		int m_fetcherX{};

		static void updateLY(HWEnv& env, LCD& lcd, int dotIndex);
		static PPUMode updateMode(HWEnv& env, LCD& lcd, int dotIndex);
		static Array<OAMData> scanOAM(HWEnv& env, LCD& lcd);
		void static scanLineX(HWEnv& env, LCD& lcd, int fetcherX, const Array<OAMData>& oamBuffer, Image& bitmap);
		static Color fetchPixelByMergeOAM(LCD& lcd, int fetcherX, const Array<OAMData>& oamBuffer, uint8 ly,
		                                  uint8 tileDataColor);
	};
}
