#include "stdafx.h"
#include "Cartridge.h"

#include "HWLogger.h"
#include "GBEmu/ConstParam.h"
#include "GBEmu/Util/Utils.h"


namespace GBEmu::HW
{
	std::unique_ptr<MBC> getMBC(const CartridgeHeader& header)
	{
		const auto type = header.Type;

		using ct = CartridgeType;

		if (type == ct::RomOnly)
		{
			return std::make_unique<MBCNone>();
		}
		if (type == ct::MBC1 || type == ct::MBC1Ram || type == ct::MBC1RamBattery)
		{
			return std::make_unique<MBC1>();
		}

		HWLogger::Error(U"adaptable MBC not implemented: " + Util::StringifyEnum(type));
		return std::make_unique<MBCNone>();
	}


	Cartridge::Cartridge(const CartridgeHeader& header, const Array<uint8>& data) :
		m_header(std::move(header)),
		m_rom(std::move(data))
	{
		m_headerSummary = U"{} | {}"_fmt(header.Title, Util::StringifyEnum(header.Type));

		m_ram.resize(m_header.RamSizeKB * ConstParam::KiB);

		m_mbc = getMBC(m_header);
	}

	uint8 Cartridge::Read(uint16 addr)
	{
		return m_mbc->Read(*this, addr);
	}

	void Cartridge::Write(uint16 addr, uint8 data)
	{
		m_mbc->Write(*this, addr, data);
	}

	String Cartridge::DebugProfile() const
	{
		String str = m_headerSummary;

		const auto&& mbc = m_mbc->DebugProfile(m_header);
		if (mbc.isEmpty() == false)
		{
			str += U"\n" + mbc;
		}

		return str;
	}
}
