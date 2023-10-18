#include "stdafx.h"
#include "AudioChNoise.h"

namespace GBEmu::HW
{
	void AudioChNoise::StepFreqTimer()
	{
		m_freqTimer--;
		if (m_freqTimer <= 0)
		{
			m_freqTimer = (m_divisor != 0 ? (m_divisor * 16) : 8) << m_shift;

			const uint16 t = (m_lfsr & 0b01) ^ ((m_lfsr & 0b10) >> 1);
			m_lfsr = (m_lfsr >> 1) | (t << 14);
			if (m_w == false) return;

			m_lfsr &= ~(1 << 6);
			m_lfsr |= t << 6;
		}
	}

	void AudioChNoise::StepVolumeEnvelope()
	{
		m_volumeEnvelope.Step();
	}

	void AudioChNoise::StepLengthCounter()
	{
		m_channelEnabled &= m_lengthCounter.Step();
	}

	int AudioChNoise::Amplitude() const
	{
		const int a = m_volumeEnvelope.InitialVolume() == 0
			              ? 0
			              : (~m_lfsr) & 0b1;
		return a * m_volumeEnvelope.Volume();
	}

	void AudioChNoise::trigger()
	{
		if (m_dacEnabled) m_channelEnabled = true;

		m_volumeEnvelope.Trigger();
		m_lengthCounter.Trigger(lengthCounterMax_63);
		m_lfsr = 0xFFFF;
	}
}
