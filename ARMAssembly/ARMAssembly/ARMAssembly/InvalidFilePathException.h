#pragma once
#include <exception>
#include <string>

class InvalidFilePathException :
	public std::exception
{
	const char* message;
public:
	virtual const char* what() const throw();

	InvalidFilePathException(const char* message);
	~InvalidFilePathException();
};

