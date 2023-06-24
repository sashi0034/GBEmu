#pragma once

namespace GBEmu::HW
{
	class IOPort
	{
	public:
		IOPort(
			uint8* joyp,
			uint8* divPtr, uint8* timaPtr, uint8* tacPtr, uint8* tmaPtr,
			uint8* nr10Ptr, uint8* nr11Ptr, uint8* nr12Ptr, uint8* nr13Ptr, uint8* nr14Ptr, uint8* nr21Ptr,
			uint8* nr22Ptr, uint8* nr23Ptr, uint8* nr24Ptr, uint8* nr30Ptr, uint8* nr31Ptr, uint8* nr32Ptr,
			uint8* nr33Ptr, uint8* nr34Ptr, uint8* nr41Ptr, uint8* nr42Ptr, uint8* nr43Ptr, uint8* nr44Ptr,
			uint8* nr50Ptr, uint8* nr51Ptr, uint8* nr52Ptr) :
			m_joyp(joyp),
			m_divPtr(divPtr),
			m_timaPtr(timaPtr),
			m_tacPtr(tacPtr),
			m_tmaPtr(tmaPtr),
			m_nr10Ptr(nr10Ptr),
			m_nr11Ptr(nr11Ptr),
			m_nr12Ptr(nr12Ptr),
			m_nr13Ptr(nr13Ptr),
			m_nr14Ptr(nr14Ptr),
			m_nr21Ptr(nr21Ptr),
			m_nr22Ptr(nr22Ptr),
			m_nr23Ptr(nr23Ptr),
			m_nr24Ptr(nr24Ptr),
			m_nr30Ptr(nr30Ptr),
			m_nr31Ptr(nr31Ptr),
			m_nr32Ptr(nr32Ptr),
			m_nr33Ptr(nr33Ptr),
			m_nr34Ptr(nr34Ptr),
			m_nr41Ptr(nr41Ptr),
			m_nr42Ptr(nr42Ptr),
			m_nr43Ptr(nr43Ptr),
			m_nr44Ptr(nr44Ptr),
			m_nr50Ptr(nr50Ptr),
			m_nr51Ptr(nr51Ptr),
			m_nr52Ptr(nr52Ptr)
		{}

		uint8 JOYP() const {return *m_joyp; }
		void SetJOYP(uint8 joyp) const { *m_joyp = joyp; }

		uint8 TIMA() const {return *m_timaPtr; }
		void SetTIMA(uint8 tima) {*m_timaPtr = tima; }

		uint8 DIV() const {return *m_divPtr; };
		void SetDIV(uint8 div) {*m_divPtr = div;}

		uint8 TAC() const {return *m_tacPtr; }
		uint8 SetTAC(uint8 tac) {*m_tacPtr = tac;};

		uint8 TMA() const {return *m_tmaPtr; }

		uint8 NR10() const { return *m_nr10Ptr; }
		uint8 NR11() const { return *m_nr11Ptr; }
		uint8 NR12() const { return *m_nr12Ptr; }
		uint8 NR13() const { return *m_nr13Ptr; }
		uint8 NR14() const { return *m_nr14Ptr; }
		uint8 NR21() const { return *m_nr21Ptr; }
		uint8 NR22() const { return *m_nr22Ptr; }
		uint8 NR23() const { return *m_nr23Ptr; }
		uint8 NR24() const { return *m_nr24Ptr; }
		uint8 NR30() const { return *m_nr30Ptr; }
		uint8 NR31() const { return *m_nr31Ptr; }
		uint8 NR32() const { return *m_nr32Ptr; }
		uint8 NR33() const { return *m_nr33Ptr; }
		uint8 NR34() const { return *m_nr34Ptr; }
		uint8 NR41() const { return *m_nr41Ptr; }
		uint8 NR42() const { return *m_nr42Ptr; }
		uint8 NR43() const { return *m_nr43Ptr; }
		uint8 NR44() const { return *m_nr44Ptr; }
		uint8 NR50() const { return *m_nr50Ptr; }
		uint8 NR51() const { return *m_nr51Ptr; }
		uint8 NR52() const { return *m_nr52Ptr; }

		void SetNR10(uint8 nr) { *m_nr10Ptr = nr; }
		void SetNR11(uint8 nr) { *m_nr11Ptr = nr; }
		void SetNR12(uint8 nr) { *m_nr12Ptr = nr; }
		void SetNR13(uint8 nr) { *m_nr13Ptr = nr; }
		void SetNR14(uint8 nr) { *m_nr14Ptr = nr; }
		void SetNR21(uint8 nr) { *m_nr21Ptr = nr; }
		void SetNR22(uint8 nr) { *m_nr22Ptr = nr; }
		void SetNR23(uint8 nr) { *m_nr23Ptr = nr; }
		void SetNR24(uint8 nr) { *m_nr24Ptr = nr; }
		void SetNR30(uint8 nr) { *m_nr30Ptr = nr; }
		void SetNR31(uint8 nr) { *m_nr31Ptr = nr; }
		void SetNR32(uint8 nr) { *m_nr32Ptr = nr; }
		void SetNR33(uint8 nr) { *m_nr33Ptr = nr; }
		void SetNR34(uint8 nr) { *m_nr34Ptr = nr; }
		void SetNR41(uint8 nr) { *m_nr41Ptr = nr; }
		void SetNR42(uint8 nr) { *m_nr42Ptr = nr; }
		void SetNR43(uint8 nr) { *m_nr43Ptr = nr; }
		void SetNR44(uint8 nr) { *m_nr44Ptr = nr; }
		void SetNR50(uint8 nr) { *m_nr50Ptr = nr; }
		void SetNR51(uint8 nr) { *m_nr51Ptr = nr; }
		void SetNR52(uint8 nr) { *m_nr52Ptr = nr; }

