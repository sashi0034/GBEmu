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
		VRAM& GetVRAM() {return m_vram; }

		Interruption& Interrupt() {return m_interrupt; }
		const Cartridge& GetCartridge() {return m_cartridge; }

		uint8 Read(HWEnv& env, uint16 addr);
		uint16 Read16(HWEnv& env, uint16 addr);

		void Write(HWEnv& env, uint16 addr, uint8 data);
		void Write16(HWEnv& env, uint16 addr, uint16 data16);

		void LoadCartridge(const FilePath& cartridgePath);
		void Initialize(HWEnv& env);

		void DumpIOPort(HWEnv& env, String& dest) const;

		static constexpr int MemorySize_0x10000 = 0x10000;
	private:
		std::array<uint8, MemorySize_0x10000> m_memory{};
		VRAM m_vram{};
		Cartridge m_cartridge{};

		Interruption m_interrupt{};

		uint8 readIO(HWEnv& env, uint16 addr) const;
		void writeIO(HWEnv& env, uint16 addr, uint8 data);
		void transferDMA(HWEnv& env, uint8 data);

		static CartridgeHeader loadCartridgeHeader(const Array<uint8>& cartridgeData);
	};
}
