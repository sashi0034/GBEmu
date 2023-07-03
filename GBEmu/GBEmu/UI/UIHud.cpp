#include "stdafx.h"
#include "UIHud.h"

#include "UIAsset.h"
#include "GBEmu/ConstParam.h"
#include "GBEmu/EmuGamepad.h"
#include "GBEmu/HW/HWEnv.h"

namespace GBEmu::UI
{
	constexpr int borderThickness = 3;

	constexpr int paddingY = 4;
	constexpr int paddingX = 12;
	constexpr ColorF hudGray = ColorF(0.3f, 0.3f, 0.3f);

	void drawGreenBorder(int width, int height, int startY)
	{
		(void)Rect{Point(-borderThickness, startY), Point(borderThickness, height) }.draw(ConstParam::ColorGamingGreen);

		(void)Rect{Point(0, startY), Point(width, height) }.draw(
			Arg::left = Color{ 16, 16, 16 },
			Arg::right = Color{ 16, 16, 16, 0 });
	}

	void UIHud::DrawLeft(UIEnv& ui, HW::HWEnv& hw, const Point& start, int width, int height)
	{
		const Transformer2D transformer{ Mat3x2::Scale(1).translated(start) };
		auto&& font = UIAsset::Instance().FontDotGothic18;

		auto&& cpu = hw.GetCPU();
		auto&& lcd = hw.GetPPU().GetLCD();
		String text{};
		text += U"OP={}\n"_fmt(hw.Debugger().LastExecutedInstruction().ToString());
		text += U"PC={:04X}  SP={:04X}\n"_fmt(cpu.PC(), cpu.SP());
		text += U"AF={:04X}  BC={:04X}  DE={:04X}  HL={:04X}\n\n"_fmt(cpu.RegAF(), cpu.RegBC(), cpu.RegDE(), cpu.RegHL());
		text += U"IE=  {:02X}  IF=  {:02X}\n\n"_fmt(hw.GetMemory().Interrupt().IE(), hw.GetMemory().Interrupt().IF());
		text += U"LCDC={:04X}  STAT={:04X}\n"_fmt(lcd.LCDC(), lcd.STAT());
		text += U"SCX =  {:02X}  SCY =  {:02X}\n"_fmt(lcd.SCX(), lcd.SCY());
		text += U"WX  =  {:02X}  WY  =  {:02X}\n\n"_fmt(lcd.WX(), lcd.WY());
		hw.GetMemory().DumpIOPort(hw, text);

		// 緑帯描画
		drawGreenBorder(width, height, 0);

		font(text).draw(Vec2(paddingX, paddingY), hudGray);
	}

