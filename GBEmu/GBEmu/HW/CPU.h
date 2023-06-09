﻿#pragma once
#include "CPUInstruction.h"
#include "CPUInstructionCB.h"
#include "HWLogger.h"
#include "Memory.h"

namespace GBEmu::HW
{
	class HWEnv;

	struct CPUInstructionProperty
	{
		bool IsPrefixedCB;
		uint8 Code;

		Optional<CPUInstruction> CodeUnprefixed() const { return IsPrefixedCB ? none : Optional(static_cast<CPUInstruction>(Code)); }
		Optional<CPUInstructionCB> CodePrefixedCB() const { return IsPrefixedCB ? Optional(static_cast<CPUInstructionCB>(Code)) : none; }
		String ToString() const;
	};

	struct CPUCycle
	{
		int Count;
	};

	struct CPUOperationZNHC;

	enum class CPUReg8 : uint8
	{
		Invalid,
		A,
		F,
		B,
		C,
		D,
		E,
		H,
		L,
	};

	enum class CPUState : uint8
	{
		Running,
		Halted,
		Stopped,
	};

	class CPU
	{
	public:
		CPU();
		CPUCycle StepOperation(HWEnv& env);

		CPUInstructionProperty FetchInstruction(HWEnv& env) const;

		void SetSP(uint16 sp) { m_sp = sp; }

		uint16 PC() const {return m_pc;}
		uint16 SP() const {return m_sp;}

		uint8 RegA() const {return m_regA; }
		uint8 RegF() const {return m_regF; }
		uint8 RegB() const {return m_regB; }
		uint8 RegC() const {return m_regC; }
		uint8 RegD() const {return m_regD; }
		uint8 RegE() const {return m_regE; }
		uint8 RegH() const {return m_regH; }
		uint8 RegL() const {return m_regL; }

		void SetA(uint8 value) { m_regA = value; }
		void SetF(uint8 value) { m_regF = value; }
		void SetB(uint8 value) { m_regB = value; }
		void SetC(uint8 value) { m_regC = value; }
		void SetD(uint8 value) { m_regD = value; }
		void SetE(uint8 value) { m_regE = value; }
		void SetH(uint8 value) { m_regH = value; }
		void SetL(uint8 value) { m_regL = value; }

		inline uint8 GetReg8(CPUReg8 kind) const;
		inline void SetReg8(CPUReg8 kind, uint8 value);

		uint16 RegAF() const {return (m_regA << 8) | m_regF;}
		uint16 RegBC() const {return (m_regB << 8) | m_regC;}
		uint16 RegDE() const {return (m_regD << 8) | m_regE;}
		uint16 RegHL() const {return (m_regH << 8) | m_regL;}

		void SetAF(uint16 value) { setRegXX(m_regA, m_regF, value); }
		void SetBC(uint16 value) { setRegXX(m_regB, m_regC, value); }
		void SetDE(uint16 value) { setRegXX(m_regD, m_regE, value); }
		void SetHL(uint16 value) { setRegXX(m_regH, m_regL, value); }

		bool FlagZ() const { return (m_regF >> 7) & 0b1;}
		bool FlagN() const { return (m_regF >> 6) & 0b1;}
		bool FlagH() const { return (m_regF >> 5) & 0b1;}
		bool FlagC() const { return (m_regF >> 4) & 0b1;}

		void SetState(const CPUState state){ m_state = state; }
		CPUState State() const { return m_state; }

		bool IME() const { return m_imeFlag; }
		void DisableIME() { m_imeFlag = false; }
		void RequestEnableIME() { m_imeRequested = true; };
		String StringifyInfo(HWEnv& env) const;
	private:
		uint16 m_pc{};
		uint16 m_sp{};

		uint8 m_regA{};
		uint8 m_regF{};
		uint8 m_regB{};
		uint8 m_regC{};
		uint8 m_regD{};
		uint8 m_regE{};
		uint8 m_regH{};
		uint8 m_regL{};

		CPUState m_state = CPUState::Running;

		// Interrupt Master Enable Flag
		bool m_imeFlag{};
		bool m_imeRequested{};

		static void setRegXX(uint8& reg1, uint8& reg2, uint16 value)
		{
			reg1 = value >> 8;
			reg2 = value & 0xff;
		}

		[[nodiscard]]
		static uint8 applyFlagZNHC(uint8 regF, CPUOperationZNHC flag);

		Optional<CPUCycle> checkInterrupt(HWEnv& env);
		Optional<CPUCycle> handleInterrupt(HWEnv& env, Memory& memory, uint8 interruptEnable, uint8 interruptFlag, uint16 interruptAddr, int interruptBit);

	};

	uint8 CPU::GetReg8(CPUReg8 kind) const
	{
		switch (kind) {
		case CPUReg8::A: return RegA();
		case CPUReg8::F: return RegF();
		case CPUReg8::B: return RegB();
		case CPUReg8::C: return RegC();
		case CPUReg8::D: return RegD();
		case CPUReg8::E: return RegE();
		case CPUReg8::H: return RegH();
		case CPUReg8::L: return RegL();
		default: HWLogger::Error(U"attempt to get invalid register"); return 0;
		}
	}

	void CPU::SetReg8(CPUReg8 kind, uint8 value)
	{
		switch (kind) {
		case CPUReg8::A: SetA(value); return;
		case CPUReg8::F: SetF(value); return;
		case CPUReg8::B: SetB(value); return;
		case CPUReg8::C: SetC(value); return;
		case CPUReg8::D: SetD(value); return;
		case CPUReg8::E: SetE(value); return;
		case CPUReg8::H: SetH(value); return;
		case CPUReg8::L: SetL(value); return;
		default: HWLogger::Error(U"attempt to set invalid register"); return;
		}
	}
}
