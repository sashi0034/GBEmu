#pragma once

namespace GBEmu::HW
{
	class HWEnv;
	class Memory;
	class PPUAddressLYDiff;
	class LCD;
	class VRAM;
	struct BGAndWindowFlag128;

	// 16 byte struct (128 bit)
	// padding needed for HLSL with 16 byte alignment
	struct BGAndWindowFlag128
	{
		uint32 WindowPriority; // 0x00
		uint32 Enable; // 0x20
		uint32 padding_0x40; // 0x40
		uint32 padding_0x60; // 0x60
	};
	static constexpr int BGAndWindowFlagBufferSize_5 = 5; // displayHeight_144 / 32 + 1

	struct PPURenderBGAndWindowArgs
	{
		HWEnv& Env;
		Memory& Memory;
		const LCD& LCD;
		VRAM& VRAM;
		const std::array<BGAndWindowFlag128, BGAndWindowFlagBufferSize_5>& WindowEnableBuffer;
		const PPUAddressLYDiff& BGAndWindowTileDataDiff;
		const PPUAddressLYDiff& BGTileMapDisplayDiff;
		const PPUAddressLYDiff& WindowTileMapDisplayDiff;
	};

	struct RenderOBJArgs
	{
		HWEnv& Env;
		Memory& Memory;
		const LCD& LCD;
		VRAM& VRAM;
		const RenderTexture& ObjMask;
	};

	namespace PPURender
	{
		void RenderBGAndWindow(const PPURenderBGAndWindowArgs& arg);
		void RenderBGCompletely(const PPURenderBGAndWindowArgs& arg);
		void RenderWindowCompletely(const PPURenderBGAndWindowArgs& arg);
		void RenderObjMaskFromBGAndWindow(const PPURenderBGAndWindowArgs& arg, const RenderTexture& objMaskBuffer);

		void RenderOBJCompletely(const RenderOBJArgs& arg);
	};
}
