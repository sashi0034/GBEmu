#include "stdafx.h"
#include "UIAsset.h"

#include "GBEmu/EmuSingleton.h"

namespace GBEmu::UI
{
	UIAsset& UIAsset::Instance()
	{
		return EmuSingleton::Instance().UIAsset();
	}
}
