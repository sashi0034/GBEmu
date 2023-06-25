#include "stdafx.h"
#include "Interruption.h"

namespace GBEmu::HW
{
	uint8 Interruption::IE() const
	{
		return m_ie;
	}

	uint8 Interruption::IF() const
	{
		return m_if;
	}

	void Interruption::SetFlag(InterruptFlag flag)
	{
		m_if = m_if | static_cast<uint8>(flag);
	}

	void Interruption::ResetFlag(InterruptFlag flag)
	{
		m_if = m_if & (~static_cast<uint8>(flag));
	}
}
