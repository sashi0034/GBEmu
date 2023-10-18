#pragma once
#include "HWEnv.h"

namespace GBEmu::HW
{
	class HWFrame
	{
	public:
		void ControlFrame(HWEnv& env);
		bool IsSuspended() const { return m_isSuspended; }

	private:
		double m_fragmentTime{};
		bool m_isSuspended{};
	};
}
