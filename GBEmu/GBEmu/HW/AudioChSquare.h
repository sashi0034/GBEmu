#pragma once
#include "AudioToggle.h"

namespace GBEmu::HW
{
	class AudioChSquare
	{
	public:
		AudioToggle& Toggle() { return m_toggle; }
	private:
		AudioToggle m_toggle;
	};
}
