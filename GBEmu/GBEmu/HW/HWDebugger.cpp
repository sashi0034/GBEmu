#include "stdafx.h"
#include "HWDebugger.h"

#include "HWEnv.h"
#include "HWLogger.h"
#include "MemoryAddress.h"
#include "GBEmu/Util/Utils.h"

#define CUSTOMIZABLE

namespace GBEmu::HW
{
	using namespace MemoryAddress;

	constexpr int size_5 = 5;
	constexpr int size_50 = 50;

	HWDebugger::HWDebugger()
	{
		m_executedInstructionLog.resize(size_50);

#ifdef CUSTOMIZABLE
		// 特定のPCになったら統計出す
		m_statisticsPC = 52113;
#endif
	}

	void HWDebugger::Update(HWEnv& env)
	{
		// トレースチェック
		const auto startedTrace = checkStartTrace(env);
		if (startedTrace.has_value())
		{
			// トレース開始
			m_tracedKey.emplace(startedTrace.value().first);
			m_traceCountdown = startedTrace.value().second;
		}

		if (env.GetCPU().PC() == m_statisticsPC)
		{
			publishStatistics(env);
			m_statisticsPC = none;
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

	void HWDebugger::OnExecuteInstruction(const CPU& cpu, const CPUInstructionProperty& fetchedInstruction)
	{
		// 命令実行経歴を残す
		m_executedInstructionLog.push_front(HWDebuggerExecutedInstruction{
			cpu.PC(),
			fetchedInstruction.ToString()
		});
		m_executedInstructionLog.pop_back();

		// 命令出現状況を記憶
		if (fetchedInstruction.IsPrefixedCB)
			m_foundInstructionCBDistribution[fetchedInstruction.Code]++;
		else
			m_foundInstructionDistribution[fetchedInstruction.Code]++;
	}

	String HWDebugger::stringifyFoundInstructionDistribution() const
	{
		String str = U"";
		for (int i=0; i<256; ++i)
		{
			str += U"{}: {}\n"_fmt(Util::StringifyEnum(static_cast<CPUInstruction>(i)), m_foundInstructionDistribution[i]);
		}
		for (int i=0; i<256; ++i)
		{
			str += U"{}: {}\n"_fmt(Util::StringifyEnum(static_cast<CPUInstructionCB>(i)), m_foundInstructionCBDistribution[i]);
		}
		return str;
	}

	Optional<std::pair<std::string, int>> HWDebugger::checkStartTrace(HWEnv& env) const
	{
		auto&& cpu = env.GetCPU();
		auto&& memory = env.GetMemory();
		using pair = std::pair<std::string, int>;

		// トレース開始チェック
#ifdef CUSTOMIZABLE

		// 命令に達したらトレース開始
		if (cpu.FetchInstruction(memory).CodeUnprefixed() == CPUInstruction::LD_SP_d16_0x31 &&
			m_tracedKey.contains("MISC") == false) return pair{"MISC", size_50};

		// EIに達したら
		// if (cpu.FetchInstruction(memory).CodeUnprefixed() == CPUInstruction::EI_0xFB &&
		// 	m_tracedKey.contains("EI") == false) return pair{"EI", size_50};

		// TimerのIEがONになったら
		// if (memory.Read(IE_0xFFFF) & (1 << 2) &&
		// 	m_tracedKey.contains("IE Timer") == false) return pair{"IE Timer", size_5};

		// HALTになったら
		// if (cpu.State() == CPUState::Halted &&
		// 	m_tracedKey.contains("HALT") == false) return pair{"HALT", size_5};

#endif
		return none;
	}

	void HWDebugger::debugTrace(HWEnv& env)
	{
		HWLogger::Info( U"CPU: { " + env.GetCPU().StringifyInfo(env.GetMemory()) + U" }\n");
	}

	void HWDebugger::publishStatistics(HWEnv& env) const
	{
#ifdef CUSTOMIZABLE
		Console.writeln(stringifyFoundInstructionDistribution());

		uint16 addr = searchMemoryBlob(env.GetMemory(), RangeUint16(0x0000, 0xFFFF), {0xF0, 0x91, 0x00});
		Console.writeln(U"{:04X}"_fmt(addr));
#endif
	}


	// データ列を受け取って、そのメモリ列が存在するならアドレスを返す
	uint16 HWDebugger::searchMemoryBlob(Memory& memory, const RangeUint16& range, const Array<uint16>& blob)
	{
		for (uint16 i=range.Min(); i<= range.Max() - blob.size() + 1; ++i)
		{
			if (memory.Read(i) != blob[0]) continue;

			bool isMatch = true;
			for (int x = 1; x<blob.size(); ++x)
			{
				if (memory.Read(i + x) == blob[x]) continue;
				isMatch = false;
				break;
			}
			if (isMatch) return i;
		}
		return 0xFFFF;
	}
}
