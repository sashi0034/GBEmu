#pragma once
#include "AudioFrequencySweep.h"
#include "AudioLengthCounter.h"
#include "AudioVolumeEnvelope.h"

namespace GBEmu::HW
{
	static constexpr int ch_1 = 1;
	static constexpr int ch_2 = 2;

	template <int ch>
	class AudioChSquare
	{
		static_assert(ch == ch_1 || ch == ch_2);
	public:
		template <int x> void WriteNR(uint8 data);
		template <int x> uint8 ReadNR() const;

		const AudioFrequency& Freq() const {return m_freq; }
	private:
		bool m_channelEnabled{};
		bool m_dacEnabled{};
		uint8 m_duty{};
		AudioFrequency m_freq{};
		AudioLengthCounter m_lengthCounter{};
		std::conditional_t<ch == ch_1, AudioFrequencySweep, void*> m_frequencySweep{}; // チャンネル1はスイープレジスタを持つ
		AudioVolumeEnvelope m_volumeEnvelope{};

		void trigger();
	};

	template <int ch>
	template <int x>
	void AudioChSquare<ch>::WriteNR(uint8 data)
	{
		static_assert(0 <= x && x <= 4);

		if constexpr (x == 0 && ch == ch_1)
		{
			m_frequencySweep.WriteNR10(data);
		}
		else if constexpr (x == 1)
		{
			m_duty = data >> 6;
			m_lengthCounter.SetCounter(64 - (data & 0b111111));
		}
		else if constexpr (x == 2)
		{
			m_volumeEnvelope.WriteNRx2(data);
			m_dacEnabled = (data & 0xF8) != 0; // 上位5ビットが0かどうか
			m_channelEnabled = m_dacEnabled;
		}
		else if constexpr (x == 3)
		{
			m_freq.LowerFreq(data);
		}
		else if constexpr (x == 4)
		{
			if ((data & 0x80) != 0) trigger(); // bit 7
			m_lengthCounter.SetEnabled((data & 0x40) != 0); // bit 6
			m_freq.HigherFreq(data); // bit 2-0
		}
	}

	template <int ch>
	template <int x>
	uint8 AudioChSquare<ch>::ReadNR() const
	{
		static_assert(0 <= x && x <= 4);

		if constexpr (x == 0 && ch == ch_1)
		{
			return m_frequencySweep.ReadNR10();
		}
		else if constexpr (x == 1)
		{
			return (m_duty << 6) | (64 - m_lengthCounter.GetCounter());
		}
		else if constexpr (x == 2)
		{
			return m_volumeEnvelope.ReadNRx2();
		}
		else if constexpr (x == 3)
		{
			return 0; // Write Only
		}
		else if constexpr (x == 4)
		{
			return m_lengthCounter.IsEnabled() << 6; // 読み込みは6ビットのみ可能
		}
		else
		{
			assert(false); return 0;
		}
	}

	template <int ch>
	void AudioChSquare<ch>::trigger()
	{
		if (m_dacEnabled) m_channelEnabled = true;

		if constexpr (ch == ch_1) m_channelEnabled &= m_frequencySweep.Trigger(m_freq);
		m_volumeEnvelope.Trigger();
		m_lengthCounter.Trigger(64);
	}
}
