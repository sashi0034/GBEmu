#pragma once

namespace GBEmu::HW
{
	class HWEnv;

	struct CPUInstructionProperty
	{
		bool IsPrefixedCB;
		uint8 Code;
	};

	class CPUOperationZNHC;

	class CPU
	{
	public:
		void StepOperation(HWEnv& env);

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

		static void setRegXX(uint8& reg1, uint8& reg2, uint16 value)
		{
			reg1 = value >> 8;
			reg2 = value & 0xff;
		}

		static uint8 applyFlagZNHC(uint8 regF, CPUOperationZNHC flag);

		CPUInstructionProperty fetchInstruction(HWEnv& env) const;

	};
}
