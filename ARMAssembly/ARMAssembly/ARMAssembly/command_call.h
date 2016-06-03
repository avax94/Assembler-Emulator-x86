#pragma once
#include "Command.h"
#include <string>
class Parser;

class command_call :
	public Command
{
	int f;
public:
	command_call(std::string command, bool flag, char condition);
	~command_call();

	virtual void first_pass() override;
	virtual void second_pass() override;
};

