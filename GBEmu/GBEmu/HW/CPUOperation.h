﻿#pragma once
#include "CPUInstruction.h"
#include "HWEnv.h"

namespace GBEmu::HW
{
	struct CPUOperationResult
	{
		const uint8 ByteLength;
		const uint8 CycleCount;

		CPUOperationResult(uint8 byteLength, uint8 cycleCount) :
			ByteLength(byteLength),
			CycleCount(cycleCount)
		{}
	};

	namespace CPUOperation
	{
		CPUOperationResult OperateInstruction(HWEnv& ENV);
	};
}
