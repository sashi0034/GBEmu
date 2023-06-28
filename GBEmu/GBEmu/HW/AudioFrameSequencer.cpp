#include "stdafx.h"
#include "AudioFrameSequencer.h"

#include "HWParams.h"

namespace GBEmu::HW
{
	constexpr int timerFrequency_512 = 512;

	void stepLengthCounter(AudioChSquare<1>& ch1, AudioChSquare<2>& ch2, AudioChWave& ch3, AudioChNoise& ch4)
	{
		ch1.StepLengthCounter();
		ch2.StepLengthCounter();
		ch3.StepLengthCounter();
		ch4.StepLengthCounter();
	}

	void AudioFrameSequencer::StepCycle(AudioChSquare<1>& ch1, AudioChSquare<2>& ch2, AudioChWave& ch3, AudioChNoise& ch4)
	{
		constexpr int timerMax = HWParam::ClockFrequencyDMG_4194304 / timerFrequency_512; // 8192

		m_timer++;

		if (m_timer < timerMax) return;

		// timerが閾値に達した
		m_timer = 0;
		m_clock = (m_clock + 1) % 8;

		switch (m_clock)
		{
		case 0:
			stepLengthCounter(ch1, ch2, ch3, ch4);
			break;
		case 1:
			break;
		case 2:
			stepLengthCounter(ch1, ch2, ch3, ch4);
			ch1.StepFreqSweep();
			break;
		case 3:
			break;
		case 4:
			stepLengthCounter(ch1, ch2, ch3, ch4);
			break;
		case 5:
			break;
		case 6:
			stepLengthCounter(ch1, ch2, ch3, ch4);
			ch1.StepFreqSweep();
			break;
		case 7:
			ch1.StepVolumeEnvelope();
			ch2.StepVolumeEnvelope();
			ch4.StepVolumeEnvelope();
			break;
		default:
			break;
		}
	}
}
