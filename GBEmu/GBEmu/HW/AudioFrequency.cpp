#include "stdafx.h"
#include "AudioFrequency.h"

namespace GBEmu::HW
{
	void AudioFrequency::LowerFreq(uint8 lower)
	{
		m_freq = (m_freq & 0x700) | lower;
	}

	void AudioFrequency::HigherFreq(uint8 higher)
	{
		m_freq = ((higher & 0x7) << 8) | (m_freq & 0xFF);
	}
}
