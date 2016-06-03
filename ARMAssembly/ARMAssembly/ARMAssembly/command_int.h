#pragma once
#include "Command.h"
#include <string>

class command_int :
	public Command
{
public:
	command_int(std::string command, bool flag, char condition);
	~command_int();
	virtual void first_pass() override;
	virtual void second_pass() override;
};

