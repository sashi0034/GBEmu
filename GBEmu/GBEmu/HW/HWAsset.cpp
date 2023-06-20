#include "stdafx.h"
#include "HWAsset.h"

namespace GBEmu::HW
{
	HWAsset* HWAsset::globalInstance = nullptr;

	HWAsset& HWAsset::Instance()
	{
		return *globalInstance;
	}

	HWAsset::HWAsset()
	{
		if (globalInstance == nullptr) globalInstance = this;
	}

	HWAsset::~HWAsset()
	{
		if (globalInstance == this) globalInstance = nullptr;
	}
}
