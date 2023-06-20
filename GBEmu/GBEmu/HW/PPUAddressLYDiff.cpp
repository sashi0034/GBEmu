#include "stdafx.h"
#include "PPUAddressLYDiff.h"

namespace GBEmu::HW
{
	void PPUAddressLYDiff::Clear()
	{
		m_diffHistory.clear();
	}

	void PPUAddressLYDiff::TrackAddressLY(uint16 address, uint8 ly)
	{
		if (m_diffHistory.size() == 0 || m_diffHistory.back().Address != address)
		{
			m_diffHistory.push_back({address, ly});
		}
	}
}
