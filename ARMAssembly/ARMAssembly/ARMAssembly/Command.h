#pragma once
#include "Entity.h"
#include "Parser.h"
#include <string>
#include "typies.h"
#define COMMAND_SIZE 4

class Command :
	public Entity
{
protected:
	Elf_Byte flag;
	vector<Entity*> args;
	std::string command;
	char condition;
public:
	Command(std::string command, bool flag, char condition);
	~Command();
	virtual void first_pass() = 0;
	virtual void second_pass() = 0;

	static bool can_shrink(int value, int bits);
	static int shrink_value(int value, int bits);
	static int parse_reg_imm_value(std::string reg);
	static int parse_register(std::string);
	static Elf_Word get_symbol_value(std::string);
	static Elf_Sym get_symbol(std::string);
	static Elf_Word generate_instruction(Elf_Word first, Elf_Word second, Elf_Word third, Elf_Word fourth);
};

