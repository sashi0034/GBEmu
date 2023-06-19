#include "stdafx.h"
#include "Interruption.h"

namespace GBEmu::HW
{
	Interruption::Interruption(uint8* iePtr, uint8* ifPtr) :
		m_iePtr(iePtr),
		m_ifPtr(ifPtr)
	{}

	uint8 Interruption::IE() const
	{
		return *m_iePtr;
	}

	uint8 Interruption::IF() const
	{
		return *m_ifPtr;
	}

	void Interruption::SetFlag(InterruptFlag flag)
	{
		*m_ifPtr = *m_ifPtr | static_cast<uint8>(flag);
	}

	void Interruption::ResetFlag(InterruptFlag flag)
	{
		*m_ifPtr = *m_ifPtr & (~static_cast<uint8>(flag));
	}
}
