#pragma once
#include "Cartridge.h"
#include "LCD.h"
#include "VRAM.h"

namespace GBEmu::HW
{
	class HWEnv;

	class Memory
	{
	public:
		Memory();

		LCD& GetLCD() {return m_lcd; }
		VRAM& GetVRAM() {return m_vram; }

		uint8 Read(uint16 addr);
		uint16 Read16(uint16 addr);

		void Write(HWEnv& env, uint16 addr, uint8 data);
		void Write16(HWEnv& env, uint16 addr, uint16 data16);

		void WriteDirect(uint16 addr, uint8 data);
		void WriteDirect16(uint16 addr, uint16 data16);

		void LoadCartridge(const FilePath& cartridgePath);

		static constexpr int MemorySize_0x10000 = 0x10000;

	private:
		Cartridge m_cartridge{};

		LCD m_lcd;

		std::array<uint8, MemorySize_0x10000> m_memory{};
		VRAM m_vram{};

		void writeIO(HWEnv& env, uint16 addr, uint8 data);

		void initMemory();

		static CartridgeHeader loadCartridgeHeader(const Array<uint8>& cartridgeData);
	};
}
