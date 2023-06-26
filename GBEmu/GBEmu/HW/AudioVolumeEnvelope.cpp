#include "stdafx.h"
#include "AudioVolumeEnvelope.h"

namespace GBEmu::HW
{
	void AudioVolumeEnvelope::Step()
	{
		if (m_period == 0) return;

		if (m_timer != 0)
		{
			m_timer--;
			if (m_timer != 0) return;
		}

		// timerが0になったとき、volume更新
		// volumeは0~15に収まる

		m_timer = m_period;

		if ((m_volume < 15 && m_direction == 1) ||
			(m_volume > 0 && m_direction == 0))
		{
			m_volume += (m_direction == 1) ? 1 : -1;
		}
	}

	void AudioVolumeEnvelope::Trigger()
	{
		m_volume = m_initialVolume;
		m_timer = m_period;
	}

	void AudioVolumeEnvelope::WriteNRx2(uint8 nr)
	{
		m_initialVolume = nr >> 4;
		m_direction = (nr >> 3) & 0b1;
		m_period = nr & 0b111;
	}

	uint8 AudioVolumeEnvelope::ReadNRx2() const
	{
		return (m_initialVolume << 4) | (m_direction << 3) | m_period;
	}
}
