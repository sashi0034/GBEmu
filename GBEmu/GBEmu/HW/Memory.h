#pragma once
#include "Cartridge.h"
#include "LCD.h"

namespace GBEmu::HW
{
	class Memory
	{
	public:
		Memory();

		LCD& GetLCD();

		uint8 Read(uint16 addr);
		uint16 Read16(uint16 addr);

		void Write(uint16 addr, uint8 data);
		void Write16(uint16 addr, uint16 data16);

		void WriteDirect(uint16 addr, uint8 data);

		void LoadCartridge(const FilePath& cartridgePath);

		static constexpr int MemorySize = 0x10000;

	private:
		Cartridge m_cartridge{};

		LCD m_lcd;

		Array<uint8> m_memory{};

		static CartridgeHeader loadCartridgeHeader(const Array<uint8>& cartridgeData);
	};
}
