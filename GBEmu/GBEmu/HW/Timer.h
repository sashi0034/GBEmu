#pragma once
#include "HWEnv.h"

namespace GBEmu::HW
{
	class Timer
	{
	public:
		Timer() = default;

		void StepCycle(HWEnv& env);
	private:
		uint16 m_divDetail{};
		Optional<int> m_timaOverflowedCountdown = none;

		uint8 getTima(Memory& memory);
		void setTima(Memory& memory, uint8 value);
		void checkIncTima(Memory& memory, int timaFreq, bool isTimaEnable);
		void checkUpdateTimaOverflowedCountdown(Memory& memory);
	};
}
