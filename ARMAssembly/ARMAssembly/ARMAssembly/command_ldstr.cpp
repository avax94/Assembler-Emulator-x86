#include "command_ldstr.h"
#include "Entity.h"
#include "Parser.h"
#include <vector>
#include "invalid_syntax_exception.h"
#include <typeinfo>
#include <string>
#include "typies.h"

using namespace std;

command_ldstr::command_ldstr(std::string command, bool flag, char condition, int f) : Command(command, flag, condition)
{
	this->f = f;
}


command_ldstr::~command_ldstr()
{
}


void command_ldstr::first_pass()
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

	if (arg1 != "class register_argument" || (arg2 != "class reg_ind_argument" && arg2 != "class reg_imm_ind"))
	{
		throw invalid_syntax_exception(message);
	}

	for (int i = 0; i < args.size(); i++)
	{
		args[i]->first_pass();
	}
}

//TODO args
void command_ldstr::second_pass()
{
	parser->increment_LC(COMMAND_SIZE);
	section *sc = parser->get_section_table()->get_current_section();
	args = parser->get_arguments();
	Elf_Word instruction;

	int reg_dst = Command::parse_register(args[0]->get_value());
	int reg_src = Command::parse_register(args[1]->get_value());
	int imm_val = Command::parse_reg_imm_value(args[1]->get_value());;

	imm_val = Command::shrink_value(imm_val, 10);

	Elf_Word first = (condition << 5) | flag << 4;
	int l_s = 0;

	if (command == LDR) { first |= LDR_C; l_s = 1; }
	else { first |= STR_C; }

	Elf_Byte second = reg_src << 3 | reg_dst >> 2;
	Elf_Byte third = ((reg_dst << 6) & F_BYTE_MASK) | (f << 3) | (l_s << 2) | imm_val >> 8;
	Elf_Byte fourth = imm_val & F_BYTE_MASK;

	Elf_Word instr = Command::generate_instruction(first, second, third, fourth);
	
	sc->put(instr);
}