#pragma once
#include "Entity.h"
#include <string>

class Directives :
	public Entity
{
public:
	Directives();
	virtual ~Directives();
	virtual void first_pass() = 0;
	virtual void second_pass() = 0;
};

