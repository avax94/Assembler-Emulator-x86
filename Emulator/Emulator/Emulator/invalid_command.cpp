#include "invalid_command.h"


invalid_command::invalid_command()
{
}


invalid_command::~invalid_command()
{
}

const char* invalid_command::what() const throw()
{
	return message;
}