		template <int X> void SetNRX1(uint8 data);
		template <int X> uint8 GetNRX1() const;

		template <int X> void SetNRX2(uint8 data);
		template <int X> uint8 GetNRX2() const;

		template <int X> void SetNRX3(uint8 data);
		template <int X> uint8 GetNRX3() const;

		template <int X> void SetNRX4(uint8 data);
		template <int X> uint8 GetNRX4() const;
	private:
		// joypad
		uint8* m_joyp;

		// timer
		uint8* m_divPtr;
		uint8* m_timaPtr;
		uint8* m_tacPtr;
		uint8* m_tmaPtr;

		uint8* m_nr10Ptr;
		uint8* m_nr11Ptr;
		uint8* m_nr12Ptr;
		uint8* m_nr13Ptr;
		uint8* m_nr14Ptr;
		uint8* m_nr21Ptr;
		uint8* m_nr22Ptr;
		uint8* m_nr23Ptr;
		uint8* m_nr24Ptr;
		uint8* m_nr30Ptr;
		uint8* m_nr31Ptr;
		uint8* m_nr32Ptr;
		uint8* m_nr33Ptr;
		uint8* m_nr34Ptr;
		uint8* m_nr41Ptr;
		uint8* m_nr42Ptr;
		uint8* m_nr43Ptr;
		uint8* m_nr44Ptr;
		uint8* m_nr50Ptr;
		uint8* m_nr51Ptr;
		uint8* m_nr52Ptr;
	};

	template <int X> void IOPort::SetNRX1(uint8 data)
	{
		static_assert(X==1 || X==2 || X==3 || X==4);

		if constexpr (X==1)
			SetNR11(data);
		else if constexpr (X==2)
			SetNR21(data);
		else if constexpr (X==3)
			SetNR31(data);
		else if constexpr (X==4)
			SetNR41(data);
	}

	template <int X>
	uint8 IOPort::GetNRX1() const
	{
		static_assert(X==1 || X==2 || X==3 || X==4);

		if constexpr (X==1)
			return NR11();
		else if constexpr (X==2)
			return NR21();
		else if constexpr (X==3)
			return NR31();
		else if constexpr (X==4)
			return NR41();
		else return 0;
	}

	template <int X> void IOPort::SetNRX2(uint8 data)
	{
		static_assert(X==1 || X==2 || X==3 || X==4);

		if constexpr (X==1)
			SetNR12(data);
		else if constexpr (X==2)
			SetNR22(data);
		else if constexpr (X==3)
			SetNR32(data);
		else if constexpr (X==4)
			SetNR42(data);
	}

	template <int X>
	uint8 IOPort::GetNRX2() const
	{
		static_assert(X==1 || X==2 || X==3 || X==4);

		if constexpr (X==1)
			return NR12();
		else if constexpr (X==2)
			return NR22();
		else if constexpr (X==3)
			return NR32();
		else if constexpr (X==4)
			return NR42();
		else return 0;
	}

	template <int X> void IOPort::SetNRX3(uint8 data)
	{
		static_assert(X==1 || X==2 || X==3 || X==4);

		if constexpr (X==1)
			SetNR13(data);
		else if constexpr (X==2)
			SetNR23(data);
		else if constexpr (X==3)
			SetNR33(data);
		else if constexpr (X==4)
			SetNR43(data);
	}

	template <int X>
	uint8 IOPort::GetNRX3() const
	{
		static_assert(X==1 || X==2 || X==3 || X==4);

		if constexpr (X==1)
			return NR13();
		else if constexpr (X==2)
			return NR23();
		else if constexpr (X==3)
			return NR33();
		else if constexpr (X==4)
			return NR43();
		else return 0;
	}

	template <int X> void IOPort::SetNRX4(uint8 data)
	{
		static_assert(X==1 || X==2 || X==3 || X==4);

		if constexpr (X==1)
			SetNR14(data);
		else if constexpr (X==2)
			SetNR24(data);
		else if constexpr (X==3)
			SetNR34(data);
		else if constexpr (X==4)
			SetNR44(data);
	}

	template <int X>
	uint8 IOPort::GetNRX4() const
	{
		static_assert(X==1 || X==2 || X==3 || X==4);

		if constexpr (X==1)
			return NR14();
		else if constexpr (X==2)
			return NR24();
		else if constexpr (X==3)
			return NR34();
		else if constexpr (X==4)
			return NR44();
		else return 0;
	}
}
