#include "Command.h"
#include "Parser.h"
#include "symbol_table.h"
#include "typies.h"
#include "invalid_syntax_exception.h"

Command::Command(std::string command, bool flag, char condition)
{
	active_command = true;
	this->condition = condition;
	this->flag = flag ? 1 : 0;
	this->command = command;
}

Command::~Command()
{
	for (int i = 0; i < args.size(); i++) { delete args[i]; }
	active_command = false;
}

int Command::parse_register(std::string reg)
{
	if (reg.size() > 3)
	{
		int idx = reg.find("r");
		string r = "" + reg[idx++];
		r += reg[idx]++;

		if (idx < reg.size() && reg[idx] != '+')
		{
			r += reg[idx];
		}

		return stoi(reg.substr(1));
	}
	else
	{
		return stoi(reg.substr(1));
	}
}

int Command::parse_reg_imm_value(std::string reg)
{
	int idx = reg.find("+");
	
	if (idx == string::npos)
	{
		return 0;
	}

	if (reg[idx + 1] != 'r')
	{
		return stoi(reg.substr(idx + 1));
	}
	else
	{
		return stoi(reg.substr(0, idx));
	}
}

int Command::shrink_value(int value, int bits)
{
	if (value < 0)
	{
		value = (1 << (bits - 1)) + value;
		value |= 1 << (bits - 1);
	}

	if (value >> bits != 0)
	{
		throw invalid_syntax_exception("Invalid value size!");
	}

	return value;
}

bool Command::can_shrink(int value, int bits)
{
	if (value < 0)
	{
		value = (2 << (bits - 1)) - value;
		value |= 1 << (bits - 1);
	}

	return (value >> bits) != 0;
}

Elf_Word Command::get_symbol_value(std::string symbol)
{
	return parser->get_symbol_table()->get_symbol_value(symbol);
}

Elf_Sym Command::get_symbol(std::string symbol)
{
	return parser->get_symbol_table()->get_symbol(symbol);
}

Elf_Word Command::generate_instruction(Elf_Word first, Elf_Word second, Elf_Word third, Elf_Word fourth)
{
	Elf_Word result = 0;
	result |= first << 24;
	result |= second << 16;
	result |= third << 8;
	result |= fourth;
	return result;
}
