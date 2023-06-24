#pragma once
#include "APUStream.h"

namespace GBEmu::HW
{
	class HWEnv;

	class APU
	{
	public:
		APU();

		void StepCycle(HWEnv& env);
	private:
		std::shared_ptr<APUStream> m_stream{};
		Audio m_audio{};
	};
}
