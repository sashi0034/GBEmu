#include "stdafx.h"
#include "PPU.h"

namespace GBEmu::HW
{
	constexpr int dotIndexFreq_70224 = 70224;

	PPU::PPU()
	{
		m_dotIndex = dotIndexFreq_70224 - 52 + 4;
	}

	PPUResult PPU::StepCycle(HWEnv& env)
	{
		m_dotIndex = (m_dotIndex + 1) % dotIndexFreq_70224;


		return PPUResult{};
	}
}
