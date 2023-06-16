#pragma once
#include "CPU.h"

namespace GBEmu::HW
{
	class HWEnv;

	struct HWDebuggerExecutedInstruction
	{
		uint16 CurrentPC;
		String NextInstruction;
	};

	class HWDebugger
	{
	public:
		HWDebugger();
		void Update(HWEnv& env);
		void Draw(HWEnv& env, const Point& leftTop) const;
		void OnExecuteInstruction(const CPU& cpu, const CPUInstructionProperty& fetchedInstruction);
	private:
		const Font m_font{16};
		static constexpr int lineMargin = 20;

		std::array<unsigned short, 256> m_foundInstructionDistribution{};
		std::array<unsigned short, 256> m_foundInstructionCBDistribution{};
		String stringifyFoundInstructionDistribution() const;

		Optional<uint16> m_statisticsPC{};
		int m_traceCountdown{};
		HashSet<std::string> m_tracedKey{};
		std::deque<HWDebuggerExecutedInstruction> m_executedInstructionLog{};

		Optional<std::pair<std::string, int>> checkStartTrace(HWEnv& env) const;
		void publishStatistics(HWEnv& env) const;
		static void debugTrace(HWEnv& env);
	};
}
