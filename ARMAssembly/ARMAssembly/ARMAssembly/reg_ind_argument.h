#pragma once
#include "register_argument.h"
#include <string>

class reg_ind_argument :
	public register_argument
{
public:
	reg_ind_argument(std::string reg);
	~reg_ind_argument();
	virtual void first_pass();
	virtual void second_pass();
};

