#include "stdafx.h"
#include "AudioFrequencySweep.h"

namespace GBEmu::HW
{
	constexpr int freqMax_2047 = 2047;

	bool AudioFrequencySweep::Step(AudioFrequency& freq)
	{
		if (m_timer != 0)
		{
			m_timer--;
			if (m_timer != 0) return true;
		}

		// timerが0のとき

		m_timer = m_period != 0 ? m_period : 8;

		const bool canUpdateFreq = m_enabled && m_period != 0;
		if (canUpdateFreq == false) return true;

		const auto newFreq = calcNewFreq(m_freqShadow, m_direction, m_shift);

		if (newFreq() > freqMax_2047) return false; // チャンネル無効

		if (newFreq() <= freqMax_2047 && m_shift != 0)
		{
			freq.SetFreq(newFreq);
			m_freqShadow = newFreq;

			// 再度オーバーフローチェック (書き戻し無し)
			if (calcNewFreq(m_freqShadow, m_direction, m_shift)() > freqMax_2047) return false; // チャンネル無効
		}

		return true;
	}

	bool AudioFrequencySweep::Trigger(const AudioFrequency& freq)
	{
		m_freqShadow = freq;
		m_timer = (m_period != 0) ? m_period : 8;
		m_enabled = m_period != 0 || m_shift != 0;
		if (m_shift != 0 && calcNewFreq(m_freqShadow, m_direction, m_shift)() > freqMax_2047) return false; // チャンネル無

		return true;
	}

	void AudioFrequencySweep::WriteNR10(uint8 nr)
	{
		m_period = (nr >> 4) & 0b111; // bit 6-4
		m_direction = (nr >> 3) &0b1; // bit 3
		m_shift = nr & 0b111; // bit 2-0
	}

	uint8 AudioFrequencySweep::ReadNR10() const
	{
		return (m_period << 4) | (m_direction << 3) | (m_shift);
	}

	AudioFrequency AudioFrequencySweep::calcNewFreq(AudioFrequency freqShadow, uint8 direction, uint8 shift)
	{
		return direction == 0
			? AudioFrequency(freqShadow() + (freqShadow() >> shift))
			: AudioFrequency(freqShadow() - (freqShadow() >> shift));
	}
}
