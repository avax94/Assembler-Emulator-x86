#pragma once
#include <string>
#include "Command.h"

class command_pop :
	public Command
{
public:
	command_pop(std::string command, bool flag, char condition);
	~command_pop();
};

