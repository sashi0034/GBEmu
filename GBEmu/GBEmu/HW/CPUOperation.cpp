﻿#include "stdafx.h"
#include "CPUOperation.h"

#include "HWEnv.h"
#include "HWLogger.h"
#include "MemoryAddress.h"

namespace GBEmu::HW::CPUOperation
{
	using ci = CPUInstruction;

	constexpr uint16 addr_0xFF00 = 0xFF00;

	inline uint8 undefined8()
	{
		assert(false);
		return 0;
	}

	[[nodiscard]]
	inline CPUOperationResult operateNOP()
	{
		return CPUOperationResult(4);
	}

	[[nodiscard]]
	inline CPUOperationResult operateSTOP_d8(HWEnv& env)
	{
		// TODO: STOPの対応を検討
		// env.GetCPU().SetState(CPUState::Stopped);
		return CPUOperationResult(4);
	}

	[[nodiscard]]
	inline CPUOperationResult operateLD_XX_d16(HWEnv& env, CPUInstruction instr)
	{
		const uint16 d16 = env.GetMemory().Read16(env, env.GetCPU().PC() + 1);

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

		return CPUOperationResult(12);
	}

	[[nodiscard]]
	inline CPUOperationResult operateLD_X_d8(HWEnv& env, CPUInstruction instr)
	{
		const uint8 d8 = env.GetMemory().Read(env, env.GetCPU().PC() + 1);

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

		return CPUOperationResult(8);
	}

	[[nodiscard]]
	inline CPUOperationResult operateLD_X_A(HWEnv& env, CPUInstruction instr)
	{
		auto&& cpu = env.GetCPU();
		auto&& memory = env.GetMemory();
		const uint8 a = cpu.RegA();

		const auto cycle4 = CPUOperationResult(4);
		const auto cycle8 = CPUOperationResult(8);
		const auto cycle16 = CPUOperationResult(16);

		switch (instr)
		{
		case ci::LD_A_A_0x7F:
			cpu.SetA(a); return cycle4;
		case ci::LD_B_A_0x47:
			cpu.SetB(a); return cycle4;
		case ci::LD_C_A_0x4F:
			cpu.SetC(a); return cycle4;
		case ci::LD_D_A_0x57:
			cpu.SetD(a); return cycle4;
		case ci::LD_E_A_0x5F:
			cpu.SetE(a); return cycle4;
		case ci::LD_H_A_0x67:
			cpu.SetH(a); return cycle4;
		case ci::LD_L_A_0x6F:
			cpu.SetL(a); return cycle4;
		case ci::LD_mBC_A_0x02:
			memory.Write(env, cpu.RegBC(), a); return cycle8;
		case ci::LD_mDE_A_0x12:
			memory.Write(env, cpu.RegDE(), a); return cycle8;
		case ci::LD_mHL_A_0x77:
			memory.Write(env, cpu.RegHL(), a); return cycle8;
		case ci::LD_ma16_A_0xEA:
			memory.Write(env, memory.Read16(env, cpu.PC() + 1), a); return cycle16;
		default:
			assert(false);
			return CPUOperationResult::Invalid();
		}
	}

	[[nodiscard]]
	inline CPUOperationResult operateLD_A_X(HWEnv& env, CPUInstruction instr)
	{
		auto&& cpu = env.GetCPU();
		auto&& memory = env.GetMemory();

		const auto cycle4 = CPUOperationResult(4);

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
				pair{memory.Read(env, cpu.RegBC()), CPUOperationResult(8)} :
			instr == ci::LD_A_mDE_0x1A ?
				pair{memory.Read(env, cpu.RegDE()), CPUOperationResult(8)} :
			instr == ci::LD_A_mHL_0x7E ?
				pair{memory.Read(env, cpu.RegHL()), CPUOperationResult(8)} :
			instr == ci::LD_A_ma16_0xFA ?
				pair{memory.Read(env, memory.Read16(env, cpu.PC() + 1)), CPUOperationResult(16)} :
			instr == ci::LD_A_d8_0x3E ?
				pair{memory.Read(env, cpu.PC() + 1), CPUOperationResult(8)} :
			std::pair{undefined8(), CPUOperationResult::Invalid()};

		cpu.SetA(dispatch.first);
		return dispatch.second;
	}

