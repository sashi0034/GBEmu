#pragma once
#include "CPU.h"

namespace GBEmu::HW
{
	class HWEnv;

	struct HWDebuggerCycleLog
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
	private:
		const Font m_font{16};
		static constexpr int lineMargin = 20;

		int m_traceCountdown{};
		HashSet<std::string> m_tracedKey{};
		std::deque<HWDebuggerCycleLog> m_cycleLogBuffer{};

		Optional<std::pair<std::string, int>> checkStartTrace(HWEnv& env) const;
		static void debugTrace(HWEnv& env);
	};
}
