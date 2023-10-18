#pragma once

namespace GBEmu::UI
{
	class UIAsset : Uncopyable
	{
	public:
		const Font FontDotGothic18{18, U"asset/font/DotGothic16/DotGothic16-Regular.ttf", FontStyle::Bold};
		const Font FontRocknRollOne24{24, U"asset/font/RocknRoll/RocknRollOne-Regular.ttf", FontStyle::Bold};

		static UIAsset& Instance();
	};
}
