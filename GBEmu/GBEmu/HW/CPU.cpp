#include "stdafx.h"
#include "CPU.h"
#include "HardwareEnv.h"
#include "CPUOperation.h"

namespace GBEmu::HW
{
	void CPU::StepOperation(HardwareEnv& env)
	{
		auto&& operation = CPUOperation::OperateInstruction(env);
	}
}
