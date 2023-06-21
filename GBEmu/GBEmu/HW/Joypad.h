#pragma once

namespace GBEmu::HW
{
	class Memory;

	class Joypad
	{
	public:
		void Update(Memory& memory, uint8 control);
	private:
		uint8 m_controlBefore{};

		static uint8 fetchInput(uint8 control);
	};
}
