#include "stdafx.h"
#include "VRAM.h"

#include "MemoryAddress.h"

namespace GBEmu::HW
{
	using namespace MemoryAddress;

	constexpr uint16 singleTileBytes_0x10 = 0x10; // 1枚のタイルは16バイト

	void VRAM::Write(uint16 addr, uint8 data)
	{
		m_vram[addr - VRamStart_0x8000] = data;

		if (addr <= BGAndWindowTileData2End_0x97FF)
		{
			m_isAtlasObsolete = true;
			m_tileDataObsoleteFlag.set((addr - VRamStart_0x8000) / singleTileBytes_0x10);
		}
	}

	uint8 VRAM::Read(uint16 addr) const
	{
		return m_vram[addr - VRamStart_0x8000];
	}

	uint16 VRAM::Read16(uint16 addr) const
	{
		return Read(addr) | (Read(addr + 1) << 8);
	}

	TextureRegion VRAM::GetTileData(uint16 baseAddr, uint8 tileId)
	{
		const int tileOffset = baseAddr == TileDataTableStart_0x8000
			// 0x8000アドレッシングでは、0x8000をベースポインタとし、符号なしのアドレッシング
			? tileId * 0x10
			// 0x8800アドレッシングでは、0x9000をベースポインタとして使用し、符号付きアドレッシング ()
			: static_cast<int8>(tileId) * 0x10;
		const uint16 tileAddr = baseAddr + tileOffset;
		const uint16 tileIndex = (tileAddr - TileDataTableStart_0x8000) / singleTileBytes_0x10;

		if (m_isAtlasObsolete) refreshAtlas();

		return m_tileAtlas(tileIndex * tileEdge_8, 0, tileEdge_8, tileEdge_8);
	}

	void VRAM::DumpDraw(const Vec2& pos)
	{
		if (m_isAtlasObsolete) refreshAtlas();

		for (int y=0;; y+=tileEdge_8)
		{
			constexpr int columns = 64;
			for (int x=0; x < columns * tileEdge_8; x+=tileEdge_8)
			{
				m_tileAtlas((x + y * columns), 0, tileEdge_8, tileEdge_8).draw(pos + Point(x, y));
				if ((x + y * columns) >= tileAmount_384 * tileEdge_8) goto exit;
			}
		}
		exit:;
	}

	void VRAM::refreshAtlas()
	{
		m_isAtlasObsolete = false;

		const ScopedRenderTarget2D target{ m_tileAtlas };

		for (uint16 addr = TileDataTableStart_0x8000; addr <= BGAndWindowTileData2End_0x97FF; addr += singleTileBytes_0x10)
		{
			const int tileIndex = (addr - TileDataTableStart_0x8000) / singleTileBytes_0x10;
			// assert(tileIndex < tileAmount_384);

			if (m_tileDataObsoleteFlag.test(tileIndex) == false) continue;
			m_tileDataObsoleteFlag.reset(tileIndex);

			for (int v=0; v<=7; ++v)
			{
				const uint16 tileLineData = Read16(addr + v * 2);

				for (int u=0; u<=7; ++u)
				{
					const int paletteIndex = ((tileLineData >> u) & 0b1) | (((tileLineData >> (u + 8)) & 0b1) << 1); // 0 ~ 3

					// ここでは、RG情報にパレットインデックスだけ保存し、描画する際シェーダー側でパレットを参照する
					(void)Rect{tileIndex * tileEdge_8 + u, v, 1, 1}.draw(
						ColorF{static_cast<float>(paletteIndex / 2), static_cast<float>(paletteIndex % 2), 0});
				}
			}
		}
	}
}
