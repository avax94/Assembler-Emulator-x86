#include "section_table.h"
#include "section.h"


section_table::section_table()
{
	str_table = new string_table(".shstrtab");
}

section_table::~section_table()
{
	delete str_table;

	for (int i = 0; i < table.size(); i++)
	{
		delete table[i];
	}
}

void section_table::put_str_tbl()
{
	put(str_table, str_table->get_name());
}

Elf_Word section_table::put(section* shdr, std::string name)
{
	//TODO check indexing
	shdr->section_info.sh_name = str_table->put(name);

	if (shdr->section_info.sh_name >= table.size())
	{
		table.push_back(shdr);
	}
	
	return shdr->section_info.sh_name;
}

void section_table::adjust_size(Elf_Word size)
{
	table[current_section]->section_info.sh_size += size;
}

void section_table::print()
{
	for (int i = 0; i < table.size(); i++)
	{
		Elf_Shdr& s = table[i]->section_info;
		cout << s.sh_name << " " <<
			str_table->get(table[i]->section_info.sh_name) << " "
			<< s.sh_info << " " << s.sh_link << " " << table[i]->section_info.sh_size << endl;
	}
}

void section_table::increment_LC(Elf_Word size)
{
	table[current_section]->location_counter += size;
}

Elf_Word section_table::get_section_id(std::string name)
{
	return str_table->exist(name);
}

bool section_table::contains(std::string name)
{
	return str_table->exist(name) > -1;	
}

void section_table::reset_LCs()
{
	for (int i = 0; i < table.size(); i++)
	{
		table[i]->location_counter = 0;
	}
}

section* section_table::get_current_section() const
{
	return table[current_section];
}

section* section_table::get_section(std::string name)
{
	return table[str_table->exist(name)];
}

section* section_table::get_section(Elf_Word idx)
{
	return table[idx];
}

Elf_Word section_table::get_section_number() const
{
	return table.size();
}

char* section_table::serialize()
{
	int sz = table.size() * SIZE_OF_ELF_SHDR;
	char* result = new char[sz];
	int idx = 0;

	for (int i = 0; i < table.size(); i++)
	{
		char* tmp = (char*)&table[i]->section_info;

		for (int j = 0; j < SIZE_OF_ELF_SHDR; j++)
		{
			result[idx++] = tmp[j];
		}
	}

	return result;
}