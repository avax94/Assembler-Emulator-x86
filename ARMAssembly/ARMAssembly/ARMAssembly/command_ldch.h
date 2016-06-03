#pragma once
#include "Command.h"
#include <string>

class command_ldch :
	public Command
{
public:
	command_ldch(std::string command, bool flag, char condition);
	~command_ldch();
	virtual void first_pass() override;
	virtual void second_pass() override;
};

