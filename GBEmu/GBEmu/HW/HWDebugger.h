#pragma once
#include "CPU.h"
#include "GBEmu/Util/Range.h"

namespace GBEmu::HW
{
	class HWEnv;

	class HWDebugger
	{
	public:
		HWDebugger();
		~HWDebugger();

		// 1フレームごとに更新
		void UpdateFrame(HWEnv& env);

		// 1サイクルごとに更新
		void UpdateCycle(HWEnv& env, CPUCycle cycle);

		void OnExecuteInstruction(const CPU& cpu, const CPUInstructionProperty& fetchedInstruction);
		void OnMemoryWrite(uint16 address, uint8 data);
		bool IsDebugSuspend() const { return m_isDebugSuspend; };
		const CPUInstructionProperty& LastExecutedInstruction() const { return m_lastExecutedInstruction; };
		double CPUWorkedRate() const {return m_cpuWorkedCycleInFrame / static_cast<double>(HWParam::PPUCyclePeriod_70224); }

		static Optional<uint16> SearchMemoryBlob(HWEnv& env, Memory& memory, const RangeUint16& range, const Array<uint16>& blob);
	private:
		// UIで使わず開発でのみ使うものはImplに入れる
		class Impl;
		struct ImplPtr : std::unique_ptr<Impl> { ~ImplPtr(); } m_impl;

		CPUInstructionProperty m_lastExecutedInstruction;
		bool m_isDebugSuspend = false;
		int m_cpuWorkedCycleInFrame{};
	};


}
