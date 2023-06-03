#pragma once
#include "CPUInstructionCB.h"
#include "CPUOperation.h"

namespace GBEmu::HW
{
	namespace CPUOperationCB
	{
		CPUOperationResult OperateInstructionCB(HWEnv& ENV, CPUInstructionCB instr);
	};
}
