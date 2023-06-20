#pragma once
#include <bitset>

#include "HWParams.h"

namespace GBEmu::HW
{
	class Memory;

	class VRAM
	{
	public:
		VRAM();
		void Write(uint16 addr, uint8 data);
		uint8 Read(uint16 addr) const;
		uint16 Read16(uint16 addr) const;

		TextureRegion GetTileData(uint16 baseAddr, uint8 tileId);

		void DumpDrawAt(const Vec2& pos, double scale);
		void CheckRefreshAtlas();

		RenderTexture& Atlas(){return m_tileAtlas; }
	private:
		void refreshAtlas();

		std::array<uint8, HWParam::VRamSize_0x2000> m_vram{};

		// TODO: フォーマットをR16G16_Float
		static constexpr int tileAmount_384 = 384; // (0x97FF - 0x8000) / 16
		static constexpr int tileEdge_8 = 8;
		std::bitset<tileAmount_384> m_tileDataOutdatedFlag{};
		bool m_isAtlasOutdated{};

		RenderTexture m_tileAtlas{Size(tileEdge_8 * tileAmount_384, tileEdge_8), TextureFormat::R16G16_Float};
	};
}
