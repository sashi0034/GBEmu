#pragma once
#include "DebugParam.h"

namespace GBEmu
{
	struct EmuConfig
	{
		String CartridgePath;
	};

	static const EmuConfig DefaultEmuConfig = EmuConfig{
		DebugParam::RomPathPuyoPuyo2
	};
}
