#pragma once

namespace GBEmu::HW
{
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

		uint8 getTIMA(Memory& memory);
		void setTIMA(Memory& memory, uint8 value);
		void checkIncTIMA(Memory& memory, uint8 newTAC, uint8 oldTAC);
		static bool canIncTIMA(uint8 newTAC, uint8 oldTAC, uint16 sysClock);
		void checkUpdateTIMAOverflowedCountdown(HWEnv& env, Memory& memory);
	};
}
