#pragma once
#include "HWEnv.h"
#include "HWParams.h"

namespace GBEmu::HW
{
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
