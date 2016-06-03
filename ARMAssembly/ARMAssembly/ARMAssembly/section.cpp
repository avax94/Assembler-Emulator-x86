#include "section.h"
#include "reallocation_table.h"

section::section(Elf_Shdr& elf, string name, reallocation_table* realloc_t)
{
	this->name = name;
	realloc_table = realloc_t;
	section_info = elf;
	location_counter = 0;
}

section::section(char* section, Elf_Shdr sd)
{
	section_data = section;
	section_info = sd;
}

section::~section()
{
	delete section_data;
	delete realloc_table;
}

std::string section::get_name() const
{
	return name;
}

Elf_Word section::get_LC() const
{
	return location_counter;
}

reallocation_table* section::get_realloc_t() const
{
	return realloc_table;
}

void section::set_section_offset(Elf_Word offset)
{
	section_info.sh_offset = offset;
}

Elf_Word section::get_section_size() const
{
	return section_info.sh_size;
}