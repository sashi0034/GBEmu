#include "stdafx.h"
#include "CPUOperation.h"

#include "HWEnv.h"
#include "HWLogger.h"
#include "MemoryAddress.h"

namespace GBEmu::HW::CPUOperation
{
	using ci = CPUInstruction;

	uint8 undefined8()
	{
		assert(false);
		return 0;
	}

	[[nodiscard]]
	CPUOperationResult operateNOP()
	{
		return CPUOperationResult(1, 4);
	}

	[[nodiscard]]
	CPUOperationResult operateSTOP_d8(HWEnv& env)
	{
		env.GetCPU().SetState(CPUState::Stopped);
		return CPUOperationResult(2, 4);
	}

	[[nodiscard]]
	CPUOperationResult operateLD_XX_d16(HWEnv& env, CPUInstruction instr)
	{
		const uint16 d16 = env.GetMemory().Read16(env.GetCPU().PC() + 1);

		switch (instr)
		{
		case ci::LD_BC_d16_0x01:
			env.GetCPU().SetBC(d16); break;
		case ci::LD_DE_d16_0x11:
			env.GetCPU().SetDE(d16); break;
		case ci::LD_HL_d16_0x21:
			env.GetCPU().SetHL(d16); break;
		case ci::LD_SP_d16_0x31:
			env.GetCPU().SetSP(d16); break;
		default:
			assert(false);
		}

		return CPUOperationResult(3, 12);
	}

	[[nodiscard]]
	CPUOperationResult operateLD_X_d8(HWEnv& env, CPUInstruction instr)
	{
		const uint8 d8 = env.GetMemory().Read(env.GetCPU().PC() + 1);

		switch (instr)
		{
		case ci::LD_B_d8_0x06:
			env.GetCPU().SetB(d8); break;
		case ci::LD_C_d8_0x0E:
			env.GetCPU().SetC(d8); break;
		case ci::LD_D_d8_0x16:
			env.GetCPU().SetD(d8); break;
		case ci::LD_E_d8_0x1E:
			env.GetCPU().SetE(d8); break;
		case ci::LD_H_d8_0x26:
			env.GetCPU().SetH(d8); break;
		case ci::LD_L_d8_0x2E:
			env.GetCPU().SetL(d8); break;
		default:
			assert(false);
		}

		return CPUOperationResult(2, 8);
	}

	[[nodiscard]]
	CPUOperationResult operateLD_X_A(HWEnv& env, CPUInstruction instr)
	{
		auto&& cpu = env.GetCPU();
		auto&& memory = env.GetMemory();
		const uint8 a = cpu.RegA();

		const auto cycle4 = CPUOperationResult(1, 4);
		const auto cycle8 = CPUOperationResult(1, 8);
		const auto cycle16 = CPUOperationResult(3, 16);

		switch (instr)
		{
		case ci::LD_A_A_0x7F:
			cpu.SetA(a); return cycle4;
		case ci::LD_B_A_0x47:
			cpu.SetB(a); return cycle4;
		case ci::LD_C_A_0x4F:
			cpu.SetC(a); return cycle4;
		case ci::LD_C_D_0x4A:
			cpu.SetD(a); return cycle4;
		case ci::LD_E_A_0x5F:
			cpu.SetE(a); return cycle4;
		case ci::LD_H_A_0x67:
			cpu.SetH(a); return cycle4;
		case ci::LD_L_A_0x6F:
			cpu.SetL(a); return cycle4;
		case ci::LD_mBC_A_0x02:
			memory.Write(cpu.RegBC(), a); return cycle8;
		case ci::LD_mDE_A_0x12:
			memory.Write(cpu.RegDE(), a); return cycle8;
		case ci::LD_mHL_A_0x77:
			memory.Write(cpu.RegHL(), a); return cycle8;
		case ci::LD_ma16_A_0xEA:
			memory.Write(memory.Read16(cpu.PC() + 1), a); return cycle16;
		default:
			assert(false);
			return CPUOperationResult::Invalid();
		}
	}

	[[nodiscard]]
	CPUOperationResult operateLD_A_X(HWEnv& env, CPUInstruction instr)
	{
		auto&& cpu = env.GetCPU();
		auto&& memory = env.GetMemory();

		const auto cycle4 = CPUOperationResult(1, 4);

		using std::pair;
		const pair<uint8, CPUOperationResult> dispatch =
			instr == ci::LD_A_A_0x7F ? pair{cpu.RegA(), cycle4} :
			instr == ci::LD_A_B_0x78 ? pair{cpu.RegB(), cycle4} :
			instr == ci::LD_A_C_0x79 ? pair{cpu.RegC(), cycle4} :
			instr == ci::LD_A_D_0x7A ? pair{cpu.RegD(), cycle4} :
			instr == ci::LD_A_E_0x7B ? pair{cpu.RegE(), cycle4} :
			instr == ci::LD_A_H_0x7C ? pair{cpu.RegH(), cycle4} :
			instr == ci::LD_A_L_0x7D ? pair{cpu.RegL(), cycle4} :
			instr == ci::LD_A_mBC_0x0A ?
				pair{memory.Read(cpu.RegBC()), CPUOperationResult(1, 8)} :
			instr == ci::LD_A_mDE_0x1A ?
				pair{memory.Read(cpu.RegDE()), CPUOperationResult(1, 8)} :
			instr == ci::LD_A_mHL_0x7E ?
				pair{memory.Read(cpu.RegHL()), CPUOperationResult(1, 8)} :
			instr == ci::LD_A_ma16_0xFA ?
				pair{memory.Read(memory.Read16(cpu.PC() + 1)), CPUOperationResult(3, 16)} :
			instr == ci::LD_A_d8_0x3E ?
				pair{memory.Read(cpu.PC() + 1), CPUOperationResult(2, 8)} :
			std::pair{undefined8(), CPUOperationResult::Invalid()};

		cpu.SetA(dispatch.first);
		return dispatch.second;
	}

