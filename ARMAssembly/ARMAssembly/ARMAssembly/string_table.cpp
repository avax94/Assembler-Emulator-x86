#include "string_table.h"
#include "Parser.h"


//TODO INHERIT SECTION
string_table::string_table(std::string name)
{
	this->name = name;
	idx = 0;
	section_info.sh_addr = 0;
	section_info.sh_entsize = 0;
	section_info.sh_flags = SHF_NONE;
	section_info.sh_info = 0;
	section_info.sh_link = 0;
	section_info.sh_name = 0;
	section_info.sh_offset = 0;
	section_info.sh_size = 0;
	section_info.sh_type = SHT_STRTAB;
}

string_table::~string_table()
{
}

Elf_Word string_table::put(std::string name)
{
	if (exist(name) != -1)
	{
		return exist(name);
	}

	Elf_Word result = idx;
	table.push_back(name);
	//+1 for null character
	table_size += name.size() + 1;
	section_info.sh_size = table_size;
	idx++;
	return result;
}

std::string string_table::get(unsigned int index)
{
	return table[index];
}

int string_table::byte_size() const
{
	return table_size;
}

char* string_table::serialize()
{
	int idx = 0;
	char* result = new char[table_size];

	for (int i = 0; i < table.size(); i++)
	{
		for (int j = 0; j < table[i].size(); j++)
		{
			result[idx++] = table[i][j];
		}

		result[idx++] = '\0';
	}

	return result;
}

int string_table::exist(std::string str)
{
	for (int i = 0; i < table.size(); i++)
	{
		if (str == table[i])
		{
			return i;
		}
	}

	return -1;
}

