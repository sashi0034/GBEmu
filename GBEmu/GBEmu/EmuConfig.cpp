#include "stdafx.h"
#include "EmuConfig.h"

#include "EmuSingleton.h"

namespace GBEmu
{
	static const HashSet<String> keyModifier = {
		U"Shift",
		U"Control",
		U"Alt",
		U"LShift",
		U"RShift",
		U"LControl",
		U"RControl",
		U"LAlt",
		U"RAlt",};

	static const HashTable<String, Input> keyInputTable =
	{
		{ U"Cancel", KeyCancel },
		{ U"Backspace", KeyBackspace },
		{ U"Tab", KeyTab },
		{ U"Clear", KeyClear },
		{ U"Enter", KeyEnter },
		{ U"Shift", KeyShift },
		{ U"Control", KeyControl },
		{ U"Alt", KeyAlt },
		{ U"Pause", KeyPause },
		{ U"Escape", KeyEscape },
		{ U"Space", KeySpace },
		{ U"PageUp", KeyPageUp },
		{ U"PageDown", KeyPageDown },
		{ U"End", KeyEnd },
		{ U"Home", KeyHome },
		{ U"Left", KeyLeft },
		{ U"Up", KeyUp },
		{ U"Right", KeyRight },
		{ U"Down", KeyDown },
		{ U"PrintScreen", KeyPrintScreen },
		{ U"Insert", KeyInsert },
		{ U"Delete", KeyDelete },
		{ U"0", Key0 },
		{ U"1", Key1 },
		{ U"2", Key2 },
		{ U"3", Key3 },
		{ U"4", Key4 },
		{ U"5", Key5 },
		{ U"6", Key6 },
		{ U"7", Key7 },
		{ U"8", Key8 },
		{ U"9", Key9 },
		{ U"A", KeyA },
		{ U"B", KeyB },
		{ U"C", KeyC },
		{ U"D", KeyD },
		{ U"E", KeyE },
		{ U"F", KeyF },
		{ U"G", KeyG },
		{ U"H", KeyH },
		{ U"I", KeyI },
		{ U"J", KeyJ },
		{ U"K", KeyK },
		{ U"L", KeyL },
		{ U"M", KeyM },
		{ U"N", KeyN },
		{ U"O", KeyO },
		{ U"P", KeyP },
		{ U"Q", KeyQ },
		{ U"R", KeyR },
		{ U"S", KeyS },
		{ U"T", KeyT },
		{ U"U", KeyU },
		{ U"V", KeyV },
		{ U"W", KeyW },
		{ U"X", KeyX },
		{ U"Y", KeyY },
		{ U"Z", KeyZ },
		{ U"Num0", KeyNum0 },
		{ U"Num1", KeyNum1 },
		{ U"Num2", KeyNum2 },
		{ U"Num3", KeyNum3 },
		{ U"Num4", KeyNum4 },
		{ U"Num5", KeyNum5 },
		{ U"Num6", KeyNum6 },
		{ U"Num7", KeyNum7 },
		{ U"Num8", KeyNum8 },
		{ U"Num9", KeyNum9 },
		{ U"NumMultiply", KeyNumMultiply },
		{ U"NumAdd", KeyNumAdd },
		{ U"NumEnter", KeyNumEnter },
		{ U"NumSubtract", KeyNumSubtract },
		{ U"NumDecimal", KeyNumDecimal },
		{ U"NumDivide", KeyNumDivide },
		{ U"F1", KeyF1 },
		{ U"F2", KeyF2 },
		{ U"F3", KeyF3 },
		{ U"F4", KeyF4 },
		{ U"F5", KeyF5 },
		{ U"F6", KeyF6 },
		{ U"F7", KeyF7 },
		{ U"F8", KeyF8 },
		{ U"F9", KeyF9 },
		{ U"F10", KeyF10 },
		{ U"F11", KeyF11 },
		{ U"F12", KeyF12 },
		{ U"F13", KeyF13 },
		{ U"F14", KeyF14 },
		{ U"F15", KeyF15 },
		{ U"F16", KeyF16 },
		{ U"F17", KeyF17 },
		{ U"F18", KeyF18 },
		{ U"F19", KeyF19 },
		{ U"F20", KeyF20 },
		{ U"F21", KeyF21 },
		{ U"F22", KeyF22 },
		{ U"F23", KeyF23 },
		{ U"F24", KeyF24 },
		{ U"NumLock", KeyNumLock },
		{ U"LShift", KeyLShift },
		{ U"RShift", KeyRShift },
		{ U"LControl", KeyLControl },
		{ U"RControl", KeyRControl },
		{ U"LAlt", KeyLAlt },
		{ U"RAlt", KeyRAlt },
		{ U"NextTrack", KeyNextTrack },
		{ U"PreviousTrack", KeyPreviousTrack },
		{ U"StopMedia", KeyStopMedia },
		{ U"PlayPauseMedia", KeyPlayPauseMedia },
		{ U"Colon_JIS", KeyColon_JIS },
		{ U"Semicolon_US", KeySemicolon_US },
		{ U"Semicolon_JIS", KeySemicolon_JIS },
		{ U"Equal_US", KeyEqual_US },
		{ U"Comma", KeyComma },
		{ U"Minus", KeyMinus },
		{ U"Period", KeyPeriod },
		{ U"Slash", KeySlash },
		{ U"GraveAccent", KeyGraveAccent },
		{ U"Command", KeyCommand },
		{ U"LeftCommand", KeyLeftCommand },
		{ U"RightCommand", KeyRightCommand },
		{ U"LBracket", KeyLBracket },
		{ U"Yen_JIS", KeyYen_JIS },
		{ U"Backslash_US", KeyBackslash_US },
		{ U"RBracket", KeyRBracket },
		{ U"Caret_JIS", KeyCaret_JIS },
		{ U"Apostrophe_US", KeyApostrophe_US },
		{ U"Underscore_JIS", KeyUnderscore_JIS },
	};

	inline std::function<bool()> registerKeymap(const TOMLReader& toml, const String& key)
	{
		Array<std::pair<bool, Input>> keys{};
		for (const auto&& value : toml[key].arrayView())
		{
			bool isModifier = keyModifier.contains(value.getString());
			keys.push_back(std::pair{isModifier, keyInputTable.at(value.getString())});
		}

		return std::function{[keys]()
		{
			for (const auto& [isModifier, input] : keys)
			{
				if (isModifier && input.pressed() == false) return false;
				if (isModifier == false && input.down() == false) return false;
			}
			return true;
		}};
	}

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

		config.Keymap.SuspendAndPlay = registerKeymap(toml, U"keymap.suspend_and_play");
		config.Keymap.StepFrame = registerKeymap(toml, U"keymap.step_frame");

		return config;
	}

	const EmuConfig& EmuConfig::Instance()
	{
		return EmuSingleton::Instance().Config();
	}
}
