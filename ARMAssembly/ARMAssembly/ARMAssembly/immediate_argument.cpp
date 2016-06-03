#include "immediate_argument.h"
#include "Parser.h"
#include <cmath>

immediate_argument::immediate_argument(Elf_Word arg) : command_argument()
{
	this->arg = arg;
	increment = 0;
}

std::string immediate_argument::get_value()
{
	return to_string(arg);
}

immediate_argument::~immediate_argument()
{
}


void immediate_argument::first_pass()
{
	if (abs_diff(arg, value) > border)
	{
		parser->increment_LC(increment);
	}
}

void immediate_argument::second_pass()
{
}