#pragma once
#include "MemoryAddress.h"
#include "GBEmu/Util/Utils.h"

namespace GBEmu::HW
{
	class Memory;
	class HWEnv;

	enum class PPUMode : uint8;

	class LCD
	{
	public:
		LCD() = default;

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

		uint8 LCDC() const;
		uint8 STAT() const;

		template <uint16 addr>
		uint8 ReadAddr() const;
		template <uint16 addr>
		void WriteAddr(uint8 data);

	private:
		uint8 m_lcdc{};
		uint8 m_stat{};
		uint8 m_bgp{};
		uint8 m_obp0{};
		uint8 m_obp1{};
		uint8 m_scx{};
		uint8 m_scy{};
		uint8 m_ly{};
		uint8 m_lyc{};
		uint8 m_wx{};
		uint8 m_wy{};
	};

	template <uint16 addr>
	uint8 LCD::ReadAddr() const
	{
		using namespace MemoryAddress;

		if constexpr (addr == LCDC_0xFF40) return m_lcdc;
		else if constexpr (addr == STAT_0xFF41) return m_stat;
		else if constexpr (addr == BGP_0xFF47) return m_bgp;
		else if constexpr (addr == OBP0_0xFF48) return m_obp0;
		else if constexpr (addr == OBP1_0xFF49) return m_obp1;
		else if constexpr (addr == SCX_0xFF43) return m_scx;
		else if constexpr (addr == SCY_0xFF42) return m_scy;
		else if constexpr (addr == LY_0xFF44) return m_ly;
		else if constexpr (addr == LYC_0xFF45) return m_lyc;
		else if constexpr (addr == WX_0xFF4B) return m_wx;
		else if constexpr (addr == WY_0xFF4A) return m_wy;
		else static_assert(Util::AlwaysFalseValue<addr>);
	}

	template <uint16 addr>
	void LCD::WriteAddr(uint8 data)
	{
		using namespace MemoryAddress;

		if constexpr (addr == LCDC_0xFF40) m_lcdc = data;
		else if constexpr (addr == STAT_0xFF41) m_stat = data;
		else if constexpr (addr == BGP_0xFF47) m_bgp = data;
		else if constexpr (addr == OBP0_0xFF48) m_obp0 = data;
		else if constexpr (addr == OBP1_0xFF49) m_obp1 = data;
		else if constexpr (addr == SCX_0xFF43) m_scx = data;
		else if constexpr (addr == SCY_0xFF42) m_scy = data;
		else if constexpr (addr == LY_0xFF44) m_ly = data;
		else if constexpr (addr == LYC_0xFF45) m_lyc = data;
		else if constexpr (addr == WX_0xFF4B) m_wx = data;
		else if constexpr (addr == WY_0xFF4A) m_wy = data;
		else static_assert(Util::AlwaysFalseValue<addr>);
	}
}
