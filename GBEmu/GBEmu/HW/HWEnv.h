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
	class HWEnv : Uncopyable
	{
	public:
		CPU& GetCPU() { return m_cpu; }
		Memory& GetMemory() { return m_memory; }
		PPU& GetPPU() { return m_ppu; }
		APU& GetAPU() { return m_apu; }
		Joypad& GetJoypad() { return m_joypad; }
		Timer& GetTimer() { return m_timer; }
		HWDebugger& Debugger() { return m_debugger; }

		const CPU& GetCPU() const { return m_cpu; }
		const Memory& GetMemory() const { return m_memory; }
		const PPU& GetPPU() const { return m_ppu; }
		const APU& GetAPU() const { return m_apu; }
		const Joypad& GetJoypad() const { return m_joypad; }
		const Timer& GetTimer() const { return m_timer; }
		const HWDebugger& Debugger() const { return m_debugger; }

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
