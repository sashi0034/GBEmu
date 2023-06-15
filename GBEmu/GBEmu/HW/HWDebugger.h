#pragma once

namespace GBEmu::HW
{
	class HWEnv;

	class HWDebugger
	{
	public:
		void Update(HWEnv& env);
		void Draw(HWEnv& env, const Point& leftTop) const;
	private:
		const Font m_font{16};
		static constexpr int lineMargin = 20;

		int m_traceCountdown{};

		static Optional<int> checkStartTrace(HWEnv& env);
		static void debugTrace(HWEnv& env);
	};
}
