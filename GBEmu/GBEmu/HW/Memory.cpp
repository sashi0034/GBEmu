#include "stdafx.h"
#include "Memory.h"

#include "HWEnv.h"
#include "MemoryAddress.h"
#include "HWLogger.h"
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
	class HWEnv;
	using namespace MemoryAddress;

	Memory::Memory() :
		m_lcd(*this)
	{}

	uint8 Memory::Read(uint16 addr)
	{
		// https://github.com/pokemium/gb-docs-ja/blob/main/cartridge/mbc/mbc1.md

		if (RangeUint16(RomBank00Start_0x0000, RomBankNNEnd_0x7FFF).IsBetween(addr) ||
			RangeUint16(ExternalRamStart_0xA000, ExternalRamEnd_0xBFFF).IsBetween(addr))
		{
			return m_cartridge.Read(addr);
		}
		else if (RangeUint16(VRamStart_0x8000, VRamEnd_0x9FFF).IsBetween(addr))
		{
			return m_vram.Read(addr);
		}

		return m_memory[addr];
	}

	uint16 Memory::Read16(uint16 addr)
	{
		return Read(addr) | (Read(addr + 1) << 8);
	}

	void Memory::Write(HWEnv& env, uint16 addr, uint8 data)
	{
		env.Debugger().OnMemoryWrite(addr, data);

		if (RangeUint16(RomBank00Start_0x0000, RomBankNNEnd_0x7FFF).IsBetween(addr) ||
			RangeUint16(ExternalRamStart_0xA000, ExternalRamEnd_0xBFFF).IsBetween(addr))
		{
			m_cartridge.Write(addr, data);
		}
		else if (RangeUint16(VRamStart_0x8000, VRamEnd_0x9FFF).IsBetween(addr))
		{
			m_vram.Write(addr, data);
		}
		else if (RangeUint16(WorkRamBank0Start_0xC000, WorkRamBank1End_0xDFFF).IsBetween(addr))
		{
			m_memory[addr] = data;
			// m_memory[EchoWorkRamStart_0xE000 + (addr - WorkRamBank0Start_0xC000)] = data; // ミラー書き込み (これだと駄目)
		}
		else if (RangeUint16(EchoWorkRamStart_0xE000, EchoWorkRamEnd_0xFDFF).IsBetween(addr))
		{
			HWLogger::Warn(U"accessed mirrored work ram: {:04X}"_fmt(addr));
			m_memory[addr] = data;
		}
		else if (RangeUint16(IOPortsStart_0xFF00, IOPortsEnd_0xFF7F).IsBetween(addr))
		{
			writeIO(env, addr, data);
		}
		else
		{
			m_memory[addr] = data;
		}
	}

	// エンディアンに注意
	void Memory::Write16(HWEnv& env, uint16 addr, uint16 data16)
	{
		Write(env, addr, data16 & 0xFF);
		Write(env, addr + 1, (data16 >> 8) & 0xFF);
	}

	void Memory::WriteDirect(uint16 addr, uint8 data)
	{
		m_memory[addr] = data;
	}

	void Memory::WriteDirect16(uint16 addr, uint16 data16)
	{
		m_memory[addr] = data16 & 0xFF;
		m_memory[addr + 1] = (data16 >> 8) & 0xFF;
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

		// メモリ内容を電源投入時のものにする
		initMemory();
	}

	void Memory::writeIO(HWEnv& env, uint16 addr, uint8 data)
	{
		// 0xFF00 - 0xFF7F

		if (addr == JOYP_0xFF00)
		{
			env.GetJoypad().Update(*this, data);
		}
		else if (addr == DIV_0xFF04)
		{
			env.GetTimer().ResetDIVDetail();
		}
		else if (addr == TIMA_0xFF05)
		{
			// TODO: 割り込み中断?
			m_memory[addr] = data;
		}
		else if (addr == TAC_0xFF07)
		{
			m_memory[addr] = 0xF8 | (data & 0xF07);
		}
		else if (addr == DMA_0xFF46)
		{
			// ROMまたはRAMからOAMへのDMA転送
			uint16 src = data * 0x100; // 多くのドキュメントには転送元アドレスを0x100で割ると書いてあるが、多分間違ってる
			for (uint16 offset = 0; offset < 0x100; ++offset)
			{
				m_memory[OAMStart_0xFE00 + offset] = Read(src + offset);
			}
			m_memory[addr] = 0xFF;
		}
		else if (addr == IF_0xFF0F)
		{
			m_memory[addr] = data | 0xE0;
		}
		// TODO: 他のも
		else
		{
			m_memory[addr] = data;
		}
	}

	void Memory::initMemory()
	{
		m_memory[0xFF05] = 0x00; // TIMA
		m_memory[0xFF06] = 0x00; // TMA
		m_memory[0xFF07] = 0x00; // TAC
		m_memory[0xFF10] = 0x80; // NR10
		m_memory[0xFF11] = 0xBF; // NR11
		m_memory[0xFF12] = 0xF3; // NR12
		m_memory[0xFF14] = 0xBF; // NR14
		m_memory[0xFF16] = 0x3F; // NR21
		m_memory[0xFF17] = 0x00; // NR22
		m_memory[0xFF19] = 0xBF; // NR24
		m_memory[0xFF1A] = 0x7F; // NR30
		m_memory[0xFF1B] = 0xFF; // NR31
		m_memory[0xFF1C] = 0x9F; // NR32
		m_memory[0xFF1E] = 0xBF; // NR33
		m_memory[0xFF20] = 0xFF; // NR41
		m_memory[0xFF21] = 0x00; // NR42
		m_memory[0xFF22] = 0x00; // NR43
		m_memory[0xFF23] = 0xBF; // NR30
		m_memory[0xFF24] = 0x77; // NR50
		m_memory[0xFF25] = 0xF3; // NR51
		m_memory[0xFF26] = 0xF0; // NR52 // GB: 0xF1, SGB: 0xFD
		m_memory[0xFF40] = 0x91; // LCDC
		m_memory[0xFF42] = 0x00; // SCY
		m_memory[0xFF43] = 0x00; // SCX
		m_memory[0xFF45] = 0x00; // LYC
		m_memory[0xFF47] = 0xFC; // BGP
		m_memory[0xFF48] = 0xFF; // OBP0
		m_memory[0xFF49] = 0xFF; // OBP1
		m_memory[0xFF4A] = 0x00; // WY
		m_memory[0xFF4B] = 0x00; // WX
		m_memory[0xFF0F] = 0xE0; // IF
		m_memory[0xFFFF] = 0x00; // IE
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

		HWLogger::Assert(header.RamSizeKB != 0xff, U"invalid ram info in cartridge");

		return header;
	}
}
