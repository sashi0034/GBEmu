#include "stdafx.h"
#include "MBC.h"

#include "Cartridge.h"
#include "MemoryAddress.h"
#include "GBEmu/Util/Range.h"

namespace GBEmu::HW
{
	using namespace MemoryAddress;

	constexpr int romBankSize = 16 * 1024; // 0x4000

	uint8 MBC1::Read(Cartridge& cartridge, uint16 addr)
	{
		if (Util::RangeUint16(RomBank00Start, RomBank00End).IsBetween(addr))
		{
			const uint16 offset = addr;
			return m_bankMode == 1
				? cartridge.ROM()[romBankIndexExtended(cartridge) * romBankSize + offset]
				: cartridge.ROM()[addr];
		}
		else if (Util::RangeUint16(RomBankNNStart, RomBankNNEnd).IsBetween(addr))
		{
			const uint16 offset = addr - RomBank00Start;
			return cartridge.ROM()[romBankIndexExtended(cartridge) * romBankSize + offset];
		}
		else if (Util::RangeUint16(ExternalRamStart, ExternalRamEnd).IsBetween(addr))
		{
			const uint16 offset = addr - ExternalRamStart;
			return cartridge.RAM()[m_bankMode == 0
				? offset
				: m_secondBankIndex * 0x2000 + offset];
		}

		assert(false);
		return 0;
	}

	void MBC1::Write(Cartridge& cartridge, uint16 addr, uint8 data)
	{
		// TODO
	}

	uint16 MBC1::romBankIndexExtended(Cartridge& cartridge) const
	{
		// 256KiB以下のカートでは、16個の16KiBバンクすべてが4byte(uint16)で指定可能であるが、
		// 5bit以上のバンクが必要な大型カードリッジはセカンダリバンクレジスタを使用して上位2bitを補う

		return hasBankMoreThanBit5(cartridge)
			? (m_secondBankIndex << 5) | m_romBankIndex
			: m_romBankIndex;
	}

	bool MBC1::hasBankMoreThanBit5(Cartridge& cartridge) const
	{
		// TODO: 512かもしれないので確認
		return cartridge.Header().RomSizeKB > 256;
	}
}
