#pragma once

namespace GBEmu::HW
{
	class HWAsset
	{
	public:
		static HWAsset& Instance();
		HWAsset();
		~HWAsset();

		const PixelShader PsTileBgDMG = HLSL{ U"asset/shader/tile_bg_dmg.hlsl", U"PS" };
		const PixelShader PsTileWindowDMG = HLSL{ U"asset/shader/tile_window_dmg.hlsl", U"PS" };
		const PixelShader PsTileObjDMG = HLSL{ U"asset/shader/tile_obj_dmg.hlsl", U"PS" };

		const PixelShader PsTileDump = HLSL{ U"asset/shader/tile_dump.hlsl", U"PS" };
	private:
		static HWAsset* globalInstance;
	};
}
