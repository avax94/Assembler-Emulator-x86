#pragma once
#include <iostream>
#include <vector>
#include "typies.h"
#include <string>
#include "section.h"

class string_table :
	public section
{
	std::vector<std::string> table;
	Elf_Word idx;
	Elf_Word table_size;
public:
	int exist(std::string);
	Elf_Word put(std::string);
	std::string get(unsigned int index);
	int byte_size() const;
	virtual char* serialize();


	string_table(std::string name);
	~string_table();
};

