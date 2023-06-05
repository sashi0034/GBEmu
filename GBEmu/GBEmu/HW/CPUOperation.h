#pragma once
#include "CPUInstruction.h"
#include "HWEnv.h"

namespace GBEmu::HW
{
	struct CPUOperationZNHC
	{
		const bool Z;
		const bool N;
		const bool H;
		const bool C;
	};

	struct CPUOperationResult
	{
		const uint8 ByteLength;
		const uint8 CycleCount;
		const Optional<CPUOperationZNHC> Flag;

		CPUOperationResult(uint8 byteLength, uint8 cycleCount) :
			ByteLength(byteLength),
			CycleCount(cycleCount),
			Flag(none)
		{}

		CPUOperationResult(uint8 byteLength, uint8 cycleCount, CPUOperationZNHC flag) :
			ByteLength(byteLength),
			CycleCount(cycleCount),
			Flag(flag)
		{}

		static CPUOperationResult Invalid()
		{
			return CPUOperationResult(0, 0);
		}
	};

	namespace CPUOperation
	{
		CPUOperationResult OperateInstruction(HWEnv& env, CPUInstruction instr);
	};
}
