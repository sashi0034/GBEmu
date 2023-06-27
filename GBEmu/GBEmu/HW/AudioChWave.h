#pragma once
#include "AudioFrequency.h"
#include "AudioLengthCounter.h"

namespace GBEmu::HW
{
	class AudioChWave
	{
	public:
		void StepFreqTimer();
		void StepLengthCounter();

		int Amplitude() const;
		bool ChannelEnabled() const { return m_channelEnabled; }

		template <int x> void WriteNR3x(uint8 data);
		template <int x> uint8 ReadNR3x() const;

		void WriteWaveRam(uint16 addr, uint8 data);
		uint8 ReadWaveRam(uint16 addr) const;
	private:
		static constexpr uint8 lengthCounterMax_255 = 255;

		bool m_channelEnabled{};
		bool m_dacEnabled{};
		AudioLengthCounter m_lengthCounter;

		int m_freqTimer{};
		AudioFrequency m_freq{};

		std::array<uint8, 16> m_waveRam{};
		uint8 m_waveOutputLevel{};
		uint8 m_waveRamOffset{};

		void trigger();
	};

	template <int x>
	void AudioChWave::WriteNR3x(uint8 data)
	{
		static_assert(0 <= x && x <= 4);

		if constexpr (x==0)
		{
			m_dacEnabled = (data >> 7) != 0;
			if (m_dacEnabled == false) m_channelEnabled = false;
		}
		else if constexpr (x==1)
		{
			m_lengthCounter.SetCounter(lengthCounterMax_255 - data);
		}
		else if constexpr (x==2)
		{
			m_waveOutputLevel = (data >> 5) & 0b11; // bit 6-5
		}
		else if constexpr (x==3)
		{
			m_freq.LowerFreq(data);
		}
		else if constexpr (x==4)
		{
			if ((data & 0x80) != 0) trigger(); // bit 7
			m_lengthCounter.SetEnabled((data & 0x40) != 0); // bit 6 (R/W)
			m_freq.HigherFreq(data); // bit 2-0
		}
	}

	template <int x>
	uint8 AudioChWave::ReadNR3x() const
	{
		static_assert(0 <= x && x <= 4);

		if constexpr (x==0)
		{
			return m_dacEnabled << 7;
		}
		else if constexpr (x==1)
		{
			return lengthCounterMax_255 - m_lengthCounter.GetCounter();
		}
		else if constexpr (x==2)
		{
			return m_waveOutputLevel << 5;
		}
		else if constexpr (x==3)
		{
			return 0; // write only
		}
		else if constexpr (x==4)
		{
			return m_lengthCounter.IsEnabled() << 6;
		}
	}
}
