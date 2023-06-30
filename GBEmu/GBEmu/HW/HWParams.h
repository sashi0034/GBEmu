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

		// https://lospec.com/palette-list/tag/gameboy/
		constexpr std::array ColorPalettesDMG{
			// default
			std::array<ColorF, 4>{
				Color{ U"#ddffd4" },
				Color{ U"#90ee90" },
				Color{ U"#1d723d" },
				Color{ U"#003300" },
			},
			// KIROKAZE GAMEBOY Created by Kirokaze
			std::array<ColorF, 4>{
				Color{ U"#e2f3e4" },
				Color{ U"#94e344" },
				Color{ U"#46878f" },
				Color{ U"#332c50" },
			},
			// COLDFIRE GB Created by Kerrie Lake
			std::array<ColorF, 4>{
				Color{ U"#f6c6a8" },
				Color{ U"#d17c7c" },
				Color{ U"#5b768d" },
				Color{ U"#46425e" },
			},
			// MINTY FRESH Created by archer
			std::array<ColorF, 4>{
				Color{ U"#fbffe0" },
				Color{ U"#95c798" },
				Color{ U"#856d52" },
				Color{ U"#40332f" },
			},
		};
	};
}
