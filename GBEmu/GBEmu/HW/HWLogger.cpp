#include "stdafx.h"
#include "HWLogger.h"

namespace GBEmu::HW
{
	void HWLogger::Info(const String& message)
	{
		Console.writeln(U"[INFO] {}"_fmt(message));
	}

	void HWLogger::Warn(const String& message)
	{
		Console.writeln(U"[WARN] {}"_fmt(message));
	}

	void HWLogger::Error(const String& message)
	{
		Console.writeln(U"[ERROR] {}"_fmt(message));
	}

	void HWLogger::Assert(bool ok, const String& message)
	{
		if (ok == false) Error(message);
	}
}
