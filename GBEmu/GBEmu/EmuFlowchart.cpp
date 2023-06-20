#include "stdafx.h"
#include "EmuFlowchart.h"

#include "EmuConfig.h"
#include "HW/HWAsset.h"
#include "HW/HWEnv.h"
#include "HW/HWFrame.h"
#include "HW/Memory.h"

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

		HW::HWEnv env{};

		env.GetMemory().LoadCartridge(config.CartridgePath);

		while (System::Update())
		{
			HW::HWFrame::EmulateFrame(env);
			constexpr double pixelScale = 4;

			// エミュレータ画面描画
			const auto hwScreen = env.GetPPU().DrawAt(Scene::Center(),  pixelScale);
			constexpr int padding = 64;
			env.Debugger().Draw(env, Scene::Center() + Point::Right(hwScreen.x / 2 + padding));

			// VRAM領域描画
			env.GetMemory().GetVRAM().DumpDrawAt(
				Scene::Center() + Point::Down(hwScreen.y / 2 + (Scene::Size().y - hwScreen.y) / 4),
				pixelScale / 2.0);
		}
	}
}
