#include "register_argument.h"


register_argument::register_argument(std::string reg)
{
	this->reg = reg;
}


register_argument::~register_argument()
{
}

void register_argument::first_pass()
{
}

void register_argument::second_pass()
{
}

std::string register_argument::get_value()
{
	return reg;
}