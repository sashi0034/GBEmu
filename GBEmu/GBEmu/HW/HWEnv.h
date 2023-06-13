#pragma once

#include "CPU.h"
#include "Memory.h"
#include "PPU.h"

namespace GBEmu::HW
{
	class CPU;
	class Memory;
	class PPU;

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
		PPU& GetPPU()
		{
			return m_ppu;
		}
	private:
		CPU m_cpu{};
		Memory m_memory{};
		PPU m_ppu;
	};
}
