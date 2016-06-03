#pragma once
#include "command_argument.h"
#include <string>
#include "typies.h"

class immediate_argument :
	public command_argument
{
	Elf_Word arg;
public:
	immediate_argument(Elf_Word arg);
	~immediate_argument(); 

	virtual std::string get_value();
	virtual void first_pass() override;
	virtual void second_pass() override;

};

