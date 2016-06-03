#include "symbol.h"


symbol_container::symbol_container(std::string name, Elf_Sym symbol)
{
	this->name = name;
	this->symbol = symbol;
}


symbol_container::~symbol_container()
{
}

symbol_container::symbol_container()
{
}
