#include "command_call.h"
#include "Entity.h"
#include "Parser.h"
#include <vector>
#include "symbol_table.h"
#include "invalid_syntax_exception.h"
#include "reallocation_table.h"
#include <typeinfo>
#include "symbol_argument.h"
#include "Label.h"
#include <string>
#include "typies.h"
#include "command_argument.h"
#include "symbol_table.h"

using namespace std;

command_call::command_call(std::string command, bool flag, char condition) : Command(command, flag, condition)
{
}


command_call::~command_call()
{
}

void command_call::first_pass()
{
	parser->increment_LC(COMMAND_SIZE);

	args = parser->get_arguments();

	string m = "Invalid arguments at line: " + to_string((unsigned int) parser->get_line_number());
	char * message = new char[m.size() + 1];
	strcpy(message, m.c_str());

	if (args.size() != 1)
	{
		throw invalid_syntax_exception(message);
	}

	string arg = typeid(*args[0]).name();
	
	if (arg != "class symbol_argument" && arg != "class immediate_argument")
	{
		throw invalid_syntax_exception(message);
	}
	
	for (int i = 0; i < args.size(); i++)
	{
		args[i]->first_pass();
	}

	section *sc = parser->get_section_table()->get_current_section();
	int val = stoi(args[0]->get_value());
	//todo check border
	if (!Command::can_shrink(val - sc->get_LC(), 19) ||
		(arg == "class symbol_argument" && (
		symbol_table::is_global(Command::get_symbol(((symbol_argument*)args[0])->get_name())) || Command::get_symbol(((symbol_argument*)args[0])->get_name()).st_shndx != current_section)))
	{
		parser->increment_LC(2 * COMMAND_SIZE);
	}


}
void command_call::second_pass()
{
	parser->increment_LC(COMMAND_SIZE);
	reallocation_table* realloc_t = parser->get_section_table()->get_current_section()->get_realloc_t();
	section *sc = parser->get_section_table()->get_current_section();
	args = parser->get_arguments();
	string arg = typeid(*args[0]).name();
	int val = stoi(args[0]->get_value());
	Elf_Word first = 0, second = 0, third = 0, fourth = 0;
	int reg = 0x10;
	Elf_Rel s_rel;

	//todo check
	if (!Command::can_shrink(val-sc->get_LC(), 19) || 
		(arg == "class symbol_argument" && (!parser->seen_label(((symbol_argument*)args[0])->get_name()) ||
	    Command::get_symbol(((symbol_argument*)args[0])->get_name()).st_shndx != current_section)))
	{
		//TODO think
		reg = 0xF;
		parser->increment_LC(COMMAND_SIZE);

		//LDCH r15, page(val)	
		fourth = val >> 16;
		first = LDCH_L_C | (condition << 5) | flag << 4;
		second = reg << 3 | (1 << 3);
		Elf_Word inst = Command::generate_instruction(first, second, third, fourth);
		sc->put(inst);

		parser->increment_LC(COMMAND_SIZE);
		second = reg << 3;
		val = val & CLEAR_MASH_H;
		inst = Command::generate_instruction(first, second, third, val);
		sc->put(inst);

		if (arg == "class symbol_argument")
		{
			Elf_Sym sym = Command::get_symbol(((symbol_argument*)args[0])->get_name());
			s_rel.rt_info = 0;
			
			Elf_Rel f_rel;
			f_rel.rt_info = 0;
			f_rel.rt_offset = sc->get_LC() - 4;
			f_rel.rt_carry_dependent = 0;

			if (symbol_table::is_global(sym))
			{
				s_rel.rt_info |= sym.st_name | RT_SYMBOL;
				f_rel.rt_info |= sym.st_name | RT_SYMBOL;
			}
			else
			{
				s_rel.rt_info |= sym.st_shndx | RT_SECTION;
				f_rel.rt_info |= sym.st_shndx | RT_SECTION;
			}

			s_rel.rt_offset = sc->get_LC() - 8;
			s_rel.rt_info |= R_386_16_H;
			s_rel.rt_carry_dependent;
		
			s_rel.rt_carry_dependent = realloc_t->put(f_rel);
			realloc_t->put(s_rel);
		}

		val = 0;
	}

	first = CALL_C | (condition << 5) | flag << 4;
	second = reg << 3 | (val >> 16);
	Elf_Word inst = Command::generate_instruction(first, second, third, val & CLEAR_MASH_H);
	sc->put(inst);
}
