#pragma once
#include "IOPort.h"

namespace GBEmu::HW
{
	class IOPort;

	template<int X>
	class AudioLengthCounter
	{
	public:
		void Step(IOPort& ioPort);
	private:
		void setCounter(IOPort& ioPort, uint8 counter);
		uint8 getCounter(IOPort& ioPort);
		bool m_isEnable{};
		static constexpr uint8 counterMask_0x3F = 0b111111;
	};

	template <int X>
	void AudioLengthCounter<X>::Step(IOPort& ioPort)
	{
		uint8 value = ioPort.GetNRX1<X>();
	}

	template <int X>
	void AudioLengthCounter<X>::setCounter(IOPort& ioPort, uint8 counter)
	{
		ioPort.SetNRX1<X>((ioPort.GetNRX1<X>() & (~counterMask_0x3F)) | (64 - counter));
	}

	template <int X>
	uint8 AudioLengthCounter<X>::getCounter(IOPort& ioPort)
	{
		return 64 - (ioPort.GetNRX1<X>() & counterMask_0x3F);
	}
}
