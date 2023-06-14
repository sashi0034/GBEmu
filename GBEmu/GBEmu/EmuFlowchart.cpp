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
		Console.writeln(U"Setup ...");

		HW::HWEnv env{};

		env.GetMemory().LoadCartridge(config.CartridgePath);

		const Font font{16};

		while (System::Update())
		{
			HW::HWFrame::EmulateFrame(env);
			env.GetPPU().Draw(Point{0, 0}, 2);

			(void)font(U"PC: {:04X}"_fmt(env.GetCPU().PC())).draw(400, 100);
		}
	}
}
