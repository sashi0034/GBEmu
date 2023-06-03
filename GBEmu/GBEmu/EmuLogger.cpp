#include "stdafx.h"
#include "EmuLogger.h"

namespace GBEmu
{
	void EmuLogger::Info(const String& message)
	{
		Console.writeln(U"[INFO] {}"_fmt(message));
	}

	void EmuLogger::Warn(const String& message)
	{
		Console.writeln(U"[WARN] {}"_fmt(message));
	}

	void EmuLogger::Error(const String& message)
	{
		Console.writeln(U"[ERROR] {}"_fmt(message));
	}

	void EmuLogger::Assert(bool ok, const String& message)
	{
		if (ok == false) Error(message);
	}
}
