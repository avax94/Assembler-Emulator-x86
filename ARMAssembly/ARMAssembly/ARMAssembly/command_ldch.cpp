#include "command_ldch.h"
#include "Entity.h"
#include "Parser.h"
#include <vector>
#include "invalid_syntax_exception.h"
#include <typeinfo>
#include <string>
#include "symbol_table.h"
#include "command_argument.h"
#include "symbol_argument.h"
#include "typies.h"
#include "reallocation_table.h"

using namespace std;

command_ldch::command_ldch(std::string command, bool flag, char condition) : Command(command, flag, condition)
{
}


command_ldch::~command_ldch()
{
}

void command_ldch::first_pass()
{
	args = parser->get_arguments();
	parser->increment_LC(COMMAND_SIZE);

	string m = "Invalid arguments at line: " + to_string(parser->get_line_number());
	char * message = new char[m.size() + 1];
	strcpy(message, m.c_str());

	if (args.size() != 2)
	{
		throw invalid_syntax_exception(message);
	}

	string arg1 = typeid(*args[0]).name();
	string arg2 = typeid(*args[1]).name();
	
	if (arg1 != "class register_argument" || (arg2 != "class symbol_argument" && arg2 != "class immediate_argument"))
	{
		throw invalid_syntax_exception(message);
	}

	if (command == "ldc")
	{
		if (arg1 != "class register_argument" || (arg2 != "class symbol_argument" && arg2 != "class immediate_argument"))
		{
			throw invalid_syntax_exception(message);
		}

		parser->increment_LC(COMMAND_SIZE);
	}
	else
	{
		if (arg1 != "class register_argument" || arg2 != "class immediate_argument")
		{
			throw invalid_syntax_exception(message);
		}
	}

	for (int i = 0; i < args.size(); i++)
	{
		args[i]->first_pass();
	}
}

void command_ldch::second_pass()
{
	section* sc = parser->get_section_table()->get_current_section();
	reallocation_table* rlc_t = sc->get_realloc_t();
	args = parser->get_arguments();
	parser->increment_LC(COMMAND_SIZE);
	int reg_idx = Command::parse_register(args[0]->get_value());

	Elf_Byte first = LDCH_L_C | (condition << 5) | flag << 4;
	Elf_Byte second = 0;
	Elf_Byte third = 0;
	Elf_Byte fourth = 0;

	if (command == "ldc")
	{
		Elf_Word value = stoul(args[1]->get_value());
		Elf_Half lower_bits = (Elf_Half)(value & CLEAR_MASH_H);
		Elf_Half high_bits = (Elf_Half)(value >> 16);
		second = reg_idx << 4;

		Elf_Word first_command = Command::generate_instruction(first, second, 0, lower_bits);
		
		second = reg_idx << 4 | (1 << 3);
		Elf_Word second_command = Command::generate_instruction(first, second, 0, high_bits);

		sc->put(first_command);
		sc->put(second_command);

		string arg = typeid(*args[1]).name();

		if (arg == "class symbol_argument")
		{
			string sym_name = ((symbol_argument*)args[1])->get_name();
			Elf_Sym sym = Command::get_symbol(sym_name);

			Elf_Rel f_rel;
			Elf_Rel s_rel;

			f_rel.rt_info = s_rel.rt_info = 0;

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

			f_rel.rt_offset = sc->get_LC() - 4;
			f_rel.rt_info |= R_386_16_L;
			f_rel.rt_carry_dependent = 0;

			parser->increment_LC(COMMAND_SIZE);

			s_rel.rt_offset = sc->get_LC() - 4;
			s_rel.rt_info |= R_386_16_H;
			s_rel.rt_carry_dependent = rlc_t->put(f_rel);

			rlc_t->put(s_rel);
		}
		else
		{
			parser->increment_LC(COMMAND_SIZE);
		}
	}
	else
	{
		int imm_value = stoi(args[1]->get_value());

		if ((imm_value >> 16) > 0)
		{
			string m = "Warning: argument at line: " + to_string(parser->get_line_number()) + " to big";
			cout << m;
		}

		if (command == LDCH)
		{
			second = reg_idx << 4 | (1 << 3);
				Elf_Word instr = Command::generate_instruction(first, second, 0, imm_value >> 16);
				sc->put(instr);
		}
		else
		{
			second = reg_idx << 4;
			Elf_Word instr = Command::generate_instruction(first, second, 0, imm_value & CLEAR_MASH_H);
			sc->put(instr);
		}
	}
}