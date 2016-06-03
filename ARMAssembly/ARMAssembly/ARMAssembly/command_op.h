#pragma once
#include "Command.h"
#include <string>

class command_op :
	public Command
{
public:
	command_op(std::string command, bool flag, char condition);
	~command_op();
	virtual void first_pass() override;
	virtual void second_pass() override;
};

