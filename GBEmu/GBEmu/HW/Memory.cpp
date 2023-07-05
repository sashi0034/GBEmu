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

	uint8 Memory::Read(HWEnv& env, uint16 addr)
	{
		// https://github.com/pokemium/gb-docs-ja/blob/main/cartridge/mbc/mbc1.md

		if (RangeUint16(RomBank00Start_0x0000, RomBankNNEnd_0x7FFF).IsBetween(addr) ||
			RangeUint16(ExternalRamStart_0xA000, ExternalRamEnd_0xBFFF).IsBetween(addr))
		{
			return m_cartridge.Read(addr);
		}
		if (RangeUint16(VRamStart_0x8000, VRamEnd_0x9FFF).IsBetween(addr))
		{
			return m_vram.Read(addr);
		}
		if (RangeUint16(IOPortsStart_0xFF00, IOPortsEnd_0xFF7F).IsBetween(addr))
		{
			return readIO(env, addr);
		}
		if (addr == IE_0xFFFF)
		{
			return m_interrupt.IE();
		}
		if (RangeUint16(EchoWorkRamStart_0xE000, EchoWorkRamEnd_0xFDFF).IsBetween(addr))
		{
			const uint16 offset = addr - EchoWorkRamStart_0xE000;
			return m_memory[WorkRamBank0Start_0xC000 + offset];
		}


		return m_memory[addr];
	}

	uint16 Memory::Read16(HWEnv& env, uint16 addr)
	{
		return Read(env, addr) | (Read(env, addr + 1) << 8);
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
		}
		else if (RangeUint16(EchoWorkRamStart_0xE000, EchoWorkRamEnd_0xFDFF).IsBetween(addr))
		{
			const uint16 offset = addr - EchoWorkRamStart_0xE000;
			m_memory[WorkRamBank0Start_0xC000 + offset] = data;
		}
		else if (RangeUint16(IOPortsStart_0xFF00, IOPortsEnd_0xFF7F).IsBetween(addr))
		{
			writeIO(env, addr, data);
		}
		else if (addr == IE_0xFFFF)
		{
			m_interrupt.SetIE(data);
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

	void Memory::DumpIOPort(HWEnv& env, String& dest)
	{
		for (uint16 addr = IOPortsStart_0xFF00; addr <= IOPortsEnd_0xFF7F; addr += 0x08)
		{
			dest += U"{:04X}:"_fmt(addr);
			for (uint16 i=0; i<0x08; ++i)
			{
				dest += U" {:02X}"_fmt(readIO(env, addr + i));
			}
			dest += U"\n";
		}
	}

	uint8 Memory::readIO(HWEnv& env, uint16 addr)
	{
		// 0xFF00 - 0xFF7F
		switch (addr)
		{
		case JOYP_0xFF00:
			return env.GetJoypad().ReadJOYP();

		case DIV_0xFF04:
			return env.GetTimer().ReadAddr<DIV_0xFF04>();
		case TIMA_0xFF05:
			return env.GetTimer().ReadAddr<TIMA_0xFF05>();
		case TAC_0xFF07:
			return env.GetTimer().ReadAddr<TAC_0xFF07>();
		case TMA_0xFF06:
			return env.GetTimer().ReadAddr<TMA_0xFF06>();

		case NR10_0xFF10:
			return env.GetAPU().ReadAddr<NR10_0xFF10>();
		case NR11_0xFF11:
			return env.GetAPU().ReadAddr<NR11_0xFF11>();
		case NR12_0xFF12:
			return env.GetAPU().ReadAddr<NR12_0xFF12>();
		case NR13_0xFF13:
			return env.GetAPU().ReadAddr<NR13_0xFF13>();
		case NR14_0xFF14:
			return env.GetAPU().ReadAddr<NR14_0xFF14>();
		case NR21_0xFF16:
			return env.GetAPU().ReadAddr<NR21_0xFF16>();
		case NR22_0xFF17:
			return env.GetAPU().ReadAddr<NR22_0xFF17>();
		case NR23_0xFF18:
			return env.GetAPU().ReadAddr<NR23_0xFF18>();
		case NR24_0xFF19:
			return env.GetAPU().ReadAddr<NR24_0xFF19>();
		case NR30_0xFF1A:
			return env.GetAPU().ReadAddr<NR30_0xFF1A>();
		case NR31_0xFF1B:
			return env.GetAPU().ReadAddr<NR31_0xFF1B>();
		case NR32_0xFF1C:
			return env.GetAPU().ReadAddr<NR32_0xFF1C>();
		case NR33_0xFF1D:
			return env.GetAPU().ReadAddr<NR33_0xFF1D>();
		case NR34_0xFF1E:
			return env.GetAPU().ReadAddr<NR34_0xFF1E>();
		case NR41_0xFF20:
			return env.GetAPU().ReadAddr<NR41_0xFF20>();
		case NR42_0xFF21:
			return env.GetAPU().ReadAddr<NR42_0xFF21>();
		case NR43_0xFF22:
			return env.GetAPU().ReadAddr<NR43_0xFF22>();
		case NR44_0xFF23:
			return env.GetAPU().ReadAddr<NR44_0xFF23>();
		case NR50_0xFF24:
			return env.GetAPU().ReadAddr<NR50_0xFF24>();
		case NR51_0xFF25:
			return env.GetAPU().ReadAddr<NR51_0xFF25>();
		case NR52_0xFF26:
			return env.GetAPU().ReadAddr<NR52_0xFF26>();

		case WaveRamStart_0xFF30 + 0x0: case WaveRamStart_0xFF30 + 0x1: case WaveRamStart_0xFF30 + 0x2:
		case WaveRamStart_0xFF30 + 0x3: case WaveRamStart_0xFF30 + 0x4: case WaveRamStart_0xFF30 + 0x5:
		case WaveRamStart_0xFF30 + 0x6: case WaveRamStart_0xFF30 + 0x7: case WaveRamStart_0xFF30 + 0x8:
		case WaveRamStart_0xFF30 + 0x9: case WaveRamStart_0xFF30 + 0xA: case WaveRamStart_0xFF30 + 0xB:
		case WaveRamStart_0xFF30 + 0xC: case WaveRamStart_0xFF30 + 0xD: case WaveRamStart_0xFF30 + 0xE:
		case WaveRamEnd_0xFF3F:
			return env.GetAPU().ReadWaveRam(addr);

		case LCDC_0xFF40:
			return env.GetPPU().GetLCD().ReadAddr<LCDC_0xFF40>();
		case STAT_0xFF41:
			return env.GetPPU().GetLCD().ReadAddr<STAT_0xFF41>();
		case BGP_0xFF47:
			return env.GetPPU().GetLCD().ReadAddr<BGP_0xFF47>();
		case OBP0_0xFF48:
			return env.GetPPU().GetLCD().ReadAddr<OBP0_0xFF48>();
		case OBP1_0xFF49:
			return env.GetPPU().GetLCD().ReadAddr<OBP1_0xFF49>();
		case SCX_0xFF43:
			return env.GetPPU().GetLCD().ReadAddr<SCX_0xFF43>();
		case SCY_0xFF42:
			return env.GetPPU().GetLCD().ReadAddr<SCY_0xFF42>();
		case LY_0xFF44:
			return env.GetPPU().GetLCD().ReadAddr<LY_0xFF44>();
		case LYC_0xFF45:
			return env.GetPPU().GetLCD().ReadAddr<LYC_0xFF45>();
		case WX_0xFF4B:
			return env.GetPPU().GetLCD().ReadAddr<WX_0xFF4B>();
		case WY_0xFF4A:
			return env.GetPPU().GetLCD().ReadAddr<WY_0xFF4A>();

		case IF_0xFF0F:
			return m_interrupt.IF();

		default:
			return m_memory[addr];
		}
	}

	void Memory::writeIO(HWEnv& env, uint16 addr, uint8 data)
	{
		// 0xFF00 - 0xFF7F
		switch (addr)
		{
		case JOYP_0xFF00:
			env.GetJoypad().WriteJOYP(data); break;

		case DIV_0xFF04:
			env.GetTimer().WriteAddr<DIV_0xFF04>(data); break;
		case TIMA_0xFF05:
			env.GetTimer().WriteAddr<TIMA_0xFF05>(data); break;
		case TAC_0xFF07:
			env.GetTimer().WriteAddr<TAC_0xFF07>(data); break;
		case TMA_0xFF06:
			env.GetTimer().WriteAddr<TMA_0xFF06>(data); break;

		case NR10_0xFF10:
			env.GetAPU().WriteAddr<NR10_0xFF10>(data); break;
		case NR11_0xFF11:
			env.GetAPU().WriteAddr<NR11_0xFF11>(data); break;
		case NR12_0xFF12:
			env.GetAPU().WriteAddr<NR12_0xFF12>(data); break;
		case NR13_0xFF13:
			env.GetAPU().WriteAddr<NR13_0xFF13>(data); break;
		case NR14_0xFF14:
			env.GetAPU().WriteAddr<NR14_0xFF14>(data); break;
		case NR21_0xFF16:
			env.GetAPU().WriteAddr<NR21_0xFF16>(data); break;
		case NR22_0xFF17:
			env.GetAPU().WriteAddr<NR22_0xFF17>(data); break;
		case NR23_0xFF18:
			env.GetAPU().WriteAddr<NR23_0xFF18>(data); break;
		case NR24_0xFF19:
			env.GetAPU().WriteAddr<NR24_0xFF19>(data); break;
		case NR30_0xFF1A:
			env.GetAPU().WriteAddr<NR30_0xFF1A>(data); break;
		case NR31_0xFF1B:
			env.GetAPU().WriteAddr<NR31_0xFF1B>(data); break;
		case NR32_0xFF1C:
			env.GetAPU().WriteAddr<NR32_0xFF1C>(data); break;
		case NR33_0xFF1D:
			env.GetAPU().WriteAddr<NR33_0xFF1D>(data); break;
		case NR34_0xFF1E:
			env.GetAPU().WriteAddr<NR34_0xFF1E>(data); break;
		case NR41_0xFF20:
			env.GetAPU().WriteAddr<NR41_0xFF20>(data); break;
		case NR42_0xFF21:
			env.GetAPU().WriteAddr<NR42_0xFF21>(data); break;
		case NR43_0xFF22:
			env.GetAPU().WriteAddr<NR43_0xFF22>(data); break;
		case NR44_0xFF23:
			env.GetAPU().WriteAddr<NR44_0xFF23>(data); break;
		case NR50_0xFF24:
			env.GetAPU().WriteAddr<NR50_0xFF24>(data); break;
		case NR51_0xFF25:
			env.GetAPU().WriteAddr<NR51_0xFF25>(data); break;
		case NR52_0xFF26:
			env.GetAPU().WriteAddr<NR52_0xFF26>(data); break;

		case WaveRamStart_0xFF30 + 0x0: case WaveRamStart_0xFF30 + 0x1: case WaveRamStart_0xFF30 + 0x2:
		case WaveRamStart_0xFF30 + 0x3: case WaveRamStart_0xFF30 + 0x4: case WaveRamStart_0xFF30 + 0x5:
		case WaveRamStart_0xFF30 + 0x6: case WaveRamStart_0xFF30 + 0x7: case WaveRamStart_0xFF30 + 0x8:
		case WaveRamStart_0xFF30 + 0x9: case WaveRamStart_0xFF30 + 0xA: case WaveRamStart_0xFF30 + 0xB:
		case WaveRamStart_0xFF30 + 0xC: case WaveRamStart_0xFF30 + 0xD: case WaveRamStart_0xFF30 + 0xE:
		case WaveRamEnd_0xFF3F:
			env.GetAPU().WriteWaveRam(addr, data); break;

		case LCDC_0xFF40:
			env.GetPPU().GetLCD().WriteAddr<LCDC_0xFF40>(data); break;
		case STAT_0xFF41:
			env.GetPPU().GetLCD().WriteAddr<STAT_0xFF41>(data); break;
		case BGP_0xFF47:
			env.GetPPU().GetLCD().WriteAddr<BGP_0xFF47>(data); break;
		case OBP0_0xFF48:
			env.GetPPU().GetLCD().WriteAddr<OBP0_0xFF48>(data); break;
		case OBP1_0xFF49:
			env.GetPPU().GetLCD().WriteAddr<OBP1_0xFF49>(data); break;
		case SCX_0xFF43:
			env.GetPPU().GetLCD().WriteAddr<SCX_0xFF43>(data); break;
		case SCY_0xFF42:
			env.GetPPU().GetLCD().WriteAddr<SCY_0xFF42>(data); break;
		case LY_0xFF44:
			env.GetPPU().GetLCD().WriteAddr<LY_0xFF44>(data); break;
		case LYC_0xFF45:
			env.GetPPU().GetLCD().WriteAddr<LYC_0xFF45>(data); break;
		case WX_0xFF4B:
			env.GetPPU().GetLCD().WriteAddr<WX_0xFF4B>(data); break;
		case WY_0xFF4A:
			env.GetPPU().GetLCD().WriteAddr<WY_0xFF4A>(data); break;

		case IF_0xFF0F:
			m_interrupt.SetIF(data); break;

		case DMA_0xFF46:
			transferDMA(env, data); break;
		default:
			m_memory[addr] = data; break;
		}
	}

	void Memory::transferDMA(HWEnv& env, uint8 data)
	{
		// ROMまたはRAMからOAMへのDMA転送
		const uint16 src = data * 0x100;
		for (uint16 offset = 0; offset < 0x100; ++offset)
		{
			m_memory[OAMStart_0xFE00 + offset] = Read(env, src + offset);
		}
		m_memory[DMA_0xFF46] = 0xFF;
	}

	void Memory::Initialize(HWEnv& env)
	{
		Write(env, 0xFF05, 0x00); // TIMA
		Write(env, 0xFF06, 0x00); // TMA
		Write(env, 0xFF07, 0x00); // TAC
		Write(env, 0xFF10, 0x80); // NR10
		Write(env, 0xFF11, 0xBF); // NR11
		Write(env, 0xFF12, 0xF3); // NR12
		Write(env, 0xFF14, 0xBF); // NR14
		Write(env, 0xFF16, 0x3F); // NR21
		Write(env, 0xFF17, 0x00); // NR22
		Write(env, 0xFF19, 0xBF); // NR24
		Write(env, 0xFF1A, 0x7F); // NR30
		Write(env, 0xFF1B, 0xFF); // NR31
		Write(env, 0xFF1C, 0x9F); // NR32
		Write(env, 0xFF1E, 0xBF); // NR33
		Write(env, 0xFF20, 0xFF); // NR41
		Write(env, 0xFF21, 0x00); // NR42
		Write(env, 0xFF22, 0x00); // NR43
		Write(env, 0xFF23, 0xBF); // NR30
		Write(env, 0xFF24, 0x77); // NR50
		Write(env, 0xFF25, 0xF3); // NR51
		Write(env, 0xFF26, 0xF1); // NR52
		Write(env, 0xFF40, 0x91); // LCDC
		Write(env, 0xFF42, 0x00); // SCY
		Write(env, 0xFF43, 0x00); // SCX
		Write(env, 0xFF45, 0x00); // LYC
		Write(env, 0xFF47, 0xFC); // BGP
		Write(env, 0xFF48, 0xFF); // OBP0
		Write(env, 0xFF49, 0xFF); // OBP1
		Write(env, 0xFF4A, 0x00); // WY
		Write(env, 0xFF4B, 0x00); // WX
		Write(env, 0xFF0F, 0xE0); // IF
		Write(env, 0xFFFF, 0x00); // IE
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
