#include "stdafx.h"
#include "UIBack.h"

namespace GBEmu::UI
{
	void UIBack::Draw()
	{
		Rect{ Scene::Size() }.draw(Color(32, 32,32));
		return;

		m_count++;

		// 上下グラデーション
		Rect{ Scene::Size() }.draw(
			Arg::top = Color{ 0, 88, 116 },
			Arg::bottom = Color{ 8, 32, 64 });

		// ドット模様
		for (int y=0; y<Scene::Size().y; y += 32)
		{
			const double animRate = (((m_count + y) % 120) / 120.0);
			for (int x = 0; x< Scene::Size().x; x += 32)
			{
				Circle{ x, y, 1 }.draw(Color{
					static_cast<uint8>(96 + 40 * animRate), static_cast<uint8>(120 + 40 * animRate), static_cast<uint8>(200 + 40 * animRate) });
			}
		}
	}
}
