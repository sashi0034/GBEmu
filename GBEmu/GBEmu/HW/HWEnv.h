#pragma once

#include "CPU.h"
#include "HWDebugger.h"
#include "Joypad.h"
#include "Memory.h"
#include "PPU.h"
#include "APU.h"
#include "Timer.h"

namespace GBEmu::HW
{
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
		APU& GetAPU()
		{
			return m_apu;
		}
		Joypad& GetJoypad()
		{
			return m_joypad;
		}
		Timer& GetTimer()
		{
			return m_timer;
		}
		HWDebugger& Debugger()
		{
			return m_debugger;
		}
	private:
		CPU m_cpu{};
		Memory m_memory{};
		PPU m_ppu{};
		APU m_apu{};
		Joypad m_joypad{};
		Timer m_timer{};

		HWDebugger m_debugger;
	};
}
