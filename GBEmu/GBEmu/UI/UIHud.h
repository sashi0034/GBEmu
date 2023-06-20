#pragma once

namespace GBEmu::HW
{
	class HWEnv;
}

namespace GBEmu::UI
{
	class UIEnv;

	class UIHud
	{
	public:
		void DrawLeft(UIEnv& ui, HW::HWEnv& hw, const Point& start, int width);
	private:

	};
}
