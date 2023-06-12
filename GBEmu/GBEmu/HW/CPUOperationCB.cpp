#include "stdafx.h"
#include "CPUOperationCB.h"

#include "HWLogger.h"

namespace GBEmu::HW::CPUOperationCB
{
	using ci = CPUInstructionCB;

	inline uint8 undefined8()
	{
		assert(false);
		return 0;
	}

	[[nodiscard]]
	CPUOperationResult operateRLC_X(HWEnv& env, CPUInstructionCB instr)
	{
		auto&& cpu = env.GetCPU();

		const CPUReg8 reg =
			instr == ci::RLC_A_0x07 ? CPUReg8::A :
			instr == ci::RLC_B_0x00 ? CPUReg8::B :
			instr == ci::RLC_C_0x01 ? CPUReg8::C :
			instr == ci::RLC_D_0x02 ? CPUReg8::D :
			instr == ci::RLC_E_0x03 ? CPUReg8::E :
			instr == ci::RLC_H_0x04 ? CPUReg8::H :
			instr == ci::RLC_L_0x05 ? CPUReg8::L :
			undefined8();

		const uint8 before = cpu.GetReg8(reg);
		const uint8 bit7 = before >> 7;

		const bool z = before == 0;
		const bool c = bit7 == 1;

		cpu.SetReg8(reg, (before << 1) | bit7);

		return CPUOperationResult::ByCalc(2, 8, CPUOperationZNHC{z, false, false, c});
	}

	[[nodiscard]]
	CPUOperationResult operateRLC_mHL(HWEnv& env)
	{
		// 0x06
		auto&& cpu = env.GetCPU();
		auto&& memory = env.GetMemory();

		const uint8 data = memory.Read(cpu.RegHL());
		const uint8 bit7 = data >> 7;

		const bool z = data == 0;
		const bool c = bit7 == 1;

		memory.Write(cpu.RegHL(), (data << 1) | bit7);

		return CPUOperationResult::ByCalc(2, 16, CPUOperationZNHC{z, false, false, c});
	}

	[[nodiscard]]
	CPUOperationResult operateRRC_X(HWEnv& env, CPUInstructionCB instr)
	{
		auto&& cpu = env.GetCPU();

		const CPUReg8 reg =
			instr == ci::RRC_A_0x0F ? CPUReg8::A :
			instr == ci::RRC_B_0x08 ? CPUReg8::B :
			instr == ci::RRC_C_0x09 ? CPUReg8::C :
			instr == ci::RRC_D_0x0A ? CPUReg8::D :
			instr == ci::RRC_E_0x0B ? CPUReg8::E :
			instr == ci::RRC_H_0x0C ? CPUReg8::H :
			instr == ci::RRC_L_0x0D ? CPUReg8::L :
			undefined8();

		const uint8 before = cpu.GetReg8(reg);
		const uint8 bit0 = before & 0b1;

		const bool z = before == 0;
		const bool c = bit0 == 1;

		cpu.SetReg8(reg, (before >> 1) | (bit0 << 7));

		return CPUOperationResult::ByCalc(2, 8, CPUOperationZNHC{z, false, false, c});
	}

	[[nodiscard]]
	CPUOperationResult operateRRC_mHL(HWEnv& env)
	{
		// 0x0E
		auto&& cpu = env.GetCPU();
		auto&& memory = env.GetMemory();

		const uint8 data = memory.Read(cpu.RegHL());
		const uint8 bit0 = data & 0b1;

		const bool z = data == 0;
		const bool c = bit0 == 1;

		memory.Write(cpu.RegHL(), (data >> 1) | (bit0 << 7));

		return CPUOperationResult::ByCalc(2, 16, CPUOperationZNHC{z, false, false, c});
	}

	[[nodiscard]]
	CPUOperationResult operateRL_X(HWEnv& env, CPUInstructionCB instr)
	{
		auto&& cpu = env.GetCPU();

		const CPUReg8 reg =
			instr == ci::RL_A_0x17 ? CPUReg8::A :
			instr == ci::RL_B_0x10 ? CPUReg8::B :
			instr == ci::RL_C_0x11 ? CPUReg8::C :
			instr == ci::RL_D_0x12 ? CPUReg8::D :
			instr == ci::RL_E_0x13 ? CPUReg8::E :
			instr == ci::RL_H_0x14 ? CPUReg8::H :
			instr == ci::RL_L_0x15 ? CPUReg8::L :
			undefined8();

		const uint8 before = cpu.GetReg8(reg);
		const uint8 bit7 = before >> 7;
		const uint8 carry = cpu.FlagC();

		const uint8 after = (before << 1) | carry;

		const bool z = after == 0;
		const bool c = bit7 == 1;

		cpu.SetReg8(reg, after);

		return CPUOperationResult::ByCalc(2, 8, CPUOperationZNHC{z, false, false, c});
	}