	[[nodiscard]]
	inline CPUOperationResult operateLD_X_X(HWEnv& env, CPUInstruction instr)
	{
		auto&& cpu = env.GetCPU();
		auto&& memory = env.GetMemory();

		const auto result_1_4 = CPUOperationResult(4);
		const auto result_1_8 = CPUOperationResult(8);
		const auto result_2_12 = CPUOperationResult(12);

		switch (instr)
		{
		// destination B
		case ci::LD_B_B_0x40: cpu.SetB(cpu.RegB()); return result_1_4;
		case ci::LD_B_C_0x41: cpu.SetB(cpu.RegC()); return result_1_4;
		case ci::LD_B_D_0x42: cpu.SetB(cpu.RegD()); return result_1_4;
		case ci::LD_B_E_0x43: cpu.SetB(cpu.RegE()); return result_1_4;
		case ci::LD_B_H_0x44: cpu.SetB(cpu.RegH()); return result_1_4;
		case ci::LD_B_L_0x45: cpu.SetB(cpu.RegL()); return result_1_4;
		case ci::LD_B_mHL_0x46: cpu.SetB(memory.Read(env, cpu.RegHL())); return result_1_8;

		// destination C
		case ci::LD_C_B_0x48: cpu.SetC(cpu.RegB()); return result_1_4;
		case ci::LD_C_C_0x49: cpu.SetC(cpu.RegC()); return result_1_4;
		case ci::LD_C_D_0x4A: cpu.SetC(cpu.RegD()); return result_1_4;
		case ci::LD_C_E_0x4B: cpu.SetC(cpu.RegE()); return result_1_4;
		case ci::LD_C_H_0x4C: cpu.SetC(cpu.RegH()); return result_1_4;
		case ci::LD_C_L_0x4D: cpu.SetC(cpu.RegL()); return result_1_4;
		case ci::LD_C_mHL_0x4E: cpu.SetC(memory.Read(env, cpu.RegHL())); return result_1_8;

		// destination D
		case ci::LD_D_B_0x50: cpu.SetD(cpu.RegB()); return result_1_4;
		case ci::LD_D_C_0x51: cpu.SetD(cpu.RegC()); return result_1_4;
		case ci::LD_D_D_0x52: cpu.SetD(cpu.RegD()); return result_1_4;
		case ci::LD_D_E_0x53: cpu.SetD(cpu.RegE()); return result_1_4;
		case ci::LD_D_H_0x54: cpu.SetD(cpu.RegH()); return result_1_4;
		case ci::LD_D_L_0x55: cpu.SetD(cpu.RegL()); return result_1_4;
		case ci::LD_D_mHL_0x56: cpu.SetD(memory.Read(env, cpu.RegHL())); return result_1_8;

		// destination E
		case ci::LD_E_B_0x58: cpu.SetE(cpu.RegB()); return result_1_4;
		case ci::LD_E_C_0x59: cpu.SetE(cpu.RegC()); return result_1_4;
		case ci::LD_E_D_0x5A: cpu.SetE(cpu.RegD()); return result_1_4;
		case ci::LD_E_E_0x5B: cpu.SetE(cpu.RegE()); return result_1_4;
		case ci::LD_E_H_0x5C: cpu.SetE(cpu.RegH()); return result_1_4;
		case ci::LD_E_L_0x5D: cpu.SetE(cpu.RegL()); return result_1_4;
		case ci::LD_E_mHL_0x5E: cpu.SetE(memory.Read(env, cpu.RegHL())); return result_1_8;

		// destination H
		case ci::LD_H_B_0x60: cpu.SetH(cpu.RegB()); return result_1_4;
		case ci::LD_H_C_0x61: cpu.SetH(cpu.RegC()); return result_1_4;
		case ci::LD_H_D_0x62: cpu.SetH(cpu.RegD()); return result_1_4;
		case ci::LD_H_E_0x63: cpu.SetH(cpu.RegE()); return result_1_4;
		case ci::LD_H_H_0x64: cpu.SetH(cpu.RegH()); return result_1_4;
		case ci::LD_H_L_0x65: cpu.SetH(cpu.RegL()); return result_1_4;
		case ci::LD_H_mHL_0x66: cpu.SetH(memory.Read(env, cpu.RegHL())); return result_1_8;

		// destination L
		case ci::LD_L_B_0x68: cpu.SetL(cpu.RegB()); return result_1_4;
		case ci::LD_L_C_0x69: cpu.SetL(cpu.RegC()); return result_1_4;
		case ci::LD_L_D_0x6A: cpu.SetL(cpu.RegD()); return result_1_4;
		case ci::LD_L_E_0x6B: cpu.SetL(cpu.RegE()); return result_1_4;
		case ci::LD_L_H_0x6C: cpu.SetL(cpu.RegH()); return result_1_4;
		case ci::LD_L_L_0x6D: cpu.SetL(cpu.RegL()); return result_1_4;
		case ci::LD_L_mHL_0x6E: cpu.SetL(memory.Read(env, cpu.RegHL())); return result_1_8;

		// destination (HL)
		case ci::LD_mHL_B_0x70: memory.Write(env, cpu.RegHL(), cpu.RegB()); return result_1_8;
		case ci::LD_mHL_C_0x71: memory.Write(env, cpu.RegHL(), cpu.RegC()); return result_1_8;
		case ci::LD_mHL_D_0x72: memory.Write(env, cpu.RegHL(), cpu.RegD()); return result_1_8;
		case ci::LD_mHL_E_0x73: memory.Write(env, cpu.RegHL(), cpu.RegE()); return result_1_8;
		case ci::LD_mHL_H_0x74: memory.Write(env, cpu.RegHL(), cpu.RegH()); return result_1_8;
		case ci::LD_mHL_L_0x75: memory.Write(env, cpu.RegHL(), cpu.RegL()); return result_1_8;
		case ci::LD_mHL_d8_0x36: memory.Write(env, cpu.RegHL(), memory.Read(env, cpu.PC() + 1)); return result_2_12;

		default:
		assert(false);
		return CPUOperationResult::Invalid();
		}
	}

	[[nodiscard]]
	inline CPUOperationResult operateLD_ma16_SP(HWEnv& env)
	{
		// 0x08
		const uint16 a16 = env.GetMemory().Read16(env, env.GetCPU().PC() + 1);
		env.GetMemory().Write16(env, a16, env.GetCPU().SP());
		return CPUOperationResult(20);
	}

	[[nodiscard]]
	inline CPUOperationResult operateLD_mHLi_A(HWEnv& env)
	{
		// 0x22
		auto&& cpu = env.GetCPU();
		env.GetMemory().Write(env, cpu.RegHL(), cpu.RegA());
		cpu.SetHL(cpu.RegHL() + 1);
		return CPUOperationResult(8);
	}

	[[nodiscard]]
	inline CPUOperationResult operateLD_A_mHLi(HWEnv& env)
	{
		// 0x2A
		auto&& cpu = env.GetCPU();
		cpu.SetA(env.GetMemory().Read(env, cpu.RegHL()));
		cpu.SetHL(cpu.RegHL() + 1);
		return CPUOperationResult(8);
	}

	[[nodiscard]]
	inline CPUOperationResult operateLD_mHLd_A(HWEnv& env)
	{
		// 0x32
		auto&& cpu = env.GetCPU();
		env.GetMemory().Write(env, cpu.RegHL(), cpu.RegA());
		cpu.SetHL(cpu.RegHL() - 1);
		return CPUOperationResult(8);
	}

	[[nodiscard]]
	inline CPUOperationResult operateLD_A_mHLd(HWEnv& env)
	{
		// 0x3A
		auto&& cpu = env.GetCPU();
		cpu.SetA(env.GetMemory().Read(env, cpu.RegHL()));
		cpu.SetHL(cpu.RegHL() - 1);
		return CPUOperationResult(8);
	}

	[[nodiscard]]
	inline CPUOperationResult operateLD_mC_A(HWEnv& env)
	{
		// 0xE2
		auto&& cpu = env.GetCPU();
		env.GetMemory().Write(env, addr_0xFF00 + cpu.RegC(), cpu.RegA());
		return CPUOperationResult(8);
	}

	[[nodiscard]]
	inline CPUOperationResult operateLD_A_mC(HWEnv& env)
	{
		// 0xF2
		auto&& cpu = env.GetCPU();
		cpu.SetA(env.GetMemory().Read(env, addr_0xFF00 + cpu.RegC()));
		return CPUOperationResult(8);
	}

	[[nodiscard]]
	inline CPUOperationResult operateLD_SP_HL(HWEnv& env)
	{
		// 0xF9
		auto&& cpu = env.GetCPU();
		cpu.SetSP(cpu.RegHL());
		return CPUOperationResult(8);
	}

