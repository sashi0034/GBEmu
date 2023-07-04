#pragma once

namespace GBEmu::HW
{
	class HWFrame;
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
		void DrawUp(UIEnv& ui, HW::HWEnv& hw, const HW::HWFrame& hwFrame, const Point& bottom, const Point& displayTop);
	};
}
