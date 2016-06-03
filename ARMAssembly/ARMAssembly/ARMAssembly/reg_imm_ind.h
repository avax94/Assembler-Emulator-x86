#pragma once
#include "register_argument.h"
#include <string>

class reg_imm_ind :
	public register_argument
{
	std::string value;
public:
	reg_imm_ind(std::string reg);
	~reg_imm_ind();

	//TODO
	virtual void first_pass(){}
	virtual void second_pass(){}
};

