#pragma once
#include "typies.h"

#define CF_MASK 0xB
#define OF_MASK 0xD
#define ZF_MASK 0xE
#define NF_MASK 0x7

struct cpu_struct
{
	Elf_Word r[20];
	Elf_Word &pc = r[16];
	Elf_Word &lr = r[17];
	Elf_Word &sp = r[18];
	Elf_Word &psw = r[19];

	cpu_struct();
	~cpu_struct();
};

void run_cpu();

