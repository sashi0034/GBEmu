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

		auto&& cpu = hw.GetCPU();
		auto&& lcd = hw.GetMemory().GetLCD();
		String text{};
		text += U"OP={}\n"_fmt(hw.Debugger().LastExecutedInstruction().ToString());
		text += U"PC={:04X}  SP={:04X}\n"_fmt(cpu.PC(), cpu.SP());
		text += U"AF={:04X}  BC={:04X}  DE={:04X}  HL={:04X}\n\n"_fmt(cpu.RegAF(), cpu.RegBC(), cpu.RegDE(), cpu.RegHL());
		text += U"IE=  {:02X}  IF=  {:02X}\n\n"_fmt(hw.GetMemory().Interrupt().IE(), hw.GetMemory().Interrupt().IF());
		text += U"LCDC={:04X}  STAT={:04X}\n"_fmt(lcd.LCDC(), lcd.STAT());
		text += U"SCX =  {:02X}  SCY =  {:02X}\n"_fmt(lcd.SCX(), lcd.SCY());
		text += U"WX  =  {:02X}  WY  =  {:02X}\n\n"_fmt(lcd.WX(), lcd.WY());
		hw.GetMemory().DumpIOPort(text);

		constexpr int borderThickness = 3;

		// 緑帯描画
		(void)Rect{Point(-borderThickness, 0), Point(borderThickness, height) }.draw(ConstParam::ColorGamingGreen);

		(void)Rect{Point(0, 0), Point(width, height) }.draw(
			Arg::left = Color{ 16, 16, 16 },
			Arg::right = Color{ 16, 16, 16, 0 });

		constexpr int paddingY = 4;
		constexpr int paddingX = 12;

		font(text).draw(Vec2(paddingX, paddingY), ColorF(0.3f, 0.3f, 0.3f));
	}
}
