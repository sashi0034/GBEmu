#pragma once

namespace GBEmu::HW
{
	struct InterruptFlag
	{
	private:
		uint8 flag;
	public:
		explicit constexpr InterruptFlag(uint8 flag) : flag{flag} {}
		operator uint8() const { return flag; }
	};

	namespace InterruptFlags
	{
		constexpr InterruptFlag Joypad {1 << 4};
		constexpr InterruptFlag Serial {1 << 3};
		constexpr InterruptFlag Timer {1 << 2};
		constexpr InterruptFlag STAT {1 << 1};
		constexpr InterruptFlag VBlank {1 << 0};
	}

	class Interruption
	{
	public:
		Interruption(uint8* iePtr, uint8* ifPtr);

		uint8 IE() const;
		uint8 IF() const;
		void SetFlag(InterruptFlag flag);
		void ResetFlag(InterruptFlag flag);
	private:
		uint8* m_iePtr;
		uint8* m_ifPtr;
	};
}
