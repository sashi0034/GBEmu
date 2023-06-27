#pragma once
#include "APUStream.h"
#include "AudioChNoise.h"
#include "AudioChSquare.h"
#include "AudioChWave.h"

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

		AudioChSquare<1> m_ch1{};
		AudioChSquare<2> m_ch2{};
		AudioChWave m_ch3{};
		AudioChNoise m_ch4{};
	};
}
