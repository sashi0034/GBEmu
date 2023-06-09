#include "stdafx.h"
#include "CPU.h"
#include "HWEnv.h"
#include "CPUOperation.h"
#include "CPUOperationCB.h"
#include "HWLogger.h"

namespace GBEmu::HW
{
	CPUCycle CPU::StepOperation(HWEnv& env)
	{
		// HALTされているとき、割り込みまで進まない
		constexpr int haltCycle = 4;
		if (m_state != CPUState::Running) return CPUCycle{haltCycle};

		auto [isPrefixedCB, code] = fetchInstruction(env);

		// 命令実行
		const auto opResult =
			isPrefixedCB
              ? CPUOperationCB::OperateInstructionCB(env, static_cast<CPUInstructionCB>(code))
              : CPUOperation::OperateInstruction(env, static_cast<CPUInstruction>(code));

		m_pc = opResult.NextPC.has_value()
			// 分岐命令など
			? opResult.NextPC.value()
			// 基本は、実行した命令長を進める
			: m_pc + opResult.ByteLength;

		// 算術演算のときはフラグ更新
		if (opResult.Flag.has_value()) m_regF = applyFlagZNHC(m_regF, opResult.Flag.value());

		return CPUCycle{opResult.CycleCount};
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

	CPUInstructionProperty CPU::fetchInstruction(HWEnv& env) const
	{
		const uint8 nextCode = env.GetMemory().Read(m_pc);

		if (nextCode != 0xCB) return {false, nextCode};

		return {true, env.GetMemory().Read(m_pc + 1)};
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
