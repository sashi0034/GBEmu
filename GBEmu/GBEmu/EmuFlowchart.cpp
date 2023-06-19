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
		HW::HWAsset::CreateInstance();

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
			env.GetPPU().Draw(Point{0, 0},  3);
			env.Debugger().Draw(env, Point{512, 100});
			env.GetMemory().GetVRAM().DumpDraw(Point{16, 480});
		}

		HW::HWAsset::DestroyInstance();
	}
}
