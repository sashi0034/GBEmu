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

cbuffer TileBgAndWindowqDmgCb : register(b1)
{
	float4 g_palette[4];
}

float4 PS(s3d::PSInput input) : SV_TARGET
{
	float4 color0 = g_texture0.Sample(g_sampler0, input.uv);

	color0 = g_palette[(int)(color0.x) * 2 + (int)(color0.y)];
	
	return color0 + g_colorAdd;
}
