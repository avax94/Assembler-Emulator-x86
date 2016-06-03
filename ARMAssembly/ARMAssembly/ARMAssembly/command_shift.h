#pragma once
#include "Command.h"

class Parser;

class command_shift :
	public Command
{
public:
	command_shift(std::string command, bool flag, char condition);
	~command_shift();

	virtual void first_pass();
	virtual void second_pass();
};

