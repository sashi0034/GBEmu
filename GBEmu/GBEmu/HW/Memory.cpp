#include "stdafx.h"
#include "Memory.h"

#include "MemoryAddress.h"
#include "GBEmu/Util/Range.h"

// http://bgb.bircd.org/pandocs.htm#memorymap

// 0000-3FFF: 16KBのROMバンク 00。これはカートリッジ内にあり、バンク 00に固定されています。
// 4000-7FFF: 16KBのROMバンク 01からNN。これもカートリッジ内にあり、バンク番号を切り替えることが可能です。
// 8000-9FFF: 8KBのビデオRAM（VRAM）。CGBモードでは、0から1への切り替え可能なバンクです。
// A000-BFFF: 8KBの外部RAM。カートリッジ内にあり、存在する場合、バンクを切り替えることが可能です。
// C000-CFFF: 4KBのワークRAM（WRAM）バンク 0。
// D000-DFFF: 4KBのワークRAM（WRAM）バンク 1。CGBモードでは、1から7への切り替え可能なバンクです。
// E000-FDFF: C000-DDFFと同じ（ECHO）。通常は使用されません。
// FE00-FE9F: スプライト属性表（OAM）。
// FEA0-FEFF: 使用不能。
// FF00-FF7F: I/Oポート。
// FF80-FFFE: ハイRAM（HRAM）。
// FFFF: 割り込み有効レジスタ。

namespace GBEmu::HW
{
	using namespace MemoryAddress;

	Memory::Memory()
	{
		m_memory.resize(MemorySize);
	}

	uint8 Memory::Read(uint16 addr)
	{
		// https://github.com/pokemium/gb-docs-ja/blob/main/cartridge/mbc/mbc1.md

		if (RangeUint16(RomBank00Start, RomBankNNEnd).IsBetween(addr) ||
			RangeUint16(ExternalRamStart, ExternalRamEnd).IsBetween(addr))
		{
			return m_cartridge.Read(addr);
		}

		return m_memory[addr];
	}


	void Memory::LoadCartridge(const FilePath& cartridgePath)
	{
		BinaryReader reader{cartridgePath};

		Array<uint8> cartridgeData{};
		const int cartridgeSize = reader.size();
		cartridgeData.resize(cartridgeSize);
		reader.read(cartridgeData.data(), cartridgeSize);

		const auto cartridgeHeader = loadCartridgeHeader(cartridgeData);

		m_cartridge = Cartridge(cartridgeHeader, cartridgeData);
	}

	CartridgeHeader Memory::loadCartridgeHeader(const Array<uint8>& cartridgeData)
	{
		CartridgeHeader header{};

		// 元のゲームボーイでは、タイトルはカートリッジヘッダーのバイト 0134 から 0143 の範囲に格納されます。タイトルは大文字の ASCII 文字を使用して表現されます。
		// タイトルが 16 文字未満の場合、範囲内の残りのバイトは 00 で埋められます。
		// しかし、ニンテンドーはゲームボーイカラー（CGB）を導入する際に、タイトルの長さにいくつかの変更を加えました。最初に、タイトル領域の長さを 15 文字に減らしました。
		// そのため、元のゲームボーイと比べて 1 文字少なくなりました。そして後に、タイトル領域をわずか 11 文字にさらに減らしました。

		constexpr int maxTitleLength = 15;
		header.Title = cartridgeData
			.slice(CartridgeAddress::TitleStart, maxTitleLength)
			.map([](uint8 data){return static_cast<char32_t>(data);})
			.join(U""_sv, U""_sv, U""_sv);

		header.Type = static_cast<CartridgeType>(cartridgeData[CartridgeAddress::CartridgeType]);


		uint8 romInfo = cartridgeData[CartridgeAddress::RomSize];
		// カートリッジの ROM サイズを指定します。通常は「32KB shl N」として計算されます。
		header.RomSizeKB = 32 << romInfo;

		const uint8 ramInfo = cartridgeData[CartridgeAddress::RamSize];

		header.RamSizeKB =
			ramInfo == 0x00 ? 0 :
			ramInfo == 0x01 ? 2 :
			ramInfo == 0x02 ? 8 :
			ramInfo == 0x03 ? 32 :
			0xff;
		assert(header.RamSizeKB != 0xff);

		return header;
	}
}
