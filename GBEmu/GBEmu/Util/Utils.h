#pragma once

namespace GBEmu
{
	namespace Util
	{
		template <class...>
		constexpr std::false_type AlwaysFalse{};

		template <auto value>
		constexpr std::false_type AlwaysFalseValue{};

		void FillTexture(const Texture& texture, float tileSize, const Size& size, const Vec2& pos);
		int AnimFrameIndexF(float controller, const int frameNumber, const int frameDuration);
		int AnimFrameIndex(int controller, const int frameNumber, const int frameDuration);
		int AnimFrameIndex(uint64 controller, const int frameNumber, const int frameDuration);
		String ConcatStringWithBreak(const Array<String>& array);

		template <typename E>
		[[nodiscard]] String StringifyEnum(E value) noexcept
		{
			const auto enumName = magic_enum::enum_name(value);
			return Unicode::Widen(enumName);
		}
	}
}
