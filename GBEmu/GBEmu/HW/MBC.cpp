#include "stdafx.h"
#include "MBC.h"

#include "Cartridge.h"
#include "MemoryAddress.h"
#include "HWLogger.h"
#include "GBEmu/Util/Range.h"

namespace GBEmu::HW
{
	using namespace MemoryAddress;

	constexpr int romBankSizeKiB_16 = 16;
	constexpr int ramBankSizeKiB_8 = 8;
	constexpr int romBankSize_0x4000 = 0x4000; // 16 * 1024
	constexpr int ramBankSize_0x2000 = 0x2000;

	constexpr int bigRomSizeKiB_1024 = 1024;

	String MBC::DebugProfile(const CartridgeHeader& cartridge)
	{
		return U"ROM BANK: {:02X}\nRAM BANK: {:02X}"_fmt(
			cartridge.RomSizeKB / romBankSizeKiB_16, cartridge.RamSizeKB / ramBankSizeKiB_8);
	}

	uint8 MBCNone::Read(const Cartridge& cartridge, uint16 addr) const
	{
		return cartridge.ROM()[addr];
	}

	void MBCNone::Write(Cartridge& cartridge, uint16 addr, uint8 data)
	{
		// RomOnlyのときは書き込み不可
		HWLogger::Error(U"invalid write at {:04X} in MBC0"_fmt(addr));
	}

	uint8 MBC1::Read(const Cartridge& cartridge, uint16 addr) const
	{
		if (RangeUint16(RomBank00Start_0x0000, RomBank00End_0x3FFF).IsBetween(addr))
		{
			const uint16 offset = addr - RomBank00Start_0x0000;
			if (m_bankMode == 1)
			{
				return cartridge.Header().RomSizeKB < bigRomSizeKiB_1024
				       ? cartridge.ROM()[offset]
				       // 大容量ROMでは、モード1で$20, $40, $60にアクセス可能
				       : cartridge.ROM()[(m_secondBankIndex << 5) * romBankSize_0x4000 + offset];
			}
			else
			{
				return cartridge.ROM()[offset];
			}
		}
		else if (RangeUint16(RomBankNNStart_0x4000, RomBankNNEnd_0x7FFF).IsBetween(addr))
		{
			const uint16 offset = addr - RomBankNNStart_0x4000;
			const int bankIndex = cartridge.Header().RomSizeKB < bigRomSizeKiB_1024
			                      ? m_romBankIndex
			                      // 大容量ROMでは、5ビット目からRAMバンクバン番号の影響を受ける
			                      : (m_secondBankIndex << 5) | m_romBankIndex;
			return cartridge.ROM()[bankIndex * romBankSize_0x4000 + offset];
		}
		else if (RangeUint16(ExternalRamStart_0xA000, ExternalRamEnd_0xBFFF).IsBetween(addr))
		{
			return cartridge.RAM()[externalRamAddress(addr)];
		}

		HWLogger::Error(U"invalid read address in MBC1: {}"_fmt(addr));
		return 0;
	}

	void MBC1::Write(Cartridge& cartridge, uint16 addr, uint8 data)
	{
		if (RangeUint16(0x0000, 0x1fff).IsBetween(addr))
		{
			m_ramEnableFlag = (data & 0xF) == 0x0a;
		}
		else if (RangeUint16(0x2000, 0x3fff).IsBetween(addr))
		{
			if (data == 0x00 || data == 0x20 || data == 0x40 || data == 0x60)
			{
				m_romBankIndex = data + 1;
			}
			else
			{
				const auto romSizeKB = cartridge.Header().RomSizeKB;
				// @formatter:off
				const auto mask =
					romSizeKB == 32 ? 0b1 :
					romSizeKB == 64 ? 0b11 :
					romSizeKB == 128 ? 0b111 :
					romSizeKB == 256 ? 0b1111 :
					0b11111;
				// @formatter:on
				m_romBankIndex = data & mask;
			}
		}
		else if (RangeUint16(0x4000, 0x5fff).IsBetween(addr))
		{
			m_secondBankIndex = data & 0b11;
		}
		else if (RangeUint16(0x6000, 0x7fff).IsBetween(addr))
		{
			m_bankMode = data & 0b1;
		}
		else if (RangeUint16(ExternalRamStart_0xA000, ExternalRamEnd_0xBFFF).IsBetween(addr))
		{
			if (m_ramEnableFlag == false) return;

			cartridge.RAM()[externalRamAddress(addr)] = data;
		}
		else
		{
			HWLogger::Error(U"invalid write address in MBC1: {}"_fmt(addr));
		}
	}

	String MBC1::DebugProfile(const CartridgeHeader& cartridge)
	{
		return U"ROM BANK: {:02X} / {:02X}\nRAM BANK: {:02X} / {:02X}"_fmt(
			m_romBankIndex, cartridge.RomSizeKB / romBankSizeKiB_16,
			m_secondBankIndex, cartridge.RamSizeKB / ramBankSizeKiB_8);
	}

	int MBC1::externalRamAddress(uint16 addr) const
	{
		const uint16 offset = addr - ExternalRamStart_0xA000;
		return m_bankMode == 0
		       ? offset
		       : m_secondBankIndex * ramBankSize_0x2000 + offset;
	}
}
