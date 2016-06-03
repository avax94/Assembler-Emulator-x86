#pragma once
#include "Command.h"
#include <string>

class command_io : 
	public Command
{
public:
	command_io(std::string command, bool flag, char condition);
	virtual ~command_io() override;
	virtual void first_pass() override;
	virtual void second_pass() override;
};

