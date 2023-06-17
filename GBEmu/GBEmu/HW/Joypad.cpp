#include "stdafx.h"
#include "Joypad.h"

#include "Memory.h"
#include "MemoryAddress.h"

namespace GBEmu::HW
{
	void Joypad::Update(Memory& memory, uint8 control)
	{
		m_controlBefore = control;
		memory.WriteDirect(MemoryAddress::JOYP_0xFF00, fetchInput(control));
	}

	void Joypad::UpdateFrame(Memory& memory)
	{
		// TODO: 除く?
		// Update(memory, m_controlBefore);
	}

	uint8 Joypad::fetchInput(uint8 control)
	{
		if ((control & (1 << 4)) == 0)
		{
			return 0b11100000 |
				(KeyRight.pressed() || KeyD.pressed() ? 0 : 1 << 0) |
				(KeyLeft.pressed() || KeyA.pressed() ? 0 : 1 << 1) |
				(KeyUp.pressed() || KeyW.pressed() ? 0 : 1 << 2) |
				(KeyDown.pressed() || KeyS.pressed() ? 0 : 1 << 3);
		}
		else if ((control & (1 << 5)) == 0)
		{
			return 0b11010000 |
				(KeySlash.pressed() || KeyZ.pressed() ? 0 : 1 << 0) | // A
				(KeyUnderscore_JIS.pressed() || KeyX.pressed() ? 0 : 1 << 1) | // B
				(KeySpace.pressed() ? 0 : 1 << 2) | // Select
				(KeyEnter.pressed() ? 0 : 1 << 3); // Start
		}
		else
		{
			return 0b11010000;
		}
	}
}
