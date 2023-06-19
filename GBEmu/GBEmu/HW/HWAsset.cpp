#include "stdafx.h"
#include "HWAsset.h"

namespace GBEmu::HW
{
	HWAsset* HWAsset::m_instance = nullptr;

	HWAsset& HWAsset::Instance()
	{
		return *m_instance;
	}

	void HWAsset::CreateInstance()
	{
		m_instance = new HWAsset();
	}

	void HWAsset::DestroyInstance()
	{
		delete m_instance;
	}
}
