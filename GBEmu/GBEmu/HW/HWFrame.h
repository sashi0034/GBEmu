#pragma once
#include "HWEnv.h"

namespace GBEmu::HW
{
	class HWFrame
	{
	public:
		void StepFrame(HWEnv& env);
	private:
		double m_fragmentTime{};
	};
}
