#include "section_directive.h"
#include "typies.h"
#include "section_table.h"
#include "typies.h"
#include "Command.h"
#include "symbol_table.h"
#include "prog_section.h"
#include "reallocation_table.h"

section_directive::section_directive(std::string directive)
{
	this->directive = directive;
}


section_directive::~section_directive()
{
}

void section_directive::first_pass()
{
	section_table* sc_tbl = parser->get_section_table();
	string subsection = directive;
	current_section_start_addr = parser->get_LC();
	Elf_Shdr sc;
	sc.sh_addr = 0;
	sc.sh_offset = 0;
	sc.sh_type = SHT_PROGBITS;
	sc.sh_size = 0;
	sc.sh_link = 0;
	sc.sh_info = 0;

	if (directive.find(TEXT_SECTION) != std::string::npos)
	{
		directive = directive.substr(0, TEXT_SECTION.size());
		sc.sh_entsize = 0;
		sc.sh_flags = SHF_EXECINSTR | SHF_ALLOC;
	}
	else if (directive.find(BSS_SECTION) != std::string::npos)
	{
		directive = directive.substr(0, BSS_SECTION.size());
		sc.sh_entsize = 0;
		sc.sh_flags = SHF_WRITE | SHF_ALLOC;
	}
	else if (directive.find(DATA_SECTION) != std::string::npos)
	{
		directive = directive.substr(0, DATA_SECTION.size());
		sc.sh_flags = SHF_WRITE | SHF_ALLOC;
		sc.sh_entsize = 0;
	}
	else
	{
		return;
	}
	

	if (sc_tbl->contains(subsection))
	{
		current_section = sc_tbl->get_section_id(subsection);
	}
	else
	{
		reallocation_table * r_tbl = new reallocation_table(subsection);
		section * _section = new prog_section(sc, subsection, r_tbl);
		current_section = sc_tbl->put(_section, subsection);
		r_tbl->set_info(current_section);
	}

	if (!parser->get_symbol_table()->exist(subsection))
	{
		Elf_Sym subsec;
		subsec.st_info = STT_SECTION | STB_GLOBAL;
		subsec.st_shndx = current_section;
		subsec.st_size = 4;
		subsec.st_value = parser->get_section_table()->get_current_section()->get_LC();
		parser->get_symbol_table()->put(subsec, subsection, true);
	}
}

void section_directive::second_pass()
{
	section_table* sc_tbl = parser->get_section_table();
	current_section = sc_tbl->get_section_id(directive);
}