#pragma once
#include "CPU.h"
#include "GBEmu/Util/Range.h"

namespace GBEmu::HW
{
	class HWEnv;

	struct HWDebugExecutedInstruction
	{
		uint16 CurrentPC;
		String NextInstruction;
	};

	struct HWDebugWroteMemory
	{
		uint16 Address;
		uint8 Data;
		HWDebugExecutedInstruction PreviousInstr;
	};

	class HWDebugger
	{
	public:
		HWDebugger();

		// 1フレームごとに更新
		void UpdateFrame(HWEnv& env);

		// 1サイクルごとに更新
		void UpdateCycle(HWEnv& env);

		void Draw(HWEnv& env, const Point& leftTop) const;
		void OnExecuteInstruction(const CPU& cpu, const CPUInstructionProperty& fetchedInstruction);
		void OnMemoryWrite(uint16 address, uint8 data);
		bool IsDebugSuspend() const { return m_isDebugSuspend; };
	private:
		const Font m_font{16};
		static constexpr int lineMargin = 20;

		std::array<unsigned short, 256> m_foundInstructionDistribution{};
		std::array<unsigned short, 256> m_foundInstructionCBDistribution{};
		String stringifyFoundInstructionDistribution() const;

		Optional<uint16> m_statisticsPC{};
		int m_traceCountdown{};
		HashSet<std::string> m_tracedKey{};
		std::deque<HWDebugExecutedInstruction> m_executedInstructionLog{};
		std::deque<HWDebugWroteMemory> m_wroteMemoryLog{};
		bool m_isDebugSuspend = false;
		bool m_isWriteMemoryLog = false;

		Optional<std::pair<std::string, int>> checkStartTrace(HWEnv& env) const;
		void publishStatistics(HWEnv& env) const;
		static void debugTrace(HWEnv& env);
		static void printSearchedMemoryBlob(HWEnv& env, const Array<uint16>& blob);
		static Optional<uint16> searchMemoryBlob(Memory& memory, const RangeUint16& range, const Array<uint16>& blob);
	};
}
