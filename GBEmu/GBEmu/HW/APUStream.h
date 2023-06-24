#pragma once

namespace GBEmu::HW
{
	class APUStream : public IAudioStream
	{
	public:
		APUStream();
		~APUStream() override = default;
		void getAudio(float* left, float* right, size_t samplesToWrite) override;
		bool hasEnded() override { return false; };
		void rewind() override {};

		int BufferRemaining() const;
		int BufferSize() const;
	private:
		Wave m_wave{};
		int m_headIndex{};
		int m_tailIndex{};
	};
}
