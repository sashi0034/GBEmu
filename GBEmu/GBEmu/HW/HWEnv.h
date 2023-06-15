#pragma once

#include "CPU.h"
#include "HWDebugger.h"
#include "Joypad.h"
#include "Memory.h"
#include "PPU.h"
#include "Timer.h"

namespace GBEmu::HW
{
	class CPU;
	class Memory;
	class PPU;
	class IHWEnvMemory;
	class Joypad;
	class Timer;

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
		Joypad& GetJoypad()
		{
			return m_joypad;
		}
		Timer& GetTimer()
		{
			return m_timer;
		}

		operator IHWEnvMemory&() {return m_ihwEnvMemory; }
	private:
		CPU m_cpu{};
		Memory m_memory{};
		PPU m_ppu{};
		Joypad m_joypad{};
		Timer m_timer{};

		// interface
		IHWEnvMemory m_ihwEnvMemory{*this};
	};

	class HWEnvHandler
	{
	public:
		HWEnv& operator()() {return m_env; }
		HWEnv& Env() {return m_env; }
		HWDebugger& Debugger() {return m_debugger; }
	private:
		HWEnv m_env;
		HWDebugger m_debugger;
	};
}
