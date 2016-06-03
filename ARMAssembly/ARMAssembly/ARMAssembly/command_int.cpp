#include "command_int.h"
#include <string>
#include "Entity.h"
#include <vector>
#include "invalid_syntax_exception.h"
#include "Parser.h"
#include "typies.h"

using namespace std;

command_int::command_int(std::string command, bool flag, char condition) : Command(command, flag, condition)
{
}


command_int::~command_int()
{
}

void command_int::first_pass()
{
	parser->increment_LC(COMMAND_SIZE);

	args = parser->get_arguments();

	string m = "Invalid arguments at line: " + to_string(parser->get_line_number());
	char * message = new char[m.size() + 1];
	strcpy(message, m.c_str());

	if (args.size() != 1)
	{
		throw invalid_syntax_exception(message);
	}

	string arg = typeid(*args[0]).name();

	if (arg != "class immediate_argument")
	{
		throw invalid_syntax_exception(message);
	}

	for (int i = 0; i < args.size(); i++)
	{
		args[i]->first_pass();
	}
}

void command_int::second_pass()
{
	parser->increment_LC(COMMAND_SIZE);
	reallocation_table* realloc_t = parser->get_section_table()->get_current_section()->get_realloc_t();
	section *sc = parser->get_section_table()->get_current_section();
	args = parser->get_arguments();
	Elf_Byte val = stoi(args[0]->get_value());
	
	if (val >> 4 > 0)
	{
		throw invalid_syntax_exception("Argument too big!");
	}

	Elf_Byte first = INT_C | (condition << 5) | flag << 4;
	Elf_Byte second = val << 4;
	Elf_Word instr = Command::generate_instruction(first, second, 0, 0);
	sc->put(instr);
}
