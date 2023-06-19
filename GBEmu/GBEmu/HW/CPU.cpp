#include "stdafx.h"
#include "CPU.h"
#include "HWEnv.h"
#include "CPUOperation.h"
#include "CPUOperationCB.h"
#include "HWLogger.h"
#include "MemoryAddress.h"
#include "GBEmu/Util/Utils.h"

namespace GBEmu::HW
{
	using namespace MemoryAddress;

	String CPUInstructionProperty::ToString() const
	{
		return IsPrefixedCB
			? Unicode::Widen(magic_enum::enum_name(static_cast<CPUInstructionCB>(Code)))
			: Unicode::Widen(magic_enum::enum_name(static_cast<CPUInstruction>(Code)));
	}

	CPU::CPU()
	{
		SetAF(0x01B0);
		SetBC(0x0013);
		SetDE(0x00D8);
		SetHL(0x014D);
		m_sp = 0xFFFE;
		m_pc = 0x0100;
	}

	CPUCycle CPU::StepOperation(HWEnv& env)
	{
		// 割り込みチェック
		const auto interrupt = checkInterrupt(env);
		if (interrupt.has_value()) return interrupt.value();

		// EIの効果は1命令分だけ遅れる
		if (m_imeRequested)
		{
			m_imeRequested = false;
			m_imeFlag = true;
		}

		// HALTされているとき、割り込みまで進まない
		if (m_state != CPUState::Running)
		{
			constexpr int haltCycle = 4;
			return CPUCycle{haltCycle};
		}

		// 命令フェッチ
		auto fetched = FetchInstruction(env.GetMemory());

		// 実行イベントを通知
		env.Debugger().OnExecuteInstruction(*this, fetched);

		// 命令実行
		const auto opResult =
			fetched.IsPrefixedCB
              ? CPUOperationCB::OperateInstructionCB(env, static_cast<CPUInstructionCB>(fetched.Code))
              : CPUOperation::OperateInstruction(env, static_cast<CPUInstruction>(fetched.Code));

		m_pc = opResult.NextPC.has_value()
			// 分岐命令など
			? opResult.NextPC.value()
			// 基本は、実行した命令長を進める
			: m_pc + opResult.ByteLength;

		// 算術演算のときはフラグ更新
		if (opResult.Flag.has_value()) m_regF = applyFlagZNHC(m_regF, opResult.Flag.value());

		return CPUCycle{opResult.CycleCount};
	}

	Optional<CPUCycle> CPU::checkInterrupt(HWEnv& env)
	{
		auto&& memory = env.GetMemory();
		const uint8 interruptEnable = memory.Interrupt().IE();
		const uint8 interruptFlag = memory.Interrupt().IF();

		if (m_imeFlag == false)
		{
			// IMEが許可されてなくても、IFが立ったらHALT終了
			if ((interruptFlag & interruptEnable) != 0) m_state = CPUState::Running;
			return none;
		}

		static const std::array<uint16, 5> interruptAddress{
			InterruptVBlank_0x0040,
			InterruptSTAT_0x0048,
			InterruptTimer_0x0050,
			InterruptSerial_0x0058,
			InterruptJoypad_0x0060
		};

		for (int i=0; i<interruptAddress.size(); ++i)
		{
			auto result =
				handleInterrupt(env, memory, interruptEnable, interruptFlag, interruptAddress[i], i);
			if (result.has_value()) return result;
		}
		return none;
	}

	Optional<CPUCycle> CPU::handleInterrupt(
		HWEnv& env, Memory& memory,
		uint8 interruptEnable, uint8 interruptFlag,
		uint16 interruptAddr, int interruptBit)
	{
		const auto maskedFlag = interruptEnable & interruptFlag;
		const bool isInterrupt = maskedFlag & (1 << interruptBit);
		if (isInterrupt == false) return none;

		// 割り込み無効化
		m_imeFlag = false;
		memory.Interrupt().ResetFlag(InterruptFlag(1 << interruptBit));

		// PCをスタックにプッシュ
		memory.WriteDirect16(m_sp - 2, m_pc);
		m_sp -= 2;

		// 割り込み先へジャンプ
		m_pc = interruptAddr;

		m_state = CPUState::Running;

		// 割り込みは5マシンサイクルを消費
		return CPUCycle(20);
	}

	uint8 CPU::GetReg8(CPUReg8 kind) const
	{
		switch (kind) {
		case CPUReg8::A: return RegA();
		case CPUReg8::F: return RegF();
		case CPUReg8::B: return RegB();
		case CPUReg8::C: return RegC();
		case CPUReg8::D: return RegD();
		case CPUReg8::E: return RegE();
		case CPUReg8::H: return RegH();
		case CPUReg8::L: return RegL();
		default: HWLogger::Error(U"attempt to get invalid register"); return 0;
		}
	}

	void CPU::SetReg8(CPUReg8 kind, uint8 value)
	{
		switch (kind) {
		case CPUReg8::A: SetA(value); return;
		case CPUReg8::F: SetF(value); return;
		case CPUReg8::B: SetB(value); return;
		case CPUReg8::C: SetC(value); return;
		case CPUReg8::D: SetD(value); return;
		case CPUReg8::E: SetE(value); return;
		case CPUReg8::H: SetH(value); return;
		case CPUReg8::L: SetL(value); return;
		default: HWLogger::Error(U"attempt to set invalid register"); return;
		}
	}

	CPUInstructionProperty CPU::FetchInstruction(Memory& memory) const
	{
		const uint8 nextCode = memory.Read(m_pc);

		if (nextCode != 0xCB) return {false, nextCode};

		return {true, memory.Read(m_pc + 1)};
	}

	String CPU::StringifyInfo(Memory& memory) const
	{
		String str{};
		str += U"PC: {:04X}, "_fmt(m_pc);
		str += U"SP: {:04X}, "_fmt(m_sp);
		str += U"AF: {:04X}, "_fmt(RegAF());
		str += U"BC: {:04X}, "_fmt(RegBC());
		str += U"DE: {:04X}, "_fmt(RegDE());
		str += U"HL: {:04X},\n"_fmt(RegHL());
		str += U"IME: {}, "_fmt(IME());
		str += U"state: {},\n"_fmt(Util::StringifyEnum(m_state));
		str += U"instr: {}"_fmt(FetchInstruction(memory).ToString());
		return str;
	}

	uint8 CPU::applyFlagZNHC(uint8 regF, CPUOperationZNHC flag)
	{
		regF = (regF & (~(1 << 7))) | static_cast<int>(flag.Z) << 7;
		regF = (regF & (~(1 << 6))) | static_cast<int>(flag.N) << 6;
		regF = (regF & (~(1 << 5))) | static_cast<int>(flag.H) << 5;
		regF = (regF & (~(1 << 4))) | static_cast<int>(flag.C) << 4;
		return regF;
	}
}
