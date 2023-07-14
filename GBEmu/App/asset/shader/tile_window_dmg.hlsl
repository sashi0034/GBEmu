//	Textures
//
Texture2D		g_texture0 : register(t0);
SamplerState	g_sampler0 : register(s0);

namespace s3d
{
	//
	//	VS Output / PS Input
	//
	struct PSInput
	{
		float4 position	: SV_POSITION;
		float4 color	: COLOR0;
		float2 uv		: TEXCOORD0;
	};
}

//
//	Constant Buffer
//
cbuffer PSConstants2D : register(b0)
{
	float4 g_colorAdd;
	float4 g_sdfParam;
	float4 g_sdfOutlineColor;
	float4 g_sdfShadowColor;
	float4 g_internal;
}

struct LCDCFlag128
{
	uint windowDisplayEnable; // 0x00
	uint bgAndWindowEnable; // 0x20
	uint objDisplayEnable; // 0x40
	uint padding_0x60; // 0x60
};

cbuffer TileBgAndWindowqDmgCb : register(b1)
{
	float4 g_palette[4];
	LCDCFlag128 g_lcdcFlagBuffer[5]; // 32 bit * 5 > displayHeight_144 bit
}

float4 PS(s3d::PSInput input) : SV_TARGET
{
	float4 color0 = g_texture0.Sample(g_sampler0, input.uv);

	const float y = input.position.y;
	const uint y32 = uint(y) & 0x1F; // y mod 32 
	const LCDCFlag128 flag = g_lcdcFlagBuffer[uint(y / 32)]; 
	const bool windowPriority = flag.windowDisplayEnable & (1 << y32);
	const bool enable = flag.bgAndWindowEnable & (1 << y32);
	
	const int paletteIndex = (uint)(color0.x) * 2 + (uint)(color0.y);
	color0 = g_palette[(uint)(enable) * paletteIndex];
	color0.a *= float(windowPriority);
	
	return color0 + g_colorAdd;
}
