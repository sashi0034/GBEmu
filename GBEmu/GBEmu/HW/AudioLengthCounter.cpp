#include "stdafx.h"
#include "AudioLengthCounter.h"

namespace GBEmu::HW
{
	bool AudioLengthCounter::Step()
	{
		if (m_enabled == false) return true;

		if (m_counter == 0) return true;
		m_counter--;

		if (m_counter == 0) return false; // チャンネル無効
		return true;
	}

	void AudioLengthCounter::Trigger(uint8 newCounter)
	{
		if (m_counter == 0) m_counter = newCounter;
	}

	void AudioLengthCounter::SetEnabled(bool enabled)
	{
		m_enabled = enabled;
	}
}