	[[nodiscard]]
	CPUOperationResult operateLD_X_X(HWEnv& env, CPUInstruction instr)
	{
		auto&& cpu = env.GetCPU();
		auto&& memory = env.GetMemory();

		const auto result_1_4 = CPUOperationResult(1, 4);
		const auto result_1_8 = CPUOperationResult(1, 8);
		const auto result_2_12 = CPUOperationResult(2, 12);

		switch (instr)
		{
		// destination B
		case ci::LD_B_B_0x40: cpu.SetB(cpu.RegB()); return result_1_4;
		case ci::LD_B_C_0x41: cpu.SetB(cpu.RegC()); return result_1_4;
		case ci::LD_B_D_0x42: cpu.SetB(cpu.RegD()); return result_1_4;
		case ci::LD_B_E_0x43: cpu.SetB(cpu.RegE()); return result_1_4;
		case ci::LD_B_H_0x44: cpu.SetB(cpu.RegH()); return result_1_4;
		case ci::LD_B_L_0x45: cpu.SetB(cpu.RegL()); return result_1_4;
		case ci::LD_B_mHL_0x46: cpu.SetB(memory.Read(cpu.RegHL())); return result_1_8;

		// destination C
		case ci::LD_C_B_0x48: cpu.SetC(cpu.RegB()); return result_1_4;
		case ci::LD_C_C_0x49: cpu.SetC(cpu.RegC()); return result_1_4;
		case ci::LD_C_D_0x4A: cpu.SetC(cpu.RegD()); return result_1_4;
		case ci::LD_C_E_0x4B: cpu.SetC(cpu.RegE()); return result_1_4;
		case ci::LD_C_H_0x4C: cpu.SetC(cpu.RegH()); return result_1_4;
		case ci::LD_C_L_0x4D: cpu.SetC(cpu.RegL()); return result_1_4;
		case ci::LD_C_mHL_0x4E: cpu.SetC(memory.Read(cpu.RegHL())); return result_1_8;

		// destination D
		case ci::LD_D_B_0x50: cpu.SetD(cpu.RegB()); return result_1_4;
		case ci::LD_D_C_0x51: cpu.SetD(cpu.RegC()); return result_1_4;
		case ci::LD_D_D_0x52: cpu.SetD(cpu.RegD()); return result_1_4;
		case ci::LD_D_E_0x53: cpu.SetD(cpu.RegE()); return result_1_4;
		case ci::LD_D_H_0x54: cpu.SetD(cpu.RegH()); return result_1_4;
		case ci::LD_D_L_0x55: cpu.SetD(cpu.RegL()); return result_1_4;
		case ci::LD_D_mHL_0x56: cpu.SetD(memory.Read(cpu.RegHL())); return result_1_8;

		// destination E
		case ci::LD_E_B_0x58: cpu.SetE(cpu.RegB()); return result_1_4;
		case ci::LD_E_C_0x59: cpu.SetE(cpu.RegC()); return result_1_4;
		case ci::LD_E_D_0x5A: cpu.SetE(cpu.RegD()); return result_1_4;
		case ci::LD_E_E_0x5B: cpu.SetE(cpu.RegE()); return result_1_4;
		case ci::LD_E_H_0x5C: cpu.SetE(cpu.RegH()); return result_1_4;
		case ci::LD_E_L_0x5D: cpu.SetE(cpu.RegL()); return result_1_4;
		case ci::LD_E_mHL_0x5E: cpu.SetE(memory.Read(cpu.RegHL())); return result_1_8;

		// destination H
		case ci::LD_H_B_0x60: cpu.SetH(cpu.RegB()); return result_1_4;
		case ci::LD_H_C_0x61: cpu.SetH(cpu.RegC()); return result_1_4;
		case ci::LD_H_D_0x62: cpu.SetH(cpu.RegD()); return result_1_4;
		case ci::LD_H_E_0x63: cpu.SetH(cpu.RegE()); return result_1_4;
		case ci::LD_H_H_0x64: cpu.SetH(cpu.RegH()); return result_1_4;
		case ci::LD_H_L_0x65: cpu.SetH(cpu.RegL()); return result_1_4;
		case ci::LD_H_mHL_0x66: cpu.SetH(memory.Read(cpu.RegHL())); return result_1_8;

		// destination L
		case ci::LD_L_B_0x68: cpu.SetL(cpu.RegB()); return result_1_4;
		case ci::LD_L_C_0x69: cpu.SetL(cpu.RegC()); return result_1_4;
		case ci::LD_L_D_0x6A: cpu.SetL(cpu.RegD()); return result_1_4;
		case ci::LD_L_E_0x6B: cpu.SetL(cpu.RegE()); return result_1_4;
		case ci::LD_L_H_0x6C: cpu.SetL(cpu.RegH()); return result_1_4;
		case ci::LD_L_L_0x6D: cpu.SetL(cpu.RegL()); return result_1_4;
		case ci::LD_L_mHL_0x6E: cpu.SetL(memory.Read(cpu.RegHL())); return result_1_8;

		// destination (HL)
		case ci::LD_mHL_B_0x70: memory.Write(cpu.RegHL(), cpu.RegB()); return result_1_8;
		case ci::LD_mHL_C_0x71: memory.Write(cpu.RegHL(), cpu.RegC()); return result_1_8;
		case ci::LD_mHL_D_0x72: memory.Write(cpu.RegHL(), cpu.RegD()); return result_1_8;
		case ci::LD_mHL_E_0x73: memory.Write(cpu.RegHL(), cpu.RegE()); return result_1_8;
		case ci::LD_mHL_H_0x74: memory.Write(cpu.RegHL(), cpu.RegH()); return result_1_8;
		case ci::LD_mHL_L_0x75: memory.Write(cpu.RegHL(), cpu.RegL()); return result_1_8;
		case ci::LD_mHL_d8_0x36: memory.Write(cpu.RegHL(), memory.Read(cpu.PC() + 1)); return result_2_12;

		default:
		assert(false);
		return CPUOperationResult::Invalid();
		}
	}

	[[nodiscard]]
	CPUOperationResult operateLD_ma16_SP(HWEnv& env)
	{
		const uint16 a16 = env.GetMemory().Read16(env.GetCPU().PC() + 1);
		env.GetMemory().Write16(a16, env.GetCPU().SP());
		return CPUOperationResult(3, 20);
	}

