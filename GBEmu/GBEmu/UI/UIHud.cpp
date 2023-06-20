#include "stdafx.h"
#include "UIHud.h"

#include "UIAsset.h"
#include "GBEmu/HW/HWEnv.h"
#include "GBEmu/Util/Utils.h"

namespace GBEmu::UI
{
	void drawBorder(const Rect& rect)
	{
		constexpr double thickness = 4.0;
		const double offset = Scene::Time() * 10;

		(void)rect.top().draw(LineStyle::SquareDot(offset), thickness);
		(void)rect.right().draw(LineStyle::SquareDot(offset), thickness);
		(void)rect.bottom().draw(LineStyle::SquareDot(offset), thickness);
		(void)rect.left().draw(LineStyle::SquareDot(offset), thickness);
	}

	void UIHud::DrawLeft(UIEnv& ui, HW::HWEnv& hw, const Point& start, int width)
	{
		const Transformer2D transformer{ Mat3x2::Scale(1).translated(start) };
		auto&& font = UIAsset::Instance().Font;

		constexpr int line = 20;
		constexpr int padding = -10;
		constexpr int line_2 = 2;

		String message{};
		message += U"PC: {:04X}\n"_fmt(hw.GetCPU().PC());
		message += U"SP: {:04X}\n"_fmt(hw.GetCPU().SP());
		message += U"IME: {}\n"_fmt(hw.GetCPU().IME());
		message += U"instr: {}\n"_fmt(hw.GetCPU().FetchInstruction(hw.GetMemory()).ToString());
		message += U"state: {}\n"_fmt(Util::StringifyEnum(hw.GetCPU().State()));

		const Rect rect1{Point(0, line * 0), Point(width, line * (5 + line_2)) };
		drawBorder(rect1);
		(void)font(message).draw(rect1.stretched(padding));


	}
}
