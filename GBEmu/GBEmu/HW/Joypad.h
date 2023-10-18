#pragma once

namespace GBEmu::HW
{
	class Memory;

	class Joypad
	{
	public:
		uint8 ReadJOYP() const { return m_joyp; }
		void WriteJOYP(uint8 joyp);

	private:
		uint8 m_joyp{};
		uint8 (*m_getter)() = []() { return static_cast<uint8>(0); };
	};
}