	[[nodiscard]]
	CPUOperationResult operateLD_mHLi_A(HWEnv& env)
	{
		// 0x22
		auto&& cpu = env.GetCPU();
		env.GetMemory().Write(cpu.RegHL(), cpu.RegA());
		cpu.SetHL(cpu.RegHL() + 1);
		return CPUOperationResult(1, 8);
	}

	[[nodiscard]]
	CPUOperationResult operateLD_A_mHLi(HWEnv& env)
	{
		// 0x2A
		auto&& cpu = env.GetCPU();
		cpu.SetA(env.GetMemory().Read(cpu.RegHL()));
		cpu.SetHL(cpu.RegHL() + 1);
		return CPUOperationResult(1, 8);
	}

	[[nodiscard]]
	CPUOperationResult operateLD_mHLd_A(HWEnv& env)
	{
		// 0x32
		auto&& cpu = env.GetCPU();
		env.GetMemory().Write(cpu.RegHL(), cpu.RegA());
		cpu.SetHL(cpu.RegHL() - 1);
		return CPUOperationResult(1, 8);
	}

	[[nodiscard]]
	CPUOperationResult operateLD_A_mHLd(HWEnv& env)
	{
		// 0x3A
		auto&& cpu = env.GetCPU();
		cpu.SetA(env.GetMemory().Read(cpu.RegHL()));
		cpu.SetHL(cpu.RegHL() - 1);
		return CPUOperationResult(1, 8);
	}

	[[nodiscard]]
	CPUOperationResult operateINC_XX(HWEnv& env, CPUInstruction instr)
	{
		auto&& cpu = env.GetCPU();
		const auto result = CPUOperationResult(1, 8);

		switch (instr)
		{
		case ci::INC_BC_0x03:
			cpu.SetBC(cpu.RegBC() + 1); return result;
		case ci::INC_DE_0x13:
			cpu.SetDE(cpu.RegDE() + 1); return result;
		case ci::INC_HL_0x23:
			cpu.SetHL(cpu.RegHL() + 1); return result;
		case ci::INC_SP_0x33:
			cpu.SetSP(cpu.SP() + 1); return result;
		default:
			assert(false);
			return CPUOperationResult::Invalid();
		}
	}

	[[nodiscard]]
	CPUOperationResult operateINC_X(HWEnv& env, CPUInstruction instr)
	{
		// https://github.com/pokemium/gb-docs-ja/blob/main/cpu/instruction/alu8.md

		auto&& cpu = env.GetCPU();

		const CPUReg8 target =
			instr == ci::INC_A_0x3C ? CPUReg8::A :
			instr == ci::INC_B_0x04 ? CPUReg8::B :
			instr == ci::INC_C_0x0C ? CPUReg8::C :
			instr == ci::INC_D_0x14 ? CPUReg8::D :
			instr == ci::INC_E_0x1C ? CPUReg8::E :
			instr == ci::INC_H_0x24 ? CPUReg8::H :
			instr == ci::INC_L_0x2C ? CPUReg8::L :
			CPUReg8::Invalid;
		assert(target != CPUReg8::Invalid);

		const bool h = (cpu.GetReg8(target) & 0xF) == 0xF;

		cpu.SetReg8(target, cpu.GetReg8(target) + 1);

		const bool z = cpu.GetReg8(target) == 0;

		return CPUOperationResult::ByCalc(1, 4, CPUOperationZNHC{z, false, h, cpu.FlagC()});
	}

	[[nodiscard]]
	CPUOperationResult operateINC_mHL(HWEnv& env)
	{
		auto&& cpu = env.GetCPU();

		const uint8 before = env.GetMemory().Read(cpu.RegHL());
		const bool h = (before & 0xF) == 0xF;

		const uint8 after = before + 1;
		env.GetMemory().Write(cpu.RegHL(), after);
		const bool z = after == 0;

		return CPUOperationResult::ByCalc(1,12, CPUOperationZNHC{z, false, h, cpu.FlagC()});
	}

	[[nodiscard]]
	CPUOperationResult operateDEC_XX(HWEnv& env, CPUInstruction instr)
	{
		auto&& cpu = env.GetCPU();
		const auto result = CPUOperationResult(1, 8);

		switch (instr)
		{
		case ci::DEC_BC_0x0B:
			cpu.SetBC(cpu.RegBC() - 1); return result;
		case ci::DEC_DE_0x1B:
			cpu.SetDE(cpu.RegDE() - 1); return result;
		case ci::DEC_HL_0x2B:
			cpu.SetHL(cpu.RegHL() - 1); return result;
		case ci::DEC_SP_0x3B:
			cpu.SetSP(cpu.SP() - 1); return result;
		default:
			assert(false);
			return CPUOperationResult::Invalid();
		}
	}

	[[nodiscard]]
	CPUOperationResult operateDEC_X(HWEnv& env, CPUInstruction instr)
	{
		auto&& cpu = env.GetCPU();

		const CPUReg8 target =
			instr == ci::DEC_A_0x3D ? CPUReg8::A :
			instr == ci::DEC_B_0x05 ? CPUReg8::B :
			instr == ci::DEC_C_0x0D ? CPUReg8::C :
			instr == ci::DEC_A_0x3D ? CPUReg8::D :
			instr == ci::DEC_E_0x1D ? CPUReg8::E :
			instr == ci::DEC_H_0x25 ? CPUReg8::H :
			instr == ci::DEC_L_0x2D ? CPUReg8::L :
			CPUReg8::Invalid;
		assert(target != CPUReg8::Invalid);

		const bool h = (cpu.GetReg8(target) & 0xF) == 0xF;

		cpu.SetReg8(target, cpu.GetReg8(target) - 1);

		const bool z = cpu.GetReg8(target) == 0;

		return CPUOperationResult::ByCalc(1, 4, CPUOperationZNHC{z, false, h, cpu.FlagC()});
	}

	[[nodiscard]]
	CPUOperationResult operateDEC_mHL(HWEnv& env)
	{
		auto&& cpu = env.GetCPU();

		const uint8 before = env.GetMemory().Read(cpu.RegHL());
		const bool h = (before & 0xF) == 0xF;

		const uint8 after = before - 1;
		env.GetMemory().Write(cpu.RegHL(), after);
		const bool z = after == 0;

		return CPUOperationResult::ByCalc(1, 12, CPUOperationZNHC{z, false, h, cpu.FlagC()});
	}

