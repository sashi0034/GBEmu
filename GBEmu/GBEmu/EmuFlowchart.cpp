#include "stdafx.h"
#include "EmuFlowchart.h"

#include "EmuConfig.h"
#include "HW/HWAsset.h"
#include "HW/HWEnv.h"
#include "HW/HWFrame.h"
#include "HW/Memory.h"
#include "UI/UIAsset.h"
#include "UI/UIEnv.h"

namespace GBEmu::EmuFlowchart
{
	bool selectFileFromExplorer(EmuConfig& config)
	{
		const auto cartridgeDir = FileSystem::PathAppend(FileSystem::InitialDirectory(), U"asset/rom");
		const auto selectedPath = Dialog::OpenFile(
			{ FileFilter{ .name = U"GAMEBOY Cartridge", .patterns = {U"gb?"} } },
			cartridgeDir,
			U"Open cartridge file");
		if (selectedPath.has_value() == false)
		{
			Console.writeln(U"cartridge is not found: {}"_fmt(selectedPath));
			return false;
		}

		config.CartridgePath = selectedPath.value();
		return true;
	}

	void RunEmu()
	{
		// シングルトン系の生成
		HW::HWAsset hwAsset{};
		UI::UIAsset uiAsset{};

		// シーン設定など
		Window::SetStyle(WindowStyle::Sizable);
		Scene::SetResizeMode(ResizeMode::Keep);
		constexpr Size sceneSize = {1920, 1080};
		Scene::Resize(sceneSize.x, sceneSize.y);
		Window::Resize(1280, 720);
		Scene::SetBackground(ColorF{ 0.3, 0.3, 0.3 });

		// エミュレータ設定
		EmuConfig config = DefaultEmuConfig;
		if (FileSystem::Exists(config.CartridgePath) == false)
		{
			if (selectFileFromExplorer(config) == false) return;
		}

		Console.open();
		Console.writeln(U"setup ...");

		HW::HWEnv hwEnv{};
		UI::UIEnv uiEnv{};

		hwEnv.GetMemory().LoadCartridge(config.CartridgePath);

		// TODO: 60FPS control
		while (System::Update())
		{
			HW::HWFrame::EmulateFrame(hwEnv);
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
		}
	}
}
