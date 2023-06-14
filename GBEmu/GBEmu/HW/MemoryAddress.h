#pragma once

namespace GBEmu::HW
{
	namespace MemoryAddress
	{
		enum : uint16 {
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

			TileDataTableStart_0x8000 = 0x8000,

			BGAndWindowTileData0Start_0x8800 = 0x8800,
			BGAndWindowTileData0End_0x97FF = 0x97FF,

			BGAndWindowTileData1Start_0x8000 = 0x8000,
			BGAndWindowTileData1End_0x8FFF = 0x8FFF,

			BGTimeMapDisplay0Start_0x9800 = 0x9800,
			BGTimeMapDisplay0End_0x9BFF = 0x9BFF,

			BGTimeMapDisplay1Start_0x9C00 = 0x9C00,
			BGTimeMapDisplay1End_0x9FFF = 0x9FFF,

			TimeMap0Start_0x9800 = 0x9800,
			TimeMap0End_0x9BFF = 0x9BFF,

			TimeMap1Start_0x9C00 = 0x9C00,
			TimeMap1End_0x9FFF = 0x9FFF,

			EchoWorkRamStart = 0xE000,
			EchoWorkRamEnd = 0xFDFF,

			OAMStart_0xFE00 = 0xFE00,
			OAMEnd_0xFE9F = 0xFE9F,

			IOPortsStart = 0xFF00,
			IOPortsEnd = 0xFF7F,

			DIV_0xFF04 = 0xFF04,
			TIMA_0xFF05 = 0xFF05,
			TMA_0xFF06 = 0xFF06,
			TAC_0xFF07 = 0xFF07,

			LCDC_0xFF40 = 0xFF40,
			STAT_0xFF41 = 0xFF41,

			LY_0xFF44 = 0xFF44,

			IF_0xFF0F = 0xFF0F, // IF: Interrupt Flag
			IE_0xFFFF = 0xFFFF, // IE: Interrupt Enable
		};
	};
}
