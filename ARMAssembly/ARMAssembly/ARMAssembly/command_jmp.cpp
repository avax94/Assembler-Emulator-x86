#include "command_jmp.h"
#include "symbol_argument.h"
#include "invalid_syntax_exception.h"
#include "reallocation_table.h"
#include "symbol_table.h"

command_jmp::command_jmp(std::string command, bool flag, char condition) : Command(command, flag, condition)
{
}


command_jmp::~command_jmp()
{
}

void command_jmp::first_pass()
{
	args = parser->get_arguments();
	section *sc = parser->get_section_table()->get_current_section();
	Elf_Word val = stoul(args[0]->get_value());

	string m = "Invalid arguments at line: " + to_string((unsigned int)parser->get_line_number());
	char * message = new char[m.size() + 1];
	strcpy(message, m.c_str());

	if (args.size() != 1)
	{
		throw invalid_syntax_exception(message);
	}

	string arg = typeid(*args[0]).name();

	if (arg != "class symbol_argument")
	{
		throw invalid_syntax_exception(message);
	}
	
	parser->increment_LC(COMMAND_SIZE);
	
	for (int i = 0; i < args.size(); i++)
	{
		args[i]->first_pass();
	}

}

void command_jmp::second_pass()
{
	reallocation_table* realloc_t = parser->get_section_table()->get_current_section()->get_realloc_t();
	section *sc = parser->get_section_table()->get_current_section();
	args = parser->get_arguments();
	Elf_Word val = stoul(args[0]->get_value());

	Elf_Word first = 0, second = 0, third = 0, fourth = 0;
	int reg = 0x10;
	Elf_Rel s_rel;

	parser->increment_LC(COMMAND_SIZE);
	Elf_Word value = abs_diff(val, sc->get_LC());

	Elf_Sym sym = Command::get_symbol(((symbol_argument*)args[0])->get_name());
	
	if (value >(1 << 18) - 1 || !parser->seen_label(((symbol_argument*)args[0])->get_name()) || sym.st_shndx != current_section)
	{	//LDC r15, val
		//MOV pc, val
		reg = 0xF;

		//LDCH r15, page(val)	
		fourth = val >> 16;
		first = LDCH_L_C | (condition << 5) | flag << 4;
		second = reg << 3 | (1 << 3);
		Elf_Word inst = Command::generate_instruction(first, second, third, fourth);
		sc->put(inst);
		//LDCL r15, offset(val)
		parser->increment_LC(COMMAND_SIZE);
		second = reg << 3;
		val = val & CLEAR_MASH_H;
		inst = Command::generate_instruction(first, second, third, val);
		sc->put(inst);

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

		f_rel.rt_info |= R_386_16_L;

		s_rel.rt_offset = sc->get_LC() - 8;
		s_rel.rt_info |= R_386_16_H;
		s_rel.rt_carry_dependent;
		s_rel.rt_carry_dependent = realloc_t->put(f_rel);
		realloc_t->put(s_rel);

		//MOV
		int reg_dst = 0x10;
		first = MOV_SHR_SHL_C | (condition << 5) | flag << 4;
		second = reg_dst << 3 | reg >> 2;
		third = (reg << 6) & F_BYTE_MASK;

		Elf_Word instr = Command::generate_instruction(first, second, third, 0);
		sc->put(instr);
	}
	else //ADD PC, value
	{
		first = 0;
		if (val > sc->get_LC())
		{//ADD
			first |= ADD_C;
		}
		else
		{//SUB
			first |= SUB_C;
		}

		second = 0x10 << 3 | 1 << 2;
		second |= value >> 16;
		Elf_Word fourth = value & CLEAR_MASH_H;
		Elf_Word instruction = Command::generate_instruction(first, second, 0, fourth);
		
		sc->put(instruction);
	}
}
