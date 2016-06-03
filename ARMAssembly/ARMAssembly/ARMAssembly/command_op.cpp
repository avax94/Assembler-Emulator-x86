#include "command_op.h"
#include "Entity.h"
#include "Parser.h"
#include <vector>
#include "invalid_syntax_exception.h"
#include <typeinfo>
#include <string>
#include "typies.h"

using namespace std;

command_op::command_op(std::string command, bool flag, char condition) : Command(command, flag, condition)
{
}


command_op::~command_op()
{
}

void command_op::first_pass()
{
	parser->increment_LC(COMMAND_SIZE);
	args = parser->get_arguments();

	string m = "Invalid arguments at line: " + to_string(parser->get_line_number());
	char * message = new char[m.size() + 1];
	strcpy(message, m.c_str());

	if (args.size() != 2)
	{
		throw invalid_syntax_exception(message);
	}

	string arg1 = typeid(*args[0]).name();
	string arg2 = typeid(*args[1]).name();

	if (arg1 != "class register_argument" || (arg2 != "class register_argument" && arg2 != "class immediate_argument"))
	{
		throw invalid_syntax_exception(message);
	}

	for (int i = 0; i < args.size(); i++)
	{
		args[i]->first_pass();
	}

}

void command_op::second_pass()
{
	parser->increment_LC(COMMAND_SIZE);
	section *sc = parser->get_section_table()->get_current_section();
	args = parser->get_arguments();
	Elf_Word instruction;

	int reg_dst = Command::parse_register(args[0]->get_value());
	string arg = typeid(*args[1]).name();
	Elf_Byte param = arg == "class immediate_argument";

	Elf_Word first = (condition << 5) | flag << 4;

	if (command == ADD) { first |= ADD_C; }
	else if (command == SUB) { first |= SUB_C; }
	else if (command == MUL) { first |= MUL_C; }
	else if (command == DIV) { first |= DIV_C; }
	else if (command == CMP) { first |= CMP_C; }

	Elf_Word second = reg_dst << 3 | param << 2;

	if (param == 1)
	{
		int val = stoi(args[1]->get_value());
		val = Command::shrink_value(val, 18);

		second |= val >> 16;
		Elf_Word fourth = val & CLEAR_MASH_H;

		instruction = Command::generate_instruction(first, second, 0, fourth);
	}
	else
	{
		int reg_src = Command::parse_register(args[1]->get_value());
		second |= reg_src >> 3;
		Elf_Word third = (reg_src << 5) & F_BYTE_MASK;
		instruction = Command::generate_instruction(first, second, third, 0);
	}

	sc->put(instruction);
}