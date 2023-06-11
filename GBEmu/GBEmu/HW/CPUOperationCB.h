#pragma once
#include "CPUInstructionCB.h"
#include "CPUOperation.h"

namespace GBEmu::HW
{
	namespace CPUOperationCB
	{
		const CPUOperationResult OperateInstructionCB(HWEnv& env, CPUInstructionCB instr);
	};
}
