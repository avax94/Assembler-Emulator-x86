#pragma once
#include "Directives.h"
#include <string>

class Parser;

class type_directive :
	public Directives
{
	std::string directive;
public:
	type_directive(std::string directive);
	~type_directive();
	virtual void first_pass() override;
	virtual void second_pass() override;
};

