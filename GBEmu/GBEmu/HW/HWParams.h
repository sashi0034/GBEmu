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

		constexpr Size DisplayResolution{160, 144};

		constexpr uint16 VRamSize_0x2000 = 0x2000;
	};
}
