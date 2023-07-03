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

		} KeyMap;

		static EmuConfig LoadToml(const FilePath& path, const FilePath& alternativePath);
		static const EmuConfig& Instance();
	};

}
