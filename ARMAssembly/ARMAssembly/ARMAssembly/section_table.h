#pragma once
#include <vector>
#include "typies.h"
#include <string>
#include "string_table.h"
#include "section.h"

class section_table
{
	std::vector<section*> table;
	string_table* str_table;
public:
	section_table();
	virtual ~section_table();

	Elf_Word get_section_number() const;
	section* get_section(std::string);
	section* get_current_section() const;
	void put_str_tbl();
	Elf_Word put(section *shdr, std::string);
	Elf_Word get_section_id(std::string);
	void adjust_size(Elf_Word size);
	void print();
	void increment_LC(Elf_Word size);
	bool contains(std::string);
	void reset_LCs();
	section* get_section(Elf_Word id);
	char* serialize();
};

