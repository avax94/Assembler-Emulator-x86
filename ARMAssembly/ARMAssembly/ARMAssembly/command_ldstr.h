#pragma once
#include "Command.h"
#include <string>

class command_ldstr :
	public Command
{
	int f;
public:
	command_ldstr(std::string command, bool flag, char condition, int f);
	~command_ldstr();
	virtual void first_pass() override;
	virtual void second_pass() override;
};

