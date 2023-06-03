#pragma once

namespace GBEmu
{
	namespace Util
	{
		void FillTexture(const Texture& texture, float tileSize, const Size& size, const Vec2& pos);
		int AnimFrameIndexF(float controller, const int frameNumber, const int frameDuration);
		int AnimFrameIndex(int controller, const int frameNumber, const int frameDuration);
		int AnimFrameIndex(uint64 controller, const int frameNumber, const int frameDuration);
		String ConcatStringWithBreak(const Array<String>& array);
	}
}
