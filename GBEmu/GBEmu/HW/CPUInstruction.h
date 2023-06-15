﻿#pragma once

namespace GBEmu::HW
{
	enum class CPUInstruction : uint8
	{
		NOP_0x00 = 0x00,
		LD_BC_d16_0x01 = 0x01,
		LD_mBC_A_0x02 = 0x02,
		INC_BC_0x03 = 0x03,
		INC_B_0x04 = 0x04,
		DEC_B_0x05 = 0x05,
		LD_B_d8_0x06 = 0x06,
		RLCA_0x07 = 0x07,
		LD_ma16_SP_0x08 = 0x08,
		ADD_HL_BC_0x09 = 0x09,
		LD_A_mBC_0x0A = 0x0A,
		DEC_BC_0x0B = 0x0B,
		INC_C_0x0C = 0x0C,
		DEC_C_0x0D = 0x0D,
		LD_C_d8_0x0E = 0x0E,
		RRCA_0x0F = 0x0F,
		STOP_d8_0x10 = 0x10,
		LD_DE_d16_0x11 = 0x11,
		LD_mDE_A_0x12 = 0x12,
		INC_DE_0x13 = 0x13,
		INC_D_0x14 = 0x14,
		DEC_D_0x15 = 0x15,
		LD_D_d8_0x16 = 0x16,
		RLA_0x17 = 0x17,
		JR_r8_0x18 = 0x18,
		ADD_HL_DE_0x19 = 0x19,
		LD_A_mDE_0x1A = 0x1A,
		DEC_DE_0x1B = 0x1B,
		INC_E_0x1C = 0x1C,
		DEC_E_0x1D = 0x1D,
		LD_E_d8_0x1E = 0x1E,
		RRA_0x1F = 0x1F,
		JR_NZ_r8_0x20 = 0x20,
		LD_HL_d16_0x21 = 0x21,
		LD_mHLi_A_0x22 = 0x22,
		INC_HL_0x23 = 0x23,
		INC_H_0x24 = 0x24,
		DEC_H_0x25 = 0x25,
		LD_H_d8_0x26 = 0x26,
		DAA_0x27 = 0x27,
		JR_Z_r8_0x28 = 0x28,
		ADD_HL_HL_0x29 = 0x29,
		LD_A_mHLi_0x2A = 0x2A,
		DEC_HL_0x2B = 0x2B,
		INC_L_0x2C = 0x2C,
		DEC_L_0x2D = 0x2D,
		LD_L_d8_0x2E = 0x2E,
		CPL_0x2F = 0x2F,
		JR_NC_r8_0x30 = 0x30,
		LD_SP_d16_0x31 = 0x31,
		LD_mHLd_A_0x32 = 0x32,
		INC_SP_0x33 = 0x33,
		INC_mHL_0x34 = 0x34,
		DEC_mHL_0x35 = 0x35,
		LD_mHL_d8_0x36 = 0x36,
		SCF_0x37 = 0x37,
		JR_C_r8_0x38 = 0x38,
		ADD_HL_SP_0x39 = 0x39,
		LD_A_mHLd_0x3A = 0x3A,
		DEC_SP_0x3B = 0x3B,
		INC_A_0x3C = 0x3C,
		DEC_A_0x3D = 0x3D,
		LD_A_d8_0x3E = 0x3E,
		CCF_0x3F = 0x3F,
		LD_B_B_0x40 = 0x40,
		LD_B_C_0x41 = 0x41,
		LD_B_D_0x42 = 0x42,
		LD_B_E_0x43 = 0x43,
		LD_B_H_0x44 = 0x44,
		LD_B_L_0x45 = 0x45,
		LD_B_mHL_0x46 = 0x46,
		LD_B_A_0x47 = 0x47,
		LD_C_B_0x48 = 0x48,
		LD_C_C_0x49 = 0x49,
		LD_C_D_0x4A = 0x4A,
		LD_C_E_0x4B = 0x4B,
		LD_C_H_0x4C = 0x4C,
		LD_C_L_0x4D = 0x4D,
		LD_C_mHL_0x4E = 0x4E,
		LD_C_A_0x4F = 0x4F,
		LD_D_B_0x50 = 0x50,
		LD_D_C_0x51 = 0x51,
		LD_D_D_0x52 = 0x52,
		LD_D_E_0x53 = 0x53,
		LD_D_H_0x54 = 0x54,
		LD_D_L_0x55 = 0x55,
		LD_D_mHL_0x56 = 0x56,
		LD_D_A_0x57 = 0x57,
		LD_E_B_0x58 = 0x58,
		LD_E_C_0x59 = 0x59,
		LD_E_D_0x5A = 0x5A,
		LD_E_E_0x5B = 0x5B,
		LD_E_H_0x5C = 0x5C,
		LD_E_L_0x5D = 0x5D,
		LD_E_mHL_0x5E = 0x5E,
		LD_E_A_0x5F = 0x5F,
		LD_H_B_0x60 = 0x60,
		LD_H_C_0x61 = 0x61,
		LD_H_D_0x62 = 0x62,
		LD_H_E_0x63 = 0x63,
		LD_H_H_0x64 = 0x64,
		LD_H_L_0x65 = 0x65,
		LD_H_mHL_0x66 = 0x66,
		LD_H_A_0x67 = 0x67,
		LD_L_B_0x68 = 0x68,
		LD_L_C_0x69 = 0x69,
		LD_L_D_0x6A = 0x6A,
		LD_L_E_0x6B = 0x6B,
		LD_L_H_0x6C = 0x6C,
		LD_L_L_0x6D = 0x6D,
		LD_L_mHL_0x6E = 0x6E,
		LD_L_A_0x6F = 0x6F,
		LD_mHL_B_0x70 = 0x70,
		LD_mHL_C_0x71 = 0x71,
		LD_mHL_D_0x72 = 0x72,
		LD_mHL_E_0x73 = 0x73,
		LD_mHL_H_0x74 = 0x74,
		LD_mHL_L_0x75 = 0x75,
		HALT_0x76 = 0x76,
		LD_mHL_A_0x77 = 0x77,
		LD_A_B_0x78 = 0x78,
		LD_A_C_0x79 = 0x79,
		LD_A_D_0x7A = 0x7A,
		LD_A_E_0x7B = 0x7B,
		LD_A_H_0x7C = 0x7C,
		LD_A_L_0x7D = 0x7D,
		LD_A_mHL_0x7E = 0x7E,
		LD_A_A_0x7F = 0x7F,
		ADD_A_B_0x80 = 0x80,
		ADD_A_C_0x81 = 0x81,
		ADD_A_D_0x82 = 0x82,
		ADD_A_E_0x83 = 0x83,
		ADD_A_H_0x84 = 0x84,
		ADD_A_L_0x85 = 0x85,
		ADD_A_mHL_0x86 = 0x86,
		ADD_A_A_0x87 = 0x87,
		ADC_A_B_0x88 = 0x88,
		ADC_A_C_0x89 = 0x89,
		ADC_A_D_0x8A = 0x8A,
		ADC_A_E_0x8B = 0x8B,
		ADC_A_H_0x8C = 0x8C,
		ADC_A_L_0x8D = 0x8D,
		ADC_A_mHL_0x8E = 0x8E,
		ADC_A_A_0x8F = 0x8F,
		SUB_A_B_0x90 = 0x90,
		SUB_A_C_0x91 = 0x91,
		SUB_A_D_0x92 = 0x92,
		SUB_A_E_0x93 = 0x93,
		SUB_A_H_0x94 = 0x94,
		SUB_A_L_0x95 = 0x95,
		SUB_A_mHL_0x96 = 0x96,
		SUB_A_A_0x97 = 0x97,
		SBC_A_B_0x98 = 0x98,
		SBC_A_C_0x99 = 0x99,
		SBC_A_D_0x9A = 0x9A,
		SBC_A_E_0x9B = 0x9B,
		SBC_A_H_0x9C = 0x9C,
		SBC_A_L_0x9D = 0x9D,
		SBC_A_mHL_0x9E = 0x9E,
		SBC_A_A_0x9F = 0x9F,
		AND_A_B_0xA0 = 0xA0,
		AND_A_C_0xA1 = 0xA1,
		AND_A_D_0xA2 = 0xA2,
		AND_A_E_0xA3 = 0xA3,
		AND_A_H_0xA4 = 0xA4,
		AND_A_L_0xA5 = 0xA5,
		AND_A_mHL_0xA6 = 0xA6,
		AND_A_A_0xA7 = 0xA7,
		XOR_A_B_0xA8 = 0xA8,
		XOR_A_C_0xA9 = 0xA9,
		XOR_A_D_0xAA = 0xAA,
		XOR_A_E_0xAB = 0xAB,
		XOR_A_H_0xAC = 0xAC,
		XOR_A_L_0xAD = 0xAD,
		XOR_A_mHL_0xAE = 0xAE,
		XOR_A_A_0xAF = 0xAF,
		OR_A_B_0xB0 = 0xB0,
		OR_A_C_0xB1 = 0xB1,
		OR_A_D_0xB2 = 0xB2,
		OR_A_E_0xB3 = 0xB3,
		OR_A_H_0xB4 = 0xB4,
		OR_A_L_0xB5 = 0xB5,
		OR_A_mHL_0xB6 = 0xB6,
		OR_A_A_0xB7 = 0xB7,
		CP_A_B_0xB8 = 0xB8,
		CP_A_C_0xB9 = 0xB9,
		CP_A_D_0xBA = 0xBA,
		CP_A_E_0xBB = 0xBB,
		CP_A_H_0xBC = 0xBC,
		CP_A_L_0xBD = 0xBD,
		CP_A_mHL_0xBE = 0xBE,
		CP_A_A_0xBF = 0xBF,
		RET_NZ_0xC0 = 0xC0,
		POP_BC_0xC1 = 0xC1,
		JP_NZ_a16_0xC2 = 0xC2,
		JP_a16_0xC3 = 0xC3,
		CALL_NZ_a16_0xC4 = 0xC4,
		PUSH_BC_0xC5 = 0xC5,
		ADD_A_d8_0xC6 = 0xC6,
		RST_00h_0xC7 = 0xC7,
		RET_Z_0xC8 = 0xC8,
		RET_0xC9 = 0xC9,
		JP_Z_a16_0xCA = 0xCA,
		Prefix_0xCB = 0xCB,
		CALL_Z_a16_0xCC = 0xCC,
		CALL_a16_0xCD = 0xCD,
		ADC_A_d8_0xCE = 0xCE,
		RST_08h_0xCF = 0xCF,
		RET_NC_0xD0 = 0xD0,
		POP_DE_0xD1 = 0xD1,
		JP_NC_a16_0xD2 = 0xD2,
		Reserved_0xD3 = 0xD3,
		CALL_NC_a16_0xD4 = 0xD4,
		PUSH_DE_0xD5 = 0xD5,
		SUB_A_d8_0xD6 = 0xD6,
		RST_10h_0xD7 = 0xD7,
		RET_C_0xD8 = 0xD8,
		RETI_0xD9 = 0xD9,
		JP_C_a16_0xDA = 0xDA,
		Reserved_0xDB = 0xDB,
		CALL_C_a16_0xDC = 0xDC,
		Reserved_0xDD = 0xDD,
		SBC_A_d8_0xDE = 0xDE,
		RST_18h_0xDF = 0xDF,
		LDH_a8_A_0xE0 = 0xE0,
		POP_HL_0xE1 = 0xE1,
		LD_mC_A_0xE2 = 0xE2,
		Reserved_0xE3 = 0xE3,
		Reserved_0xE4 = 0xE4,
		PUSH_HL_0xE5 = 0xE5,
		AND_A_d8_0xE6 = 0xE6,
		RST_20h_0xE7 = 0xE7,
		ADD_SP_r8_0xE8 = 0xE8,
		JP_HL_0xE9 = 0xE9,
		LD_ma16_A_0xEA = 0xEA,
		Reserved_0xEB = 0xEB,
		Reserved_0xEC = 0xEC,
		Reserved_0xED = 0xED,
		XOR_A_d8_0xEE = 0xEE,
		RST_28h_0xEF = 0xEF,
		LDH_A_a8_0xF0 = 0xF0,
		POP_AF_0xF1 = 0xF1,
		LD_A_mC_0xF2 = 0xF2,
		DI_0xF3 = 0xF3,
		Reserved_0xF4 = 0xF4,
		PUSH_AF_0xF5 = 0xF5,
		OR_A_d8_0xF6 = 0xF6,
		RST_30h_0xF7 = 0xF7,
		LD_HL_SPir8_0xF8 = 0xF8,
		LD_SP_HL_0xF9 = 0xF9,
		LD_A_ma16_0xFA = 0xFA,
		EI_0xFB = 0xFB,
		Reserved_0xFC = 0xFC,
		Reserved_0xFD = 0xFD,
		CP_A_d8_0xFE = 0xFE,
		RST_38h_0xFF = 0xFF,
	};
}
