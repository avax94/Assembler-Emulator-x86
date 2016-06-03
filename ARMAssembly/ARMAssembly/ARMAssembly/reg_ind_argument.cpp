#include "reg_ind_argument.h"


reg_ind_argument::reg_ind_argument(std::string reg) : register_argument(reg)
{
	this->reg = reg.substr(1, reg.size() - 1);
}


reg_ind_argument::~reg_ind_argument()
{
}

void reg_ind_argument::first_pass()
{
}

void reg_ind_argument::second_pass()
{
}