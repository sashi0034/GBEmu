#pragma once

namespace GBEmu::HW
{
	class IOPort;
	class HWEnv;
	class Memory;

	class Timer
	{
	public:
		Timer() = default;

		void StepCycle(HWEnv& env);
		void ResetDIVDetail();
	private:
		uint16 m_divDetail{};
		Optional<int> m_timaOverflowedCountdown = none;
		uint8 m_oldTAC{};

		void checkIncTIMA(IOPort& ioPort, uint8 newTAC, uint8 oldTAC);
		static bool canIncTIMA(uint8 newTAC, uint8 oldTAC, uint16 sysClock);
		void checkUpdateTIMAOverflowedCountdown(Memory& memory);
	};
}
