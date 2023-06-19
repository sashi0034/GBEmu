//	Textures
//
SamplerState	g_sampler0 : register(s0);
SamplerState	g_sampler1 : register(s1);
SamplerState	g_sampler2 : register(s2);

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

cbuffer TileDMGCb : register(b1)
{
	float4 g_palette[4];
}

float4 PS(s3d::PSInput input) : SV_TARGET
{
	float4 color0 = input.color;

	if (color0.x == 0 && color0.y == 0)
	{
		color0 = g_palette[0];	
	}
	else if (color0.x == 0 && color0.y == 1)
	{
		color0 = g_palette[1];	
	}
	else if (color0.x == 1 && color0.y == 0)
	{
		color0 = g_palette[2];	
	}
	else if (color0.x == 1 && color0.y == 1)
	{
		color0 = g_palette[3];	
	}
	
	return color0 + g_colorAdd;
}
