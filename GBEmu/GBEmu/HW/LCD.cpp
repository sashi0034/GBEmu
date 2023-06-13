﻿#include "stdafx.h"
#include "LCD.h"

#include "Memory.h"
#include "MemoryAddress.h"

namespace GBEmu::HW
{
	using namespace MemoryAddress;

	bool LCD::IsLCDDisplayEnable() const
	{
		return lcdc() & (1 << 7);
	}

	uint16 LCD::WindowTileMapDisplayAddress() const
	{
		return lcdc() & (1 << 6) ? TimeMap1Start_0x9C00 : TimeMap0Start_0x9800;
	}

	bool LCD::IsWindowDisplayEnable() const
	{
		return lcdc() & (1 << 5);
	}

	uint16 LCD::BGAndWindowTileDataAddress() const
	{
		return lcdc() & (1 << 4) ? BGAndWindowTileData1Start_0x8000 : BGAndWindowTileData0Start_0x8800;
	}

	uint16 LCD::BGTimeMapDisplayAddress() const
	{
		return lcdc() & (1 << 3) ? BGTimeMapDisplay1Start_0x9C00 : BGTimeMapDisplay0Start_0x9800;
	}

	uint16 LCD::OBJHeight() const
	{
		return lcdc() & (1 << 2) ? 16 : 8;
		// widthは常に8
	}

	bool LCD::IsOBJDisplayEnable() const
	{
		return lcdc() & (1 << 1);
	}

	bool LCD::IsBGAndWindowEnable() const
	{
		return lcdc() & (1 << 0);
	}

	bool LCD::IsLYCoincidenceInterruptEnable() const
	{
		return stat() & (1 << 6);
	}

	bool LCD::IsOAMInterruptEnable() const
	{
		return stat() & (1 << 5);
	}

	bool LCD::IsVBlankInterruptEnable() const
	{
		return stat() & (1 << 4);
	}

	bool LCD::IsHBlankInterruptEnable() const
	{
		return stat() & (1 << 3);
	}

	bool LCD::LYCoincidenceFlag() const
	{
		return stat() & (1 << 2);
	}

	void LCD::WriteMode(PPUMode mode)
	{
		m_memoryRef.Write(
			STAT_0xFF41,
			(stat() & ~0b11) | (IsLCDDisplayEnable() ? static_cast<uint8>(mode) : 0));
	}

	uint8 LCD::lcdc() const
	{
		return m_memoryRef.Read(LCDC_0xFF40);
	}

	uint8 LCD::stat() const
	{
		return m_memoryRef.Read(STAT_0xFF41);
	}
}
