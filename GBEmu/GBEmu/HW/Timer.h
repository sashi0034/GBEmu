#pragma once
#include "MemoryAddress.h"

namespace GBEmu::HW
{
	class HWEnv;
	class Memory;

	class Timer
	{
	public:
		Timer() = default;

		void StepCycle(HWEnv& env);

		template <uint16 addr> void WriteAddr(uint8 data);
		template <uint16 addr> uint8 ReadAddr() const;
	private:
		uint16 m_divDetail{};

		uint8 m_tima{};
		Optional<int> m_timaOverflowedCountdown = none;

		uint8 m_tac{};
		uint8 m_tacBefore{};

		uint8 m_tma{};

		void checkIncTIMA(uint8 newTAC, uint8 oldTAC);
		static bool canIncTIMA(uint8 newTAC, uint8 oldTAC, uint16 sysClock);
		void checkUpdateTIMAOverflowedCountdown(Memory& memory);
	};

	template <uint16 addr>
	void Timer::WriteAddr(uint8 data)
	{
		using namespace MemoryAddress;

		if constexpr (addr == DIV_0xFF04) m_divDetail = 0x00; // DIVに書き込むと0にリセット
		else if constexpr (addr == TIMA_0xFF05) m_tima = data;
		else if constexpr (addr == TAC_0xFF07) m_tac = data;
		else if constexpr (addr == TMA_0xFF06) m_tma = data;
		else {assert(false); }
	}

	template <uint16 addr>
	uint8 Timer::ReadAddr() const
	{
		using namespace MemoryAddress;

		if constexpr (addr == DIV_0xFF04) return m_divDetail >> 8; // DIVレジスタは256クロックサイクルごとにインクリメントされるので、下位8bitを捨てる
		else if constexpr (addr == TIMA_0xFF05) return m_tima;
		else if constexpr (addr == TAC_0xFF07) return m_tac;
		else if constexpr (addr == TMA_0xFF06) return m_tma;
	}
}