	[[nodiscard]]
	inline CPUOperationResult operateLD_HL_SPir8(HWEnv& env)
	{
		// 0xF8
		auto&& cpu = env.GetCPU();
		const int8 r8 = static_cast<int8>(env.GetMemory().Read(env, cpu.PC() + 1));

		const bool h = ((cpu.SP() & 0xF) + (r8 & 0xF)) > 0xF; // bit3からオーバーフローした場合にセット
		const bool c = ((cpu.SP() & 0xFF) + (r8 & 0xFF)) > 0xFF; // bit7からオーバーフローした場合にセット

		cpu.SetHL(cpu.SP() + r8);

		return CPUOperationResult::ByCalc(12, CPUOperationZNHC{false, false, h, c});
	}

	[[nodiscard]]
	inline CPUOperationResult operateLDH_X_X(HWEnv& env, CPUInstruction instr)
	{
		auto&& cpu = env.GetCPU();
		auto&& memory = env.GetMemory();
		const uint8 a8 = memory.Read(env, cpu.PC() + 1);

		switch (instr)
		{
		case ci::LDH_a8_A_0xE0:
			memory.Write(env, addr_0xFF00 + a8, cpu.RegA());
			return CPUOperationResult(12);
		case ci::LDH_A_a8_0xF0:
			cpu.SetA(memory.Read(env, addr_0xFF00 + a8));
			return CPUOperationResult(12);
		default:
			assert(false);
			return CPUOperationResult::Invalid();
		}
	}

	[[nodiscard]]
	inline CPUOperationResult operateINC_XX(HWEnv& env, CPUInstruction instr)
	{
		auto&& cpu = env.GetCPU();
		const auto result = CPUOperationResult(8);

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
	inline CPUOperationResult operateINC_X(HWEnv& env, CPUInstruction instr)
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

		return CPUOperationResult::ByCalc(4, CPUOperationZNHC{z, false, h, cpu.FlagC()});
	}

	[[nodiscard]]
	inline CPUOperationResult operateINC_mHL(HWEnv& env)
	{
		auto&& cpu = env.GetCPU();

		const uint8 before = env.GetMemory().Read(env, cpu.RegHL());
		const bool h = (before & 0xF) == 0xF;

		const uint8 after = before + 1;
		env.GetMemory().Write(env, cpu.RegHL(), after);
		const bool z = after == 0;

		return CPUOperationResult::ByCalc(12,CPUOperationZNHC{z, false, h, cpu.FlagC()});
	}

