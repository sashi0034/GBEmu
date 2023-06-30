#pragma once
#include "HWEnv.h"

namespace GBEmu::HW
{
	class HWFrame
	{
	public:
		void ControlFrame(HWEnv& env);
	private:
		double m_fragmentTime{};
	};
}