	[[nodiscard]]
	CPUOperationResult operateRLCA(HWEnv& env)
	{
		auto&& cpu = env.GetCPU();

		const uint8 bit7 = cpu.RegA() >> 7;
		cpu.SetA((cpu.RegA() << 1) | bit7);

		// 資料によっては、Z: Set if result is zero となっているのものあるのでしっかりテストしたい
		return CPUOperationResult::ByCalc(1, 4, CPUOperationZNHC{false, false, false, bit7 == 1});
	}

	[[nodiscard]]
	CPUOperationResult operateRLA(HWEnv& env)
	{
		// 0x0F
		auto&& cpu = env.GetCPU();

		const uint8 bit7 = cpu.RegA() >> 7;
		const uint8 carry = static_cast<uint8>(cpu.FlagC());

		cpu.SetA((cpu.RegA() << 1) | carry);

		// 資料によっては、Z: Set if result is zero となっているのものあるのでしっかりテストしたい
		return CPUOperationResult::ByCalc(1, 4, CPUOperationZNHC{false, false, false, bit7 == 1});
	}

	[[nodiscard]]
	CPUOperationResult operateRRA(HWEnv& env)
	{
		// 0x1F
		auto&& cpu = env.GetCPU();

		const uint8 bit0 = cpu.RegA() &0b1;
		const uint8 carry = static_cast<uint8>(cpu.FlagC());

		cpu.SetA((cpu.RegA() >> 1) | (carry << 7));

		return CPUOperationResult::ByCalc(1, 4, CPUOperationZNHC{false, false, false, bit0 == 0b1});
	}

	[[nodiscard]]
	CPUOperationResult operateRRCA(HWEnv& env)
	{
		auto&& cpu = env.GetCPU();

		const uint8 bit0 = cpu.RegA() & 0b1;
		const bool c = bit0 == 0b1;
		cpu.SetA((cpu.RegA() >> 1) | (bit0 << 7));

		// 資料によっては、Z: Set if result is zero となっているのものあるのでしっかりテストしたい
		return CPUOperationResult::ByCalc(1, 4, CPUOperationZNHC{false, false, false, c});
	}

	[[nodiscard]]
	CPUOperationResult operateADD_HL_XX(HWEnv& env, CPUInstruction instr)
	{
		auto&& cpu = env.GetCPU();

		const uint16 src =
			instr == ci::ADD_HL_BC_0x09 ? cpu.RegBC() :
			instr == ci::ADD_HL_DE_0x19 ? cpu.RegDE() :
			instr == ci::ADD_HL_HL_0x29 ? cpu.RegHL() :
			instr == ci::ADD_HL_SP_0x39 ? cpu.SP() :
			undefined8();

		const bool h = ((cpu.RegHL() & 0x7fff) + (src & 0x7fff)) > 0x7fff; // bit11からオーバーフローした場合にセット
		const bool c = ((cpu.RegHL() & 0xffff) + (src & 0xffff)) > 0xffff; // bit15からオーバーフローした場合にセット

		cpu.SetHL(cpu.RegHL() + src);

		return CPUOperationResult::ByCalc(1, 8, CPUOperationZNHC{cpu.FlagZ(), false, h, c});
	}

	[[nodiscard]]
	CPUOperationResult operateADD_A_X(HWEnv& env, CPUInstruction instr)
	{
		auto&& cpu = env.GetCPU();
		auto&& memory = env.GetMemory();

		const auto result_1_4 = CPUOperationResult(1, 4);

		using std::pair;
		pair<const uint8, CPUOperationResult> dispatch =
			instr == ci::ADD_A_A_0x87 ? pair{ cpu.RegA(), result_1_4 } :
			instr == ci::ADD_A_B_0x80 ? pair{ cpu.RegB(), result_1_4 } :
			instr == ci::ADD_A_C_0x81 ? pair{ cpu.RegC(), result_1_4 } :
			instr == ci::ADD_A_D_0x82 ? pair{ cpu.RegD(), result_1_4 } :
			instr == ci::ADD_A_E_0x83 ? pair{ cpu.RegE(), result_1_4 } :
			instr == ci::ADD_A_H_0x84 ? pair{ cpu.RegH(), result_1_4 } :
			instr == ci::ADD_A_L_0x85 ? pair{ cpu.RegL(), result_1_4 } :
			instr == ci::ADD_A_mHL_0x86 ?
				pair{ memory.Read(cpu.RegHL()), CPUOperationResult(1, 8) } :
			instr == ci::ADD_A_d8_0xC6 ?
				pair{ memory.Read(cpu.PC() + 1), CPUOperationResult(2, 8) } :
			pair{ undefined8(), CPUOperationResult::Invalid() };

		const uint8 add = dispatch.first;

		const bool h = ((cpu.RegA() & 0xF) + (add & 0xF)) > 0xF; // bit3からオーバーフローした場合にセット
		const bool c = ((cpu.RegA() & 0xFF) + (add & 0xFF)) > 0xFF; // bit7からオーバーフローした場合にセット

		cpu.SetA(cpu.RegA() + add);

		dispatch.second.Flag = CPUOperationZNHC{cpu.RegA() == 0, false, h, c};

		return dispatch.second;
	}

