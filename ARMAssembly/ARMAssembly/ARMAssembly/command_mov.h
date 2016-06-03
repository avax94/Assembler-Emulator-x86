#pragma once
#include "Command.h"
#include <string>

class command_mov :
	public Command
{
public:
	command_mov(std::string command, bool flag, char condition);
	~command_mov();
	virtual void first_pass() override;
	virtual void second_pass() override;
};

