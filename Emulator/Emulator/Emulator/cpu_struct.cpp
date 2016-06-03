#include "cpu_struct.h"
#include "typies.h"
#include <conio.h>
#include "memory.h"
#include <iostream>
#include "invalid_command.h"


cpu_struct cpu_e;

cpu_struct::cpu_struct()
{
}

cpu_struct::~cpu_struct()
{
}

int signExt(int imm, int bits)
{
	if (imm >> bits - 1)
	{
		for (int i = bits; i < sizeof(int) * 8; i++)
		{
			imm |= 1 << i;
		}
	}

	return imm;
}

void adjust_flags(Elf_Byte op, int val1 = 0, int val2 = 0)
{
	int val3 = 0;

	switch (op)
	{
	case ADD_C: case SUB_C: case CMP_C:
	{
		long long v1 = val1, v2 = val2;
		val3 = op == ADD_C ? val1 + val2 : val2 - val1;
		//CARRY
		long long v3 = op == ADD_C ? v1 + v2 : v2 - v1;
		v3 >>= 32;
		cpu_e.psw &= CF_MASK;
		cpu_e.psw |= (v3 & 1);
		//OVERFLOW
		cpu_e.psw &= OF_MASK;
		if (val1 <= 0 && val2 <= 0)
		{
			if (val3 > 0)
			{
				cpu_e.psw |= ~OF_MASK;
			}
		}
		else if (val1 > 0 && val2 > 0)
		{
			if (val3 < 0)
			{
				cpu_e.psw |= ~OF_MASK;
			}
		}
		//ZERO
		cpu_e.psw &= ZF_MASK;

		if (val3 == 0)
		{
			cpu_e.psw |= ~ZF_MASK;
		}
		//NEGATIVE
		cpu_e.psw &= NF_MASK;
		if (val3 < 0)
		{
			cpu_e.psw |= ~NF_MASK;
		}

		break;
	}
	case MUL_C: case DIV_C:
		val3 = op == MUL_C ? val1 * val2 : val2 / val1;

		//ZERO
		cpu_e.psw &= ZF_MASK;

		if (val3 == 0)
		{
			cpu_e.psw |= ~ZF_MASK;
		}
		//NEGATIVE
		cpu_e.psw &= NF_MASK;
		if (val3 < 0)
		{
			cpu_e.psw |= ~NF_MASK;
		}

		break;
	case AND_C: case OR_C: case NOT_C: case TEST_C:
		val3 = (op == AND_C || TEST_C) ? val1 & val2 : (op == OR_C ? val1 | val2 : ~val2);
		//ZERO
		cpu_e.psw &= ZF_MASK;

		if (val3 == 0)
		{
			cpu_e.psw |= ~ZF_MASK;
		}
		//NEGATIVE
		cpu_e.psw &= NF_MASK;
		if (val3 < 0)
		{
			cpu_e.psw |= ~NF_MASK;
		}
	case MOV_SHR_SHL_C:
		val3 = val2 << val1;
		//CARRY
		cpu_e.psw &= CF_MASK;

		int pos = 32 - val1;
		if ((1 << pos) & val3)
		{
			cpu_e.psw |= ~CF_MASK;
		}

		//ZERO
		cpu_e.psw &= ZF_MASK;

		if (val3 == 0)
		{
			cpu_e.psw |= ~ZF_MASK;
		}
		//NEGATIVE
		cpu_e.psw &= NF_MASK;
		if (val3 < 0)
		{
			cpu_e.psw |= ~NF_MASK;
		}
	}
}

