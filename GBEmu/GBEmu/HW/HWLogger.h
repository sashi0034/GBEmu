#pragma once

namespace GBEmu::HW
{
	class HWLogger
	{
	public:
		static void Info(const String& message);
		static void Warn(const String& message);
		static void Error(const String& message);
		static void Assert(bool ok, const String& message);
	};
}