	[[nodiscard]]
	inline CPUOperationResult operateDEC_XX(HWEnv& env, CPUInstruction instr)
	{
		auto&& cpu = env.GetCPU();
		const auto result = CPUOperationResult(8);

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
	inline CPUOperationResult operateDEC_X(HWEnv& env, CPUInstruction instr)
	{
		auto&& cpu = env.GetCPU();

		const CPUReg8 target =
			instr == ci::DEC_A_0x3D ? CPUReg8::A :
			instr == ci::DEC_B_0x05 ? CPUReg8::B :
			instr == ci::DEC_C_0x0D ? CPUReg8::C :
			instr == ci::DEC_D_0x15 ? CPUReg8::D :
			instr == ci::DEC_E_0x1D ? CPUReg8::E :
			instr == ci::DEC_H_0x25 ? CPUReg8::H :
			instr == ci::DEC_L_0x2D ? CPUReg8::L :
			CPUReg8::Invalid;
		assert(target != CPUReg8::Invalid);

		const bool h = (cpu.GetReg8(target) & 0xF) == 0;

		cpu.SetReg8(target, cpu.GetReg8(target) - 1);

		const bool z = cpu.GetReg8(target) == 0;

		return CPUOperationResult::ByCalc(4, CPUOperationZNHC{z, true, h, cpu.FlagC()});
	}

	[[nodiscard]]
	inline CPUOperationResult operateDEC_mHL(HWEnv& env)
	{
		// 0x35
		auto&& cpu = env.GetCPU();

		const uint8 before = env.GetMemory().Read(env, cpu.RegHL());
		const bool h = (before & 0xF) == 0;

		const uint8 after = before - 1;
		env.GetMemory().Write(env, cpu.RegHL(), after);
		const bool z = after == 0;

		return CPUOperationResult::ByCalc(12, CPUOperationZNHC{z, true, h, cpu.FlagC()});
	}

	[[nodiscard]]
	inline CPUOperationResult operateRLCA(HWEnv& env)
	{
		auto&& cpu = env.GetCPU();

		const uint8 bit7 = cpu.RegA() >> 7;
		cpu.SetA((cpu.RegA() << 1) | bit7);

		// 資料によっては、Z: Set if result is zero となっているのものあるのでしっかりテストしたい
		return CPUOperationResult::ByCalc(4, CPUOperationZNHC{false, false, false, bit7 == 1});
	}

	[[nodiscard]]
	inline CPUOperationResult operateRLA(HWEnv& env)
	{
		// 0x0F
		auto&& cpu = env.GetCPU();

		const uint8 bit7 = cpu.RegA() >> 7;
		const uint8 carry = static_cast<uint8>(cpu.FlagC());

		cpu.SetA((cpu.RegA() << 1) | carry);

		// 資料によっては、Z: Set if result is zero となっているのものあるのでしっかりテストしたい
		return CPUOperationResult::ByCalc(4, CPUOperationZNHC{false, false, false, bit7 == 1});
	}

	[[nodiscard]]
	inline CPUOperationResult operateRRA(HWEnv& env)
	{
		// 0x1F
		auto&& cpu = env.GetCPU();

		const uint8 bit0 = cpu.RegA() &0b1;
		const uint8 carry = static_cast<uint8>(cpu.FlagC());

		cpu.SetA((cpu.RegA() >> 1) | (carry << 7));

		return CPUOperationResult::ByCalc(4, CPUOperationZNHC{false, false, false, bit0 == 0b1});
	}

	[[nodiscard]]
	inline CPUOperationResult operateRRCA(HWEnv& env)
	{
		auto&& cpu = env.GetCPU();

		const uint8 bit0 = cpu.RegA() & 0b1;
		const bool c = bit0 == 0b1;
		cpu.SetA((cpu.RegA() >> 1) | (bit0 << 7));

		// 資料によっては、Z: Set if result is zero となっているのものあるのでしっかりテストしたい
		return CPUOperationResult::ByCalc(4, CPUOperationZNHC{false, false, false, c});
	}

	[[nodiscard]]
	inline CPUOperationResult operateADD_HL_XX(HWEnv& env, CPUInstruction instr)
	{
		auto&& cpu = env.GetCPU();

		const uint16 src =
			instr == ci::ADD_HL_BC_0x09 ? cpu.RegBC() :
			instr == ci::ADD_HL_DE_0x19 ? cpu.RegDE() :
			instr == ci::ADD_HL_HL_0x29 ? cpu.RegHL() :
			instr == ci::ADD_HL_SP_0x39 ? cpu.SP() :
			undefined8();

		const bool h = ((cpu.RegHL() & 0x07FF) + (src & 0x07FF)) > 0x07FF; // bit11からオーバーフローした場合にセット
		const bool c = ((cpu.RegHL() & 0xffff) + (src & 0xffff)) > 0xffff; // bit15からオーバーフローした場合にセット

		cpu.SetHL(cpu.RegHL() + src);

		return CPUOperationResult::ByCalc(8, CPUOperationZNHC{cpu.FlagZ(), false, h, c});
	}

	[[nodiscard]]
	inline CPUOperationResult operateADD_A_X(HWEnv& env, CPUInstruction instr)
	{
		auto&& cpu = env.GetCPU();
		auto&& memory = env.GetMemory();

		const auto result_1_4 = CPUOperationResult(4);

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
				pair{ memory.Read(env, cpu.RegHL()), CPUOperationResult(8) } :
			instr == ci::ADD_A_d8_0xC6 ?
				pair{ memory.Read(env, cpu.PC() + 1), CPUOperationResult(8) } :
			pair{ undefined8(), CPUOperationResult::Invalid() };

		const uint8 add = dispatch.first;

		const bool h = ((cpu.RegA() & 0xF) + (add & 0xF)) > 0xF; // bit3からオーバーフローした場合にセット
		const bool c = ((cpu.RegA() & 0xFF) + (add & 0xFF)) > 0xFF; // bit7からオーバーフローした場合にセット

		cpu.SetA(cpu.RegA() + add);

		dispatch.second.Flag = CPUOperationZNHC{cpu.RegA() == 0, false, h, c};

		return dispatch.second;
	}

	[[nodiscard]]
	inline CPUOperationResult operateADD_SP_r8(HWEnv& env)
	{
		// 0xE8
		auto&& cpu = env.GetCPU();
		auto&& memory = env.GetMemory();

		const int8 r8 = static_cast<int8>(memory.Read(env, cpu.PC() + 1));
		const uint16 sp = cpu.SP();

		const bool h = ((sp & 0xF) + (r8 & 0xF)) > 0xF; // bit3からオーバーフローした場合にセット
		const bool c = ((sp & 0xFF) + (r8 & 0xFF)) > 0xFF; // bit7からオーバーフローした場合にセット

		cpu.SetSP(cpu.SP() + r8);

		return CPUOperationResult::ByCalc(16, CPUOperationZNHC{false, false, h, c});
	}

	[[nodiscard]]
	inline CPUOperationResult operateADC_A_X(HWEnv& env, CPUInstruction instr)
	{
		auto&& cpu = env.GetCPU();
		auto&& memory = env.GetMemory();

		const auto result_1_4 = CPUOperationResult(4);

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
				pair{ memory.Read(env, cpu.RegHL()), CPUOperationResult(8) } :
			instr == ci::ADC_A_d8_0xCE ?
				pair{ memory.Read(env, cpu.PC() + 1), CPUOperationResult(8) } :
			pair{ undefined8(), CPUOperationResult::Invalid() };

		const uint8 add = dispatch.first;
		const uint8 carry = (uint8)cpu.FlagC();

		const bool h = ((cpu.RegA() & 0xF) + (add & 0xF) + carry) > 0xF; // bit3からオーバーフローした場合にセット
		const bool c = ((cpu.RegA() & 0xFF) + (add & 0xFF) + carry) > 0xFF; // bit7からオーバーフローした場合にセット

		cpu.SetA(cpu.RegA() + (add + carry));

		dispatch.second.Flag = CPUOperationZNHC{cpu.RegA() == 0, false, h, c};

		return dispatch.second;
	}

	[[nodiscard]]
	inline CPUOperationResult operateSUB_A_X(HWEnv& env, CPUInstruction instr)
	{
		auto&& cpu = env.GetCPU();
		auto&& memory = env.GetMemory();

		const auto result_1_4 = CPUOperationResult(4);

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
				pair{ memory.Read(env, cpu.RegHL()), CPUOperationResult(8) } :
			instr == ci::SUB_A_d8_0xD6 ?
				pair{ memory.Read(env, cpu.PC() + 1), CPUOperationResult(8) } :
			pair{ undefined8(), CPUOperationResult::Invalid() };

		const uint8 sub = dispatch.first;

		const bool h = ((cpu.RegA() & 0xF) - (sub & 0xF)) < 0; // bit4からボローした場合にセット
		const bool c = cpu.RegA() < sub; // ボローした場合にセット

		cpu.SetA(cpu.RegA() - sub);

		dispatch.second.Flag = CPUOperationZNHC{cpu.RegA() == 0, true, h, c};

		return dispatch.second;
	}

	[[nodiscard]]
	inline CPUOperationResult operateSBC_A_X(HWEnv& env, CPUInstruction instr)
	{
		auto&& cpu = env.GetCPU();
		auto&& memory = env.GetMemory();

		const auto result_1_4 = CPUOperationResult(4);

		using std::pair;
		pair<const uint8, CPUOperationResult> dispatch =
			instr == ci::SBC_A_A_0x9F ? pair{ cpu.RegA(), result_1_4 } :
			instr == ci::SBC_A_B_0x98 ? pair{ cpu.RegB(), result_1_4 } :
			instr == ci::SBC_A_C_0x99 ? pair{ cpu.RegC(), result_1_4 } :
			instr == ci::SBC_A_D_0x9A ? pair{ cpu.RegD(), result_1_4 } :
			instr == ci::SBC_A_E_0x9B ? pair{ cpu.RegE(), result_1_4 } :
			instr == ci::SBC_A_H_0x9C ? pair{ cpu.RegH(), result_1_4 } :
			instr == ci::SBC_A_L_0x9D ? pair{ cpu.RegL(), result_1_4 } :
			instr == ci::SBC_A_mHL_0x9E ?
				pair{ memory.Read(env, cpu.RegHL()), CPUOperationResult(8) } :
			instr == ci::SBC_A_d8_0xDE ?
				pair{ memory.Read(env, cpu.PC() + 1), CPUOperationResult(8) } :
			pair{ undefined8(), CPUOperationResult::Invalid() };

		const uint8 sub = dispatch.first;
		const uint8 carry = cpu.FlagC();

		const bool h = ((cpu.RegA() & 0xF) - ((sub & 0xF) + carry)) < 0; // bit4からボローした場合にセット
		const bool c = cpu.RegA() < sub + carry; // ボローした場合にセット

		cpu.SetA(cpu.RegA() - (sub + carry));

		dispatch.second.Flag = CPUOperationZNHC{cpu.RegA() == 0, true, h, c};

		return dispatch.second;
	}

	[[nodiscard]]
	inline CPUOperationResult operateAND_A_X(HWEnv& env, CPUInstruction instr)
	{
		auto&& cpu = env.GetCPU();
		auto&& memory = env.GetMemory();

		const auto result_1_4 = CPUOperationResult(4);

		using std::pair;
		pair<const uint8, CPUOperationResult> dispatch =
			instr == ci::AND_A_A_0xA7 ? pair{ cpu.RegA(), result_1_4 } :
			instr == ci::AND_A_B_0xA0 ? pair{ cpu.RegB(), result_1_4 } :
			instr == ci::AND_A_C_0xA1 ? pair{ cpu.RegC(), result_1_4 } :
			instr == ci::AND_A_D_0xA2 ? pair{ cpu.RegD(), result_1_4 } :
			instr == ci::AND_A_E_0xA3 ? pair{ cpu.RegE(), result_1_4 } :
			instr == ci::AND_A_H_0xA4 ? pair{ cpu.RegH(), result_1_4 } :
			instr == ci::AND_A_L_0xA5 ? pair{ cpu.RegL(), result_1_4 } :
			instr == ci::AND_A_mHL_0xA6 ?
				pair{ memory.Read(env, cpu.RegHL()), CPUOperationResult(8) } :
			instr == ci::AND_A_d8_0xE6 ?
				pair{ memory.Read(env, cpu.PC() + 1), CPUOperationResult(8) } :
			pair{ undefined8(), CPUOperationResult::Invalid() };

		cpu.SetA(cpu.RegA() & dispatch.first);

		dispatch.second.Flag = CPUOperationZNHC{cpu.RegA() == 0, false, true, false};

		return dispatch.second;
	}

	[[nodiscard]]
	inline CPUOperationResult operateXOR_A_X(HWEnv& env, CPUInstruction instr)
	{
		auto&& cpu = env.GetCPU();
		auto&& memory = env.GetMemory();

		const auto result_1_4 = CPUOperationResult(4);

		using std::pair;
		pair<const uint8, CPUOperationResult> dispatch =
			instr == ci::XOR_A_A_0xAF ? pair{ cpu.RegA(), result_1_4 } :
			instr == ci::XOR_A_B_0xA8 ? pair{ cpu.RegB(), result_1_4 } :
			instr == ci::XOR_A_C_0xA9 ? pair{ cpu.RegC(), result_1_4 } :
			instr == ci::XOR_A_D_0xAA ? pair{ cpu.RegD(), result_1_4 } :
			instr == ci::XOR_A_E_0xAB ? pair{ cpu.RegE(), result_1_4 } :
			instr == ci::XOR_A_H_0xAC ? pair{ cpu.RegH(), result_1_4 } :
			instr == ci::XOR_A_L_0xAD ? pair{ cpu.RegL(), result_1_4 } :
			instr == ci::XOR_A_mHL_0xAE ?
				pair{ memory.Read(env, cpu.RegHL()), CPUOperationResult(8) } :
			instr == ci::XOR_A_d8_0xEE ?
				pair{ memory.Read(env, cpu.PC() + 1), CPUOperationResult(8) } :
			pair{ undefined8(), CPUOperationResult::Invalid() };

		cpu.SetA(cpu.RegA() ^ dispatch.first);

		dispatch.second.Flag = CPUOperationZNHC{cpu.RegA() == 0, false, false, false};

		return dispatch.second;
	}

	[[nodiscard]]
	inline CPUOperationResult operateOR_A_X(HWEnv& env, CPUInstruction instr)
	{
		auto&& cpu = env.GetCPU();
		auto&& memory = env.GetMemory();

		const auto result_1_4 = CPUOperationResult(4);

		using std::pair;
		pair<const uint8, CPUOperationResult> dispatch =
			instr == ci::OR_A_A_0xB7 ? pair{ cpu.RegA(), result_1_4 } :
			instr == ci::OR_A_B_0xB0 ? pair{ cpu.RegB(), result_1_4 } :
			instr == ci::OR_A_C_0xB1 ? pair{ cpu.RegC(), result_1_4 } :
			instr == ci::OR_A_D_0xB2 ? pair{ cpu.RegD(), result_1_4 } :
			instr == ci::OR_A_E_0xB3 ? pair{ cpu.RegE(), result_1_4 } :
			instr == ci::OR_A_H_0xB4 ? pair{ cpu.RegH(), result_1_4 } :
			instr == ci::OR_A_L_0xB5 ? pair{ cpu.RegL(), result_1_4 } :
			instr == ci::OR_A_mHL_0xB6 ?
				pair{ memory.Read(env, cpu.RegHL()), CPUOperationResult(8) } :
			instr == ci::OR_A_d8_0xF6 ?
				pair{ memory.Read(env, cpu.PC() + 1), CPUOperationResult(8) } :
			pair{ undefined8(), CPUOperationResult::Invalid() };

		cpu.SetA(cpu.RegA() | dispatch.first);

		dispatch.second.Flag = CPUOperationZNHC{cpu.RegA() == 0, false, false, false};

		return dispatch.second;
	}

	[[nodiscard]]
	inline CPUOperationResult operateCP_A_X(HWEnv& env, CPUInstruction instr)
	{
		auto&& cpu = env.GetCPU();
		auto&& memory = env.GetMemory();

		const auto result_1_4 = CPUOperationResult(4);

		using std::pair;
		pair<const uint8, CPUOperationResult> dispatch =
			instr == ci::CP_A_A_0xBF ? pair{ cpu.RegA(), result_1_4 } :
			instr == ci::CP_A_B_0xB8 ? pair{ cpu.RegB(), result_1_4 } :
			instr == ci::CP_A_C_0xB9 ? pair{ cpu.RegC(), result_1_4 } :
			instr == ci::CP_A_D_0xBA ? pair{ cpu.RegD(), result_1_4 } :
			instr == ci::CP_A_E_0xBB ? pair{ cpu.RegE(), result_1_4 } :
			instr == ci::CP_A_H_0xBC ? pair{ cpu.RegH(), result_1_4 } :
			instr == ci::CP_A_L_0xBD ? pair{ cpu.RegL(), result_1_4 } :
			instr == ci::CP_A_mHL_0xBE ?
				pair{ memory.Read(env, cpu.RegHL()), CPUOperationResult(8) } :
			instr == ci::CP_A_d8_0xFE ?
				pair{ memory.Read(env, cpu.PC() + 1), CPUOperationResult(8) } :
			pair{ undefined8(), CPUOperationResult::Invalid() };

		const uint8 a = cpu.RegA();
		const uint8 r = dispatch.first;
		dispatch.second.Flag = CPUOperationZNHC{
			a == r,
			true,
			((a & 0xF) - (r & 0xF)) < 0,
			a < r};

		return dispatch.second;
	}

	[[nodiscard]]
	inline CPUOperationResult operateJR_X_r8(HWEnv& env, CPUInstruction instr)
	{
		auto&& cpu = env.GetCPU();
		const int8 r8 = static_cast<int8>(env.GetMemory().Read(env, cpu.PC() + 1));

		const bool toJump =
			instr == ci::JR_r8_0x18 ? true :
			instr == ci::JR_NZ_r8_0x20 ? (cpu.FlagZ() == false) :
			instr == ci::JR_Z_r8_0x28 ? cpu.FlagZ() :
			instr == ci::JR_NC_r8_0x30 ? (cpu.FlagC() == false) :
			instr == ci::JR_C_r8_0x38 ? cpu.FlagC() :
			undefined8();

		constexpr int bytes = InformCPUInstruction()[static_cast<int>(ci::JR_r8_0x18)].ByteLength; // 2
		return toJump
			// 命令実行時では、このPCは命令実行前のPCを指しているので、nextPCにはbyteLengthを足しておく
			? CPUOperationResult::ByJump(12, cpu.PC() + bytes + r8)
			: CPUOperationResult(8);
	}

	[[nodiscard]]
	inline CPUOperationResult operateJP_X_a16(HWEnv& env, CPUInstruction instr)
	{
		auto&& cpu = env.GetCPU();

		const bool toJump =
			instr == ci::JP_a16_0xC3 ? true :
			instr == ci::JP_NZ_a16_0xC2 ? (cpu.FlagZ() == false) :
			instr == ci::JP_Z_a16_0xCA ? cpu.FlagZ() :
			instr == ci::JP_NC_a16_0xD2 ? (cpu.FlagC() == false) :
			instr == ci::JP_C_a16_0xDA ? cpu.FlagC() :
			undefined8();

		return toJump
			? CPUOperationResult::ByJump(16, env.GetMemory().Read16(env, cpu.PC() + 1))
			: CPUOperationResult(12);
	}

	[[nodiscard]]
	inline CPUOperationResult operateJP_HL(HWEnv& env)
	{
		// 0xE9
		auto&& cpu = env.GetCPU();
		return CPUOperationResult::ByJump(4, cpu.RegHL());
	}

	[[nodiscard]]
	inline CPUOperationResult operateCALL_X_a16(HWEnv& env, CPUInstruction instr)
	{
		auto&& cpu = env.GetCPU();
		auto&& memory = env.GetMemory();

		const bool toCall =
			instr == ci::CALL_a16_0xCD ? true :
			instr == ci::CALL_NZ_a16_0xC4 ? (cpu.FlagZ() == false) :
			instr == ci::CALL_Z_a16_0xCC ? cpu.FlagZ() :
			instr == ci::CALL_NC_a16_0xD4 ? (cpu.FlagC() == false) :
			instr == ci::CALL_C_a16_0xDC ? cpu.FlagC() :
			undefined8();

		constexpr int bytes = InformCPUInstruction()[static_cast<int>(ci::CALL_a16_0xCD)].ByteLength; // 3
		if (toCall)
		{
			memory.Write(env, cpu.SP() - 1, (cpu.PC() + bytes) >> 8);
			memory.Write(env, cpu.SP() - 2, (cpu.PC() + bytes) & 0xFF);
			cpu.SetSP(cpu.SP() - 2);
			return CPUOperationResult::ByJump(24, env.GetMemory().Read16(env, cpu.PC() + 1));
		}
		else
		{
			return CPUOperationResult(12);
		}
	}

	[[nodiscard]]
	inline CPUOperationResult operateRST_XXh(HWEnv& env, CPUInstruction instr)
	{
		auto&& cpu = env.GetCPU();
		auto&& memory = env.GetMemory();

		const uint16 addr =
			instr == ci::RST_00h_0xC7 ? 0x00 :
			instr == ci::RST_08h_0xCF ? 0x08 :
			instr == ci::RST_10h_0xD7 ? 0x10 :
			instr == ci::RST_18h_0xDF ? 0x18 :
			instr == ci::RST_20h_0xE7 ? 0x20 :
			instr == ci::RST_28h_0xEF ? 0x28 :
			instr == ci::RST_30h_0xF7 ? 0x30 :
			instr == ci::RST_38h_0xFF ? 0x38 :
			undefined8();

		constexpr uint16 bytes = 1;
		memory.Write(env, cpu.SP() - 1, (cpu.PC() + bytes) >> 8);
		memory.Write(env, cpu.SP() - 2, (cpu.PC() + bytes) & 0xFF);
		cpu.SetSP(cpu.SP() - 2);
		return CPUOperationResult::ByJump(16, addr);
	}

	[[nodiscard]]
	inline CPUOperationResult operatePUSH_XX(HWEnv& env, CPUInstruction instr)
	{
		auto&& cpu = env.GetCPU();
		auto&& memory = env.GetMemory();

		using tuple = std::tuple<uint8, uint8>;
		const auto [data1, data2] =
			instr == ci::PUSH_AF_0xF5 ? tuple{cpu.RegA(), cpu.RegF()} :
			instr == ci::PUSH_BC_0xC5 ? tuple{cpu.RegB(), cpu.RegC()} :
			instr == ci::PUSH_DE_0xD5 ? tuple{cpu.RegD(), cpu.RegE()} :
			instr == ci::PUSH_HL_0xE5 ? tuple{cpu.RegH(), cpu.RegL()} :
			tuple{undefined8(), undefined8()};

		memory.Write(env, cpu.SP() - 1, data1);
		memory.Write(env, cpu.SP() - 2, data2);
		cpu.SetSP(cpu.SP() - 2);

		return CPUOperationResult(16);
	}

	[[nodiscard]]
	inline CPUOperationResult operateRET_X(HWEnv& env, CPUInstruction instr)
	{
		auto&& cpu = env.GetCPU();

		using tuple = std::tuple<bool, uint8, uint8>;
		const auto [toRet, cycleOnRet, cycleOnSkip] =
			instr == ci::RET_0xC9 ?
				tuple{true, 16, 16} :
			instr == ci::RET_NZ_0xC0 ?
				tuple{(cpu.FlagZ() == false), 20, 8} :
			instr == ci::RET_Z_0xC8 ?
				tuple{cpu.FlagZ(), 20, 8} :
			instr == ci::RET_NC_0xD0 ?
				tuple{(cpu.FlagC() == false), 20, 8} :
			instr == ci::RET_C_0xD8 ?
				tuple{cpu.FlagC(), 20, 8} :
			tuple{undefined8(), undefined8(), undefined8()};

		if (toRet)
		{
			const uint16 sp = cpu.SP();
			cpu.SetSP(sp + 2);
			return CPUOperationResult::ByJump(cycleOnRet, env.GetMemory().Read16(env, sp));
		}
		else
		{
			return CPUOperationResult(cycleOnSkip);
		}
	}

	[[nodiscard]]
	inline CPUOperationResult operateRETI(HWEnv& env)
	{
		// 0xD9
		auto&& cpu = env.GetCPU();
		const uint16 sp = cpu.SP();
		cpu.SetSP(sp + 2);
		cpu.RequestEnableIME();
		return CPUOperationResult::ByJump(16, env.GetMemory().Read16(env, sp));
	}

	[[nodiscard]]
	inline CPUOperationResult operatePOP_XX(HWEnv& env, CPUInstruction instr)
	{
		auto&& cpu = env.GetCPU();

		const uint16 r16 = env.GetMemory().Read16(env, cpu.SP());

		switch (instr)
		{
		case ci::POP_AF_0xF1:
			cpu.SetAF(r16 & 0xFFF0); break; // Fはフラグ部分4~7bitのみに影響を与える
		case ci::POP_BC_0xC1:
			cpu.SetBC(r16); break;
		case ci::POP_DE_0xD1:
			cpu.SetDE(r16); break;
		case ci::POP_HL_0xE1:
			cpu.SetHL(r16); break;
		default:
			assert(false);
		}

		cpu.SetSP(cpu.SP() + 2);

		return CPUOperationResult(12);
	}

	[[nodiscard]]
	inline CPUOperationResult operateCPL(HWEnv& env)
	{
		// 0x2F
		auto&& cpu = env.GetCPU();

		cpu.SetA(cpu.RegA() ^ 0xFF);

		return CPUOperationResult::ByCalc(4, CPUOperationZNHC{cpu.FlagZ(), true, true, cpu.FlagC()});
	}

	[[nodiscard]]
	inline CPUOperationResult operateSCF(HWEnv& env)
	{
		// 0x37
		auto&& cpu = env.GetCPU();
		return CPUOperationResult::ByCalc(4, CPUOperationZNHC{cpu.FlagZ(), false, false, true});
	}

	[[nodiscard]]
	inline CPUOperationResult operateCCF(HWEnv& env)
	{
		// 0x3F
		auto&& cpu = env.GetCPU();
		return CPUOperationResult::ByCalc(4, CPUOperationZNHC{cpu.FlagZ(), false, false, !cpu.FlagC()});
	}

	[[nodiscard]]
	inline CPUOperationResult operateDI(HWEnv& env)
	{
		// 0xF3
		auto&& cpu = env.GetCPU();
		cpu.DisableIME();
		return CPUOperationResult(4);
	}

	[[nodiscard]]
	inline CPUOperationResult operateEI(HWEnv& env)
	{
		// 0xFB
		auto&& cpu = env.GetCPU();
		cpu.RequestEnableIME();
		return CPUOperationResult(4);
	}

	[[nodiscard]]
	inline CPUOperationResult operateDAA(HWEnv& env)
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

		return CPUOperationResult::ByCalc(4, CPUOperationZNHC{z, cpu.FlagN(), false, c});
	}

