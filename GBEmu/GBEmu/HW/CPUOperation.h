#pragma once
#include "CPUInstruction.h"
#include "HWEnv.h"

namespace GBEmu::HW
{
	struct CPUOperationZNHC
	{
		bool Z{};
		bool N{};
		bool H{};
		bool C{};
	};

	struct CPUOperationResult
	{
		uint8 CycleCount;
		Optional<CPUOperationZNHC> Flag;
		Optional<uint16> NextPC;

		[[nodiscard]]
		CPUOperationResult(uint8 cycleCount) :
			CycleCount(cycleCount),
			Flag(none),
			NextPC(none)
		{}

		[[nodiscard]]
		static CPUOperationResult ByCalc(uint8 cycleCount, CPUOperationZNHC flag)
		{
			return CPUOperationResult(cycleCount, flag, none);
		}

		[[nodiscard]]
		static CPUOperationResult ByJump(uint8 cycleCount, uint16 nextPC)
		{
			return CPUOperationResult(cycleCount, none, nextPC);
		}

		[[nodiscard]]
		static CPUOperationResult Invalid()
		{
			return CPUOperationResult(0);
		}

		[[nodiscard]]
		static CPUOperationResult Default()
		{
			return CPUOperationResult(4);
		}
	private:
		CPUOperationResult(uint8 cycleCount, const Optional<CPUOperationZNHC>& flag, const Optional<uint16>& nextPC) :
			CycleCount(cycleCount),
			Flag(flag),
			NextPC(nextPC){}
	};

	namespace CPUOperation
	{
		const CPUOperationResult OperateInstruction(HWEnv& env, CPUInstruction instr);
	};
}
