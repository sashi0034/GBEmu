#pragma once
#include "Cartridge.h"
#include "Interruption.h"
#include "LCD.h"
#include "VRAM.h"

namespace GBEmu::HW
{
	class HWEnv;

	class Memory
	{
	public:
		Memory();
		VRAM& GetVRAM() {return m_vram; }

		LCD& GetLCD() {return m_lcd; }
		Interruption& Interrupt() {return m_interrupt; }

		uint8 Read(uint16 addr);
		uint16 Read16(uint16 addr);

		void Write(HWEnv& env, uint16 addr, uint8 data);
		void Write16(HWEnv& env, uint16 addr, uint16 data16);

		// TODO: WriteDirectを廃止して、IOPortクラスをつくる
		void WriteDirect(uint16 addr, uint8 data);

		void LoadCartridge(const FilePath& cartridgePath);

		void DumpIOPorts(String& dest) const;

		static constexpr int MemorySize_0x10000 = 0x10000;

	private:
		std::array<uint8, MemorySize_0x10000> m_memory{};
		VRAM m_vram{};
		Cartridge m_cartridge{};

		LCD m_lcd;
		Interruption m_interrupt;

		void writeIO(HWEnv& env, uint16 addr, uint8 data);

		void initMemory();

		static CartridgeHeader loadCartridgeHeader(const Array<uint8>& cartridgeData);
	};
}
