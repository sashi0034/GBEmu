#pragma once

#include "CPU.h"
#include "Memory.h"

namespace GBEmu::HW
{
	class CPU;
	class Memory;

	class HWEnv
	{
	public:
		CPU& GetCPU()
		{
			return m_cpu;
		}
		Memory& GetMemory()
		{
			return m_memory;
		}
	private:
		CPU m_cpu{};
		Memory m_memory{};
	};
}
