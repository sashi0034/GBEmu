#pragma once

namespace GBEmu::HW
{
	namespace HWParam
	{
		constexpr int MachineCycle = 4;

		constexpr Size DisplayResolution{160, 144};
		constexpr int DisplayHeight_144 = DisplayResolution.y;
		constexpr int DisplayWidth_160 = DisplayResolution.x;

		constexpr uint16 VRamSize_0x2000 = 0x2000;
	};
}
