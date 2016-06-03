#pragma once
#include <string>

class Entity
{
public:
	Entity() {};
	virtual ~Entity(){};
	virtual std::string get_value(){ return ""; }
	virtual void first_pass(){}
	virtual void second_pass() = 0;
};

