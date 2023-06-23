#pragma once

namespace GBEmu::HW
{
	class IOPort
	{
	public:
		IOPort(uint8* joyp, uint8* divPtr, uint8* timaPtr, uint8* tacPtr, uint8* tmaPtr) :
			m_joyp(joyp),
			m_divPtr(divPtr),
			m_timaPtr(timaPtr),
			m_tacPtr(tacPtr),
			m_tmaPtr(tmaPtr)
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
		// uint8 SetTMA(uint8 tma) {*m_tmaPtr = tma; }
	private:
		// joypad
		uint8* m_joyp;

		// timer
		uint8* m_divPtr;
		uint8* m_timaPtr;
		uint8* m_tacPtr;
		uint8* m_tmaPtr;
	};
}