	[[nodiscard]]
	inline CPUOperationResult operateHALT(HWEnv& env)
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
			const uint8 interruptEnable = memory.Interrupt().IE();
			const uint8 interruptFlag = memory.Interrupt().IF();
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

		return CPUOperationResult(4);
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
		case ci::SBC_A_B_0x98: return operateSBC_A_X(env, instr);
		case ci::SBC_A_C_0x99: return operateSBC_A_X(env, instr);
		case ci::SBC_A_D_0x9A: return operateSBC_A_X(env, instr);
		case ci::SBC_A_E_0x9B: return operateSBC_A_X(env, instr);
		case ci::SBC_A_H_0x9C: return operateSBC_A_X(env, instr);
		case ci::SBC_A_L_0x9D: return operateSBC_A_X(env, instr);
		case ci::SBC_A_mHL_0x9E: return operateSBC_A_X(env, instr);
		case ci::SBC_A_A_0x9F: return operateSBC_A_X(env, instr);
		case ci::AND_A_B_0xA0: return operateAND_A_X(env, instr);
		case ci::AND_A_C_0xA1: return operateAND_A_X(env, instr);
		case ci::AND_A_D_0xA2: return operateAND_A_X(env, instr);
		case ci::AND_A_E_0xA3: return operateAND_A_X(env, instr);
		case ci::AND_A_H_0xA4: return operateAND_A_X(env, instr);
		case ci::AND_A_L_0xA5: return operateAND_A_X(env, instr);
		case ci::AND_A_mHL_0xA6: return operateAND_A_X(env, instr);
		case ci::AND_A_A_0xA7: return operateAND_A_X(env, instr);
		case ci::XOR_A_B_0xA8: return operateXOR_A_X(env, instr);
		case ci::XOR_A_C_0xA9: return operateXOR_A_X(env, instr);
		case ci::XOR_A_D_0xAA: return operateXOR_A_X(env, instr);
		case ci::XOR_A_E_0xAB: return operateXOR_A_X(env, instr);
		case ci::XOR_A_H_0xAC: return operateXOR_A_X(env, instr);
		case ci::XOR_A_L_0xAD: return operateXOR_A_X(env, instr);
		case ci::XOR_A_mHL_0xAE: return operateXOR_A_X(env, instr);
		case ci::XOR_A_A_0xAF: return operateXOR_A_X(env, instr);
		case ci::OR_A_B_0xB0: return operateOR_A_X(env, instr);
		case ci::OR_A_C_0xB1: return operateOR_A_X(env, instr);
		case ci::OR_A_D_0xB2: return operateOR_A_X(env, instr);
		case ci::OR_A_E_0xB3: return operateOR_A_X(env, instr);
		case ci::OR_A_H_0xB4: return operateOR_A_X(env, instr);
		case ci::OR_A_L_0xB5: return operateOR_A_X(env, instr);
		case ci::OR_A_mHL_0xB6: return operateOR_A_X(env, instr);
		case ci::OR_A_A_0xB7: return operateOR_A_X(env, instr);
		case ci::CP_A_B_0xB8: return operateCP_A_X(env, instr);
		case ci::CP_A_C_0xB9: return operateCP_A_X(env, instr);
		case ci::CP_A_D_0xBA: return operateCP_A_X(env, instr);
		case ci::CP_A_E_0xBB: return operateCP_A_X(env, instr);
		case ci::CP_A_H_0xBC: return operateCP_A_X(env, instr);
		case ci::CP_A_L_0xBD: return operateCP_A_X(env, instr);
		case ci::CP_A_mHL_0xBE: return operateCP_A_X(env, instr);
		case ci::CP_A_A_0xBF: return operateCP_A_X(env, instr);
		case ci::RET_NZ_0xC0: return operateRET_X(env, instr);
		case ci::POP_BC_0xC1: return operatePOP_XX(env, instr);
		case ci::JP_NZ_a16_0xC2: return operateJP_X_a16(env, instr);
		case ci::JP_a16_0xC3: return operateJP_X_a16(env, instr);
		case ci::CALL_NZ_a16_0xC4: return operateCALL_X_a16(env, instr);
		case ci::PUSH_BC_0xC5: return operatePUSH_XX(env, instr);
		case ci::ADD_A_d8_0xC6: return operateADD_A_X(env, instr);
		case ci::RST_00h_0xC7: return operateRST_XXh(env, instr);
		case ci::RET_Z_0xC8: return operateRET_X(env, instr);
		case ci::RET_0xC9: return operateRET_X(env, instr);
		case ci::JP_Z_a16_0xCA: return operateJP_X_a16(env, instr);;
		case ci::Prefix_0xCB: assert(false); return CPUOperationResult::Invalid();
		case ci::CALL_Z_a16_0xCC: return operateCALL_X_a16(env, instr);
		case ci::CALL_a16_0xCD: return operateCALL_X_a16(env, instr);
		case ci::ADC_A_d8_0xCE: return operateADC_A_X(env, instr);
		case ci::RST_08h_0xCF: return operateRST_XXh(env, instr);
		case ci::RET_NC_0xD0: return operateRET_X(env, instr);
		case ci::POP_DE_0xD1: return operatePOP_XX(env, instr);
		case ci::JP_NC_a16_0xD2: return operateJP_X_a16(env, instr);;
		case ci::Reserved_0xD3: return CPUOperationResult::Default();
		case ci::CALL_NC_a16_0xD4: return operateCALL_X_a16(env, instr);
		case ci::PUSH_DE_0xD5: return operatePUSH_XX(env, instr);;
		case ci::SUB_A_d8_0xD6: return operateSUB_A_X(env, instr);
		case ci::RST_10h_0xD7: return operateRST_XXh(env, instr);
		case ci::RET_C_0xD8: return operateRET_X(env, instr);
		case ci::RETI_0xD9: return operateRETI(env);
		case ci::JP_C_a16_0xDA: return operateJP_X_a16(env, instr);;
		case ci::Reserved_0xDB: return CPUOperationResult::Default();
		case ci::CALL_C_a16_0xDC: return operateCALL_X_a16(env, instr);
		case ci::Reserved_0xDD: return CPUOperationResult::Default();
		case ci::SBC_A_d8_0xDE: return operateSBC_A_X(env, instr);
		case ci::RST_18h_0xDF: return operateRST_XXh(env, instr);
		case ci::LDH_a8_A_0xE0: return operateLDH_X_X(env, instr);
		case ci::POP_HL_0xE1: return operatePOP_XX(env, instr);
		case ci::LD_mC_A_0xE2: return operateLD_mC_A(env);
		case ci::Reserved_0xE3: return CPUOperationResult::Default();
		case ci::Reserved_0xE4: return CPUOperationResult::Default();
		case ci::PUSH_HL_0xE5: return operatePUSH_XX(env, instr);;
		case ci::AND_A_d8_0xE6: return operateAND_A_X(env, instr);
		case ci::RST_20h_0xE7: return operateRST_XXh(env, instr);
		case ci::ADD_SP_r8_0xE8: return operateADD_SP_r8(env);
		case ci::JP_HL_0xE9: return operateJP_HL(env);
		case ci::LD_ma16_A_0xEA: return operateLD_X_A(env, instr);
		case ci::Reserved_0xEB: return CPUOperationResult::Default();
		case ci::Reserved_0xEC: return CPUOperationResult::Default();
		case ci::Reserved_0xED: return CPUOperationResult::Default();
		case ci::XOR_A_d8_0xEE: return operateXOR_A_X(env, instr);
		case ci::RST_28h_0xEF: return operateRST_XXh(env, instr);
		case ci::LDH_A_a8_0xF0: return operateLDH_X_X(env, instr);
		case ci::POP_AF_0xF1: return operatePOP_XX(env, instr);
		case ci::LD_A_mC_0xF2: return operateLD_A_mC(env);
		case ci::DI_0xF3: return operateDI(env);
		case ci::Reserved_0xF4: return CPUOperationResult::Default();
		case ci::PUSH_AF_0xF5: return operatePUSH_XX(env, instr);
		case ci::OR_A_d8_0xF6: return operateOR_A_X(env, instr);
		case ci::RST_30h_0xF7: return operateRST_XXh(env, instr);
		case ci::LD_HL_SPir8_0xF8: return operateLD_HL_SPir8(env);
		case ci::LD_SP_HL_0xF9: return operateLD_SP_HL(env);
		case ci::LD_A_ma16_0xFA: return operateLD_A_X(env, instr);
		case ci::EI_0xFB: return operateEI(env);
		case ci::Reserved_0xFC: return CPUOperationResult::Default();
		case ci::Reserved_0xFD: return CPUOperationResult::Default();
		case ci::CP_A_d8_0xFE: return operateCP_A_X(env, instr);
		case ci::RST_38h_0xFF: return operateRST_XXh(env, instr);
		default: ;
		}

		HWLogger::Error(U"missed instruction unprefixed: {:X}"_fmt(static_cast<uint8>(instr)));
		return CPUOperationResult(0);
	}
}
