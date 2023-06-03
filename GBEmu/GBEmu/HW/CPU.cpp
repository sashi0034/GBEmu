#include "stdafx.h"
#include "CPU.h"
#include "HardwareEnv.h"
#include "CPUOperation.h"

namespace GBEmu::HW
{
	void CPU::StepOperation(HardwareEnv& env)
	{
		// uint8 readCode = env.GetMemory().ReadAt()

		auto&& operation = CPUOperation::OperateInstruction(env);



	}
}
