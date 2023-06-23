#include "stdafx.h"
#include "Joypad.h"

#include "Memory.h"
#include "MemoryAddress.h"
#include "GBEmu/EmuGamepad.h"

namespace GBEmu::HW
{
	void Joypad::Update(Memory& memory, uint8 control)
	{
		m_controlBefore = control;
		memory.WriteDirect(MemoryAddress::JOYP_0xFF00, fetchInput(control));
	}

	uint8 Joypad::fetchInput(uint8 control)
	{
		auto&& pad = EmuGamepad::Instance();

		if ((control & (1 << 4)) == 0)
		{
			return 0b11100000 |
				(pad.IsPressed(EmuGamepadKey::Right) ? 0 : 1 << 0) |
				(pad.IsPressed(EmuGamepadKey::Left) ? 0 : 1 << 1) |
				(pad.IsPressed(EmuGamepadKey::Up) ? 0 : 1 << 2) |
				(pad.IsPressed(EmuGamepadKey::Down) ? 0 : 1 << 3);
		}
		else if ((control & (1 << 5)) == 0)
		{
			return 0b11010000 |
				(pad.IsPressed(EmuGamepadKey::A) ? 0 : 1 << 0) | // A
				(pad.IsPressed(EmuGamepadKey::B) ? 0 : 1 << 1) | // B
				(pad.IsPressed(EmuGamepadKey::Select) ? 0 : 1 << 2) | // Select
				(pad.IsPressed(EmuGamepadKey::Start) ? 0 : 1 << 3); // Start
		}
		else
		{
			return 0xCF;
		}
	}
}
