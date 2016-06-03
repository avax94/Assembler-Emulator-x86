#pragma once
#include "typies.h"
#include <vector>
#include <string>
#include "section.h"

class reallocation_table :
	public section
{
	std::vector<Elf_Rel> table;
public:
	Elf_Byte put(Elf_Rel rel);
	void set_info(Elf_Word st_info);

	reallocation_table(std::string curr_sec_name);
	~reallocation_table();

	virtual char* serialize();
};

