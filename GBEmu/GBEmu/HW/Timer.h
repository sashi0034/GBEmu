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
		Array<int> m_timaOverflowedCountdown{};

		uint8 getTima(Memory& memory);
		void setTima(Memory& memory, uint8 value);
		void checkIncTima(Memory& memory, int timaFreq, bool isTimaEnable);
		void checkUpdateTimaOverflowedCountdown(Memory& memory);
	};
}
