#include "stdafx.h"
#include "Timer.h"

#include "HWEnv.h"
#include "HWParams.h"

namespace GBEmu::HW
{
	void Timer::StepCycle(HWEnv& env)
	{
		auto&& memory = env.GetMemory();

		m_divDetail++;

		// Timer制御レジスタ
		m_tacBefore = m_tac;
		checkIncTIMA(m_tac, m_tacBefore);

		checkUpdateTIMAOverflowedCountdown(memory);
	}

	void Timer::checkIncTIMA(uint8 newTAC, uint8 oldTAC)
	{
		if (m_timaOverflowedCountdown.has_value()) return;
		if (canIncTIMA(newTAC, oldTAC, m_divDetail) == false) return;

		// TIMAをインクリメント
		m_tima++;

		if (m_tima != 0) return;

		// オーバーフロー
		// TIMAがオーバーフローした後の1マシンサイクルでは、TIMAの値がTMAからの値ではなく0x00になる
		m_timaOverflowedCountdown = HWParam::MachineCycle; // Timer割り込みを遅延実行
		// TIMA周波数はいずれも十分大きいから、m_timaOverflowedCountdownを直接上書きして大丈夫
	}

	bool Timer::canIncTIMA(uint8 newTAC, uint8 oldTAC, uint16 sysClock)
	{
		// TACに書き込むとき、以前に選択されたマルチプレクサの入力が1で、新しい入力が0の場合、TIMAも増加
		static const std::array<uint16, 4> clockArray = {1024, 16, 64, 256};

		const auto oldClock = clockArray[oldTAC & 0b11];
		const auto newClock = clockArray[newTAC & 0b11];

		const bool oldEnable = oldTAC & 0b100;
		const bool newEnable = newTAC & 0b100;

		if (oldEnable == false) return false;

		const uint16 sysClockBefore = sysClock - 1;

		return (newEnable == false)
		       ? ((sysClockBefore & (oldClock / 2)) != 0)
		       : (sysClockBefore & (oldClock / 2)) != 0 && (sysClock & (newClock / 2)) == 0;
	}

	void Timer::checkUpdateTIMAOverflowedCountdown(Memory& memory)
	{
		if (m_timaOverflowedCountdown == none) return;

		m_timaOverflowedCountdown = m_timaOverflowedCountdown.value() - 1;
		if (m_timaOverflowedCountdown.value() > 0) return;;
		m_timaOverflowedCountdown = none;

		// Timer割り込み要求
		memory.Interrupt().SetFlag(InterruptFlags::Timer);

		// TIMAをTMAの値にする
		m_tima = m_tma;
	}
}
