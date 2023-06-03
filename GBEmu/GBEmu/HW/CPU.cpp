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

	}

	CPUInstructionProperty CPU::fetchInstruction(HWEnv& env) const
	{
		uint8 nextCode = env.GetMemory().Read(m_pc);

		if (nextCode != 0xCB) return {false, nextCode};

		return {true, env.GetMemory().Read(m_pc + 1)};
	}
}
