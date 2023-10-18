#pragma once

namespace GBEmu
{
	struct EmuConfig
	{
		struct
		{
			String Path{};
		} Cartridge;

		struct
		{
			std::array<ColorF, 4> Palette{};
		} DMG;

		struct
		{
			std::function<bool()> SuspendAndPlay{};
			std::function<bool()> StepFrame{};
		} Keymap;

		static EmuConfig LoadToml(const FilePath& path, const FilePath& alternativePath);
		static const EmuConfig& Instance();
	};
}
