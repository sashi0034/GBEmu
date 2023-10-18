#include "stdafx.h"
#include "Joypad.h"

#include "GBEmu/EmuGamepad.h"

namespace GBEmu::HW
{
	void Joypad::WriteJOYP(uint8 joyp)
	{
		if ((joyp & (1 << 4)) == 0)
		{
			m_getter = []() -> uint8
			{
				auto&& pad = EmuGamepad::Instance();
				return 0b11100000 |
					(pad.IsPressed(EmuGamepadKey::Right) ? 0 : 1 << 0) |
					(pad.IsPressed(EmuGamepadKey::Left) ? 0 : 1 << 1) |
					(pad.IsPressed(EmuGamepadKey::Up) ? 0 : 1 << 2) |
					(pad.IsPressed(EmuGamepadKey::Down) ? 0 : 1 << 3);
			};
		}
		else if ((joyp & (1 << 5)) == 0)
		{
			m_getter = []() -> uint8
			{
				auto&& pad = EmuGamepad::Instance();
				return 0b11010000 |
					(pad.IsPressed(EmuGamepadKey::A) ? 0 : 1 << 0) | // A
					(pad.IsPressed(EmuGamepadKey::B) ? 0 : 1 << 1) | // B
					(pad.IsPressed(EmuGamepadKey::Select) ? 0 : 1 << 2) | // Select
					(pad.IsPressed(EmuGamepadKey::Start) ? 0 : 1 << 3); // Start
			};
		}

		m_joyp = m_getter();
	}
}
