#pragma once

namespace GBEmu::UI
{
	class UIAsset
	{
	public:
		const Font Font{ 16, Typeface::Bold };

		static UIAsset& Instance();
		UIAsset();
		~UIAsset();
	private:
		static UIAsset* globalInstance;
	};
}
