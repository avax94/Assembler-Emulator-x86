#include "command_log.h"
#include "Entity.h"
#include "Parser.h"
#include <vector>
#include "invalid_syntax_exception.h"
#include <typeinfo>
#include <string>
#include "typies.h"

using namespace std;

command_log::command_log(std::string command, bool flag, char condition) : Command(command, flag, condition)
{
}


command_log::~command_log()
{
}


void command_log::first_pass()
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

	if (arg1 != "class register_argument" || arg2 != "class register_argument")
	{
		throw invalid_syntax_exception(message);
	}

	for (int i = 0; i < args.size(); i++)
	{
		args[i]->first_pass();
	}
}

void command_log::second_pass()
{
	parser->increment_LC(COMMAND_SIZE);
	section *sc = parser->get_section_table()->get_current_section();
	args = parser->get_arguments();
	int reg_dst = Command::parse_register(args[0]->get_value());
	int reg_src = Command::parse_register(args[1]->get_value());

	Elf_Byte first = (condition << 5) | flag << 4;

	if (command == AND) { first |= AND_C; }
	else if (command == OR) { first |= OR_C; }
	else if (command == NOT) { first |= NOT_C; }
	else if (command == TEST) { first |= TEST_C; }
	else { throw invalid_syntax_exception("Unkown command!"); }

	Elf_Byte second = reg_dst << 3 | reg_src >> 2;
	Elf_Byte third = (reg_src << 6) & F_BYTE_MASK;

	Elf_Word instr = Command::generate_instruction(first, second, third, 0);
	sc->put(instr);
}