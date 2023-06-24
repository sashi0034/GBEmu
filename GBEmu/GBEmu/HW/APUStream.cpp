#include "stdafx.h"
#include "APUStream.h"

#include "HWParams.h"

namespace GBEmu::HW
{
	APUStream::APUStream()
	{
		m_wave.resize(HWParam::AudioSampleRate);
	}

	void APUStream::getAudio(float* left, float* right, size_t samplesToWrite)
	{
		const int buffer = BufferRemaining();
		const int writable = std::min(static_cast<int>(samplesToWrite), buffer);

		// バッファの内容をストリームする
		for (int i=0; i<writable; ++i)
		{
			*left = m_wave[m_headIndex].left;
			*right = m_wave[m_headIndex].right;
			left++;
			right++;
			m_headIndex = (m_headIndex + 1) % m_wave.size();
		}

		// samplesToWriteの空いている部分は0でパディング
		for (int i=writable; i<samplesToWrite; ++i)
		{
			*left = 0;
			*right = 0;
			left++;
			right++;
		}
	}

	int APUStream::BufferRemaining() const
	{
		return m_headIndex <= m_tailIndex ?
			// tailもheadも折り返していないとき
			m_tailIndex - m_headIndex :
			// tailだけ折り返している状態のとき
			m_tailIndex + (m_wave.size() - m_headIndex);
	}

	int APUStream::BufferSize() const
	{
		return m_wave.size();
	}
}
