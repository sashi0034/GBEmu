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
		uint8 IE() const;
		uint8 IF() const;

		void SetIE(uint8 data){ m_ie = data; }
		void SetIF(uint8 data){ m_if = data | 0xE0; }

		void SetFlag(InterruptFlag flag);
		void ResetFlag(InterruptFlag flag);
	private:
		uint8 m_ie{};
		uint8 m_if{};
	};
}
