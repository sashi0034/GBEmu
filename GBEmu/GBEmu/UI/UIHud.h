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
		void DrawLeft(UIEnv& ui, HW::HWEnv& hw, const Point& start, int width, int height);
		void DrawRight(UIEnv& ui, HW::HWEnv& hw, const Point& start, int width, int height);
		void DrawUp(UIEnv& ui, HW::HWEnv& hw, const Point& bottom);
	};
}
