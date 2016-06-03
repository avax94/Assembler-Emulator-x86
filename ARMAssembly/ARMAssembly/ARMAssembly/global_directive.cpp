#include "global_directive.h"
#include <vector>
#include <typeinfo>
#include "Parser.h"
#include "invalid_syntax_exception.h"
#include "symbol_argument.h"
#include "typies.h"
#include <string>

using namespace std;

global_directive::global_directive()
{
}


global_directive::~global_directive()
{
}

void global_directive::first_pass()
{
	vector<Entity*> args = parser->get_arguments();

	string m = "Invalid arguments at line: " + to_string(parser->get_line_number());
	char * message = new char[m.size() + 1];
	strcpy(message, m.c_str());

	for (int i = 0; i < args.size(); i++)
	{
		string arg = typeid(*args[i]).name();
		
		if (arg != "class symbol_argument")
		{
			throw invalid_syntax_exception(message);
		}

		((symbol_argument*)args[i])->set_flag(STB_GLOBAL | STT_UNDEFINED);
		args[i]->first_pass();
		delete args[i];
	}
}

void global_directive::second_pass()
{
	vector<Entity*> args = parser->get_arguments();
	
	for (int i = 0; i < args.size(); i++)
	{
		delete args[i];
	}
}