#pragma once

namespace GBEmu::HW
{
	class HWAsset
	{
	public:
		static HWAsset& Instance();
		static void CreateInstance();
		static void DestroyInstance();

		const PixelShader PsTileDMG = HLSL{ U"asset/shader/tile_dmg.hlsl", U"PS" };
	private:
		static HWAsset* m_instance;
	};
}
