﻿#pragma once

namespace GBEmu::HW
{
	namespace MemoryAddress
	{
		enum : uint16
		{
			RomBank00Start_0x0000 = 0x0000,
			RomBank00End_0x3FFF = 0x3fff,

			InterruptVBlank_0x0040 = 0x0040,
			InterruptSTAT_0x0048 = 0x0048,
			InterruptTimer_0x0050 = 0x0050,
			InterruptSerial_0x0058 = 0x0058,
			InterruptJoypad_0x0060 = 0x0060,

			RomBankNNStart_0x4000 = 0x4000,
			RomBankNNEnd_0x7FFF = 0x7fff,

			ExternalRamStart_0xA000 = 0xa000,
			ExternalRamEnd_0xBFFF = 0xbfff,

			WorkRamBank0Start_0xC000 = 0xC000,
			WorkRamBank0End = 0xCFFF,

			WorkRamBank1Start = 0xD000,
			WorkRamBank1End_0xDFFF = 0xDFFF,

			VRamStart_0x8000 = 0x8000,
			VRamEnd_0x9FFF = 0x9FFF,

			TileDataTableStart_0x8000 = 0x8000,

			BGAndWindowTileData0Start_0x8800 = 0x8800,
			BGAndWindowTileData0End_0x87FF = 0x87FF,

			BGAndWindowTileData1Start_0x8000 = 0x8000,
			BGAndWindowTileData1End_0x8FFF = 0x8FFF,

			BGAndWindowTileData2Start_0x9000 = 0x9000,
			BGAndWindowTileData2End_0x97FF = 0x97FF,

			BGTimeMapDisplay0Start_0x9800 = 0x9800,
			BGTimeMapDisplay0End_0x9BFF = 0x9BFF,

			BGTimeMapDisplay1Start_0x9C00 = 0x9C00,
			BGTimeMapDisplay1End_0x9FFF = 0x9FFF,

			TimeMap0Start_0x9800 = 0x9800,
			TimeMap0End_0x9BFF = 0x9BFF,

			TimeMap1Start_0x9C00 = 0x9C00,
			TimeMap1End_0x9FFF = 0x9FFF,

			EchoWorkRamStart_0xE000 = 0xE000,
			EchoWorkRamEnd_0xFDFF = 0xFDFF,

			OAMStart_0xFE00 = 0xFE00,
			OAMEnd_0xFE9F = 0xFE9F,

			IOPortsStart_0xFF00 = 0xFF00,
			IOPortsEnd_0xFF7F = 0xFF7F,

			JOYP_0xFF00 = 0xFF00,

			DIV_0xFF04 = 0xFF04,
			TIMA_0xFF05 = 0xFF05,
			TMA_0xFF06 = 0xFF06,
			TAC_0xFF07 = 0xFF07,

			NR10_0xFF10 = 0xFF10,
			NR11_0xFF11 = 0xFF11,
			NR12_0xFF12 = 0xFF12,
			NR13_0xFF13 = 0xFF13,
			NR14_0xFF14 = 0xFF14,
			NR21_0xFF16 = 0xFF16,
			NR22_0xFF17 = 0xFF17,
			NR23_0xFF18 = 0xFF18,
			NR24_0xFF19 = 0xFF19,
			NR30_0xFF1A = 0xFF1A,
			NR31_0xFF1B = 0xFF1B,
			NR32_0xFF1C = 0xFF1C,
			NR33_0xFF1D = 0xFF1D,
			NR34_0xFF1E = 0xFF1E,
			NR41_0xFF20 = 0xFF20,
			NR42_0xFF21 = 0xFF21,
			NR43_0xFF22 = 0xFF22,
			NR44_0xFF23 = 0xFF23,
			NR50_0xFF24 = 0xFF24,
			NR51_0xFF25 = 0xFF25,
			NR52_0xFF26 = 0xFF26,

			WaveRamStart_0xFF30 = 0xFF30,
			WaveRamEnd_0xFF3F = 0xFF3F,

			LCDC_0xFF40 = 0xFF40,
			STAT_0xFF41 = 0xFF41,

			SCY_0xFF42 = 0xFF42,
			SCX_0xFF43 = 0xFF43,

			LY_0xFF44 = 0xFF44,
			LYC_0xFF45 = 0xFF45,

			DMA_0xFF46 = 0xFF46,

			WY_0xFF4A = 0xFF4A,
			WX_0xFF4B = 0xFF4B,

			BGP_0xFF47 = 0xFF47,
			OBP0_0xFF48 = 0xFF48,
			OBP1_0xFF49 = 0xFF49,

			IF_0xFF0F = 0xFF0F, // IF: Interrupt Flag
			IE_0xFFFF = 0xFFFF, // IE: Interrupt Enable
		};
	};
}
