#pragma once

namespace GBEmu::HW
{
	class Cartridge;
	struct CartridgeHeader;

	// Memory Bank Controller
	class MBC
	{
	public:
		virtual ~MBC() = default;
		virtual uint8 Read(Cartridge& cartridge, uint16 addr) { return 0; };
		virtual void Write(Cartridge& cartridge, uint16 addr, uint8 data) { return; };
		virtual String DebugProfile(const CartridgeHeader& cartridge);
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
		~MBC1() override = default;
		uint8 Read(Cartridge& cartridge, uint16 addr) override;
		void Write(Cartridge& cartridge, uint16 addr, uint8 data) override;
		String DebugProfile(const CartridgeHeader& cartridge) override;

	private:
		uint16 m_romBankIndex = 1;
		uint16 m_secondBankIndex{};
		uint8 m_bankMode{};
		bool m_ramEnableFlag{};

		int externalRamAddress(uint16 addr) const;
	};
}
