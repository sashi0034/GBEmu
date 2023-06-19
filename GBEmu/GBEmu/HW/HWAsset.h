#pragma once

namespace GBEmu::HW
{
	class HWAsset
	{
	public:
		static HWAsset& Instance();
		static void CreateInstance();
		static void DestroyInstance();

		const PixelShader PsTileBgDMG = HLSL{ U"asset/shader/tile_bg_dmg.hlsl", U"PS" };
		const PixelShader PsTileWindowDMG = HLSL{ U"asset/shader/tile_window_dmg.hlsl", U"PS" };
		const PixelShader PsTileObjDMG = HLSL{ U"asset/shader/tile_obj_dmg.hlsl", U"PS" };
	private:
		static HWAsset* m_instance;
	};
}
