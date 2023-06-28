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
		void StepFreqTimer();
		template<int chan = ch, std::enable_if_t<chan == ch_1, int> = 0> void StepFreqSweep();
		void StepLengthCounter();
		void StepVolumeEnvelope();

		template <int x> void WriteNR(uint8 data);
		template <int x> uint8 ReadNR() const;

		const AudioFrequency& Freq() const {return m_freq; }
		int Amplitude() const;
		bool ChannelEnabled() const { return m_channelEnabled; }
	private:
		static constexpr int lengthCounterMax_63 = 63;

		bool m_channelEnabled{};
		bool m_dacEnabled{};
		uint8 m_dutyIndex{};
		uint8 m_dutyDigit{};
		int m_freqTimer{};
		AudioFrequency m_freq{};

		AudioLengthCounter m_lengthCounter{};
		std::conditional_t<ch == ch_1, AudioFrequencySweep, void*> m_freqSweep{}; // チャンネル1のみスイープレジスタを持つ
		AudioVolumeEnvelope m_volumeEnvelope{};

		void trigger();
	};

	template <int ch>
	void AudioChSquare<ch>::StepFreqTimer()
	{
		m_freqTimer--;
		if (m_freqTimer <= 0)
		{
			m_freqTimer = (2048 - m_freq()) * 4;
			m_dutyDigit = (m_dutyDigit + 1) % 8;
		}
	}

	template <int ch>
	template<int chan = ch, std::enable_if_t<chan == ch_1, int> = 0>
	void AudioChSquare<ch>::StepFreqSweep()
	{
		if constexpr (ch == ch_1)
		{
			m_channelEnabled &= m_freqSweep.Step(m_freq);
		}
	}

	template <int ch>
	void AudioChSquare<ch>::StepLengthCounter()
	{
		 m_channelEnabled &= m_lengthCounter.Step();
	}

	template <int ch>
	void AudioChSquare<ch>::StepVolumeEnvelope()
	{
		m_volumeEnvelope.Step();
	}

	template <int ch>
	template <int x>
	void AudioChSquare<ch>::WriteNR(uint8 data)
	{
		static_assert(0 <= x && x <= 4);

		if constexpr (x == 0 && ch == ch_1)
		{
			m_freqSweep.WriteNR10(data);
		}
		else if constexpr (x == 1)
		{
			m_dutyIndex = data >> 6;
			m_lengthCounter.SetCounter(lengthCounterMax_63 - (data & lengthCounterMax_63));
		}
		else if constexpr (x == 2)
		{
			m_volumeEnvelope.WriteNRx2(data);
			m_dacEnabled = (data & 0xF8) != 0; // 上位5ビットが0かどうか
			if (m_dacEnabled == false) m_channelEnabled = false;
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
			return m_freqSweep.ReadNR10();
		}
		else if constexpr (x == 1)
		{
			return (m_dutyIndex << 6) | (lengthCounterMax_63 - m_lengthCounter.GetCounter());
		}
		else if constexpr (x == 2)
		{
			return m_volumeEnvelope.ReadNRx2();
		}
		else if constexpr (x == 3)
		{
			return 0; // write only
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
	int AudioChSquare<ch>::Amplitude() const
	{
		constexpr std::array<uint8, 4> dutyTable = {
			0b00000001, 0b00000011, 0b00001111, 0b11111100,
		};
		const int wave = (dutyTable[m_dutyIndex] >> (7 - m_dutyDigit)) & 0b1;
		return wave * m_volumeEnvelope.Volume();
	}

	template <int ch>
	void AudioChSquare<ch>::trigger()
	{
		if (m_dacEnabled) m_channelEnabled = true;

		if constexpr (ch == ch_1) m_channelEnabled &= m_freqSweep.Trigger(m_freq);
		m_volumeEnvelope.Trigger();
		m_lengthCounter.Trigger(64);
	}
}
