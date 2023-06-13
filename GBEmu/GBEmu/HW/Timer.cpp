#include "stdafx.h"
#include "Timer.h"

#include "MemoryAddress.h"
#include "HWParams.h"

namespace GBEmu::HW
{
	using namespace MemoryAddress;

	void Timer::StepCycle(HWEnv& env)
	{
		auto&& memory = env.GetMemory();

		// 本来DIVレジスタは256クロックサイクルごとにインクリメントされるので、メモリに反映するとき下位8bitを捨てる
		m_divDetail++;
		memory.WriteDirect(DIV_0xFF04, m_divDetail >> 8);

		// Timer制御レジスタ
		const uint8 tac = env.GetMemory().Read(TAC_0xFF07);

		const int timaFreq =
			tac & 0b11 == 0b00 ? 1024 :
			tac & 0b11 == 0b01 ? 16 :
			tac & 0b11 == 0b10 ? 64 :
			256; // when 0b11

		const bool isTimaEnable = tac & 0b100 != 0;

		checkIncTima(memory, timaFreq, isTimaEnable);

		checkUpdateTimaOverflowedCountdown(env, memory);
	}

	uint8 Timer::getTima(Memory& memory)
	{
		return memory.Read(TIMA_0xFF05);
	}

	void Timer::setTima(Memory& memory, uint8 value)
	{
		memory.WriteDirect(TIMA_0xFF05, value);
	}

	void Timer::checkIncTima(Memory& memory, const int timaFreq, const bool isTimaEnable)
	{
		if (m_timaOverflowedCountdown)
		if (isTimaEnable == false) return;
		if (m_divDetail % timaFreq != 0) return;

		// TIMAをインクリメント
		setTima(memory, getTima(memory) + 1);

		if (getTima(memory) != 0) return;

		// オーバーフロー
		// TIMAがオーバーフローした後の1マシンサイクルでは、TIMAの値がTMAからの値ではなく0x00になる
		setTima(memory, memory.Read(0x00));
		m_timaOverflowedCountdown = HWParam::MachineCycle; // Timer割り込みを遅延実行
		// TIMA周波数はいずれも十分大きいから、m_timaOverflowedCountdownを直接上書きして大丈夫
	}

	void Timer::checkUpdateTimaOverflowedCountdown(HWEnv& env, Memory& memory)
	{
		if (m_timaOverflowedCountdown == none) return;

		m_timaOverflowedCountdown = m_timaOverflowedCountdown.value() - 1;
		if (m_timaOverflowedCountdown.value() > 0) return;;
		m_timaOverflowedCountdown = none;

		// Timer割り込み要求
		memory.Write(env, IF_0xFF0F, memory.Read(IF_0xFF0F) | HWParam::InterruptTimer);

		// TIMAをTMAの値にする
		setTima(memory, memory.Read(TMA_0xFF06));
	}
}
