#pragma once

namespace GBEmu::HW
{
	namespace HWParam
	{
		constexpr int FPS = 60;

		constexpr int MachineCycle = 4;

		constexpr Size DisplayResolution{160, 144};
		constexpr int DisplayHeight_144 = DisplayResolution.y;
		constexpr int DisplayWidth_160 = DisplayResolution.x;

		constexpr uint16 VRamSize_0x2000 = 0x2000;

		constexpr int PPUCyclePeriod_70224 = 70224;

		constexpr int AudioSampleRate = 44100;

		constexpr int AudioChSquare_1 = 1;
		constexpr int AudioChSquare_2 = 2;
		constexpr int AudioChWave_3 = 3;
		constexpr int AudioChNoise_4 = 4;

		constexpr int ClockFrequencyDMG_4194304 = 4194304; // 0x400000
	};
}
