#pragma once

namespace GBEmu::HW
{
	class Memory;

	enum class PPUMode;

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

		// STAT
		bool IsLYCoincidenceInterruptEnable() const;
		bool IsOAMInterruptEnable() const;
		bool IsVBlankInterruptEnable() const;
		bool IsHBlankInterruptEnable() const;
		bool LYCoincidenceFlag() const;
		void WriteMode(PPUMode mode);
	private:
		uint8 lcdc() const;
		uint8 stat() const;
		Memory& m_memoryRef;
	};
}
