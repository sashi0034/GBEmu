#include "stdafx.h"
#include "UIAsset.h"

namespace GBEmu::UI
{
	UIAsset* UIAsset::globalInstance = nullptr;

	UIAsset& UIAsset::Instance()
	{
		return *globalInstance;
	}

	UIAsset::UIAsset()
	{
		if (globalInstance == nullptr) globalInstance = this;
	}

	UIAsset::~UIAsset()
	{
		if (globalInstance == this) globalInstance = nullptr;
	}
}