	[[nodiscard]]
	CPUOperationResult operateRL_mHL(HWEnv& env)
	{
		// 0x0E
		auto&& cpu = env.GetCPU();
		auto&& memory = env.GetMemory();

		const uint8 before = memory.Read(cpu.RegHL());
		const uint8 bit7 = before >> 7;
		const uint8 carry = cpu.FlagC();

		const uint8 after = (before << 1) | carry;

		const bool z = after == 0;
		const bool c = bit7 == 1;

		memory.Write(cpu.RegHL(), after);

		return CPUOperationResult::ByCalc(2, 16, CPUOperationZNHC{z, false, false, c});
	}

	[[nodiscard]]
	CPUOperationResult operateRR_X(HWEnv& env, CPUInstructionCB instr)
	{
		auto&& cpu = env.GetCPU();

		const CPUReg8 reg =
			instr == ci::RR_A_0x1F ? CPUReg8::A :
			instr == ci::RR_B_0x18 ? CPUReg8::B :
			instr == ci::RR_C_0x19 ? CPUReg8::C :
			instr == ci::RR_D_0x1A ? CPUReg8::D :
			instr == ci::RR_E_0x1B ? CPUReg8::E :
			instr == ci::RR_H_0x1C ? CPUReg8::H :
			instr == ci::RR_L_0x1D ? CPUReg8::L :
			undefined8();

		const uint8 before = cpu.GetReg8(reg);
		const uint8 bit0 = before & 0b1;
		const uint8 carry = cpu.FlagC();

		const uint8 after = (before >> 1) | (carry << 7);

		const bool z = after == 0;
		const bool c = bit0 == 1;

		cpu.SetReg8(reg, after);

		return CPUOperationResult::ByCalc(2, 8, CPUOperationZNHC{z, false, false, c});
	}

	[[nodiscard]]
	CPUOperationResult operateRR_mHL(HWEnv& env)
	{
		// 0x1E
		auto&& cpu = env.GetCPU();
		auto&& memory = env.GetMemory();

		const uint8 before = memory.Read(cpu.RegHL());
		const uint8 bit0 = before & 0b1;
		const uint8 carry = cpu.FlagC();

		const uint8 after = (before >> 1) | (carry << 7);

		const bool z = after == 0;
		const bool c = bit0 == 1;

		memory.Write(cpu.RegHL(), after);

		return CPUOperationResult::ByCalc(2, 16, CPUOperationZNHC{z, false, false, c});
	}

	[[nodiscard]]
	CPUOperationResult operateSLA_X(HWEnv& env, CPUInstructionCB instr)
	{
		auto&& cpu = env.GetCPU();

		const CPUReg8 reg =
			instr == ci::SLA_A_0x27 ? CPUReg8::A :
			instr == ci::SLA_B_0x20 ? CPUReg8::B :
			instr == ci::SLA_C_0x21 ? CPUReg8::C :
			instr == ci::SLA_D_0x22 ? CPUReg8::D :
			instr == ci::SLA_E_0x23 ? CPUReg8::E :
			instr == ci::SLA_H_0x24 ? CPUReg8::H :
			instr == ci::SLA_L_0x25 ? CPUReg8::L :
			undefined8();

		const uint8 before = cpu.GetReg8(reg);
		const uint8 after = before << 1;

		const bool z = after == 0;
		const bool c = before & (1 << 7);

		cpu.SetReg8(reg, after);

		return CPUOperationResult::ByCalc(2, 8, CPUOperationZNHC{z, false, false, c});
	}


