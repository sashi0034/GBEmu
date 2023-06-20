#pragma once

namespace GBEmu::HW
{
	class Memory;
	class PPUAddressLYDiff;
	class LCD;
	class VRAM;

	struct PPURenderBGAndWindowArgs
	{
		Memory& Memory;
		const LCD& LCD;
		VRAM& VRAM;
		const std::array<unsigned, 5>& WindowEnableBuffer;
		const PPUAddressLYDiff& BGAndWindowTileDataDiff;
		const PPUAddressLYDiff& BGTileMapDisplayDiff;
		const PPUAddressLYDiff& WindowTileMapDisplayDiff;
	};

	struct RenderOBJArgs
	{
		Memory& memory;
		const LCD& lcd;
		VRAM& vram;
		const RenderTexture& objMask;
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
