#include "symbol_table.h"
#include <algorithm>
#include "string_table.h"
#include "Label.h"
#include "Parser.h"
#include "section_table.h"

using namespace std;

symbol_table::symbol_table(section_table* sc_tbl)
{
	str_table = new string_table(".strtab");
	section_info.sh_link = sc_tbl->put(str_table, str_table->get_name());

	section_info.sh_addr = 0;
	section_info.sh_entsize = SYMBOL_ENTRY_SIZE;
	section_info.sh_flags = SHF_NONE;
	section_info.sh_type = SHT_SYMTAB;
	section_info.sh_name = 0;
	section_info.sh_offset = 0;
	section_info.sh_size = 0;
	section_info.sh_info = 0;
	section_info.sh_link = 0;

	this->name = ".symtab";
	sc_tbl->put(this, ".symtab");
}


symbol_table::~symbol_table()
{
	delete str_table;
}

symbol_table::symbol_table()
{
	str_table = nullptr;
}

Elf_Word symbol_table::get_symbol_value(std::string name)
{
	int idx = exist(name);

	if (idx != -1)
	{
		return table[idx].symbol.st_value;
	}

	return 0;
}

Elf_Sym symbol_table::get_symbol(std::string name)
{
	Elf_Word idx = exist(name);
	return table[idx].symbol;
}


void symbol_table::put(Elf_Sym symbol, std::string name, bool force_overwrite)
{
	//TODO rethink
	int idx = exist(name);
	symbol_container sym(name, symbol);

	if (idx == -1)
	{
		if (is_global(symbol))
		{
			sym.symbol.st_name = str_table->put(name);
		}

		section_info.sh_size += SYMBOL_ENTRY_SIZE;
		table.push_back(sym);
	}
	else
	{
		if (force_overwrite)
		{
			if (is_global(symbol))
			{
				//we only put global symbols in strtable
				table[idx].symbol.st_name = str_table->put(name);
				//set to global
				table[idx].symbol.st_info &= 0x0F;
				table[idx].symbol.st_info |= STB_GLOBAL;
			}
			else
			{
				table[idx].symbol.st_info &= 0xF0;
				table[idx].symbol.st_info |= (symbol.st_info & 0x0F);
				table[idx].symbol.st_value = symbol.st_value;
				table[idx].symbol.st_shndx = current_section;
			}
		}
	}
}

bool symbol_table::is_global(Elf_Sym& sym)
{
	return ((sym.st_info & STB_GLOBAL) >> 4) != 0;
}

bool symbol_table::is_local(Elf_Sym& sym)
{
	return !(is_global(sym));
}

void symbol_table::erase_locals()
{
	for (int i = 0; i < table.size(); i++)
	{
		if (is_local(table[i].symbol))
		{
			table.erase(table.begin() + i);
			i--;
			section_info.sh_size -= SYMBOL_ENTRY_SIZE;
		}
	}
}

void symbol_table::print()
{
	/*for (int i = 0; i < table.size(); i++)
	{
		cout << table[i].st_name << " " <<
			str_table->get(table[i].st_name) << " " << table[i].st_value << " " << table[i].st_shndx <<
			endl;
	}*/
}

int symbol_table::exist(std::string name)
{
	for (int i = 0; i < table.size(); i++)
	{
		if (name == table[i].name)
		{
			return i;
		}
	}

	return -1;
}

char* symbol_table::serialize()
{
	char * result = new char[SYMBOL_ENTRY_SIZE * table.size()];
	int idx = 0;

	for (unsigned int i = 0; i < table.size(); i++)
	{
		char* tmp = (char*) &table[i].symbol;

		for (int j = 0; j < SYMBOL_ENTRY_SIZE; j++)
		{
			result[idx++] = tmp[j];
		}
	}

	return result;
}

void symbol_table::write_to_string_table()
{
	for (int i = 0; i < table.size(); i++)
	{
		table[i].symbol.st_name = str_table->put(table[i].name);
	}
}

Elf_Word symbol_table::count_glob_before(std::string)
{
	Elf_Word cnt = 0;
	for (int i = 0; i < table.size(); i++)
	{
		if (is_global(table[i].symbol))
		{
			if (table[i].name == name)
			{
				return cnt;
			}
			else
			{
				cnt++;
			}
		}
	}

	return -1;
}