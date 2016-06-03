#pragma once
#include <string>
#include "typies.h"

struct symbol_container
{
public:
	std::string name;
	Elf_Sym symbol;
	symbol_container();
	symbol_container(std::string name, Elf_Sym symbol);
	~symbol_container();
};

