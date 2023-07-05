#pragma once
#include "MBC.h"

namespace GBEmu::HW
{
	enum class CartridgeType : uint8 {
		RomOnly = 0x00,
		MBC1 = 0x01,
		MBC1Ram = 0x02,
		MBC1RamBattery = 0x03,
		MBC2 = 0x05,
		MBC2Battery = 0x06,
		RomRam = 0x08,
		RomRamBattery = 0x09,
		MMM01 = 0x0B,
		MMM01Ram = 0x0C,
		MMM01RamBattery = 0x0D,
		MBC3TimerBattery = 0x0F,
		MBC3TimerRamBattery = 0x10,
		MBC3 = 0x11,
		MBC3Ram = 0x12,
		MBC3RamBattery = 0x13,
		MBC4 = 0x15,
		MBC4Ram = 0x16,
		MBC4RamBattery = 0x17,
		MBC5 = 0x19,
		MBC5Ram = 0x1A,
		MBC5RamBattery = 0x1B,
		MBC5Rumble = 0x1C,
		MBC5RumbleRam = 0x1D,
		MBC5RumbleRamBattery = 0x1E,
		PocketCamera = 0xFC,
		BandaiTama5 = 0xFD,
		HuC3 = 0xFE,
		HuC1RamBattery = 0xFF
	};

	namespace CartridgeAddress
	{
		enum : uint16{
			TitleStart = 0x0134,
			TitleEnd = 0x0143,
			CartridgeType = 0x147,
			RomSize = 0x148,
			RamSize = 0x149,
		};
	};

	struct CartridgeHeader
	{
		String Title;
		CartridgeType Type;
		int RomSizeKB;
		int RamSizeKB;
	};

	class Cartridge
	{
	public:
		Cartridge() = default;
		Cartridge(const CartridgeHeader& header, const Array<uint8>& data);

		CartridgeHeader& Header(){return m_header;}
		const Array<uint8>& ROM(){return m_rom;}
		Array<uint8>& RAM(){return m_ram;}
		uint8 Read(uint16 addr);
		void Write(uint16 addr, uint16 data);
	private:
		CartridgeHeader m_header{};
		Array<uint8> m_rom{};
		Array<uint8> m_ram{};
		std::unique_ptr<MBC> m_mbc{};
	};
}
