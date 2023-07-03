#pragma once
#include "EmuConfig.h"
#include "EmuGamepad.h"
#include "HW/HWAsset.h"
#include "UI/UIAsset.h"

namespace GBEmu
{
	class EmuSingleton
	{
	public:
		EmuSingleton();
		~EmuSingleton();
		static EmuSingleton& Instance(){ return *globalInstance; }

		void RegisterConfig(const EmuConfig& config) {m_config = config;}
		const EmuConfig& Config() {return m_config;}

		EmuGamepad& Gamepad() {return m_emuGamepad;}
		HW::HWAsset& HWAsset(){return m_hwAsset;}
		UI::UIAsset& UIAsset(){return m_uiAsset;}
	private:
		EmuConfig m_config{};
		EmuGamepad m_emuGamepad{};
		HW::HWAsset m_hwAsset{};
		UI::UIAsset m_uiAsset{};

		static EmuSingleton* globalInstance;
	};
}
