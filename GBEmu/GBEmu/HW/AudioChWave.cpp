#include "stdafx.h"
#include "AudioChWave.h"

#include "MemoryAddress.h"

namespace GBEmu::HW
{
	void AudioChWave::StepFreqTimer()
	{
		m_freqTimer--;
		if (m_freqTimer <= 0)
		{
			m_freqTimer = (2048 - m_freq()) * 2;
			m_waveRamOffset = (m_waveRamOffset + 1) % 32;
		}
	}

	void AudioChWave::StepLengthCounter()
	{
		m_channelEnabled &= m_lengthCounter.Step();
	}

	int AudioChWave::Amplitude() const
	{
		const uint8 data = m_waveRam[m_waveRamOffset / 2];
		const uint8 wave = (data >> ((1 - (m_waveRamOffset % 2)) * 4)) & 0xF;
		constexpr int shift[4] = {4, 0, 1, 2};
		return wave >> shift[m_waveOutputLevel];
	}

	void AudioChWave::WriteWaveRam(uint16 addr, uint8 data)
	{
		m_waveRam[addr - MemoryAddress::WaveRamStart_0xFF30] = data;
	}

	uint8 AudioChWave::ReadWaveRam(uint16 addr) const
	{
		return m_waveRam[addr - MemoryAddress::WaveRamStart_0xFF30];
	}

	void AudioChWave::trigger()
	{
		if (m_dacEnabled) m_channelEnabled = true;

		m_lengthCounter.Trigger(lengthCounterMax_255);

		m_waveRamOffset = 1;
	}
}
