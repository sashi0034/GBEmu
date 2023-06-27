#pragma once
#include "AudioChNoise.h"
#include "AudioChSquare.h"
#include "AudioChWave.h"

namespace GBEmu::HW
{
	class AudioFrameSequencer
	{
	public:
		void StepCycle(AudioChSquare<1>& ch1, AudioChSquare<2>& ch2, AudioChWave& ch3, AudioChNoise& ch4);
	private:
		uint32 m_timer{};
		uint8 m_clock{}; // 0~7
	};
}
