#include "stdafx.h"
#include "HWAsset.h"

#include "GBEmu/EmuSingleton.h"

namespace GBEmu::HW
{
	HWAsset& HWAsset::Instance()
	{
		return EmuSingleton::Instance().HWAsset();
	}
}
