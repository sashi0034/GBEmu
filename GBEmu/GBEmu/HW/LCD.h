#pragma once

namespace GBEmu::HW
{
	class Memory;
	class HWEnv;

	enum class PPUMode : uint8;

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
		void UpdateLYCoincidenceFlag(HWEnv& env);
		void SetMode(HWEnv& env, PPUMode mode);

		// position and scrolling
		uint8 SY() const;
		uint8 SX() const;
		uint8 LY() const;
		void SetLY(HWEnv& env, uint8 ly);
		uint8 LYC() const;
		uint8 WX() const;
		uint8 WY() const;

		uint8 BGPaletteData(uint8 colorNumber) const;
		uint8 ObjectPaletteData(bool isUseOBP1, uint8 colorNumber) const;
	private:
		uint8 lcdc() const;
		uint8 stat() const;
		Memory& m_memoryRef;
	};
}
