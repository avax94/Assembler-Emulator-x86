#pragma once
#include "Directives.h"
#include <string>

class Parser;

class global_directive :
	public Directives
{
public:
	global_directive();
	~global_directive();
	virtual void first_pass() override;
	virtual void second_pass() override;
};