	[[nodiscard]]
	CPUOperationResult operateADC_A_X(HWEnv& env, CPUInstruction instr)
	{
		auto&& cpu = env.GetCPU();
		auto&& memory = env.GetMemory();

		const auto result_1_4 = CPUOperationResult(1, 4);

		using std::pair;
		pair<const uint8, CPUOperationResult> dispatch =
			instr == ci::ADC_A_A_0x8F ? pair{ cpu.RegA(), result_1_4 } :
			instr == ci::ADC_A_B_0x88 ? pair{ cpu.RegB(), result_1_4 } :
			instr == ci::ADC_A_C_0x89 ? pair{ cpu.RegC(), result_1_4 } :
			instr == ci::ADC_A_D_0x8A ? pair{ cpu.RegD(), result_1_4 } :
			instr == ci::ADC_A_E_0x8B ? pair{ cpu.RegE(), result_1_4 } :
			instr == ci::ADC_A_H_0x8C ? pair{ cpu.RegH(), result_1_4 } :
			instr == ci::ADC_A_L_0x8D ? pair{ cpu.RegL(), result_1_4 } :
			instr == ci::ADC_A_mHL_0x8E ?
				pair{ memory.Read(cpu.RegHL()), CPUOperationResult(1, 8) } :
			instr == ci::ADC_A_d8_0xCE ?
				pair{ memory.Read(cpu.PC() + 1), CPUOperationResult(2, 8) } :
			pair{ undefined8(), CPUOperationResult::Invalid() };

		const uint8 add = dispatch.first;
		const uint8 carry = (uint8)cpu.FlagC();

		const bool h = ((cpu.RegA() & 0xF) + (add & 0xF) + carry) > 0xF; // bit3からオーバーフローした場合にセット
		const bool c = ((cpu.RegA() & 0xFF) + (add & 0xFF) + carry) > 0xFF; // bit7からオーバーフローした場合にセット

		cpu.SetA(cpu.RegA() + add + carry);

		dispatch.second.Flag = CPUOperationZNHC{cpu.RegA() == 0, false, h, c};

		return dispatch.second;
	}

	[[nodiscard]]
	CPUOperationResult operateSUB_A_X(HWEnv& env, CPUInstruction instr)
	{
		auto&& cpu = env.GetCPU();
		auto&& memory = env.GetMemory();

		const auto result_1_4 = CPUOperationResult(1, 4);

		using std::pair;
		pair<const uint8, CPUOperationResult> dispatch =
			instr == ci::SUB_A_A_0x97 ? pair{ cpu.RegA(), result_1_4 } :
			instr == ci::SUB_A_B_0x90 ? pair{ cpu.RegB(), result_1_4 } :
			instr == ci::SUB_A_C_0x91 ? pair{ cpu.RegC(), result_1_4 } :
			instr == ci::SUB_A_D_0x92 ? pair{ cpu.RegD(), result_1_4 } :
			instr == ci::SUB_A_E_0x93 ? pair{ cpu.RegE(), result_1_4 } :
			instr == ci::SUB_A_H_0x94 ? pair{ cpu.RegH(), result_1_4 } :
			instr == ci::SUB_A_L_0x95 ? pair{ cpu.RegL(), result_1_4 } :
			instr == ci::SUB_A_mHL_0x96 ?
				pair{ memory.Read(cpu.RegHL()), CPUOperationResult(1, 8) } :
			instr == ci::SUB_A_d8_0xD6 ?
				pair{ memory.Read(cpu.PC() + 1), CPUOperationResult(2, 8) } :
			pair{ undefined8(), CPUOperationResult::Invalid() };

		const uint8 sub = dispatch.first;

		const bool h = ((cpu.RegA() & 0xF) - (sub & 0xF)) < 0; // bit4からボローした場合にセット
		const bool c = cpu.RegA() < sub; // ボローした場合にセット

		cpu.SetA(cpu.RegA() - sub);

		dispatch.second.Flag = CPUOperationZNHC{cpu.RegA() == 0, true, h, c};

		return dispatch.second;
	}

	[[nodiscard]]
	CPUOperationResult operateJR_X_r8(HWEnv& env, CPUInstruction instr)
	{
		auto&& cpu = env.GetCPU();
		const uint8 r8 = env.GetMemory().Read(cpu.PC() + 1);

		const bool toJump =
			instr == ci::JR_r8_0x18 ? true :
			instr == ci::JR_NZ_r8_0x20 ? (cpu.FlagZ() == false) :
			instr == ci::JR_Z_r8_0x28 ? cpu.FlagZ() :
			instr == ci::JR_NC_r8_0x30 ? (cpu.FlagC() == false) :
			instr == ci::JR_C_r8_0x38 ? cpu.FlagC() :
			undefined8();

		return toJump
			// 命令実行時では、このPCは命令実行前のPCを指しているので、nextPCにはbyteLengthを足しておく
			? CPUOperationResult::ByJump(2, 12, cpu.PC() + 2 + r8)
			: CPUOperationResult(2, 8);
	}

	[[nodiscard]]
	CPUOperationResult operateCPL(HWEnv& env)
	{
		// 0x2F
		auto&& cpu = env.GetCPU();

		cpu.SetA(cpu.RegA() ^ 0xFF);

		return CPUOperationResult::ByCalc(1, 4, CPUOperationZNHC{cpu.FlagZ(), true, true, cpu.FlagC()});
	}

	[[nodiscard]]
	CPUOperationResult operateSCF(HWEnv& env)
	{
		// 0x37
		auto&& cpu = env.GetCPU();
		return CPUOperationResult::ByCalc(1, 4, CPUOperationZNHC{cpu.FlagZ(), false, false, true});
	}

	[[nodiscard]]
	CPUOperationResult operateCCF(HWEnv& env)
	{
		// 0x3F
		auto&& cpu = env.GetCPU();
		return CPUOperationResult::ByCalc(1, 4, CPUOperationZNHC{cpu.FlagZ(), false, false, !cpu.FlagC()});
	}

	[[nodiscard]]
	CPUOperationResult operateDAA(HWEnv& env)
	{
		// 0x27
		auto&& cpu = env.GetCPU();

		int16 result = cpu.RegA();

		if (cpu.FlagN())
		{
			if (cpu.FlagH()) result = (result - 0x6) & 0xFF;
			if (cpu.FlagC()) result -= 0x60;
		}
		else
		{
			if (cpu.FlagH() || (result & 0xF) > 0x9) result += 0x6;
			if (cpu.FlagC() || result > 0x9F) result += 0x60;
		}

		const bool c = cpu.FlagC() || (result & 0x100) == 0x100;
		cpu.SetA(result & 0xFF);
		const bool z = cpu.RegA() == 0;

		return CPUOperationResult::ByCalc(1, 4, CPUOperationZNHC{z, cpu.FlagN(), false, c});
	}

