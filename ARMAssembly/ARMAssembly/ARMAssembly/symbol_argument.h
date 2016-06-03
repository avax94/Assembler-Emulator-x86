#pragma once
#include <iostream>
#include "command_argument.h"
#include "typies.h"
#include <string>

class Parser;

class symbol_argument :
	public command_argument
{
	std::string symbol_name;
	Elf_Byte flag;
public:
	symbol_argument(std::string);
	~symbol_argument();

	void set_flag(Elf_Byte);
	virtual std::string get_value();
	std::string get_name();
	virtual void first_pass() override;
	virtual void second_pass() override;
};

