#include "stdafx.h"
#include "EmuSingleton.h"

namespace GBEmu
{
	EmuSingleton* EmuSingleton::globalInstance = nullptr;

	EmuSingleton::EmuSingleton()
	{
		if (globalInstance == nullptr) globalInstance = this;
	}

	EmuSingleton::~EmuSingleton()
	{
		if (globalInstance == this) globalInstance = nullptr;
	}
}
