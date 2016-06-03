#pragma once
#include "Command.h"
#include <string>

class command_log :
	public Command
{
public:
	command_log(std::string command, bool flag, char condition);
	~command_log();
	virtual void first_pass() override;
	virtual void second_pass() override;
};

