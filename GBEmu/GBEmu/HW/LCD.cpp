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

	void LCD::UpdateLYCoincidenceFlag(HWEnv& env)
	{
		m_memoryRef.Write(env, STAT_0xFF41,
			0x80 | (stat() & ~0b100) | (LY() == LYC() ? 0b100 : 0));
	}

	void LCD::SetMode(HWEnv& env, PPUMode mode)
	{
		m_memoryRef.Write(env, STAT_0xFF41,
			0x80 | (stat() & ~0b11) | (IsLCDDisplayEnable() ? static_cast<uint8>(mode) : 0));
	}

	uint8 LCD::SY() const
	{
		return m_memoryRef.Read(0xFF42);
	}

	uint8 LCD::SX() const
	{
		return m_memoryRef.Read(0xFF43);
	}

	uint8 LCD::LY() const
	{
		return m_memoryRef.Read(LY_0xFF44);
	}

	void LCD::SetLY(HWEnv& env, uint8 ly)
	{
		m_memoryRef.Write(env, LY_0xFF44, ly);
	}

	uint8 LCD::LYC() const
	{
		return m_memoryRef.Read(0xFF45);
	}

	uint8 LCD::WX() const
	{
		return m_memoryRef.Read(0xFF4B);
	}

	uint8 LCD::WY() const
	{
		return m_memoryRef.Read(0xFF4A);
	}

	uint8 LCD::BGPaletteData(uint8 colorNumber) const
	{
		const uint8 bgp = m_memoryRef.Read(0xFF47);
		return (bgp >> (colorNumber * 2)) & 0b11;
	}

	uint8 LCD::ObjectPaletteData(bool isUseOBP1, uint8 colorNumber) const
	{
		const uint8 obp = m_memoryRef.Read(isUseOBP1 ? 0xFF49 : 0xFF48);
		return (obp >> (colorNumber * 2)) & 0b11;
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
