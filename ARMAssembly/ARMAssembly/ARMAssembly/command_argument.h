#pragma once
#include "Entity.h"
#include "typies.h"
#include "Parser.h"

class command_argument :
	public Entity
{
protected:
	Elf_Word value;
	Elf_Word border;
	Elf_Word increment;
public:
	command_argument();
	~command_argument();
	virtual void first_pass();
	virtual void second_pass();
	
	void set_value(Elf_Word value);
	void set_border(Elf_Word border);
	void set_increment(Elf_Word increment);

};

