#pragma once

namespace GBEmu::HW
{
	namespace MemoryAddress
	{
		enum : uint8 {
			RomBank00Start = 0x0000,
			RomBank00End = 0x3fff,

			RomBankNNStart = 0x4000,
			RomBankNNEnd = 0x7fff,

			ExternalRamStart = 0xa000,
			ExternalRamEnd = 0xbfff,

			WorkRamBank0Start = 0xC000,
			WorkRamBank0End = 0xCFFF,

			WorkRamBank1Start = 0xD000,
			WorkRamBank1End = 0xDFFF,

			EchoWorkRamStart = 0xE000,
			EchoWorkRamEnd = 0xFDFF,

			IOPortsStart = 0xFF00,
			IOPortsEnd = 0xFF7F,
		};
	};
}
