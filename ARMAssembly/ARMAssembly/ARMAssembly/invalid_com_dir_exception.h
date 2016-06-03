#pragma once
#include "invalid_syntax_exception.h"
class invalid_com_dir_exception :
	public invalid_syntax_exception
{
public:
	invalid_com_dir_exception(char*);
	~invalid_com_dir_exception();
};

