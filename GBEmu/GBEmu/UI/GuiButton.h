#pragma once
#include "UIParam.h"

namespace GBEmu::UI
{
	class GuiButton
	{
	public:
		explicit GuiButton() = default;

		inline void operator()(const Point& start, const Size& size) const;
		inline void operator()(const Rect& rect) const;
	private:
		static constexpr int shadow = 12;
		static constexpr int roundness = 8;
	};


	inline void GuiButton::operator()(const Point& start, const Size& size) const
	{
		(*this)(Rect(start, size));
	}

	inline void GuiButton::operator()(const Rect& rect) const
	{
		(void)rect.rounded(roundness)
			.drawShadow(Vec2{ 0, shadow }, shadow)
			.draw(UIParam::ColorButtonUsual)
			.drawFrame(1.0, UIParam::ColorButtonBorder);
	}
}
