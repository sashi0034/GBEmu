#pragma once

namespace GBEmu::UI
{
	class UIAsset
	{
	public:
		const Font FontDotGothic18{ 18, U"example/font/DotGothic16/DotGothic16-Regular.ttf", FontStyle::Bold };

		static UIAsset& Instance();
		UIAsset();
		~UIAsset();
	private:
		static UIAsset* globalInstance;
	};
}
