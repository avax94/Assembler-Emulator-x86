#include "reallocation_table.h"
#include <string>
#include "typies.h"
#include "section.h"
#include "Parser.h"

reallocation_table::reallocation_table(std::string name)
{
	this->name = ".rel" + name;

	section_info.sh_addr = 0;
	section_info.sh_entsize = SIZE_OF_ELF_REL;
	section_info.sh_flags = SHF_NONE;
	section_info.sh_type = SHT_REL;
	section_info.sh_link = parser->get_section_table()->get_section_id(".symtab");
	section_info.sh_info = 0;
	section_info.sh_name = 0;
	section_info.sh_offset = 0;
	section_info.sh_size = 0;
	parser->get_section_table()->put(this, this->name);
}


reallocation_table::~reallocation_table()
{
}

Elf_Byte reallocation_table::put(Elf_Rel r)
{
	section_info.sh_size += SIZE_OF_ELF_REL;
	table.push_back(r);
	return table.size() - 1;
}

void reallocation_table::set_info(Elf_Word st_info)
{
	section_info.sh_info = st_info;
}

char* reallocation_table::serialize()
{
	char * result = new char[SIZE_OF_ELF_REL * table.size()];
	int idx = 0;

	for (unsigned int i = 0; i < table.size(); i++)
	{
		char* tmp = (char*)&table[i];

		for (int j = 0; j < SIZE_OF_ELF_REL; j++)
		{
			result[idx++] = tmp[j];
		}
	}

	return result;
}