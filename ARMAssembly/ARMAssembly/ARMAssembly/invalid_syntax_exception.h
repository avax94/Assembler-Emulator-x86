#pragma once
#include <exception>
#include <string>

class invalid_syntax_exception :
	public std::exception
{
	char * message;
public:
	virtual const char* what() const throw();

	invalid_syntax_exception(char * message);
	~invalid_syntax_exception();
};

