#pragma once
#include "Cartridge.h"

namespace GBEmu::HW
{
	class Memory
	{
	public:
		Memory();

		void LoadCartridge(FilePath cartridgePath);

		static constexpr int MemorySize = 0x10000;

	private:
		Array<uint8> m_cartridgeData{};
		Array<uint8> m_memory{};

		CartridgeHeader m_cartridgeHeader{};

		static CartridgeHeader loadCartridgeHeader(const Array<uint8>& cartridgeData);
	};
}
