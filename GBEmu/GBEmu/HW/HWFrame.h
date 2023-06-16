#pragma once
#include "HWEnv.h"

namespace GBEmu::HW
{
	namespace HWFrame
	{
		// ハードウェアの1フレームをエミュレート
		void EmulateFrame(HWEnv& env);
	};
}
