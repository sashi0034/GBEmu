#include "stdafx.h"
#include "CPU.h"
#include "HWEnv.h"
#include "CPUOperation.h"
#include "CPUOperationCB.h"

namespace GBEmu::HW
{
	void CPU::StepOperation(HWEnv& env)
	{
		auto nextInstruction = fetchInstruction(env);

		auto&& operation = nextInstruction.IsPrefixedCB
			? CPUOperationCB::OperateInstructionCB(env, static_cast<CPUInstructionCB>(nextInstruction.Code))
			: CPUOperation::OperateInstruction(env, static_cast<CPUInstruction>(nextInstruction.Code));

		// TODO: PC進める

		// TODO: applyFlagZNHCする

	}

	CPUInstructionProperty CPU::fetchInstruction(HWEnv& env) const
	{
		uint8 nextCode = env.GetMemory().Read(m_pc);

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
