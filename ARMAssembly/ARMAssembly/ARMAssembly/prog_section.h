#pragma once
#include "section.h"
#include "reallocation_table.h"
#include <vector>

class File;

class prog_section :
	public section
{
	std::vector<Elf_Byte> instructions;
public:
	prog_section(Elf_Shdr sc, string name, reallocation_table* r_tbl);
	~prog_section();

	virtual void put(Elf_Byte c);
	virtual void put(Elf_Word instr);
	virtual void put(Elf_Double c);
	virtual char* serialize();
	void print(File* f);
};

