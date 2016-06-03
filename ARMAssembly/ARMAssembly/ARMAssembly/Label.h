#pragma once
#include "Entity.h"
#include <iostream>
#include <string>
#include "typies.h"

class Parser;

class Label : 
	public Entity
{
	Elf_Addr value;
	std::string label_name;
public:
	Label(std::string, Elf_Addr value);
	~Label();

	void set_flag(Elf_Byte flags);
	Elf_Byte get_flag() const;

	std::string get_label_name() const;

	virtual void first_pass() override;
	virtual void second_pass() override;
};

