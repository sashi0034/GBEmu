#pragma once
#include "UIBack.h"
#include "UIHud.h"

namespace GBEmu::UI
{
	class UIEnv
	{
	public:
		UIBack& Back() { return m_back; }
		UIHud& Hud() { return m_hud; }

	private:
		UIBack m_back;
		UIHud m_hud;
	};
}
