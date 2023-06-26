#pragma once
#include "AudioFrequency.h"

namespace GBEmu::HW
{
	class AudioFrequencySweep
	{
	public:
		[[nodiscard]] bool Step(AudioFrequency& freq);
		[[nodiscard]] bool Trigger(const AudioFrequency& freq);

		void WriteNR10(uint8 nr);
		uint8 ReadNR10() const;
	private:
		uint8 m_period{};
		uint8 m_direction{};
		uint8 m_shift{};

		uint8 m_timer{};
		AudioFrequency m_freqShadow{};
		bool m_enabled{};
		static AudioFrequency calcNewFreq(AudioFrequency freqShadow, uint8 direction, uint8 shift);
	};
}
