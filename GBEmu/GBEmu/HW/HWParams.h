#pragma once

namespace GBEmu::HW
{
	namespace HWParam
	{
		constexpr int MachineCycle = 4;

		constexpr int InterruptJoypad = 1 << 4;
		constexpr int InterruptSerial = 1 << 3;
		constexpr int InterruptTimer = 1 << 2;
		constexpr int InterruptSTAT = 1 << 1;
		constexpr int InterruptVBlank = 1 << 0;
	};
}
