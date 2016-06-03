#pragma once
#include "typies.h"
#include <vector>

class hard_disk
{
public:
	hard_disk();
	~hard_disk();

	void write(Elf_Double page, const Elf_Byte* data, int N);
	Elf_Byte* read(Elf_Double page, int N);
};

