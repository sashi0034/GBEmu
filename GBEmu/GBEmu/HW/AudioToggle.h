#pragma once

namespace GBEmu
{
	class AudioToggle
	{
	public:
		bool IsEnableSound() const { return m_isEnableSound; };
		void SetSound(bool isEnable) { m_isEnableSound = isEnable; };

		bool IsEnableDAC() const { return m_isEnableDAC; }
		void SetDAC(bool isEnable) { m_isEnableDAC = isEnable; };
	private:
		bool m_isEnableSound{};
		bool m_isEnableDAC{};
	};
}
