#include "stdafx.h"
#include "HWDebugger.h"

#include "HWEnv.h"
#include "HWLogger.h"
#include "MemoryAddress.h"
#include "GBEmu/Util/Utils.h"

namespace GBEmu::HW
{
	using namespace MemoryAddress;

	constexpr int size_5 = 5;
	constexpr int size_50 = 50;

	HWDebugger::HWDebugger()
	{
		m_cycleLogBuffer.resize(size_50);
	}

	void HWDebugger::Update(HWEnv& env)
	{
		// フレームごとに簡易ログを残す
		m_cycleLogBuffer.push_front(HWDebuggerCycleLog{
			env.GetCPU().PC(),
			env.GetCPU().FetchInstruction(env.GetMemory()).ToString()
		});
		m_cycleLogBuffer.pop_back();

		// トレースチェック
		const auto startedTrace = checkStartTrace(env);
		if (startedTrace.has_value())
		{
			// トレース開始
			m_tracedKey.emplace(startedTrace.value().first);
			m_traceCountdown = startedTrace.value().second;
		}

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

	Optional<std::pair<std::string, int>> HWDebugger::checkStartTrace(HWEnv& env) const
	{
		auto&& cpu = env.GetCPU();
		auto&& memory = env.GetMemory();
		using pair = std::pair<std::string, int>;

		// EIに達したらトレース開始
		if (cpu.FetchInstruction(memory).CodeUnprefixed() == CPUInstruction::EI_0xFB &&
			m_tracedKey.contains("EI") == false) return pair{"EI", size_50};

		// TimerのIEがONになったら
		if (memory.Read(IE_0xFFFF) & (1 << 2) &&
			m_tracedKey.contains("IE Timer") == false) return pair{"IE Timer", size_5};

		// HALTになったら
		// if (cpu.State() == CPUState::Halted &&
		// 	m_tracedKey.contains("HALT") == false) return pair{"HALT", size_5};

		return none;
	}

	void HWDebugger::debugTrace(HWEnv& env)
	{
		HWLogger::Info( U"CPU: { " + env.GetCPU().StringifyInfo(env.GetMemory()) + U" }\n");
	}
}
