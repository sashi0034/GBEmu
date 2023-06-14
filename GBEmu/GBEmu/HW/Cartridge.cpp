#include "stdafx.h"
#include "Cartridge.h"

#include "GBEmu/ConstParam.h"


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

		assert(false);
		return std::make_unique<MBC>();
	}


	Cartridge::Cartridge(const CartridgeHeader& header, const Array<uint8>& data) :
		   m_header(std::move(header)),
		   m_rom(std::move(data))
	{
		m_ram.resize(m_header.RamSizeKB * ConstParam::KiB);

		m_mbc = getMBC(m_header);
	}

	uint8 Cartridge::Read(uint16 addr)
	{
		return m_mbc->Read(*this, addr);
	}

	void Cartridge::Write(uint16 addr, uint16 data)
	{
		m_mbc->Write(*this, addr, data);
	}
}
