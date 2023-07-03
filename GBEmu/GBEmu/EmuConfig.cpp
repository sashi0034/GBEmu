#include "stdafx.h"
#include "EmuConfig.h"

#include "EmuSingleton.h"

namespace GBEmu
{
	EmuConfig EmuConfig::LoadToml(const FilePath& path, const FilePath& alternativePath)
	{
		TOMLReader toml{ path };
		if (not toml)
		{
			toml = TOMLReader(alternativePath);
		}

		auto config = EmuConfig{};

		config.Cartridge.Path = toml[U"cartridge.path"].getString();

		for (int i=0; i<4; ++i)
			config.DMG.Palette[i] = toml[U"dmg.palette"].arrayView()[i].get<Color>();

		return config;
	}

	const EmuConfig& EmuConfig::Instance()
	{
		return EmuSingleton::Instance().Config();
	}
}
