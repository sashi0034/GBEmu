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
		const uint8 ByteLength;
		const uint8 CycleCount;
		const Optional<CPUOperationZNHC> Flag;
		const Optional<uint16> NextPC;

		[[nodiscard]]
		CPUOperationResult(uint8 byteLength, uint8 cycleCount) :
			ByteLength(byteLength),
			CycleCount(cycleCount),
			Flag(none),
			NextPC(none)
		{}

		[[nodiscard]]
		static CPUOperationResult ByCalc(uint8 byteLength, uint8 cycleCount, CPUOperationZNHC flag)
		{
			return CPUOperationResult(byteLength, cycleCount, flag, none);
		}

		[[nodiscard]]
		static CPUOperationResult ByJump(uint8 byteLength, uint8 cycleCount, uint16 nextPC)
		{
			return CPUOperationResult(byteLength, cycleCount, none, nextPC);
		}

		[[nodiscard]]
		static CPUOperationResult Invalid()
		{
			return CPUOperationResult(0, 0);
		}

		[[nodiscard]]
		static CPUOperationResult Default()
		{
			return CPUOperationResult(1, 4);
		}
	private:
		CPUOperationResult(uint8 byteLength, uint8 cycleCount, const Optional<CPUOperationZNHC>& flag, const Optional<uint16>& nextPC) :
			ByteLength(byteLength),
			CycleCount(cycleCount),
			Flag(flag),
			NextPC(nextPC){}
	};

	namespace CPUOperation
	{
		CPUOperationResult OperateInstruction(HWEnv& env, CPUInstruction instr);
	};
}
