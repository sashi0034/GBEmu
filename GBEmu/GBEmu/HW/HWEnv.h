#pragma once

#include "CPU.h"
#include "Memory.h"
#include "PPU.h"

namespace GBEmu::HW
{
	class CPU;
	class Memory;
	class PPU;
	class IHWEnvMemory;

	class IHWEnvMemory
	{
	public:
		explicit IHWEnvMemory(HWEnv& env) : m_env(env){}
		PPU& GetPPU();
	private:
		HWEnv& m_env;
	};

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

		operator IHWEnvMemory&() {return m_ihwEnvMemory; }
	private:
		CPU m_cpu{};
		Memory m_memory{};
		PPU m_ppu{};

		// interface
		IHWEnvMemory m_ihwEnvMemory{*this};
	};
}
