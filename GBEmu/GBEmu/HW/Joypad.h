#pragma once

namespace GBEmu::HW
{
	class Memory;

	class Joypad
	{
	public:
		void Update(Memory& memory, uint8 control);
		void UpdateFrame(Memory& memory);
	private:
		uint8 m_controlBefore{};

		static uint8 fetchInput(uint8 control);
	};
}
