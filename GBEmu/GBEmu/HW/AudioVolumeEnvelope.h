#pragma once

namespace GBEmu::HW
{
	class AudioVolumeEnvelope
	{
	public:
		void Step();
		void Trigger();

		void WriteNRx2(uint8 nr);
		uint8 ReadNRx2() const;

		uint8 Volume() const { return m_volume; }
		uint8 InitialVolume() const { return m_initialVolume; }
	private:
		uint8 m_initialVolume{};
		uint8 m_direction{};
		uint8 m_period{};

		uint8 m_volume{};
		uint8 m_timer{};
	};
}
