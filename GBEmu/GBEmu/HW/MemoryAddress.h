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
		};
	};
}
