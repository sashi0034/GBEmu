#include "stdafx.h"
#include "Utils.h"

namespace GBEmu
{
	void Util::FillTexture(const Texture& texture, float tileSize, const Size& size, const Vec2& pos)
	{
		const ScopedRenderStates2D sampler{ SamplerState::RepeatLinear };
		const float scale = texture.size().x / tileSize;
		(void)texture.mapped(size * scale).scaled(1 / scale).draw(pos);
	}

	int Util::AnimFrameIndexF(float controller, const int frameNumber, const int frameDuration)
	{
		return static_cast<int>(controller * 1000) / frameDuration % frameNumber;
	}

	int Util::AnimFrameIndex(int controller, const int frameNumber, const int frameDuration)
	{
		return controller / frameDuration % frameNumber;
	}

	int Util::AnimFrameIndex(uint64 controller, const int frameNumber, const int frameDuration)
	{
		return controller / frameDuration % frameNumber;
	}

	String Util::ConcatStringWithBreak(const Array<String>& array)
	{
		String result{};
		for (int i=0; i<array.size(); ++i)
		{
			result += array[i];
			if (i < array.size() - 1) result += U"\n";
		}
		return result;
	}
}