void run_cpu()
{
	while (true)
	{
		Elf_Word instr_info = memory::read_memory(cpu_e.pc);
		Elf_Word instr = instr_info;
		bool flag = ((instr >> 28) & 1) == 1;
		instr &= F_BYTE_MASK;
		instr >>= 24;
		instr &= 0x0F;
		int reg_dst = -1;
		int reg_src = -1;
		int imm = 0;
		int val1;
		int val2;

		switch (instr)
		{
		case INT_C:
			cpu_e.sp += 4;
			memory::write_word(cpu_e.sp, cpu_e.psw);
			cpu_e.lr = cpu_e.pc;
			//TODO finish
			break;
		case ADD_C: case SUB_C: case MUL_C: case DIV_C: case CMP_C:
			reg_dst = (instr_info & T_BYTE_MASK) >> 19;
			val2 = cpu_e.r[reg_dst];

			if (reg_dst == 19)
				throw invalid_command();

			if (1 << 18 & instr_info)
			{
				imm = instr_info & 0x0003FFFF;
				imm = signExt(imm, 18);
				cpu_e.r[reg_dst] = instr == ADD_C ? cpu_e.r[reg_dst] + imm : 
					(instr == MUL_C ? cpu_e.r[reg_dst] * imm : 
					(instr == DIV_C ? cpu_e.r[reg_dst] / imm : 
					 cpu_e.r[reg_dst] - imm));
				val1 = imm;
			}
			else
			{
				reg_src = (instr_info >> 13) & 0x0000001F;
				cpu_e.r[reg_dst] = instr == ADD_C ? cpu_e.r[reg_dst] + cpu_e.r[reg_src] : 
					(instr == MUL_C ? cpu_e.r[reg_dst] * cpu_e.r[reg_src] :
					(instr == DIV_C ? cpu_e.r[reg_dst] / cpu_e.r[reg_src] : 
					 cpu_e.r[reg_dst] - cpu_e.r[reg_src]));
				val1 = cpu_e.r[reg_src];
			}

			if (flag)
			{
				adjust_flags(instr, val1, val2);
			}
			break;
		case AND_C: case OR_C: case NOT_C: case TEST_C:
			reg_dst = (instr_info >> 19) & 0x1F;
			reg_src = (instr_info >> 14) & 0x1F;
			val2 = cpu_e.r[reg_dst];
			val1 = cpu_e.r[reg_src];
			cpu_e.r[reg_dst] = instr == OR_C ? cpu_e.r[reg_dst] | cpu_e.r[reg_src] :
				(instr == NOT_C ? ~cpu_e.r[reg_src] : 
				 cpu_e.r[reg_dst] & cpu_e.r[reg_src]);

			if ((reg_dst > 15 && reg_dst != 18) || (reg_src > 15 && reg_src != 18))
				throw invalid_command();

			if (flag)
			{
				adjust_flags(instr, val1, val2);
			}
			break;
		case LDR_C: case STR_C:
		{
			reg_src = (instr_info >> 19) & 0x1F;
			reg_dst = (instr_info >> 14) & 0x1F;
			int f = (instr_info >> 11) & 0x7;
			int ls = (instr_info >> 10) & 0x1;
			imm = instr_info & 0x3FF;
			cpu_e.r[reg_dst] &= cpu_e.r[reg_src];

			if ((reg_src == 0x10 && f != 0) || reg_src == 0x13 || ls == 0)
				throw invalid_command();

			if (f == 4) cpu_e.r[reg_src] += 4;
			else if (f == 5) cpu_e.r[reg_src] -= 4;
			if (instr == LDR_C)
				cpu_e.r[reg_dst] = memory::read_word(cpu_e.r[reg_src] + signExt(imm, sizeof(Elf_Addr)));
			else
				memory::write_word(cpu_e.r[reg_src] + signExt(imm, sizeof(Elf_Word) * 8), cpu_e.r[reg_dst]);

			if (f == 2) cpu_e.r[reg_src] += 4;
			else if (f == 3) cpu_e.r[reg_src] -= 4;

			break;
		}
		case CALL_C:
			reg_dst = (instr_info >> 19) & 0x1F;
			imm = instr_info & 0x7FFFF;
			cpu_e.lr = cpu_e.pc;
			cpu_e.pc = cpu_e.r[reg_dst] + signExt(imm, sizeof(Elf_Addr));

			break;
		case IN_OUT_C:
			//TODO implement
			break;
		case MOV_SHR_SHL_C:
		{
			reg_dst = (instr_info >> 19) & 0x1F;
			reg_src = (instr_info >> 14) & 0x1F;
			imm = (instr_info >> 9) & 0x1F;
			int lr = (instr_info >> 8) & 0x1;
			val1 = imm;
			val2 = cpu_e.r[reg_dst];

			if (lr == 1)
			{
				cpu_e.r[reg_dst] = cpu_e.r[reg_src] << imm;
			}
			else
			{
				cpu_e.r[reg_dst] = cpu_e.r[reg_src] >> imm;
			}

			if (reg_dst == 0x10)
			{
				cpu_e.psw = memory::read_memory(cpu_e.sp);
				cpu_e.sp -= 4;
			}

			if (flag)
			{
				adjust_flags(instr, val1, val2);
			}
			break;
		}
		case LDCH_L_C:
			reg_dst = (instr_info >> 20) & 0xF;
			int hl = (instr_info >> 19) & 0x1;
			imm = instr_info & CLEAR_MASH_H;

			if (hl == 1)
			{
				cpu_e.r[reg_dst] &= CLEAR_MASH_H;
				cpu_e.r[reg_dst] |= (imm << 16);
			}
			else
			{
				cpu_e.r[reg_dst] &= CLEAR_MASK_L;
				cpu_e.r[reg_dst] |= imm;
			}

			break;
		}
	}
}
//
//
//
//void main()
//{
//	while (true)
//		if (_kbhit())
//		{
//			_getch();
//			std::cout << "BUTTON PRESSED" << endl;
//		}
//}