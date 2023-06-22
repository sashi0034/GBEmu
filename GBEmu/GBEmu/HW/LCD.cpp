#include "stdafx.h"
#include "LCD.h"

#include "MemoryAddress.h"

namespace GBEmu::HW
{
	using namespace MemoryAddress;

	LCD::LCD(
		uint8* lcdcPtr,
		uint8* statPtr,
		uint8* bgpPtr,
		uint8* obp0Ptr,
		uint8* obp1Ptr,
		uint8* scxPtr,
		uint8* scyPtr,
		uint8* lyPtr,
		uint8* lycPtr,
		uint8* wxPtr,
		uint8* wyPtr) :
			m_lcdcPtr(lcdcPtr),
			m_statPtr(statPtr),
			m_bgpPtr(bgpPtr),
			m_obp0Ptr(obp0Ptr),
			m_obp1Ptr(obp1Ptr),
			m_scxPtr(scxPtr),
			m_scyPtr(scyPtr),
			m_lyPtr(lyPtr),
			m_lycPtr(lycPtr),
			m_wxPtr(wxPtr),
			m_wyPtr(wyPtr)
	{}

	bool LCD::IsLCDDisplayEnable() const
	{
		return LCDC() & (1 << 7);
	}

	uint16 LCD::WindowTileMapDisplayAddress() const
	{
		return LCDC() & (1 << 6) ? TimeMap1Start_0x9C00 : TimeMap0Start_0x9800;
	}

	bool LCD::IsWindowDisplayEnable() const
	{
		return LCDC() & (1 << 5);
	}

	uint16 LCD::BGAndWindowTileDataAddress() const
	{
		return LCDC() & (1 << 4) ? BGAndWindowTileData1Start_0x8000 : BGAndWindowTileData2Start_0x9000;
	}

	uint16 LCD::BGTileMapDisplayAddress() const
	{
		return LCDC() & (1 << 3) ? BGTimeMapDisplay1Start_0x9C00 : BGTimeMapDisplay0Start_0x9800;
	}

	uint16 LCD::OBJHeight() const
	{
		return LCDC() & (1 << 2) ? 16 : 8;
		// widthは常に8
	}

	bool LCD::IsOBJDisplayEnable() const
	{
		return LCDC() & (1 << 1);
	}

	bool LCD::IsBGAndWindowEnable() const
	{
		return LCDC() & (1 << 0);
	}

	bool LCD::IsLYCoincidenceInterruptEnable() const
	{
		return STAT() & (1 << 6);
	}

	bool LCD::IsOAMInterruptEnable() const
	{
		return STAT() & (1 << 5);
	}

	bool LCD::IsVBlankInterruptEnable() const
	{
		return STAT() & (1 << 4);
	}

	bool LCD::IsHBlankInterruptEnable() const
	{
		return STAT() & (1 << 3);
	}

	bool LCD::LYCoincidenceFlag() const
	{
		return STAT() & (1 << 2);
	}

	void LCD::UpdateLYCoincidenceFlag()
	{
		*m_statPtr = 0x80 | (STAT() & ~0b100) | (LY() == LYC() ? 0b100 : 0);
	}

	void LCD::SetMode(PPUMode mode)
	{
		*m_statPtr = 0x80 | (STAT() & ~0b11) | (IsLCDDisplayEnable() ? static_cast<uint8>(mode) : 0);
	}

	uint8 LCD::SCY() const
	{
		return *m_scyPtr;
	}

	uint8 LCD::SCX() const
	{
		return *m_scxPtr;
	}

	uint8 LCD::LY() const
	{
		return *m_lyPtr;
	}

	void LCD::SetLY(uint8 ly)
	{
		*m_lyPtr = ly;
	}

	uint8 LCD::LYC() const
	{
		return *m_lycPtr;
	}

	uint8 LCD::WX() const
	{
		return *m_wxPtr;
	}

	uint8 LCD::WY() const
	{
		return *m_wyPtr;
	}

	uint8 LCD::BGPaletteData(uint8 colorNumber) const
	{
		const uint8 bgp = *m_bgpPtr;
		return (bgp >> (colorNumber * 2)) & 0b11;
	}

	uint8 LCD::ObjectPaletteData(bool isUseOBP1, uint8 colorNumber) const
	{
		const uint8 obp = isUseOBP1 ? *m_obp1Ptr : *m_obp0Ptr;
		return (obp >> (colorNumber * 2)) & 0b11;
	}

	uint8 LCD::LCDC() const
	{
		return *m_lcdcPtr;
	}

	uint8 LCD::STAT() const
	{
		return *m_statPtr;
	}
}
