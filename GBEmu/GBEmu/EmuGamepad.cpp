#include "stdafx.h"
#include "EmuGamepad.h"

#include "EmuSingleton.h"

namespace GBEmu
{
	using enum EmuGamepadKey;

	EmuGamepad::EmuGamepad()
	{
		constexpr int playerIndex = 0;

		inputOf(Left) = KeyLeft | KeyA;
		inputOf(Right) = KeyRight | KeyD;
		inputOf(Up) = KeyUp | KeyW;
		inputOf(Down) = KeyDown | KeyS;
		inputOf(A) = KeyUnderscore_JIS | KeyX;
		inputOf(B) = KeySlash | KeyZ;
		inputOf(Select) = KeySpace;
		inputOf(Start) = KeyEnter;
		m_inputModifier = KeyShift | KeyControl | KeyAlt;

		if (const auto gamepad = Gamepad(playerIndex))
		{
			applyGamepad(gamepad);
		}
	}

	void EmuGamepad::applyGamepad(detail::Gamepad_impl gamepad)
	{
		if (gamepad.buttons.size() < 18) return;

		// TODO: いろんなコントローラーに対応
		// 今はHORIPAD Sの配置に対応
		inputOf(Left) = inputOf(Left) | gamepad.buttons[17];
		inputOf(Right) = inputOf(Right) | gamepad.buttons[15];
		inputOf(Up) = inputOf(Up) | gamepad.buttons[14];
		inputOf(Down) = inputOf(Down) | gamepad.buttons[16];
		inputOf(A) = inputOf(A) | gamepad.buttons[2];
		inputOf(B) = inputOf(B) | gamepad.buttons[1];
		inputOf(Select) = inputOf(Select) | gamepad.buttons[8];
		inputOf(Start) = inputOf(Start) | gamepad.buttons[9];
	}

	EmuGamepad& EmuGamepad::Instance()
	{
		return EmuSingleton::Instance().Gamepad();
	}

	bool EmuGamepad::IsPressed(EmuGamepadKey key)
	{
		// 修飾キーが押されていたら、無効
		if (m_inputModifier.pressed()) return false;

		return inputOf(key).pressed();
	}

	InputGroup& EmuGamepad::inputOf(EmuGamepadKey key)
	{
		return m_inputGroups[static_cast<int>(key)];
	}
}
