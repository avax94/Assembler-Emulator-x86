#pragma once
#include "Directives.h"
#include <string>
#include "Parser.h"

class section_directive :
	public Directives
{
	std::string directive;
public:
	section_directive(std::string directive);
	~section_directive();
	virtual void first_pass() override;
	virtual void second_pass() override;
};

