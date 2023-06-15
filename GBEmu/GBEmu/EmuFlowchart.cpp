#include "stdafx.h"
#include "EmuFlowchart.h"

#include "DebugParam.h"
#include "EmuConfig.h"
#include "HW/HWEnv.h"
#include "HW/HWFrame.h"
#include "HW/Memory.h"

namespace GBEmu::EmuFlowchart
{
	void RunEmu()
	{
		EmuConfig config{DebugParam::RomPathTestCpuInstrs};

		if (FileSystem::Exists(config.CartridgePath) == false)
		{
			Console.writeln(U"cartridge is not found: {}"_fmt(config.CartridgePath));
			return;
		}

		Console.open();
		Console.writeln(U"setup ...");

		HW::HWEnvHandler env{};

		env().GetMemory().LoadCartridge(config.CartridgePath);

		while (System::Update())
		{
			HW::HWFrame::EmulateFrame(env);
			env().GetPPU().Draw(Point{0, 0}, 2);
			env.Debugger().Draw(env(), Point{400, 100});
		}
	}
}
