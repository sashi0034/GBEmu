//	Textures
//
Texture2D		g_texture0 : register(t0);
SamplerState	g_sampler0 : register(s0);

#define DISPLAY_W 160
#define DISPLAY_H 144

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

cbuffer DisplaySpecialCb : register(b1)
{
	float g_pixelScale;
}

float4 PS(s3d::PSInput input) : SV_TARGET
{
	float4 texColor = g_texture0.Sample(g_sampler0, input.uv);

	const float dot =
		((input.uv.x * g_pixelScale * DISPLAY_W) % g_pixelScale) +
		((input.uv.y * g_pixelScale * DISPLAY_H) % g_pixelScale);
	
	if (dot < g_pixelScale)
	{
		texColor *= 1.1;
	}
	
	return (texColor * input.color) + g_colorAdd;
}
