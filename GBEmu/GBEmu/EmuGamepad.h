#pragma once

namespace GBEmu
{
	enum class EmuGamepadKey
	{
		Right,
		Left,
		Up,
		Down,
		A,
		B,
		Select,
		Start,
		Max
	};

	class EmuGamepad
	{
	public:
		EmuGamepad();
		static EmuGamepad& Instance();

		bool IsPressed(EmuGamepadKey key);

	private:
		std::array<InputGroup, static_cast<int>(EmuGamepadKey::Max)> m_inputGroups;
		InputGroup m_inputModifier{};

		void applyGamepad(detail::Gamepad_impl gamepad);
		InputGroup& inputOf(EmuGamepadKey key);
	};
}
