#pragma once

namespace GBEmu::HW
{
	class AudioLengthCounter
	{
	public:
		[[nodiscard]] bool Step();
		void Trigger(uint8 newCounter);

		bool IsEnabled() const { return m_enabled; }
		void SetEnabled(bool enabled);

		void SetCounter(uint8 counter) { m_counter = counter; }
		uint8 GetCounter() const { return m_counter; }

	private:
		uint8 m_counter{};
		bool m_enabled{};
	};
}
