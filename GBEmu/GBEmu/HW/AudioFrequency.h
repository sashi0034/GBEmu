#pragma once

namespace GBEmu::HW
{
	class AudioFrequency
	{
	public:
		explicit AudioFrequency() = default;
		explicit AudioFrequency(int freq) : m_freq(freq) { return; }

		void SetFreq(AudioFrequency freq) { m_freq = freq(); }
		void LowerFreq(uint8 lower);
		void HigherFreq(uint8 higher);

		int operator()() const { return m_freq; }

	private:
		int m_freq{};
	};
}
