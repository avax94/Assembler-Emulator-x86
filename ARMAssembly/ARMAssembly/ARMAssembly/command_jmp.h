#pragma once
#include "Command.h"
class command_jmp :
	public Command
{
public:
	command_jmp(std::string command, bool flag, char condition);
	~command_jmp();

	virtual void first_pass();
	virtual void second_pass();
};

