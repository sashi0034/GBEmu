#include "stdafx.h"
#include "APU.h"

#include "HWEnv.h"
#include "HWParams.h"

namespace GBEmu::HW
{
	class HWDebugger;

	APU::APU() :
		m_stream(std::make_shared<APUStream>()),
		m_audio(Audio(m_stream))
	{}

	void APU::UpdateFrame(HWEnv& env)
	{
		// NR52電源制御
		if (m_powerControl == false)
		{
			if (m_audio.isPlaying()) m_audio.pause();
		}
		else
		{
			if (m_audio.isPlaying() == false) m_audio.play();
		}
	}

	void APU::StepCycle(HWEnv& env)
	{
		// NR52電源制御
		if (m_powerControl == false) return;

		// 各チャンネルの更新チェック
		m_ch1.StepFreqTimer();
		m_ch2.StepFreqTimer();
		m_ch3.StepFreqTimer();
		m_ch4.StepFreqTimer();
		m_frameSequencer.StepCycle(m_ch1, m_ch2, m_ch3, m_ch4);

		m_outputTimer += 1.0f;
		constexpr float outputPeriod = 1.0f * HWParam::ClockFrequencyDMG_4194304 / HWParam::AudioSampleRate; // 95.1
		if (m_outputTimer >= outputPeriod)
		{
			m_outputTimer -= outputPeriod;

			pushSample(env.Debugger());
		}
	}

	void APU::pushSample(HWDebugger& debugger) const
	{
		// バッファが十分あるなら追加しない
		constexpr int sufficientBuffer = HWParam::AudioSampleRate / 2;
		if (m_stream->BufferRemaining() > sufficientBuffer) return;

		const std::array<int, 4> amplitudes {
			m_ch1.Amplitude() * m_ch1.ChannelEnabled(),
			m_ch2.Amplitude() * m_ch2.ChannelEnabled(),
			m_ch3.Amplitude() * m_ch3.ChannelEnabled(),
			m_ch4.Amplitude() * m_ch4.ChannelEnabled() };

		// マスターボリューム
		const float leftVol = ((m_channelControl >> 4) & 0b111) + 1;
		const float rightVol = ((m_channelControl >> 0) & 0b111) + 1;
		constexpr float maxVol = 8.0f;

		// DAC (digital-to-analog convertor)
		float leftAmp = 0;
		float rightAmp = 0;
		for (int i=0; i<4; i++)
		{
			// 0の入力は-1.0を生成し、15の入力は+1.0を生成といった比例した出力にする
			const float amp = (amplitudes[i] / 7.5f) - 1.0f;
			if ((m_selection >> (i + 4)) & 0b1) leftAmp += amp;
			if ((m_selection >> i) & 0b1) rightAmp += amp;

			debugger.OnAudioSample(i + 1, amp * ((leftVol + rightVol) / (maxVol * 2)));
		}
		// 各チャンネルのAmplitudeは多分[-1, 1]に収まっているので4が最大
		constexpr float maxAmp = 4.0f;

		m_stream->PushSample((leftAmp / maxAmp) * (leftVol / maxVol), (rightAmp / maxAmp) * (rightVol / maxVol));
	}
}
