#include "symbol_argument.h"
#include "Parser.h"
#include "typies.h"
#include "symbol_table.h"
#include <cmath>
#include <limits>


symbol_argument::symbol_argument(std::string symbol_name) : command_argument()
{
	this->symbol_name = symbol_name;
	flag = STB_LOCAL | STT_UNDEFINED;
	increment = 0;
}


symbol_argument::~symbol_argument()
{
}

void symbol_argument::first_pass()
{
	Elf_Sym symbol;
	symbol.st_info = flag;
	symbol.st_shndx = current_section;
	symbol.st_value = 0;
	symbol.st_size = sizeof(Elf_Addr);
	bool to_force = flag == (STB_GLOBAL | STT_UNDEFINED);
	parser->get_symbol_table()->put(symbol, symbol_name, to_force);

	Elf_Word sym_val = parser->get_symbol_table()->get_symbol_value(symbol_name);
}

void symbol_argument::second_pass()
{

}

void symbol_argument::set_flag(Elf_Byte flag)
{
	this->flag = flag;
}

string symbol_argument::get_value()
{
	return to_string(parser->get_symbol_table()->get_symbol_value(symbol_name));
}
string symbol_argument::get_name()
{
	return symbol_name;
}