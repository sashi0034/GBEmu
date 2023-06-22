#pragma once

namespace GBEmu::HW
{
	class Cartridge;

	// Memory Bank Controller
	class MBC
	{
	public:
		virtual ~MBC() = default;
		virtual uint8 Read(Cartridge& cartridge, uint16 addr){return 0;};
		virtual void Write(Cartridge& cartridge, uint16 addr, uint8 data){};
	};

	class MBCNone : public MBC
	{
	public:
		uint8 Read(Cartridge& cartridge, uint16 addr) override;
		void Write(Cartridge& cartridge, uint16 addr, uint8 data) override;
	};

	class MBC1 : public MBC
	{
	public:
		MBC1() = default;
		int externalRamAddress(uint16 addr) const;
		uint8 Read(Cartridge& cartridge, uint16 addr) override;
		void Write(Cartridge& cartridge, uint16 addr, uint8 data) override;

	private:
		uint16 m_romBankIndex = 1;
		uint16 m_secondBankIndex{};
		uint8 m_bankMode{};
		bool m_ramEnableFrag{};

		uint16 romBankIndexExtended(Cartridge& cartridge) const;
	};
}
