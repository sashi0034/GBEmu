#include "stdafx.h"
#include "UIBack.h"

namespace GBEmu::UI
{
	void UIBack::Draw()
	{
		m_count++;

		// 上下グラデーション
		Rect{Scene::Size()}.draw(
			Arg::top = Color{32, 40, 32},
			Arg::bottom = Color{16, 24, 16});

		// ドット模様
		constexpr int step = 32;
		for (int y = 0; y < Scene::Size().y; y += step)
		{
			const double animRate = ((m_count + y / step) % 60) / 60.0;
			for (int x = 0; x < Scene::Size().x; x += step)
			{
				Circle{x, y, 1}.draw(Color{
					static_cast<uint8>(64 + 160 * animRate), static_cast<uint8>(196 + 48 * animRate),
					static_cast<uint8>(48 + 64 * animRate)
				});
			}
		}
	}
}
