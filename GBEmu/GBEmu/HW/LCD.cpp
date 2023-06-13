#include "stdafx.h"
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

	uint8 LCD::lcdc() const
	{
		return m_memoryRef.Read(LCDC_0xFF40);
	}
}
