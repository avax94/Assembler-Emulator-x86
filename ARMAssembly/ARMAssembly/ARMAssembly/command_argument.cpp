#include "command_argument.h"
#include "command_argument.h"
#include "Parser.h"

command_argument::command_argument()
{
}


command_argument::~command_argument()
{
}

void command_argument::first_pass()
{
	if (value > border)
	{
		parser->increment_LC(increment);
	}
}

void command_argument::second_pass()
{
	
}

void command_argument::set_border(Elf_Word border)
{
	this->border = border;
}

void command_argument::set_increment(Elf_Word increment)
{
	this->increment = increment;
}

void command_argument::set_value(Elf_Word value)
{
	this->value = value;
}