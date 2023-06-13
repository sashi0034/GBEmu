#include "stdafx.h"
#include "HWEnv.h"

namespace GBEmu::HW
{
	PPU& IHWEnvMemory::GetPPU()
	{
		return m_env.GetPPU();
	}

}
