#pragma once

namespace GBEmu::HW
{
	class Memory;
	class HWEnv;

	enum class PPUMode : uint8;

	class LCD
	{
	public:
		LCD() = default;
		LCD(
			uint8* lcdcPtr,
			uint8* statPtr,
			uint8* bg0Ptr,
			uint8* obp0Ptr,
			uint8* obp1Ptr,
			uint8* scxPtr,
			uint8* scyPtr,
			uint8* lyPtr,
			uint8* lycPtr,
			uint8* wxPtr,
			uint8* wyPtr);
		// LCDC
		bool IsLCDDisplayEnable() const;
		uint16 WindowTileMapDisplayAddress() const;
		bool IsWindowDisplayEnable() const;
		uint16 BGAndWindowTileDataAddress() const;
		uint16 BGTileMapDisplayAddress() const;
		uint16 OBJHeight() const;
		bool IsOBJDisplayEnable() const;
		bool IsBGAndWindowEnable() const;

		// STAT
		bool IsLYCoincidenceInterruptEnable() const;
		bool IsOAMInterruptEnable() const;
		bool IsVBlankInterruptEnable() const;
		bool IsHBlankInterruptEnable() const;
		bool LYCoincidenceFlag() const;
		void UpdateLYCoincidenceFlag();
		void SetMode(PPUMode mode);

		// position and scrolling
		uint8 SCY() const;
		uint8 SCX() const;
		uint8 LY() const;
		void SetLY(uint8 ly);
		uint8 LYC() const;
		uint8 WX() const;
		uint8 WY() const;

		uint8 BGPaletteData(uint8 colorNumber) const;
		uint8 ObjectPaletteData(bool isUseOBP1, uint8 colorNumber) const;
	private:
		// TODO: メモリ参照をしない
		uint8 lcdc() const;
		uint8 stat() const;

		uint8* m_lcdcPtr;
		uint8* m_statPtr;
		uint8* m_bg0Ptr;
		uint8* m_obp0Ptr;
		uint8* m_obp1Ptr;
		uint8* m_scxPtr;
		uint8* m_scyPtr;
		uint8* m_lyPtr;
		uint8* m_lycPtr;
		uint8* m_wxPtr;
		uint8* m_wyPtr;
	};
}
