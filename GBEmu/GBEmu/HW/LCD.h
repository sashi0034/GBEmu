#pragma once

namespace GBEmu::HW
{
	class Memory;

	class LCD
	{
	public:
		explicit LCD(Memory& memoryRef) : m_memoryRef(memoryRef){}

		// LCDC
		bool IsLCDDisplayEnable() const;
		uint16 WindowTileMapDisplayAddress() const;
		bool IsWindowDisplayEnable() const;
		uint16 BGAndWindowTileDataAddress() const;
		uint16 BGTimeMapDisplayAddress() const;
		uint16 OBJHeight() const;
		bool IsOBJDisplayEnable() const;
		bool IsBGAndWindowEnable() const;

	private:
		uint8 lcdc() const;
		Memory& m_memoryRef;
	};
}