	void UIHud::DrawRight(UIEnv& ui, HW::HWEnv& hw, const Point& start, int width, int height)
	{
		const Transformer2D transformer{ Mat3x2::Scale(1).translated(start) };
		auto&& font = UIAsset::Instance().FontDotGothic18;

		const int audioHeight = height / 6 - paddingY * 2;
		const int audioStart = height - audioHeight - paddingY * 1;

		const int playHeight = height / 3 - paddingY * 2;
		constexpr int playStart = paddingY; // audioStart - playHeight - paddingY * 2;

		// 緑帯描画
		drawGreenBorder(width, audioHeight, audioStart);
		drawGreenBorder(width, playHeight, playStart);

		// CPU稼働率を表示
		constexpr Size monitorSize{320, 24};
		const float cpuWorkedRate = hw.Debugger().CPUWorkedRate();
		auto monitorColor = Color(40, 40, 24);
		const Point monitorStart = Point(paddingX, playStart + paddingY + 16);
		(void)Rect(monitorStart, Size(static_cast<int>(monitorSize.x * cpuWorkedRate), monitorSize.y))
			.draw(monitorColor.setR(monitorColor.r * (0.5 + cpuWorkedRate)));
		const Rect monitorRect = Rect(monitorStart, monitorSize);
		(void)monitorRect.drawFrame(1.0, hudGray);
		(void)font(U"CPU {:8.4f} %"_fmt(cpuWorkedRate * 100))
			.drawAt(monitorRect.center(), hudGray); // fmtの{:N.Mf}のNは「全体で」N文字になるみたい

		// ボタン描画
		constexpr Size buttonSize{40, 40};
		const Point buttonPad = Point(paddingX, playStart + 80 + paddingY);
		constexpr int buttonRound = 8;
		static const auto buttonLeft = Rect(buttonPad + Point(0, buttonSize.y), buttonSize).rounded(buttonRound);
		static const auto buttonRight = Rect(buttonPad + Point(buttonSize.x * 2, buttonSize.y), buttonSize).rounded(buttonRound);
		static const auto buttonUp = Rect(buttonPad + Point(buttonSize.x, 0), buttonSize).rounded(buttonRound);
		static const auto buttonDown = Rect(buttonPad + Point(buttonSize.x,  buttonSize.y * 2), buttonSize).rounded(buttonRound);

		constexpr int buttonCircleRound = 20;
		static const auto buttonA = Rect(buttonPad + Point(int(buttonSize.x * 3.5),  buttonSize.y * 0), buttonSize).rounded(buttonCircleRound);
		static const auto buttonB = Rect(buttonPad + Point(int(buttonSize.x * 3.5),  buttonSize.y * 2), buttonSize).rounded(buttonCircleRound);

		static const auto buttonSelect = Rect(buttonPad + Point(buttonSize.x * 5,  buttonSize.y * 0), buttonSize + buttonSize.x0()).rounded(buttonRound);
		static const auto buttonStart = Rect(buttonPad + Point(buttonSize.x * 5,  buttonSize.y * 2), buttonSize + buttonSize.x0()).rounded(buttonRound);

		using bt = std::tuple<RoundRect, EmuGamepadKey, String>;
		static const std::array buttonArray{
			bt{buttonLeft, EmuGamepadKey::Left, U"L"},
			bt{buttonRight, EmuGamepadKey::Right, U"R"},
			bt{buttonUp, EmuGamepadKey::Up, U"Up"},
			bt{buttonDown, EmuGamepadKey::Down, U"Dn"},
			bt{buttonA, EmuGamepadKey::A, U"A"},
			bt{buttonB, EmuGamepadKey::B, U"B"},
			bt{buttonSelect, EmuGamepadKey::Select, U"Select"},
			bt{buttonStart, EmuGamepadKey::Start, U"Start"},};

		auto&& gamepad = EmuGamepad::Instance();
		for (auto&& button : buttonArray)
		{
			if (gamepad.IsPressed(std::get<1>(button)))
			{
				// ボタン押してるとき
				(void)std::get<0>(button).draw(hudGray);
				(void)font(std::get<2>(button)).drawAt(std::get<0>(button).center(), ColorF(0.1f, 0.1f, 0.1f));
			}
			else
			{
				// ボタン押してないとき
				(void)font(std::get<2>(button)).drawAt(std::get<0>(button).rect.center(), hudGray);
			}
		}

		// 音声波形描画
		constexpr Size audioMargin{paddingX, paddingY};
		(void)hw.Debugger().AudioGraph()
			.resized(Size(width, audioHeight) - audioMargin * 2)
			.draw(Point(0, audioStart) + audioMargin.xy(), Color(U"#789c70"));
	}

	void UIHud::DrawUp(UIEnv& ui, HW::HWEnv& hw, const Point& bottom)
	{
		constexpr Size sizeSS{56, 28};
		constexpr Size sizeS{80, 32};

		// 背景
		(void)Rect{Point(0, 0), Point(Scene::Width(), bottom.y) }.draw(
			Arg::top = Color{ 16, 16, 16 },
			Arg::bottom = Color{ 16, 16, 16, 0 }
			);
		// (void)Rect{Point(0, bottom.y), Point(Scene::Width(), borderThickness) }
		// 	.draw(ColorF(ConstParam::ColorGamingGreen));

		// TODO: 現状のSiv3DのGUI部分はカスタマイズ性が乏しく貧弱なので、将来的に追加される予定の機能が登場したらボタン関連を作成していく
	}
}
