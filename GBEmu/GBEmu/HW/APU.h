#pragma once
#include "APUStream.h"

namespace GBEmu::HW
{
	class HWEnv;

	// 参考: https://github.com/voidproc/dmge/tree/main/dmge/APU
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
