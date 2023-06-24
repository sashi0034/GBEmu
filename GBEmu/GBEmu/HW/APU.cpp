#include "stdafx.h"
#include "APU.h"

namespace GBEmu::HW
{
	APU::APU() :
		m_stream(std::make_shared<APUStream>()),
		m_audio(Audio(m_stream))
	{}

	void APU::StepCycle(HWEnv& env)
	{
	}
}
