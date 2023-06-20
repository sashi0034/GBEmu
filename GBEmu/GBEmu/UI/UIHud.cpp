#include "stdafx.h"
#include "UIHud.h"

#include "UIAsset.h"
#include "GBEmu/ConstParam.h"
#include "GBEmu/HW/HWEnv.h"

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

	void UIHud::DrawLeft(UIEnv& ui, HW::HWEnv& hw, const Point& start, int width, int height)
	{
		const Transformer2D transformer{ Mat3x2::Scale(1).translated(start) };
		auto&& font = UIAsset::Instance().FontDotGothic18;

		constexpr int lineH = 24;

		// Array<String> message{};
		// message.push_back(U"OP={}"_fmt(hw.GetCPU().FetchInstruction(hw.GetMemory()).ToString()));
		// message.push_back(U"PC={:04X}"_fmt(hw.GetCPU().PC()));
		// message.push_back(U"SP={:04X}"_fmt(hw.GetCPU().SP()));

		String message1{};
		message1 += U"OP={}"_fmt(hw.GetCPU().FetchInstruction(hw.GetMemory()).ToString());
		message1 += U"\nPC={:04X}"_fmt(hw.GetCPU().PC());
		message1 += U"\nSP={:04X}"_fmt(hw.GetCPU().SP());

		constexpr int borderThickness = 3;

		// 緑帯描画
		(void)Rect{Point(-borderThickness, 0), Point(borderThickness, height) }.draw(ConstParam::ColorGamingGreen);

		(void)Rect{Point(0, 0), Point(width, height) }.draw(
			Arg::left = Color{ 16, 16, 16 },
			Arg::right = Color{ 16, 16, 16, 0 });

		constexpr int paddingY = 4;
		constexpr int paddingX = 12;

		font(message1).draw(Vec2(paddingX, paddingY), ColorF(0.5f, 0.5f, 0.5f));
		// for (int i=0; i<message.size(); ++i)
		// {
		// 	font(message[i]).draw(Vec2(paddingX, paddingY) + Vec2::Down(lineH * i), ColorF(0.5f, 0.5f, 0.5f));
		// }
	}
}
