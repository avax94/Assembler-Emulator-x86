#pragma once
#include "typies.h"
#include <vector>
#include <string>
#include "section.h"
#include "section_table.h"
#include "symbol.h"
using namespace std;

class Label;
class string_table;

class symbol_table :
	public section
{
	std::vector<symbol_container> table;
	string_table* str_table;
	section* my_section;
	
public:
	void put(Elf_Sym symbol, std::string name, bool force_overwrite);
	void erase_locals();
	char * serialize();
	Elf_Sym get_symbol(std::string);
	Elf_Word get_symbol_value(std::string);
	symbol_table(section_table* sc_tbl);
	~symbol_table();
	void print();
	int exist(std::string);
	void write_to_string_table();
	Elf_Word count_glob_before(std::string);
	symbol_table();

	static bool is_global(Elf_Sym&);
	static bool is_local(Elf_Sym&);
};

