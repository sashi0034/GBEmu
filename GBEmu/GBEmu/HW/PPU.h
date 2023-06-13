#pragma once
#include "HWParams.h"

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

	class PPU
	{
	public:
		PPU();

		PPUResult StepCycle(HWEnv& env);
	private:
		RenderTexture m_bitmap{HWParam::DisplayResolution, ColorF{1.0}};

		int m_dotIndex{};
	};
}
