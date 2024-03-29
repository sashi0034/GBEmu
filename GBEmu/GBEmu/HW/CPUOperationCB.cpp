﻿#include "stdafx.h"
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

	inline CPUReg8 undefinedReg8()
	{
		assert(false);
		return CPUReg8::Invalid;
	}

	[[nodiscard]]
	inline CPUOperationResult operateRLC_X(HWEnv& env, CPUInstructionCB instr)
	{
		auto&& cpu = env.GetCPU();

		// @formatter:off
		const CPUReg8 reg =
			instr == ci::RLC_A_0x07 ? CPUReg8::A :
			instr == ci::RLC_B_0x00 ? CPUReg8::B :
			instr == ci::RLC_C_0x01 ? CPUReg8::C :
			instr == ci::RLC_D_0x02 ? CPUReg8::D :
			instr == ci::RLC_E_0x03 ? CPUReg8::E :
			instr == ci::RLC_H_0x04 ? CPUReg8::H :
			instr == ci::RLC_L_0x05 ? CPUReg8::L :
			undefinedReg8();
		// @formatter:on

		const uint8 before = cpu.GetReg8(reg);
		const uint8 bit7 = before >> 7;

		const bool z = before == 0;
		const bool c = bit7 == 1;

		cpu.SetReg8(reg, (before << 1) | bit7);

		return CPUOperationResult::ByCalc(8, CPUOperationZNHC{z, false, false, c});
	}

	[[nodiscard]]
	inline CPUOperationResult operateRLC_mHL(HWEnv& env)
	{
		// 0x06
		auto&& cpu = env.GetCPU();
		auto&& memory = env.GetMemory();

		const uint8 data = memory.Read(env, cpu.RegHL());
		const uint8 bit7 = data >> 7;

		const bool z = data == 0;
		const bool c = bit7 == 1;

		memory.Write(env, cpu.RegHL(), (data << 1) | bit7);

		return CPUOperationResult::ByCalc(16, CPUOperationZNHC{z, false, false, c});
	}

	[[nodiscard]]
	inline CPUOperationResult operateRRC_X(HWEnv& env, CPUInstructionCB instr)
	{
		auto&& cpu = env.GetCPU();

		// @formatter:off
		const CPUReg8 reg =
			instr == ci::RRC_A_0x0F ? CPUReg8::A :
			instr == ci::RRC_B_0x08 ? CPUReg8::B :
			instr == ci::RRC_C_0x09 ? CPUReg8::C :
			instr == ci::RRC_D_0x0A ? CPUReg8::D :
			instr == ci::RRC_E_0x0B ? CPUReg8::E :
			instr == ci::RRC_H_0x0C ? CPUReg8::H :
			instr == ci::RRC_L_0x0D ? CPUReg8::L :
			undefinedReg8();
		// @formatter:on

		const uint8 before = cpu.GetReg8(reg);
		const uint8 bit0 = before & 0b1;

		const bool z = before == 0;
		const bool c = bit0 == 1;

		cpu.SetReg8(reg, (before >> 1) | (bit0 << 7));

		return CPUOperationResult::ByCalc(8, CPUOperationZNHC{z, false, false, c});
	}

	[[nodiscard]]
	inline CPUOperationResult operateRRC_mHL(HWEnv& env)
	{
		// 0x0E
		auto&& cpu = env.GetCPU();
		auto&& memory = env.GetMemory();

		const uint8 data = memory.Read(env, cpu.RegHL());
		const uint8 bit0 = data & 0b1;

		const bool z = data == 0;
		const bool c = bit0 == 1;

		memory.Write(env, cpu.RegHL(), (data >> 1) | (bit0 << 7));

		return CPUOperationResult::ByCalc(16, CPUOperationZNHC{z, false, false, c});
	}

	[[nodiscard]]
	inline CPUOperationResult operateRL_X(HWEnv& env, CPUInstructionCB instr)
	{
		auto&& cpu = env.GetCPU();

		// @formatter:off
		const CPUReg8 reg =
			instr == ci::RL_A_0x17 ? CPUReg8::A :
			instr == ci::RL_B_0x10 ? CPUReg8::B :
			instr == ci::RL_C_0x11 ? CPUReg8::C :
			instr == ci::RL_D_0x12 ? CPUReg8::D :
			instr == ci::RL_E_0x13 ? CPUReg8::E :
			instr == ci::RL_H_0x14 ? CPUReg8::H :
			instr == ci::RL_L_0x15 ? CPUReg8::L :
			undefinedReg8();
		// @formatter:on

		const uint8 before = cpu.GetReg8(reg);
		const uint8 bit7 = before >> 7;
		const uint8 carry = cpu.FlagC();

		const uint8 after = (before << 1) | carry;

		const bool z = after == 0;
		const bool c = bit7 == 1;

		cpu.SetReg8(reg, after);

		return CPUOperationResult::ByCalc(8, CPUOperationZNHC{z, false, false, c});
	}

	[[nodiscard]]
	inline CPUOperationResult operateRL_mHL(HWEnv& env)
	{
		// 0x0E
		auto&& cpu = env.GetCPU();
		auto&& memory = env.GetMemory();

		const uint8 before = memory.Read(env, cpu.RegHL());
		const uint8 bit7 = before >> 7;
		const uint8 carry = cpu.FlagC();

		const uint8 after = (before << 1) | carry;

		const bool z = after == 0;
		const bool c = bit7 == 1;

		memory.Write(env, cpu.RegHL(), after);

		return CPUOperationResult::ByCalc(16, CPUOperationZNHC{z, false, false, c});
	}

	[[nodiscard]]
	inline CPUOperationResult operateRR_X(HWEnv& env, CPUInstructionCB instr)
	{
		auto&& cpu = env.GetCPU();

		// @formatter:off
		const CPUReg8 reg =
			instr == ci::RR_A_0x1F ? CPUReg8::A :
			instr == ci::RR_B_0x18 ? CPUReg8::B :
			instr == ci::RR_C_0x19 ? CPUReg8::C :
			instr == ci::RR_D_0x1A ? CPUReg8::D :
			instr == ci::RR_E_0x1B ? CPUReg8::E :
			instr == ci::RR_H_0x1C ? CPUReg8::H :
			instr == ci::RR_L_0x1D ? CPUReg8::L :
			undefinedReg8();
		// @formatter:on

		const uint8 before = cpu.GetReg8(reg);
		const uint8 bit0 = before & 0b1;
		const uint8 carry = cpu.FlagC();

		const uint8 after = (before >> 1) | (carry << 7);

		const bool z = after == 0;
		const bool c = bit0 == 1;

		cpu.SetReg8(reg, after);

		return CPUOperationResult::ByCalc(8, CPUOperationZNHC{z, false, false, c});
	}

	[[nodiscard]]
	inline CPUOperationResult operateRR_mHL(HWEnv& env)
	{
		// 0x1E
		auto&& cpu = env.GetCPU();
		auto&& memory = env.GetMemory();

		const uint8 before = memory.Read(env, cpu.RegHL());
		const uint8 bit0 = before & 0b1;
		const uint8 carry = cpu.FlagC();

		const uint8 after = (before >> 1) | (carry << 7);

		const bool z = after == 0;
		const bool c = bit0 == 1;

		memory.Write(env, cpu.RegHL(), after);

		return CPUOperationResult::ByCalc(16, CPUOperationZNHC{z, false, false, c});
	}

	[[nodiscard]]
	inline CPUOperationResult operateSLA_X(HWEnv& env, CPUInstructionCB instr)
	{
		auto&& cpu = env.GetCPU();

		// @formatter:off
		const CPUReg8 reg =
			instr == ci::SLA_A_0x27 ? CPUReg8::A :
			instr == ci::SLA_B_0x20 ? CPUReg8::B :
			instr == ci::SLA_C_0x21 ? CPUReg8::C :
			instr == ci::SLA_D_0x22 ? CPUReg8::D :
			instr == ci::SLA_E_0x23 ? CPUReg8::E :
			instr == ci::SLA_H_0x24 ? CPUReg8::H :
			instr == ci::SLA_L_0x25 ? CPUReg8::L :
			undefinedReg8();
		// @formatter:on

		const uint8 before = cpu.GetReg8(reg);
		const uint8 after = before << 1;

		const bool z = after == 0;
		const bool c = before & (1 << 7);

		cpu.SetReg8(reg, after);

		return CPUOperationResult::ByCalc(8, CPUOperationZNHC{z, false, false, c});
	}

	[[nodiscard]]
	inline CPUOperationResult operateSLA_mHL(HWEnv& env)
	{
		// 0x26
		auto&& cpu = env.GetCPU();
		auto&& memory = env.GetMemory();

		const uint8 before = memory.Read(env, cpu.RegHL());
		const uint8 after = before << 1;

		const bool z = after == 0;
		const bool c = before & (1 << 7);

		memory.Write(env, cpu.RegHL(), after);

		return CPUOperationResult::ByCalc(16, CPUOperationZNHC{z, false, false, c});
	}

	[[nodiscard]]
	inline CPUOperationResult operateSRA_X(HWEnv& env, CPUInstructionCB instr)
	{
		auto&& cpu = env.GetCPU();

		// @formatter:off
		const CPUReg8 reg =
			instr == ci::SRA_A_0x2F ? CPUReg8::A :
			instr == ci::SRA_B_0x28 ? CPUReg8::B :
			instr == ci::SRA_C_0x29 ? CPUReg8::C :
			instr == ci::SRA_D_0x2A ? CPUReg8::D :
			instr == ci::SRA_E_0x2B ? CPUReg8::E :
			instr == ci::SRA_H_0x2C ? CPUReg8::H :
			instr == ci::SRA_L_0x2D ? CPUReg8::L :
			undefinedReg8();
		// @formatter:on

		const uint8 before = cpu.GetReg8(reg);
		const uint8 after = (before >> 1) | (before & (1 << 7));

		const bool z = after == 0;
		const bool c = before & 0b1;

		cpu.SetReg8(reg, after);

		return CPUOperationResult::ByCalc(8, CPUOperationZNHC{z, false, false, c});
	}

	[[nodiscard]]
	inline CPUOperationResult operateSRA_mHL(HWEnv& env)
	{
		// 0x2E
		auto&& cpu = env.GetCPU();
		auto&& memory = env.GetMemory();

		const uint8 before = memory.Read(env, cpu.RegHL());
		const uint8 after = (before >> 1) | (before & (1 << 7));

		const bool z = after == 0;
		const bool c = before & 0b1;

		memory.Write(env, cpu.RegHL(), after);

		return CPUOperationResult::ByCalc(16, CPUOperationZNHC{z, false, false, c});
	}

	[[nodiscard]]
	inline CPUOperationResult operateSRL_X(HWEnv& env, CPUInstructionCB instr)
	{
		auto&& cpu = env.GetCPU();

		// @formatter:off
		const CPUReg8 reg =
			instr == ci::SRL_A_0x3F ? CPUReg8::A :
			instr == ci::SRL_B_0x38 ? CPUReg8::B :
			instr == ci::SRL_C_0x39 ? CPUReg8::C :
			instr == ci::SRL_D_0x3A ? CPUReg8::D :
			instr == ci::SRL_E_0x3B ? CPUReg8::E :
			instr == ci::SRL_H_0x3C ? CPUReg8::H :
			instr == ci::SRL_L_0x3D ? CPUReg8::L :
			undefinedReg8();
		// @formatter:on

		const uint8 before = cpu.GetReg8(reg);
		const uint8 after = before >> 1;

		const bool z = after == 0;
		const bool c = before & 0b1;

		cpu.SetReg8(reg, after);

		return CPUOperationResult::ByCalc(8, CPUOperationZNHC{z, false, false, c});
	}

	[[nodiscard]]
	inline CPUOperationResult operateSRL_mHL(HWEnv& env)
	{
		// 0x3E
		auto&& cpu = env.GetCPU();
		auto&& memory = env.GetMemory();

		const uint8 before = memory.Read(env, cpu.RegHL());
		const uint8 after = before >> 1;

		const bool z = after == 0;
		const bool c = before & 0b1;

		memory.Write(env, cpu.RegHL(), after);

		return CPUOperationResult::ByCalc(16, CPUOperationZNHC{z, false, false, c});
	}

	[[nodiscard]]
	inline CPUOperationResult operateSWAP_X(HWEnv& env, CPUInstructionCB instr)
	{
		auto&& cpu = env.GetCPU();

		// @formatter:off
		const CPUReg8 reg =
			instr == ci::SWAP_A_0x37 ? CPUReg8::A :
			instr == ci::SWAP_B_0x30 ? CPUReg8::B :
			instr == ci::SWAP_C_0x31 ? CPUReg8::C :
			instr == ci::SWAP_D_0x32 ? CPUReg8::D :
			instr == ci::SWAP_E_0x33 ? CPUReg8::E :
			instr == ci::SWAP_H_0x34 ? CPUReg8::H :
			instr == ci::SWAP_L_0x35 ? CPUReg8::L :
			undefinedReg8();
		// @formatter:on

		const uint8 before = cpu.GetReg8(reg);
		const uint8 after = (before >> 4) | (before << 4);

		const bool z = after == 0;

		cpu.SetReg8(reg, after);

		// ドキュメントによっては、N, H, Cが不変となっているので確認したい
		return CPUOperationResult::ByCalc(8, CPUOperationZNHC{z, false, false, false});
	}

	[[nodiscard]]
	inline CPUOperationResult operateSWAP_mHL(HWEnv& env)
	{
		// 0x36
		auto&& cpu = env.GetCPU();
		auto&& memory = env.GetMemory();

		const uint8 before = memory.Read(env, cpu.RegHL());
		const uint8 after = (before >> 4) | (before << 4);

		const bool z = after == 0;

		memory.Write(env, cpu.RegHL(), after);

		// ドキュメントによっては、N, H, Cが不変となっているので確認したい
		return CPUOperationResult::ByCalc(16, CPUOperationZNHC{z, false, false, false});
	}

	[[nodiscard]]
	inline CPUOperationResult operateBIT_N_X(HWEnv& env, CPUInstructionCB instr)
	{
		auto&& cpu = env.GetCPU();

		const uint8 u3 = (static_cast<uint8>(instr) - static_cast<uint8>(ci::BIT_0_B_0x40)) / 8;
		const uint8 r8 = (static_cast<uint8>(instr) - static_cast<uint8>(ci::BIT_0_B_0x40)) % 8;

		// @formatter:off
		const CPUReg8 reg =
			r8 == 7 ? CPUReg8::A :
			r8 == 0 ? CPUReg8::B :
			r8 == 1 ? CPUReg8::C :
			r8 == 2 ? CPUReg8::D :
			r8 == 3 ? CPUReg8::E :
			r8 == 4 ? CPUReg8::H :
			r8 == 5 ? CPUReg8::L :
			undefinedReg8();
		// @formatter:on

		const uint8 value = cpu.GetReg8(reg);
		const bool z = (value & (1 << u3)) == 0;

		return CPUOperationResult::ByCalc(8, CPUOperationZNHC{z, false, true, cpu.FlagC()});
	}

	[[nodiscard]]
	inline CPUOperationResult operateBIT_N_mHL(HWEnv& env, CPUInstructionCB instr)
	{
		auto&& cpu = env.GetCPU();

		const uint8 u3 = (static_cast<uint8>(instr) - static_cast<uint8>(ci::BIT_0_B_0x40)) / 8;

		const uint8 value = env.GetMemory().Read(env, cpu.RegHL());
		const bool z = (value & (1 << u3)) == 0;

		return CPUOperationResult::ByCalc(12, CPUOperationZNHC{z, false, true, cpu.FlagC()});
	}

	[[nodiscard]]
	inline CPUOperationResult operateRES_N_X(HWEnv& env, CPUInstructionCB instr)
	{
		auto&& cpu = env.GetCPU();

		const uint8 u3 = (static_cast<uint8>(instr) - static_cast<uint8>(ci::RES_0_B_0x80)) / 8;
		const uint8 r8 = (static_cast<uint8>(instr) - static_cast<uint8>(ci::RES_0_B_0x80)) % 8;

		// @formatter:off
		const CPUReg8 reg =
			r8 == 7 ? CPUReg8::A :
			r8 == 0 ? CPUReg8::B :
			r8 == 1 ? CPUReg8::C :
			r8 == 2 ? CPUReg8::D :
			r8 == 3 ? CPUReg8::E :
			r8 == 4 ? CPUReg8::H :
			r8 == 5 ? CPUReg8::L :
			undefinedReg8();
		// @formatter:on

		const uint8 value = cpu.GetReg8(reg);
		cpu.SetReg8(reg, value & (~(1 << u3)));

		return CPUOperationResult(8);
	}

	[[nodiscard]]
	inline CPUOperationResult operateRES_N_mHL(HWEnv& env, CPUInstructionCB instr)
	{
		auto&& cpu = env.GetCPU();
		auto&& memory = env.GetMemory();

		const uint8 u3 = (static_cast<uint8>(instr) - static_cast<uint8>(ci::RES_0_B_0x80)) / 8;

		const uint8 value = memory.Read(env, cpu.RegHL());
		memory.Write(env, cpu.RegHL(), value & (~(1 << u3)));

		return CPUOperationResult(16);
	}

	[[nodiscard]]
	inline CPUOperationResult operateSET_N_X(HWEnv& env, CPUInstructionCB instr)
	{
		auto&& cpu = env.GetCPU();

		const uint8 u3 = (static_cast<uint8>(instr) - static_cast<uint8>(ci::SET_0_B_0xC0)) / 8;
		const uint8 r8 = (static_cast<uint8>(instr) - static_cast<uint8>(ci::SET_0_B_0xC0)) % 8;

		// @formatter:off
		const CPUReg8 reg =
			r8 == 7 ? CPUReg8::A :
			r8 == 0 ? CPUReg8::B :
			r8 == 1 ? CPUReg8::C :
			r8 == 2 ? CPUReg8::D :
			r8 == 3 ? CPUReg8::E :
			r8 == 4 ? CPUReg8::H :
			r8 == 5 ? CPUReg8::L :
			undefinedReg8();
		// @formatter:on

		const uint8 value = cpu.GetReg8(reg);
		cpu.SetReg8(reg, value | (1 << u3));

		return CPUOperationResult(8);
	}

	[[nodiscard]]
	inline CPUOperationResult operateSET_N_mHL(HWEnv& env, CPUInstructionCB instr)
	{
		auto&& cpu = env.GetCPU();
		auto&& memory = env.GetMemory();

		const uint8 u3 = (static_cast<uint8>(instr) - static_cast<uint8>(ci::SET_0_B_0xC0)) / 8;

		const uint8 value = memory.Read(env, cpu.RegHL());
		memory.Write(env, cpu.RegHL(), value | (1 << u3));

		return CPUOperationResult(16);
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
		case ci::RR_mHL_0x1E: return operateRR_mHL(env);
		case ci::RR_A_0x1F: return operateRR_X(env, instr);
		case ci::SLA_B_0x20: return operateSLA_X(env, instr);
		case ci::SLA_C_0x21: return operateSLA_X(env, instr);
		case ci::SLA_D_0x22: return operateSLA_X(env, instr);
		case ci::SLA_E_0x23: return operateSLA_X(env, instr);
		case ci::SLA_H_0x24: return operateSLA_X(env, instr);
		case ci::SLA_L_0x25: return operateSLA_X(env, instr);
		case ci::SLA_mHL_0x26: return operateSLA_mHL(env);
		case ci::SLA_A_0x27: return operateSLA_X(env, instr);
		case ci::SRA_B_0x28: return operateSRA_X(env, instr);
		case ci::SRA_C_0x29: return operateSRA_X(env, instr);
		case ci::SRA_D_0x2A: return operateSRA_X(env, instr);
		case ci::SRA_E_0x2B: return operateSRA_X(env, instr);
		case ci::SRA_H_0x2C: return operateSRA_X(env, instr);
		case ci::SRA_L_0x2D: return operateSRA_X(env, instr);
		case ci::SRA_mHL_0x2E: return operateSRA_mHL(env);
		case ci::SRA_A_0x2F: return operateSRA_X(env, instr);
		case ci::SWAP_B_0x30: return operateSWAP_X(env, instr);
		case ci::SWAP_C_0x31: return operateSWAP_X(env, instr);
		case ci::SWAP_D_0x32: return operateSWAP_X(env, instr);
		case ci::SWAP_E_0x33: return operateSWAP_X(env, instr);
		case ci::SWAP_H_0x34: return operateSWAP_X(env, instr);
		case ci::SWAP_L_0x35: return operateSWAP_X(env, instr);
		case ci::SWAP_HL_0x36: return operateSWAP_mHL(env);
		case ci::SWAP_A_0x37: return operateSWAP_X(env, instr);
		case ci::SRL_B_0x38: return operateSRL_X(env, instr);
		case ci::SRL_C_0x39: return operateSRL_X(env, instr);
		case ci::SRL_D_0x3A: return operateSRL_X(env, instr);
		case ci::SRL_E_0x3B: return operateSRL_X(env, instr);
		case ci::SRL_H_0x3C: return operateSRL_X(env, instr);
		case ci::SRL_L_0x3D: return operateSRL_X(env, instr);
		case ci::SRL_mHL_0x3E: return operateSRL_mHL(env);
		case ci::SRL_A_0x3F: return operateSRL_X(env, instr);
		case ci::BIT_0_B_0x40: return operateBIT_N_X(env, instr);
		case ci::BIT_0_C_0x41: return operateBIT_N_X(env, instr);
		case ci::BIT_0_D_0x42: return operateBIT_N_X(env, instr);
		case ci::BIT_0_E_0x43: return operateBIT_N_X(env, instr);
		case ci::BIT_0_H_0x44: return operateBIT_N_X(env, instr);
		case ci::BIT_0_L_0x45: return operateBIT_N_X(env, instr);
		case ci::BIT_0_mHL_0x46: return operateBIT_N_mHL(env, instr);
		case ci::BIT_0_A_0x47: return operateBIT_N_X(env, instr);
		case ci::BIT_1_B_0x48: return operateBIT_N_X(env, instr);
		case ci::BIT_1_C_0x49: return operateBIT_N_X(env, instr);
		case ci::BIT_1_D_0x4A: return operateBIT_N_X(env, instr);
		case ci::BIT_1_E_0x4B: return operateBIT_N_X(env, instr);
		case ci::BIT_1_H_0x4C: return operateBIT_N_X(env, instr);
		case ci::BIT_1_L_0x4D: return operateBIT_N_X(env, instr);
		case ci::BIT_1_mHL_0x4E: return operateBIT_N_mHL(env, instr);
		case ci::BIT_1_A_0x4F: return operateBIT_N_X(env, instr);
		case ci::BIT_2_B_0x50: return operateBIT_N_X(env, instr);
		case ci::BIT_2_C_0x51: return operateBIT_N_X(env, instr);
		case ci::BIT_2_D_0x52: return operateBIT_N_X(env, instr);
		case ci::BIT_2_E_0x53: return operateBIT_N_X(env, instr);
		case ci::BIT_2_H_0x54: return operateBIT_N_X(env, instr);
		case ci::BIT_2_L_0x55: return operateBIT_N_X(env, instr);
		case ci::BIT_2_mHL_0x56: return operateBIT_N_mHL(env, instr);
		case ci::BIT_2_A_0x57: return operateBIT_N_X(env, instr);
		case ci::BIT_3_B_0x58: return operateBIT_N_X(env, instr);
		case ci::BIT_3_C_0x59: return operateBIT_N_X(env, instr);
		case ci::BIT_3_D_0x5A: return operateBIT_N_X(env, instr);
		case ci::BIT_3_E_0x5B: return operateBIT_N_X(env, instr);
		case ci::BIT_3_H_0x5C: return operateBIT_N_X(env, instr);
		case ci::BIT_3_L_0x5D: return operateBIT_N_X(env, instr);
		case ci::BIT_3_mHL_0x5E: return operateBIT_N_mHL(env, instr);
		case ci::BIT_3_A_0x5F: return operateBIT_N_X(env, instr);
		case ci::BIT_4_B_0x60: return operateBIT_N_X(env, instr);
		case ci::BIT_4_C_0x61: return operateBIT_N_X(env, instr);
		case ci::BIT_4_D_0x62: return operateBIT_N_X(env, instr);
		case ci::BIT_4_E_0x63: return operateBIT_N_X(env, instr);
		case ci::BIT_4_H_0x64: return operateBIT_N_X(env, instr);
		case ci::BIT_4_L_0x65: return operateBIT_N_X(env, instr);
		case ci::BIT_4_mHL_0x66: return operateBIT_N_mHL(env, instr);
		case ci::BIT_4_A_0x67: return operateBIT_N_X(env, instr);
		case ci::BIT_5_B_0x68: return operateBIT_N_X(env, instr);
		case ci::BIT_5_C_0x69: return operateBIT_N_X(env, instr);
		case ci::BIT_5_D_0x6A: return operateBIT_N_X(env, instr);
		case ci::BIT_5_E_0x6B: return operateBIT_N_X(env, instr);
		case ci::BIT_5_H_0x6C: return operateBIT_N_X(env, instr);
		case ci::BIT_5_L_0x6D: return operateBIT_N_X(env, instr);
		case ci::BIT_5_mHL_0x6E: return operateBIT_N_mHL(env, instr);
		case ci::BIT_5_A_0x6F: return operateBIT_N_X(env, instr);
		case ci::BIT_6_B_0x70: return operateBIT_N_X(env, instr);
		case ci::BIT_6_C_0x71: return operateBIT_N_X(env, instr);
		case ci::BIT_6_D_0x72: return operateBIT_N_X(env, instr);
		case ci::BIT_6_E_0x73: return operateBIT_N_X(env, instr);
		case ci::BIT_6_H_0x74: return operateBIT_N_X(env, instr);
		case ci::BIT_6_L_0x75: return operateBIT_N_X(env, instr);
		case ci::BIT_6_mHL_0x76: return operateBIT_N_mHL(env, instr);
		case ci::BIT_6_A_0x77: return operateBIT_N_X(env, instr);
		case ci::BIT_7_B_0x78: return operateBIT_N_X(env, instr);
		case ci::BIT_7_C_0x79: return operateBIT_N_X(env, instr);
		case ci::BIT_7_D_0x7A: return operateBIT_N_X(env, instr);
		case ci::BIT_7_E_0x7B: return operateBIT_N_X(env, instr);
		case ci::BIT_7_H_0x7C: return operateBIT_N_X(env, instr);
		case ci::BIT_7_L_0x7D: return operateBIT_N_X(env, instr);
		case ci::BIT_7_mHL_0x7E: return operateBIT_N_mHL(env, instr);
		case ci::BIT_7_A_0x7F: return operateBIT_N_X(env, instr);
		case ci::RES_0_B_0x80: return operateRES_N_X(env, instr);
		case ci::RES_0_C_0x81: return operateRES_N_X(env, instr);
		case ci::RES_0_D_0x82: return operateRES_N_X(env, instr);
		case ci::RES_0_E_0x83: return operateRES_N_X(env, instr);
		case ci::RES_0_H_0x84: return operateRES_N_X(env, instr);
		case ci::RES_0_L_0x85: return operateRES_N_X(env, instr);
		case ci::RES_0_mHL_0x86: return operateRES_N_mHL(env, instr);
		case ci::RES_0_A_0x87: return operateRES_N_X(env, instr);
		case ci::RES_1_B_0x88: return operateRES_N_X(env, instr);
		case ci::RES_1_C_0x89: return operateRES_N_X(env, instr);
		case ci::RES_1_D_0x8A: return operateRES_N_X(env, instr);
		case ci::RES_1_E_0x8B: return operateRES_N_X(env, instr);
		case ci::RES_1_H_0x8C: return operateRES_N_X(env, instr);
		case ci::RES_1_L_0x8D: return operateRES_N_X(env, instr);
		case ci::RES_1_mHL_0x8E: return operateRES_N_mHL(env, instr);
		case ci::RES_1_A_0x8F: return operateRES_N_X(env, instr);
		case ci::RES_2_B_0x90: return operateRES_N_X(env, instr);
		case ci::RES_2_C_0x91: return operateRES_N_X(env, instr);
		case ci::RES_2_D_0x92: return operateRES_N_X(env, instr);
		case ci::RES_2_E_0x93: return operateRES_N_X(env, instr);
		case ci::RES_2_H_0x94: return operateRES_N_X(env, instr);
		case ci::RES_2_L_0x95: return operateRES_N_X(env, instr);
		case ci::RES_2_mHL_0x96: return operateRES_N_mHL(env, instr);
		case ci::RES_2_A_0x97: return operateRES_N_X(env, instr);
		case ci::RES_3_B_0x98: return operateRES_N_X(env, instr);
		case ci::RES_3_C_0x99: return operateRES_N_X(env, instr);
		case ci::RES_3_D_0x9A: return operateRES_N_X(env, instr);
		case ci::RES_3_E_0x9B: return operateRES_N_X(env, instr);
		case ci::RES_3_H_0x9C: return operateRES_N_X(env, instr);
		case ci::RES_3_L_0x9D: return operateRES_N_X(env, instr);
		case ci::RES_3_mHL_0x9E: return operateRES_N_mHL(env, instr);
		case ci::RES_3_A_0x9F: return operateRES_N_X(env, instr);
		case ci::RES_4_B_0xA0: return operateRES_N_X(env, instr);
		case ci::RES_4_C_0xA1: return operateRES_N_X(env, instr);
		case ci::RES_4_D_0xA2: return operateRES_N_X(env, instr);
		case ci::RES_4_E_0xA3: return operateRES_N_X(env, instr);
		case ci::RES_4_H_0xA4: return operateRES_N_X(env, instr);
		case ci::RES_4_L_0xA5: return operateRES_N_X(env, instr);
		case ci::RES_4_mHL_0xA6: return operateRES_N_mHL(env, instr);
		case ci::RES_4_A_0xA7: return operateRES_N_X(env, instr);
		case ci::RES_5_B_0xA8: return operateRES_N_X(env, instr);
		case ci::RES_5_C_0xA9: return operateRES_N_X(env, instr);
		case ci::RES_5_D_0xAA: return operateRES_N_X(env, instr);
		case ci::RES_5_E_0xAB: return operateRES_N_X(env, instr);
		case ci::RES_5_H_0xAC: return operateRES_N_X(env, instr);
		case ci::RES_5_L_0xAD: return operateRES_N_X(env, instr);
		case ci::RES_5_mHL_0xAE: return operateRES_N_mHL(env, instr);
		case ci::RES_5_A_0xAF: return operateRES_N_X(env, instr);
		case ci::RES_6_B_0xB0: return operateRES_N_X(env, instr);
		case ci::RES_6_C_0xB1: return operateRES_N_X(env, instr);
		case ci::RES_6_D_0xB2: return operateRES_N_X(env, instr);
		case ci::RES_6_E_0xB3: return operateRES_N_X(env, instr);
		case ci::RES_6_H_0xB4: return operateRES_N_X(env, instr);
		case ci::RES_6_L_0xB5: return operateRES_N_X(env, instr);
		case ci::RES_6_mHL_0xB6: return operateRES_N_mHL(env, instr);
		case ci::RES_6_A_0xB7: return operateRES_N_X(env, instr);
		case ci::RES_7_B_0xB8: return operateRES_N_X(env, instr);
		case ci::RES_7_C_0xB9: return operateRES_N_X(env, instr);
		case ci::RES_7_D_0xBA: return operateRES_N_X(env, instr);
		case ci::RES_7_E_0xBB: return operateRES_N_X(env, instr);
		case ci::RES_7_H_0xBC: return operateRES_N_X(env, instr);
		case ci::RES_7_L_0xBD: return operateRES_N_X(env, instr);
		case ci::RES_7_mHL_0xBE: return operateRES_N_mHL(env, instr);
		case ci::RES_7_A_0xBF: return operateRES_N_X(env, instr);
		case ci::SET_0_B_0xC0: return operateSET_N_X(env, instr);
		case ci::SET_0_C_0xC1: return operateSET_N_X(env, instr);
		case ci::SET_0_D_0xC2: return operateSET_N_X(env, instr);
		case ci::SET_0_E_0xC3: return operateSET_N_X(env, instr);
		case ci::SET_0_H_0xC4: return operateSET_N_X(env, instr);
		case ci::SET_0_L_0xC5: return operateSET_N_X(env, instr);
		case ci::SET_0_mHL_0xC6: return operateSET_N_mHL(env, instr);
		case ci::SET_0_A_0xC7: return operateSET_N_X(env, instr);
		case ci::SET_1_B_0xC8: return operateSET_N_X(env, instr);
		case ci::SET_1_C_0xC9: return operateSET_N_X(env, instr);
		case ci::SET_1_D_0xCA: return operateSET_N_X(env, instr);
		case ci::SET_1_E_0xCB: return operateSET_N_X(env, instr);
		case ci::SET_1_H_0xCC: return operateSET_N_X(env, instr);
		case ci::SET_1_L_0xCD: return operateSET_N_X(env, instr);
		case ci::SET_1_mHL_0xCE: return operateSET_N_mHL(env, instr);
		case ci::SET_1_A_0xCF: return operateSET_N_X(env, instr);
		case ci::SET_2_B_0xD0: return operateSET_N_X(env, instr);
		case ci::SET_2_C_0xD1: return operateSET_N_X(env, instr);
		case ci::SET_2_D_0xD2: return operateSET_N_X(env, instr);
		case ci::SET_2_E_0xD3: return operateSET_N_X(env, instr);
		case ci::SET_2_H_0xD4: return operateSET_N_X(env, instr);
		case ci::SET_2_L_0xD5: return operateSET_N_X(env, instr);
		case ci::SET_2_mHL_0xD6: return operateSET_N_mHL(env, instr);
		case ci::SET_2_A_0xD7: return operateSET_N_X(env, instr);
		case ci::SET_3_B_0xD8: return operateSET_N_X(env, instr);
		case ci::SET_3_C_0xD9: return operateSET_N_X(env, instr);
		case ci::SET_3_D_0xDA: return operateSET_N_X(env, instr);
		case ci::SET_3_E_0xDB: return operateSET_N_X(env, instr);
		case ci::SET_3_H_0xDC: return operateSET_N_X(env, instr);
		case ci::SET_3_L_0xDD: return operateSET_N_X(env, instr);
		case ci::SET_3_mHL_0xDE: return operateSET_N_mHL(env, instr);
		case ci::SET_3_A_0xDF: return operateSET_N_X(env, instr);
		case ci::SET_4_B_0xE0: return operateSET_N_X(env, instr);
		case ci::SET_4_C_0xE1: return operateSET_N_X(env, instr);
		case ci::SET_4_D_0xE2: return operateSET_N_X(env, instr);
		case ci::SET_4_E_0xE3: return operateSET_N_X(env, instr);
		case ci::SET_4_H_0xE4: return operateSET_N_X(env, instr);
		case ci::SET_4_L_0xE5: return operateSET_N_X(env, instr);
		case ci::SET_4_mHL_0xE6: return operateSET_N_mHL(env, instr);
		case ci::SET_4_A_0xE7: return operateSET_N_X(env, instr);
		case ci::SET_5_B_0xE8: return operateSET_N_X(env, instr);
		case ci::SET_5_C_0xE9: return operateSET_N_X(env, instr);
		case ci::SET_5_D_0xEA: return operateSET_N_X(env, instr);
		case ci::SET_5_E_0xEB: return operateSET_N_X(env, instr);
		case ci::SET_5_H_0xEC: return operateSET_N_X(env, instr);
		case ci::SET_5_L_0xED: return operateSET_N_X(env, instr);
		case ci::SET_5_mHL_0xEE: return operateSET_N_mHL(env, instr);
		case ci::SET_5_A_0xEF: return operateSET_N_X(env, instr);
		case ci::SET_6_B_0xF0: return operateSET_N_X(env, instr);
		case ci::SET_6_C_0xF1: return operateSET_N_X(env, instr);
		case ci::SET_6_D_0xF2: return operateSET_N_X(env, instr);
		case ci::SET_6_E_0xF3: return operateSET_N_X(env, instr);
		case ci::SET_6_H_0xF4: return operateSET_N_X(env, instr);
		case ci::SET_6_L_0xF5: return operateSET_N_X(env, instr);
		case ci::SET_6_mHL_0xF6: return operateSET_N_mHL(env, instr);
		case ci::SET_6_A_0xF7: return operateSET_N_X(env, instr);
		case ci::SET_7_B_0xF8: return operateSET_N_X(env, instr);
		case ci::SET_7_C_0xF9: return operateSET_N_X(env, instr);
		case ci::SET_7_D_0xFA: return operateSET_N_X(env, instr);
		case ci::SET_7_E_0xFB: return operateSET_N_X(env, instr);
		case ci::SET_7_H_0xFC: return operateSET_N_X(env, instr);
		case ci::SET_7_L_0xFD: return operateSET_N_X(env, instr);
		case ci::SET_7_mHL_0xFE: return operateSET_N_mHL(env, instr);
		case ci::SET_7_A_0xFF: return operateSET_N_X(env, instr);
		default: ;
		}

		HWLogger::Error(U"missed instruction prefixed CB: {:X}"_fmt(static_cast<uint8>(instr)));
		return CPUOperationResult(0);
	}
}
