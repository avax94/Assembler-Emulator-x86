#pragma once
#include "Command.h"
class command_push :
	public Command
{
public:
	command_push(std::string command, bool flag, char condition);
	~command_push();
};

