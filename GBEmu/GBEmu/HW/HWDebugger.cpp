#include "stdafx.h"
#include "HWDebugger.h"

#include "HWEnv.h"
#include "HWLogger.h"
#include "GBEmu/Util/Utils.h"

namespace GBEmu::HW
{
	void HWDebugger::Update(HWEnv& env)
	{
		const auto trace = checkStartTrace(env);
		if (trace.has_value()) m_traceCountdown = trace.value();

		if (m_traceCountdown > 0)
		{
			m_traceCountdown--;
			debugTrace(env);
		}
	}

	void HWDebugger::Draw(HWEnv& env, const Point& leftTop) const
	{
		Array<String> message{};

		message.push_back(U"PC: {:04X}"_fmt(env.GetCPU().PC()));
		message.push_back(U"SP: {:04X}"_fmt(env.GetCPU().SP()));
		message.push_back(U"IME: {}"_fmt(env.GetCPU().IME()));
		message.push_back(U"instr: {}"_fmt(env.GetCPU().FetchInstruction(env.GetMemory()).ToString()));
		message.push_back(U"state: {}"_fmt(Util::StringifyEnum(env.GetCPU().State())));

		for (int i=0; i<message.size(); ++i)
		{
			(void)m_font(message[i]).draw(leftTop + Point::Down(lineMargin * i));
		}
	}

	Optional<int> HWDebugger::checkStartTrace(HWEnv& env)
	{
		auto&& cpu = env.GetCPU();
		auto&& memory = env.GetMemory();

		// EIに達したらトレース開始
		if (cpu.FetchInstruction(memory).CodeUnprefixed() == CPUInstruction::EI_0xFB) return 50;

		return none;
	}

	void HWDebugger::debugTrace(HWEnv& env)
	{
		HWLogger::Info( U"CPU: { " + env.GetCPU().StringifyInfo(env.GetMemory()) + U" }\n");
	}
}
