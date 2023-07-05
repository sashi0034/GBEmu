#pragma once

#include "AudioFrequency.h"
#include "AudioLengthCounter.h"
#include "AudioVolumeEnvelope.h"

namespace GBEmu::HW
{
	class AudioChNoise
	{
	public:
		void StepFreqTimer();
		void StepVolumeEnvelope();
		void StepLengthCounter();
		int Amplitude() const;
		bool ChannelEnabled() const { return m_channelEnabled; }

		template <int x> void WriteNR4x(uint8 data);
		template <int x> uint8 ReadNR4x() const;
	private:
		static constexpr uint8 lengthCounterMax_63 = 63; // 0x3F

		bool m_channelEnabled{};
		bool m_dacEnabled{};
		int m_freqTimer{};
		AudioFrequency m_freq{};

		uint8 m_shift{};
		bool m_w{};
		uint8 m_divisor{};
		uint16 m_lfsr{};

		AudioLengthCounter m_lengthCounter{};
		AudioVolumeEnvelope m_volumeEnvelope{};

		void trigger();
	};

	template <int x>
	void AudioChNoise::WriteNR4x(uint8 data)
	{
		if constexpr (x==1)
		{
			m_lengthCounter.SetCounter(lengthCounterMax_63 - (data & lengthCounterMax_63)); // bit 5-0
		}
		else if constexpr (x==2)
		{
			m_volumeEnvelope.WriteNRx2(data);
			m_dacEnabled = (data & 0xF8) != 0;
			if (m_dacEnabled == false) m_channelEnabled = false;
		}
		else if constexpr (x==3)
		{
			m_shift = data >> 4; // bit 7-4
			m_w = (data >> 3) & 0b1; // bit 3
			m_divisor = data & 0b111; // bit 2-0
		}
		else if constexpr (x==4)
		{
			if (data & 0x80) trigger(); // write only
			m_lengthCounter.SetEnabled((data & 0x40) != 0);
		}
	}

	template <int x>
	uint8 AudioChNoise::ReadNR4x() const
	{
		if constexpr (x==1)
		{
			return lengthCounterMax_63 - m_lengthCounter.GetCounter();
		}
		else if constexpr (x==2)
		{
			return m_volumeEnvelope.ReadNRx2();
		}
		else if constexpr (x==3)
		{
			return (m_shift << 4) | (m_w << 3) | m_divisor;
		}
		else if constexpr (x==4)
		{
			return m_lengthCounter.IsEnabled() << 6;
		}
	}
}
