#include "command_io.h"
#include <vector>
#include <string>
#include "Entity.h"
#include "invalid_syntax_exception.h"
#include "typies.h"


using namespace std;

command_io::command_io(std::string command, bool flag, char condition) : Command(command, flag, condition)
{
}


command_io::~command_io()
{
}

void command_io::first_pass()
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

	if (arg1 != "class register_argument" || arg2 != "class reg_ind_argument")
	{
		throw invalid_syntax_exception(message);
	}

	for (int i = 0; i < args.size(); i++)
	{
		args[i]->first_pass();
	}
}

void command_io::second_pass()
{
	parser->increment_LC(COMMAND_SIZE);
	section *sc = parser->get_section_table()->get_current_section();
	args = parser->get_arguments();
	int reg_dst = Command::parse_register(args[0]->get_value());
	int reg_src = Command::parse_register(args[1]->get_value());
	Elf_Byte in = command == "in";

	Elf_Byte first = IN_OUT_C | (condition << 5) | flag << 4;
	Elf_Byte second = reg_dst << 4 | reg_src;
	Elf_Byte third = in << 7;

	Elf_Word instr = Command::generate_instruction(first, second, third, 0);
	sc->put(instr);
}