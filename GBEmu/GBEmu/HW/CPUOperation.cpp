#include "stdafx.h"
#include "CPUOperation.h"

#include "HWEnv.h"
#include "HWLogger.h"

namespace GBEmu::HW::CPUOperation
{
	using ci = CPUInstruction;

	CPUOperationResult operateNOP()
	{
		return CPUOperationResult(1, 4);
	}

	CPUOperationResult operateLD_XX_d16(HWEnv& env, CPUInstruction instr)
	{
		const uint16 d16 = env.GetMemory().Read(env.GetCPU().PC() + 1);

		switch (instr)
		{
		case ci::LD_BC_d16:
			env.GetCPU().SetBC(d16); break;
		case ci::LD_DE_d16:
			env.GetCPU().SetDE(d16); break;
		case ci::LD_HL_d16:
			env.GetCPU().SetHL(d16); break;
		case ci::LD_SP_d16:
			env.GetCPU().SetSP(d16); break;
		default:
			assert(false);
		}

		return CPUOperationResult(3, 12);
	}

	CPUOperationResult OperateInstruction(HWEnv& env, CPUInstruction instr)
	{
		switch (instr)
		{
		case ci::NOP: return operateNOP();
		case ci::LD_BC_d16: return operateLD_XX_d16(env, instr);
		case ci::LD_mBC_A: break;
		case ci::INC_BC: break;
		case ci::INC_B: break;
		case ci::DEC_B: break;
		case ci::LD_B_d8: break;
		case ci::RLCA: break;
		case ci::LD_ma16_SP: break;
		case ci::ADD_HL_BC: break;
		case ci::LD_A_mBC: break;
		case ci::DEC_BC: break;
		case ci::INC_C: break;
		case ci::DEC_C: break;
		case ci::LD_C_d8: break;
		case ci::RRCA: break;
		case ci::STOP_d8: break;
		case ci::LD_DE_d16: operateLD_XX_d16(env, instr);
		case ci::LD_mDE_A: break;
		case ci::INC_DE: break;
		case ci::INC_D: break;
		case ci::DEC_D: break;
		case ci::LD_D_d8: break;
		case ci::RLA: break;
		case ci::JR_r8: break;
		case ci::ADD_HL_DE: break;
		case ci::LD_A_mDE: break;
		case ci::DEC_DE: break;
		case ci::INC_E: break;
		case ci::DEC_E: break;
		case ci::LD_E_d8: break;
		case ci::RRA: break;
		case ci::JR_NZ_r8: break;
		case ci::LD_HL_d16: operateLD_XX_d16(env, instr);
		case ci::LDI_mHL_A: break;
		case ci::INC_HL: break;
		case ci::INC_H: break;
		case ci::DEC_H: break;
		case ci::LD_H_d8: break;
		case ci::DAA: break;
		case ci::JR_Z_r8: break;
		case ci::ADD_HL_HL: break;
		case ci::LD_A_mHLp: break;
		case ci::DEC_HL: break;
		case ci::INC_L: break;
		case ci::DEC_L: break;
		case ci::LD_L_d8: break;
		case ci::CPL: break;
		case ci::JR_NC_r8: break;
		case ci::LD_SP_d16: operateLD_XX_d16(env, instr);
		case ci::LD_mHL_A: break;
		case ci::INC_SP: break;
		case ci::INC_mHL: break;
		case ci::DEC_mHL: break;
		case ci::LD_mHL_d8: break;
		case ci::SCF: break;
		case ci::JR_C_r8: break;
		case ci::ADD_HL_SP: break;
		case ci::LD_A_mHLm: break;
		case ci::DEC_SP: break;
		case ci::INC_A: break;
		case ci::DEC_A: break;
		case ci::LD_A_d8: break;
		case ci::CCF: break;
		case ci::LD_B_B: break;
		case ci::LD_B_C: break;
		case ci::LD_B_D: break;
		case ci::LD_B_E: break;
		case ci::LD_B_H: break;
		case ci::LD_B_L: break;
		case ci::LD_B_HL: break;
		case ci::LD_B_A: break;
		case ci::LD_C_B: break;
		case ci::LD_C_C: break;
		case ci::LD_C_D: break;
		case ci::LD_C_E: break;
		case ci::LD_C_H: break;
		case ci::LD_C_L: break;
		case ci::LD_C_HL: break;
		case ci::LD_C_A: break;
		case ci::LD_D_B: break;
		case ci::LD_D_C: break;
		case ci::LD_D_D: break;
		case ci::LD_D_E: break;
		case ci::LD_D_H: break;
		case ci::LD_D_L: break;
		case ci::LD_D_HL: break;
		case ci::LD_D_A: break;
		case ci::LD_E_B: break;
		case ci::LD_E_C: break;
		case ci::LD_E_D: break;
		case ci::LD_E_E: break;
		case ci::LD_E_H: break;
		case ci::LD_E_L: break;
		case ci::LD_E_HL: break;
		case ci::LD_E_A: break;
		case ci::LD_H_B: break;
		case ci::LD_H_C: break;
		case ci::LD_H_D: break;
		case ci::LD_H_E: break;
		case ci::LD_H_H: break;
		case ci::LD_H_L: break;
		case ci::LD_H_HL: break;
		case ci::LD_H_A: break;
		case ci::LD_L_B: break;
		case ci::LD_L_C: break;
		case ci::LD_L_D: break;
		case ci::LD_L_E: break;
		case ci::LD_L_H: break;
		case ci::LD_L_L: break;
		case ci::LD_L_HL: break;
		case ci::LD_L_A: break;
		case ci::LD_HL_B: break;
		case ci::LD_HL_C: break;
		case ci::LD_HL_D: break;
		case ci::LD_HL_E: break;
		case ci::LD_HL_H: break;
		case ci::LD_HL_L: break;
		case ci::HALT: break;
		case ci::LD_HL_A: break;
		case ci::LD_A_B: break;
		case ci::LD_A_C: break;
		case ci::LD_A_D: break;
		case ci::LD_A_E: break;
		case ci::LD_A_H: break;
		case ci::LD_A_L: break;
		case ci::LD_A_HL: break;
		case ci::LD_A_A: break;
		case ci::ADD_A_B: break;
		case ci::ADD_A_C: break;
		case ci::ADD_A_D: break;
		case ci::ADD_A_E: break;
		case ci::ADD_A_H: break;
		case ci::ADD_A_L: break;
		case ci::ADD_A_HL: break;
		case ci::ADD_A_A: break;
		case ci::ADC_A_B: break;
		case ci::ADC_A_C: break;
		case ci::ADC_A_D: break;
		case ci::ADC_A_E: break;
		case ci::ADC_A_H: break;
		case ci::ADC_A_L: break;
		case ci::ADC_A_HL: break;
		case ci::ADC_A_A: break;
		case ci::SUB_B: break;
		case ci::SUB_C: break;
		case ci::SUB_D: break;
		case ci::SUB_E: break;
		case ci::SUB_H: break;
		case ci::SUB_L: break;
		case ci::SUB_HL: break;
		case ci::SUB_A: break;
		case ci::SBC_A_B: break;
		case ci::SBC_A_C: break;
		case ci::SBC_A_D: break;
		case ci::SBC_A_E: break;
		case ci::SBC_A_H: break;
		case ci::SBC_A_L: break;
		case ci::SBC_A_HL: break;
		case ci::SBC_A_A: break;
		case ci::AND_B: break;
		case ci::AND_C: break;
		case ci::AND_D: break;
		case ci::AND_E: break;
		case ci::AND_H: break;
		case ci::AND_L: break;
		case ci::AND_HL: break;
		case ci::AND_A: break;
		case ci::XOR_B: break;
		case ci::XOR_C: break;
		case ci::XOR_D: break;
		case ci::XOR_E: break;
		case ci::XOR_H: break;
		case ci::XOR_L: break;
		case ci::XOR_HL: break;
		case ci::XOR_A: break;
		case ci::OR_B: break;
		case ci::OR_C: break;
		case ci::OR_D: break;
		case ci::OR_E: break;
		case ci::OR_H: break;
		case ci::OR_L: break;
		case ci::OR_HL: break;
		case ci::OR_A: break;
		case ci::CP_B: break;
		case ci::CP_C: break;
		case ci::CP_D: break;
		case ci::CP_E: break;
		case ci::CP_H: break;
		case ci::CP_L: break;
		case ci::CP_HL: break;
		case ci::CP_A: break;
		case ci::RET_NZ: break;
		case ci::POP_BC: break;
		case ci::JP_NZ_a16: break;
		case ci::JP_a16: break;
		case ci::CALL_NZ_a16: break;
		case ci::PUSH_BC: break;
		case ci::ADD_A_d8: break;
		case ci::RST_00H: break;
		case ci::RET_Z: break;
		case ci::RET: break;
		case ci::JP_Z_a16: break;
		case ci::PREFIX: break;
		case ci::CALL_Z_a16: break;
		case ci::CALL_a16: break;
		case ci::ADC_A_d8: break;
		case ci::RST_08H: break;
		case ci::RET_NC: break;
		case ci::POP_DE: break;
		case ci::JP_NC_a16: break;
		case ci::Reserved_D3: break;
		case ci::CALL_NC_a16: break;
		case ci::PUSH_DE: break;
		case ci::SUB_d8: break;
		case ci::RST_10H: break;
		case ci::RET_C: break;
		case ci::RETI: break;
		case ci::JP_C_a16: break;
		case ci::Reserved_DB: break;
		case ci::CALL_C_a16: break;
		case ci::Reserved_DD: break;
		case ci::SBC_A_d8: break;
		case ci::RST_18H: break;
		case ci::LDH_a8_A: break;
		case ci::POP_HL: break;
		case ci::LD_mC_A: break;
		case ci::Reserved_E3: break;
		case ci::Reserved_E4: break;
		case ci::PUSH_HL: break;
		case ci::AND_d8: break;
		case ci::RST_20H: break;
		case ci::ADD_SP_r8: break;
		case ci::JP_HL: break;
		case ci::LD_a16_A: break;
		case ci::Reserved_EB: break;
		case ci::Reserved_EC: break;
		case ci::Reserved_ED: break;
		case ci::XOR_d8: break;
		case ci::RST_28H: break;
		case ci::LDH_A_a8: break;
		case ci::POP_AF: break;
		case ci::LD_A_mC: break;
		case ci::DI: break;
		case ci::Reserved_F4: break;
		case ci::PUSH_AF: break;
		case ci::OR_d8: break;
		case ci::RST_30H: break;
		case ci::LD_HL_SP_r8: break;
		case ci::LD_SP_HL: break;
		case ci::LD_A_a16: break;
		case ci::EI: break;
		case ci::Reserved_FC: break;
		case ci::Reserved_FD: break;
		case ci::CP_d8: break;
		case ci::RST_38H: break;
		default: ;
		}

		HWLogger::Error(U"missed instruction unprefixed: {}"_fmt(instr));
		return CPUOperationResult(0, 0);
	}
}