	const CPUOperationResult OperateInstructionCB(HWEnv& env, CPUInstructionCB instr)
	{
		switch (instr)
		{
		case ci::RLC_B_0x00: return operateRLC_X(env, instr);
		case ci::RLC_C_0x01: return operateRLC_X(env, instr);
		case ci::RLC_D_0x02: return operateRLC_X(env, instr);
		case ci::RLC_E_0x03: return operateRLC_X(env, instr);
		case ci::RLC_H_0x04: return operateRLC_X(env, instr);
		case ci::RLC_L_0x05: return operateRLC_X(env, instr);
		case ci::RLC_mHL_0x06: return operateRLC_mHL(env);
		case ci::RLC_A_0x07: return operateRLC_X(env, instr);
		case ci::RRC_B_0x08: return operateRRC_X(env, instr);
		case ci::RRC_C_0x09: return operateRRC_X(env, instr);
		case ci::RRC_D_0x0A: return operateRRC_X(env, instr);
		case ci::RRC_E_0x0B: return operateRRC_X(env, instr);
		case ci::RRC_H_0x0C: return operateRRC_X(env, instr);
		case ci::RRC_L_0x0D: return operateRRC_X(env, instr);
		case ci::RRC_mHL_0x0E: return operateRRC_mHL(env);
		case ci::RRC_A_0x0F: return operateRRC_X(env, instr);
		case ci::RL_B_0x10: return operateRL_X(env, instr);
		case ci::RL_C_0x11: return operateRL_X(env, instr);
		case ci::RL_D_0x12: return operateRL_X(env, instr);
		case ci::RL_E_0x13: return operateRL_X(env, instr);
		case ci::RL_H_0x14: return operateRL_X(env, instr);
		case ci::RL_L_0x15: return operateRL_X(env, instr);
		case ci::RL_mHL_0x16: return operateRL_mHL(env);
		case ci::RL_A_0x17: return operateRL_X(env, instr);
		case ci::RR_B_0x18: return operateRR_X(env, instr);
		case ci::RR_C_0x19: return operateRR_X(env, instr);
		case ci::RR_D_0x1A: return operateRR_X(env, instr);
		case ci::RR_E_0x1B: return operateRR_X(env, instr);
		case ci::RR_H_0x1C: return operateRR_X(env, instr);
		case ci::RR_L_0x1D: return operateRR_X(env, instr);
		case ci::RR_HL_0x1E: return operateRR_mHL(env);
		case ci::RR_A_0x1F: return operateRR_X(env, instr);
		case ci::SLA_B_0x20: return operateSLA_X(env, instr);
		case ci::SLA_C_0x21: return operateSLA_X(env, instr);
		case ci::SLA_D_0x22: return operateSLA_X(env, instr);
		case ci::SLA_E_0x23: return operateSLA_X(env, instr);
		case ci::SLA_H_0x24: return operateSLA_X(env, instr);
		case ci::SLA_L_0x25: return operateSLA_X(env, instr);
		case ci::SLA_HL_0x26: break;
		case ci::SLA_A_0x27: return operateSLA_X(env, instr);
		case ci::SRA_B_0x28: break;
		case ci::SRA_C_0x29: break;
		case ci::SRA_D_0x2A: break;
		case ci::SRA_E_0x2B: break;
		case ci::SRA_H_0x2C: break;
		case ci::SRA_L_0x2D: break;
		case ci::SRA_HL_0x2E: break;
		case ci::SRA_A_0x2F: break;
		case ci::SWAP_B_0x30: break;
		case ci::SWAP_C_0x31: break;
		case ci::SWAP_D_0x32: break;
		case ci::SWAP_E_0x33: break;
		case ci::SWAP_H_0x34: break;
		case ci::SWAP_L_0x35: break;
		case ci::SWAP_HL_0x36: break;
		case ci::SWAP_A_0x37: break;
		case ci::SRL_B_0x38: break;
		case ci::SRL_C_0x39: break;
		case ci::SRL_D_0x3A: break;
		case ci::SRL_E_0x3B: break;
		case ci::SRL_H_0x3C: break;
		case ci::SRL_L_0x3D: break;
		case ci::SRL_HL_0x3E: break;
		case ci::SRL_A_0x3F: break;
		case ci::BIT_0_B_0x40: break;
		case ci::BIT_0_C_0x41: break;
		case ci::BIT_0_D_0x42: break;
		case ci::BIT_0_E_0x43: break;
		case ci::BIT_0_H_0x44: break;
		case ci::BIT_0_L_0x45: break;
		case ci::BIT_0_HL_0x46: break;
		case ci::BIT_0_A_0x47: break;
		case ci::BIT_1_B_0x48: break;
		case ci::BIT_1_C_0x49: break;
		case ci::BIT_1_D_0x4A: break;
		case ci::BIT_1_E_0x4B: break;
		case ci::BIT_1_H_0x4C: break;
		case ci::BIT_1_L_0x4D: break;
		case ci::BIT_1_HL_0x4E: break;
		case ci::BIT_1_A_0x4F: break;
		case ci::BIT_2_B_0x50: break;
		case ci::BIT_2_C_0x51: break;
		case ci::BIT_2_D_0x52: break;
		case ci::BIT_2_E_0x53: break;
		case ci::BIT_2_H_0x54: break;
		case ci::BIT_2_L_0x55: break;
		case ci::BIT_2_HL_0x56: break;
		case ci::BIT_2_A_0x57: break;
		case ci::BIT_3_B_0x58: break;
		case ci::BIT_3_C_0x59: break;
		case ci::BIT_3_D_0x5A: break;
		case ci::BIT_3_E_0x5B: break;
		case ci::BIT_3_H_0x5C: break;
		case ci::BIT_3_L_0x5D: break;
		case ci::BIT_3_HL_0x5E: break;
		case ci::BIT_3_A_0x5F: break;
		case ci::BIT_4_B_0x60: break;
		case ci::BIT_4_C_0x61: break;
		case ci::BIT_4_D_0x62: break;
		case ci::BIT_4_E_0x63: break;
		case ci::BIT_4_H_0x64: break;
		case ci::BIT_4_L_0x65: break;
		case ci::BIT_4_HL_0x66: break;
		case ci::BIT_4_A_0x67: break;
		case ci::BIT_5_B_0x68: break;
		case ci::BIT_5_C_0x69: break;
		case ci::BIT_5_D_0x6A: break;
		case ci::BIT_5_E_0x6B: break;
		case ci::BIT_5_H_0x6C: break;
		case ci::BIT_5_L_0x6D: break;
		case ci::BIT_5_HL_0x6E: break;
		case ci::BIT_5_A_0x6F: break;
		case ci::BIT_6_B_0x70: break;
		case ci::BIT_6_C_0x71: break;
		case ci::BIT_6_D_0x72: break;
		case ci::BIT_6_E_0x73: break;
		case ci::BIT_6_H_0x74: break;
		case ci::BIT_6_L_0x75: break;
		case ci::BIT_6_HL_0x76: break;
		case ci::BIT_6_A_0x77: break;
		case ci::BIT_7_B_0x78: break;
		case ci::BIT_7_C_0x79: break;
		case ci::BIT_7_D_0x7A: break;
		case ci::BIT_7_E_0x7B: break;
		case ci::BIT_7_H_0x7C: break;
		case ci::BIT_7_L_0x7D: break;
		case ci::BIT_7_HL_0x7E: break;
		case ci::BIT_7_A_0x7F: break;
		case ci::RES_0_B_0x80: break;
		case ci::RES_0_C_0x81: break;
		case ci::RES_0_D_0x82: break;
		case ci::RES_0_E_0x83: break;
		case ci::RES_0_H_0x84: break;
		case ci::RES_0_L_0x85: break;
		case ci::RES_0_HL_0x86: break;
		case ci::RES_0_A_0x87: break;
		case ci::RES_1_B_0x88: break;
		case ci::RES_1_C_0x89: break;
		case ci::RES_1_D_0x8A: break;
		case ci::RES_1_E_0x8B: break;
		case ci::RES_1_H_0x8C: break;
		case ci::RES_1_L_0x8D: break;
		case ci::RES_1_HL_0x8E: break;
		case ci::RES_1_A_0x8F: break;
		case ci::RES_2_B_0x90: break;
		case ci::RES_2_C_0x91: break;
		case ci::RES_2_D_0x92: break;
		case ci::RES_2_E_0x93: break;
		case ci::RES_2_H_0x94: break;
		case ci::RES_2_L_0x95: break;
		case ci::RES_2_HL_0x96: break;
		case ci::RES_2_A_0x97: break;
		case ci::RES_3_B_0x98: break;
		case ci::RES_3_C_0x99: break;
		case ci::RES_3_D_0x9A: break;
		case ci::RES_3_E_0x9B: break;
		case ci::RES_3_H_0x9C: break;
		case ci::RES_3_L_0x9D: break;
		case ci::RES_3_HL_0x9E: break;
		case ci::RES_3_A_0x9F: break;
		case ci::RES_4_B_0xA0: break;
		case ci::RES_4_C_0xA1: break;
		case ci::RES_4_D_0xA2: break;
		case ci::RES_4_E_0xA3: break;
		case ci::RES_4_H_0xA4: break;
		case ci::RES_4_L_0xA5: break;
		case ci::RES_4_HL_0xA6: break;
		case ci::RES_4_A_0xA7: break;
		case ci::RES_5_B_0xA8: break;
		case ci::RES_5_C_0xA9: break;
		case ci::RES_5_D_0xAA: break;
		case ci::RES_5_E_0xAB: break;
		case ci::RES_5_H_0xAC: break;
		case ci::RES_5_L_0xAD: break;
		case ci::RES_5_HL_0xAE: break;
		case ci::RES_5_A_0xAF: break;
		case ci::RES_6_B_0xB0: break;
		case ci::RES_6_C_0xB1: break;
		case ci::RES_6_D_0xB2: break;
		case ci::RES_6_E_0xB3: break;
		case ci::RES_6_H_0xB4: break;
		case ci::RES_6_L_0xB5: break;
		case ci::RES_6_HL_0xB6: break;
		case ci::RES_6_A_0xB7: break;
		case ci::RES_7_B_0xB8: break;
		case ci::RES_7_C_0xB9: break;
		case ci::RES_7_D_0xBA: break;
		case ci::RES_7_E_0xBB: break;
		case ci::RES_7_H_0xBC: break;
		case ci::RES_7_L_0xBD: break;
		case ci::RES_7_HL_0xBE: break;
		case ci::RES_7_A_0xBF: break;
		case ci::SET_0_B_0xC0: break;
		case ci::SET_0_C_0xC1: break;
		case ci::SET_0_D_0xC2: break;
		case ci::SET_0_E_0xC3: break;
		case ci::SET_0_H_0xC4: break;
		case ci::SET_0_L_0xC5: break;
		case ci::SET_0_HL_0xC6: break;
		case ci::SET_0_A_0xC7: break;
		case ci::SET_1_B_0xC8: break;
		case ci::SET_1_C_0xC9: break;
		case ci::SET_1_D_0xCA: break;
		case ci::SET_1_E_0xCB: break;
		case ci::SET_1_H_0xCC: break;
		case ci::SET_1_L_0xCD: break;
		case ci::SET_1_HL_0xCE: break;
		case ci::SET_1_A_0xCF: break;
		case ci::SET_2_B_0xD0: break;
		case ci::SET_2_C_0xD1: break;
		case ci::SET_2_D_0xD2: break;
		case ci::SET_2_E_0xD3: break;
		case ci::SET_2_H_0xD4: break;
		case ci::SET_2_L_0xD5: break;
		case ci::SET_2_HL_0xD6: break;
		case ci::SET_2_A_0xD7: break;
		case ci::SET_3_B_0xD8: break;
		case ci::SET_3_C_0xD9: break;
		case ci::SET_3_D_0xDA: break;
		case ci::SET_3_E_0xDB: break;
		case ci::SET_3_H_0xDC: break;
		case ci::SET_3_L_0xDD: break;
		case ci::SET_3_HL_0xDE: break;
		case ci::SET_3_A_0xDF: break;
		case ci::SET_4_B_0xE0: break;
		case ci::SET_4_C_0xE1: break;
		case ci::SET_4_D_0xE2: break;
		case ci::SET_4_E_0xE3: break;
		case ci::SET_4_H_0xE4: break;
		case ci::SET_4_L_0xE5: break;
		case ci::SET_4_HL_0xE6: break;
		case ci::SET_4_A_0xE7: break;
		case ci::SET_5_B_0xE8: break;
		case ci::SET_5_C_0xE9: break;
		case ci::SET_5_D_0xEA: break;
		case ci::SET_5_E_0xEB: break;
		case ci::SET_5_H_0xEC: break;
		case ci::SET_5_L_0xED: break;
		case ci::SET_5_HL_0xEE: break;
		case ci::SET_5_A_0xEF: break;
		case ci::SET_6_B_0xF0: break;
		case ci::SET_6_C_0xF1: break;
		case ci::SET_6_D_0xF2: break;
		case ci::SET_6_E_0xF3: break;
		case ci::SET_6_H_0xF4: break;
		case ci::SET_6_L_0xF5: break;
		case ci::SET_6_HL_0xF6: break;
		case ci::SET_6_A_0xF7: break;
		case ci::SET_7_B_0xF8: break;
		case ci::SET_7_C_0xF9: break;
		case ci::SET_7_D_0xFA: break;
		case ci::SET_7_E_0xFB: break;
		case ci::SET_7_H_0xFC: break;
		case ci::SET_7_L_0xFD: break;
		case ci::SET_7_HL_0xFE: break;
		case ci::SET_7_A_0xFF: break;
		default: ;
		}

		HWLogger::Error(U"missed instruction prefixed CB: {:X}"_fmt(static_cast<uint8>(instr)));
		return CPUOperationResult(0, 0);
	}
}
