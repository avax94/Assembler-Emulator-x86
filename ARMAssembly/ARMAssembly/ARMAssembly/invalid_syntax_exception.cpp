#include "invalid_syntax_exception.h"


invalid_syntax_exception::invalid_syntax_exception(char * message)
{
	this->message = message;
}


invalid_syntax_exception::~invalid_syntax_exception()
{
}

const char* invalid_syntax_exception::what() const throw()
{
	return message;
}