	[[nodiscard]]
	CPUOperationResult operateHALT(HWEnv& env)
	{
		// 0x76
		auto&& cpu = env.GetCPU();
		auto&& memory = env.GetMemory();

		if (cpu.IME())
		{
			cpu.SetState(CPUState::Halted);
		}
		else
		{
			const uint8 interruptEnable = memory.Read(MemoryAddress::InterruptEnable);
			const uint8 interruptFlag = memory.Read(MemoryAddress::InterruptFlag);
			if ((interruptEnable & interruptFlag) != 0)
			{
				// https://github.com/pokemium/gb-docs-ja/blob/main/cpu/instruction/misc.md#halt
				// ただし、ハードウェアのバグにより、HALTの次のバイトが2回連続して読み込まれることに注意
				// ?
			}
			else
			{
				cpu.SetState(CPUState::Halted);
			}
		}

		return CPUOperationResult(1, 4);
	}


	const CPUOperationResult OperateInstruction(HWEnv& env, CPUInstruction instr)
	{
		switch (instr)
		{
		case ci::NOP_0x00: return operateNOP();
		case ci::LD_BC_d16_0x01: return operateLD_XX_d16(env, instr);
		case ci::LD_mBC_A_0x02: return operateLD_X_A(env, instr);
		case ci::INC_BC_0x03: return operateINC_XX(env, instr);
		case ci::INC_B_0x04: return operateINC_X(env, instr);
		case ci::DEC_B_0x05: return operateDEC_X(env, instr);
		case ci::LD_B_d8_0x06: return operateLD_X_d8(env, instr);
		case ci::RLCA_0x07: return operateRLCA(env);
		case ci::LD_ma16_SP_0x08: return operateLD_ma16_SP(env);
		case ci::ADD_HL_BC_0x09: return operateADD_HL_XX(env, instr);
		case ci::LD_A_mBC_0x0A: return operateLD_A_X(env, instr);
		case ci::DEC_BC_0x0B: return operateDEC_XX(env, instr);
		case ci::INC_C_0x0C: return operateINC_X(env, instr);
		case ci::DEC_C_0x0D: return operateDEC_X(env, instr);
		case ci::LD_C_d8_0x0E: return operateLD_X_d8(env, instr);;
		case ci::RRCA_0x0F: return operateRRCA(env);
		case ci::STOP_d8_0x10: return operateSTOP_d8(env);
		case ci::LD_DE_d16_0x11: return operateLD_XX_d16(env, instr);
		case ci::LD_mDE_A_0x12: return operateLD_X_A(env, instr);
		case ci::INC_DE_0x13: return operateINC_XX(env, instr);
		case ci::INC_D_0x14: return operateINC_X(env, instr);
		case ci::DEC_D_0x15: return operateDEC_X(env, instr);
		case ci::LD_D_d8_0x16: return operateLD_X_d8(env, instr);;
		case ci::RLA_0x17: return operateRLA(env);
		case ci::JR_r8_0x18: return operateJR_X_r8(env, instr);
		case ci::ADD_HL_DE_0x19: return operateADD_HL_XX(env, instr);
		case ci::LD_A_mDE_0x1A: return operateLD_A_X(env, instr);
		case ci::DEC_DE_0x1B: return operateDEC_XX(env, instr);
		case ci::INC_E_0x1C: return operateINC_X(env, instr);
		case ci::DEC_E_0x1D: return operateDEC_X(env, instr);
		case ci::LD_E_d8_0x1E: return operateLD_X_d8(env, instr);;
		case ci::RRA_0x1F: return operateRRA(env);
		case ci::JR_NZ_r8_0x20: return operateJR_X_r8(env, instr);
		case ci::LD_HL_d16_0x21: return operateLD_XX_d16(env, instr);
		case ci::LD_mHLi_A_0x22: return operateLD_mHLi_A(env);
		case ci::INC_HL_0x23: return operateINC_XX(env, instr);
		case ci::INC_H_0x24: return operateINC_X(env, instr);
		case ci::DEC_H_0x25: return operateDEC_X(env, instr);
		case ci::LD_H_d8_0x26: return operateLD_X_d8(env, instr);;
		case ci::DAA_0x27: return operateDAA(env);
		case ci::JR_Z_r8_0x28: return operateJR_X_r8(env, instr);
		case ci::ADD_HL_HL_0x29: return operateADD_HL_XX(env, instr);
		case ci::LD_A_mHLi_0x2A: return operateLD_A_mHLi(env);
		case ci::DEC_HL_0x2B: return operateDEC_XX(env, instr);
		case ci::INC_L_0x2C: return operateINC_X(env, instr);
		case ci::DEC_L_0x2D: return operateDEC_X(env, instr);
		case ci::LD_L_d8_0x2E: return operateLD_X_d8(env, instr);;
		case ci::CPL_0x2F: return operateCPL(env);
		case ci::JR_NC_r8_0x30: return operateJR_X_r8(env, instr);
		case ci::LD_SP_d16_0x31: return operateLD_XX_d16(env, instr);
		case ci::LD_mHLd_A_0x32: return operateLD_mHLd_A(env);
		case ci::INC_SP_0x33: return operateINC_XX(env, instr);
		case ci::INC_mHL_0x34: return operateINC_mHL(env);
		case ci::DEC_mHL_0x35: return operateDEC_mHL(env);
		case ci::LD_mHL_d8_0x36: return operateLD_X_X(env, instr);
		case ci::SCF_0x37: return operateSCF(env);
		case ci::JR_C_r8_0x38: return operateJR_X_r8(env, instr);
		case ci::ADD_HL_SP_0x39: return operateADD_HL_XX(env, instr);
		case ci::LD_A_mHLd_0x3A: return operateLD_A_mHLd(env);
		case ci::DEC_SP_0x3B: return operateDEC_XX(env, instr);
		case ci::INC_A_0x3C: return operateINC_X(env, instr);
		case ci::DEC_A_0x3D: return operateDEC_X(env, instr);
		case ci::LD_A_d8_0x3E: return operateLD_A_X(env, instr);
		case ci::CCF_0x3F: return operateCCF(env);
		case ci::LD_B_B_0x40: return operateLD_X_X(env, instr);
		case ci::LD_B_C_0x41: return operateLD_X_X(env, instr);
		case ci::LD_B_D_0x42: return operateLD_X_X(env, instr);
		case ci::LD_B_E_0x43: return operateLD_X_X(env, instr);
		case ci::LD_B_H_0x44: return operateLD_X_X(env, instr);
		case ci::LD_B_L_0x45: return operateLD_X_X(env, instr);
		case ci::LD_B_mHL_0x46: return operateLD_X_X(env, instr);
		case ci::LD_B_A_0x47: return operateLD_X_A(env, instr);
		case ci::LD_C_B_0x48: return operateLD_X_X(env, instr);
		case ci::LD_C_C_0x49: return operateLD_X_X(env, instr);
		case ci::LD_C_D_0x4A: return operateLD_X_X(env, instr);
		case ci::LD_C_E_0x4B: return operateLD_X_X(env, instr);
		case ci::LD_C_H_0x4C: return operateLD_X_X(env, instr);
		case ci::LD_C_L_0x4D: return operateLD_X_X(env, instr);
		case ci::LD_C_mHL_0x4E: return operateLD_X_X(env, instr);
		case ci::LD_C_A_0x4F: return operateLD_X_A(env, instr);
		case ci::LD_D_B_0x50: return operateLD_X_X(env, instr);
		case ci::LD_D_C_0x51: return operateLD_X_X(env, instr);
		case ci::LD_D_D_0x52: return operateLD_X_X(env, instr);
		case ci::LD_D_E_0x53: return operateLD_X_X(env, instr);
		case ci::LD_D_H_0x54: return operateLD_X_X(env, instr);
		case ci::LD_D_L_0x55: return operateLD_X_X(env, instr);
		case ci::LD_D_mHL_0x56: return operateLD_X_X(env, instr);
		case ci::LD_D_A_0x57: return operateLD_X_A(env, instr);
		case ci::LD_E_B_0x58: return operateLD_X_X(env, instr);
		case ci::LD_E_C_0x59: return operateLD_X_X(env, instr);
		case ci::LD_E_D_0x5A: return operateLD_X_X(env, instr);
		case ci::LD_E_E_0x5B: return operateLD_X_X(env, instr);
		case ci::LD_E_H_0x5C: return operateLD_X_X(env, instr);
		case ci::LD_E_L_0x5D: return operateLD_X_X(env, instr);
		case ci::LD_E_mHL_0x5E: return operateLD_X_X(env, instr);
		case ci::LD_E_A_0x5F: return operateLD_X_A(env, instr);
		case ci::LD_H_B_0x60: return operateLD_X_X(env, instr);
		case ci::LD_H_C_0x61: return operateLD_X_X(env, instr);
		case ci::LD_H_D_0x62: return operateLD_X_X(env, instr);
		case ci::LD_H_E_0x63: return operateLD_X_X(env, instr);
		case ci::LD_H_H_0x64: return operateLD_X_X(env, instr);
		case ci::LD_H_L_0x65: return operateLD_X_X(env, instr);
		case ci::LD_H_mHL_0x66: return operateLD_X_X(env, instr);
		case ci::LD_H_A_0x67: return operateLD_X_A(env, instr);
		case ci::LD_L_B_0x68: return operateLD_X_X(env, instr);
		case ci::LD_L_C_0x69: return operateLD_X_X(env, instr);
		case ci::LD_L_D_0x6A: return operateLD_X_X(env, instr);
		case ci::LD_L_E_0x6B: return operateLD_X_X(env, instr);
		case ci::LD_L_H_0x6C: return operateLD_X_X(env, instr);
		case ci::LD_L_L_0x6D: return operateLD_X_X(env, instr);
		case ci::LD_L_mHL_0x6E: return operateLD_X_X(env, instr);
		case ci::LD_L_A_0x6F: return operateLD_X_A(env, instr);
		case ci::LD_mHL_B_0x70: return operateLD_X_X(env, instr);
		case ci::LD_mHL_C_0x71: return operateLD_X_X(env, instr);
		case ci::LD_mHL_D_0x72: return operateLD_X_X(env, instr);
		case ci::LD_mHL_E_0x73: return operateLD_X_X(env, instr);
		case ci::LD_mHL_H_0x74: return operateLD_X_X(env, instr);
		case ci::LD_mHL_L_0x75: return operateLD_X_X(env, instr);
		case ci::HALT_0x76: return operateHALT(env);
		case ci::LD_mHL_A_0x77: return operateLD_X_A(env, instr);
		case ci::LD_A_B_0x78: return operateLD_A_X(env, instr);
		case ci::LD_A_C_0x79: return operateLD_A_X(env, instr);
		case ci::LD_A_D_0x7A: return operateLD_A_X(env, instr);
		case ci::LD_A_E_0x7B: return operateLD_A_X(env, instr);
		case ci::LD_A_H_0x7C: return operateLD_A_X(env, instr);
		case ci::LD_A_L_0x7D: return operateLD_A_X(env, instr);
		case ci::LD_A_mHL_0x7E: return operateLD_A_X(env, instr);
		case ci::LD_A_A_0x7F: return operateLD_X_A(env, instr);
		case ci::ADD_A_B_0x80: return operateADD_A_X(env, instr);
		case ci::ADD_A_C_0x81: return operateADD_A_X(env, instr);
		case ci::ADD_A_D_0x82: return operateADD_A_X(env, instr);
		case ci::ADD_A_E_0x83: return operateADD_A_X(env, instr);
		case ci::ADD_A_H_0x84: return operateADD_A_X(env, instr);
		case ci::ADD_A_L_0x85: return operateADD_A_X(env, instr);
		case ci::ADD_A_mHL_0x86: return operateADD_A_X(env, instr);
		case ci::ADD_A_A_0x87: return operateADD_A_X(env, instr);
		case ci::ADC_A_B_0x88: return operateADC_A_X(env, instr);
		case ci::ADC_A_C_0x89: return operateADC_A_X(env, instr);
		case ci::ADC_A_D_0x8A: return operateADC_A_X(env, instr);
		case ci::ADC_A_E_0x8B: return operateADC_A_X(env, instr);
		case ci::ADC_A_H_0x8C: return operateADC_A_X(env, instr);
		case ci::ADC_A_L_0x8D: return operateADC_A_X(env, instr);
		case ci::ADC_A_mHL_0x8E: return operateADC_A_X(env, instr);
		case ci::ADC_A_A_0x8F: return operateADC_A_X(env, instr);
		case ci::SUB_A_B_0x90: return operateSUB_A_X(env, instr);
		case ci::SUB_A_C_0x91: return operateSUB_A_X(env, instr);
		case ci::SUB_A_D_0x92: return operateSUB_A_X(env, instr);
		case ci::SUB_A_E_0x93: return operateSUB_A_X(env, instr);
		case ci::SUB_A_H_0x94: return operateSUB_A_X(env, instr);
		case ci::SUB_A_L_0x95: return operateSUB_A_X(env, instr);
		case ci::SUB_A_mHL_0x96: return operateSUB_A_X(env, instr);
		case ci::SUB_A_A_0x97: return operateSUB_A_X(env, instr);
		case ci::SBC_A_B_0x98: break;
		case ci::SBC_A_C_0x99: break;
		case ci::SBC_A_D_0x9A: break;
		case ci::SBC_A_E_0x9B: break;
		case ci::SBC_A_H_0x9C: break;
		case ci::SBC_A_L_0x9D: break;
		case ci::SBC_A_HL_0x9E: break;
		case ci::SBC_A_A_0x9F: break;
		case ci::AND_B_0xA0: break;
		case ci::AND_C_0xA1: break;
		case ci::AND_D_0xA2: break;
		case ci::AND_E_0xA3: break;
		case ci::AND_H_0xA4: break;
		case ci::AND_L_0xA5: break;
		case ci::AND_HL_0xA6: break;
		case ci::AND_A_0xA7: break;
		case ci::XOR_B_0xA8: break;
		case ci::XOR_C_0xA9: break;
		case ci::XOR_D_0xAA: break;
		case ci::XOR_E_0xAB: break;
		case ci::XOR_H_0xAC: break;
		case ci::XOR_L_0xAD: break;
		case ci::XOR_HL_0xAE: break;
		case ci::XOR_A_0xAF: break;
		case ci::OR_B_0xB0: break;
		case ci::OR_C_0xB1: break;
		case ci::OR_D_0xB2: break;
		case ci::OR_E_0xB3: break;
		case ci::OR_H_0xB4: break;
		case ci::OR_L_0xB5: break;
		case ci::OR_HL_0xB6: break;
		case ci::OR_A_0xB7: break;
		case ci::CP_B_0xB8: break;
		case ci::CP_C_0xB9: break;
		case ci::CP_D_0xBA: break;
		case ci::CP_E_0xBB: break;
		case ci::CP_H_0xBC: break;
		case ci::CP_L_0xBD: break;
		case ci::CP_HL_0xBE: break;
		case ci::CP_A_0xBF: break;
		case ci::RET_NZ_0xC0: break;
		case ci::POP_BC_0xC1: break;
		case ci::JP_NZ_a16_0xC2: break;
		case ci::JP_a16_0xC3: break;
		case ci::CALL_NZ_a16_0xC4: break;
		case ci::PUSH_BC_0xC5: break;
		case ci::ADD_A_d8_0xC6: return operateADD_A_X(env, instr);
		case ci::RST_00H_0xC7: break;
		case ci::RET_Z_0xC8: break;
		case ci::RET_0xC9: break;
		case ci::JP_Z_a16_0xCA: break;
		case ci::PREFIX_0xCB: break;
		case ci::CALL_Z_a16_0xCC: break;
		case ci::CALL_a16_0xCD: break;
		case ci::ADC_A_d8_0xCE: return operateADC_A_X(env, instr);
		case ci::RST_08H_0xCF: break;
		case ci::RET_NC_0xD0: break;
		case ci::POP_DE_0xD1: break;
		case ci::JP_NC_a16_0xD2: break;
		case ci::Reserved_0xD3: return CPUOperationResult::Default();
		case ci::CALL_NC_a16_0xD4: break;
		case ci::PUSH_DE_0xD5: break;
		case ci::SUB_A_d8_0xD6: return operateSUB_A_X(env, instr);
		case ci::RST_10H_0xD7: break;
		case ci::RET_C_0xD8: break;
		case ci::RETI_0xD9: break;
		case ci::JP_C_a16_0xDA: break;
		case ci::Reserved_0xDB: return CPUOperationResult::Default();
		case ci::CALL_C_a16_0xDC: break;
		case ci::Reserved_0xDD: return CPUOperationResult::Default();
		case ci::SBC_A_d8_0xDE: break;
		case ci::RST_18H_0xDF: break;
		case ci::LDH_a8_A_0xE0: break;
		case ci::POP_HL_0xE1: break;
		case ci::LD_mC_A_0xE2: break;
		case ci::Reserved_0xE3: return CPUOperationResult::Default();
		case ci::Reserved_0xE4: return CPUOperationResult::Default();
		case ci::PUSH_HL_0xE5: break;
		case ci::AND_d8_0xE6: break;
		case ci::RST_20H_0xE7: break;
		case ci::ADD_SP_r8_0xE8: break;
		case ci::JP_HL_0xE9: break;
		case ci::LD_ma16_A_0xEA: return operateLD_X_A(env, instr);
		case ci::Reserved_0xEB: return CPUOperationResult::Default();
		case ci::Reserved_0xEC: return CPUOperationResult::Default();
		case ci::Reserved_0xED: return CPUOperationResult::Default();
		case ci::XOR_d8_0xEE: break;
		case ci::RST_28H_0xEF: break;
		case ci::LDH_A_a8_0xF0: break;
		case ci::POP_AF_0xF1: break;
		case ci::LD_A_mC_0xF2: break;
		case ci::DI_0xF3: break;
		case ci::Reserved_0xF4: return CPUOperationResult::Default();
		case ci::PUSH_AF_0xF5: break;
		case ci::OR_d8_0xF6: break;
		case ci::RST_30H_0xF7: break;
		case ci::LD_HL_SP_r8_0xF8: break;
		case ci::LD_SP_HL_0xF9: break;
		case ci::LD_A_ma16_0xFA: return operateLD_A_X(env, instr);
		case ci::EI_0xFB: break;
		case ci::Reserved_0xFC: return CPUOperationResult::Default();
		case ci::Reserved_0xFD: return CPUOperationResult::Default();
		case ci::CP_d8_0xFE: break;
		case ci::RST_38H_0xFF: break;
		default: ;
		}

		HWLogger::Error(U"missed instruction unprefixed: {:X}"_fmt(static_cast<uint8>(instr)));
		return CPUOperationResult(0, 0);
	}
}
