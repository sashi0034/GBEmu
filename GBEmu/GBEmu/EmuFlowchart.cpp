#include "stdafx.h"
#include "EmuFlowchart.h"

#include "EmuConfig.h"
#include "EmuLogger.h"
#include "EmuSingleton.h"
#include "HW/HWEnv.h"
#include "HW/HWFrame.h"
#include "HW/Memory.h"
#include "UI/UIEnv.h"

namespace GBEmu::EmuFlowchart
{
	String selectFileFromExplorer()
	{
		const auto cartridgeDir = FileSystem::PathAppend(FileSystem::InitialDirectory(), U"asset/rom");
		const auto selectedPath = Dialog::OpenFile(
			{ FileFilter{ .name = U"GAMEBOY Cartridge", .patterns = {U"gb?"} } },
			cartridgeDir,
			U"Open cartridge file");
		if (selectedPath.has_value() == false)
		{
			Console.writeln(U"cartridge is not found: {}"_fmt(selectedPath));
			return U"";
		}

		return selectedPath.value();
	}

	void RunEmu()
	{
		EmuSingleton emuSingleton{};

		// シーン設定など
		Window::SetTitle(U"GBEmu");
		Window::SetStyle(WindowStyle::Sizable);
		Scene::SetResizeMode(ResizeMode::Keep);
		constexpr Size sceneSize = {1920, 1080};
		Scene::Resize(sceneSize.x, sceneSize.y);
		Window::Resize(1280, 720);
		Scene::SetBackground(ColorF{ 0.3, 0.3, 0.3 });
		System::SetTerminationTriggers(UserAction::CloseButtonClicked);

		// エミュレータ設定
		try
		{
			auto config = EmuConfig::LoadToml(U"config.toml", U"asset/config_example.toml");
			emuSingleton.RegisterConfig(config);
		}
		catch (...)
		{
			EmuLogger::Error(U"failed to load config.toml");
		}

		const auto cartridgePath = FileSystem::Exists(EmuConfig::Instance().Cartridge.Path)
			? EmuConfig::Instance().Cartridge.Path
			: selectFileFromExplorer();
		if (cartridgePath.empty()) return;

		Console.open();
		Console.writeln(U"setup ...");

		HW::HWEnv hwEnv{};
		UI::UIEnv uiEnv{};

		HW::HWFrame hwFrame{};

		hwEnv.GetMemory().LoadCartridge(cartridgePath);
		hwEnv.GetMemory().Initialize(hwEnv);

		while (System::Update())
		{
			hwFrame.ControlFrame(hwEnv);
			constexpr double pixelScale = 5;

			// 背景描画
			uiEnv.Back().Draw();

			// エミュレータ画面描画
			const auto hwScreen = hwEnv.GetPPU().DrawAt(Scene::Center(),  pixelScale);

			// VRAM領域描画
			hwEnv.GetMemory().GetVRAM().DumpDrawAt(
				Scene::Center() + Point::Down(hwScreen.y / 2 + (Scene::Size().y - hwScreen.y) / 4),
				pixelScale / 2.0);

			// HUD描画
			constexpr int padding = 64;
			const int hudWidth = Scene::Center().x - (hwScreen.x / 2) - (padding * 2);
			uiEnv.Hud().DrawLeft(uiEnv, hwEnv, Point(padding, Scene::Center().y - hwScreen.y / 2), hudWidth, hwScreen.y);
			uiEnv.Hud().DrawRight(uiEnv, hwEnv, Point(Scene::Width() - padding - hudWidth, Scene::Center().y - hwScreen.y / 2), hudWidth, hwScreen.y);
			uiEnv.Hud().DrawUp(uiEnv, hwEnv, hwFrame, Scene::Center().moveBy(0, -hwScreen.y / 2 - padding), Scene::Center().moveBy(0, -hwScreen.y / 2));
		}
	}
}
