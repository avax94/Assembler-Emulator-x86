#pragma once
#include <exception>
using namespace std;

class invalid_command :
	public exception
{
	char * message = "Invalid command!";
public:
	virtual const char* what() const throw();

	invalid_command();
	invalid_command(char * message);
	~invalid_command();
};

