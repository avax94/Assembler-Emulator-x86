#include "InvalidFilePathException.h"


InvalidFilePathException::InvalidFilePathException(const char* message)
{
	this->message = message;
}


InvalidFilePathException::~InvalidFilePathException() { }

const char* InvalidFilePathException::what() const throw()
{
	return message;
}
