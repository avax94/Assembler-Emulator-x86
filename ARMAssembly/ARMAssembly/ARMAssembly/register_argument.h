#pragma once
#include "Entity.h"
#include <string>

class register_argument :
	public Entity
{
protected:
	std::string reg;
public:
	register_argument(std::string);
	~register_argument();
	virtual void first_pass() override;
	virtual void second_pass() override;
	virtual std::string get_value();
};

