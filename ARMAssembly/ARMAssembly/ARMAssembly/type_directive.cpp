#include "type_directive.h"
#include "Parser.h"
#include <typeinfo>
#include "Entity.h"
#include "typies.h"
#include "immediate_argument.h"
#include "invalid_syntax_exception.h"

type_directive::type_directive(std::string directive)
{
	this->directive = directive;
}


type_directive::~type_directive()
{
}

void type_directive::first_pass()
{
	vector<Entity*> args = parser->get_arguments();
	string m = "Invalid arguments at line: " + to_string((unsigned int)parser->get_line_number());
	char * message = new char[m.size() + 1];
	strcpy(message, m.c_str());

	//SKIP DIRECTIVE
	if (directive == ".skip")
	{
		if (args.size() != 1)
		{
			throw invalid_syntax_exception(message);
		}

		string arg1 = typeid(*args[0]).name();
		
		if (arg1 != "class immediate_argument")
		{
			throw invalid_syntax_exception(message);
		}

		Elf_Word skip = std::stoul(args[0]->get_value());
		args[0]->first_pass();
		parser->increment_LC(skip);

		delete args[0];
	}
	else if (directive == ".word")
	{
		if (args.size() == 0)
		{
			throw invalid_syntax_exception(message);
		}

		for (int i = 0; i < args.size(); i++)
		{
			string arg1 = typeid(*args[i]).name();

			if (arg1 != "class immediate_argument" && arg1 != "class symbol_argument")
			{
				throw invalid_syntax_exception(message);
			}

			args[i]->first_pass();
			delete args[i];
		}

		parser->increment_LC(args.size() * sizeof(Elf_Word));
	}
	else if (directive == ".long")
	{
		if (args.size() == 0)
		{
			throw invalid_syntax_exception(message);
		}

		for (int i = 0; i < args.size(); i++)
		{
			string arg1 = typeid(*args[i]).name();

			if (arg1 != "class immediate_argument" && arg1 != "class symbol_argument")
			{
				throw invalid_syntax_exception(message);
			}

			args[i]->first_pass();
			delete args[i];
		}

		parser->increment_LC(args.size() * 2 * sizeof(Elf_Word));
	}
	else if (directive == ".char")
	{
		if (args.size() == 0)
		{
			throw invalid_syntax_exception(message);
		}

		for (int i = 0; i < args.size(); i++)
		{
			string arg1 = typeid(*args[i]).name();

			if (arg1 != "class immediate_argument")
			{
				throw invalid_syntax_exception(message);
			}

			Elf_Word value = stoul(args[i]->get_value());

			if (value > (1 << 8) - 1)
			{
				throw invalid_syntax_exception(message);
			}

			args[i]->first_pass();
			delete args[i];
		}
	
		parser->increment_LC(args.size() * sizeof(Elf_Byte));
	}
	else if (directive == ".align")
	{
		//TODO alling this shit LCLCLCL
	}
}

void type_directive::second_pass()
{
	vector<Entity*> args = parser->get_arguments();
	section* sc = parser->get_section_table()->get_current_section();
	
	//SKIP DIRECTIVE
	if (directive == ".skip")
	{
		string arg1 = typeid(*args[0]).name();

		Elf_Word skip = std::stoul(args[0]->get_value());
		args[0]->second_pass();
		parser->increment_LC(skip);

		for (int i = 0; i < skip; i++)
		{
			sc->put((Elf_Byte)0);
		}

		delete args[0];
	}
	else if (directive == ".word")
	{
		for (int i = 0; i < args.size(); i++)
		{
			sc->put((Elf_Word)std::stoul(args[i]->get_value()));
			args[i]->second_pass();
			delete args[i];
		}

		parser->increment_LC(args.size() * sizeof(Elf_Word));
	}
	else if (directive == ".long")
	{
		for (int i = 0; i < args.size(); i++)
		{
			sc->put((Elf_Double)stoul(args[i]->get_value()));
			args[i]->second_pass();
			delete args[i];
		}

		parser->increment_LC(args.size() * 2 * sizeof(Elf_Word));
	}
	else if (directive == ".char")
	{
		for (int i = 0; i < args.size(); i++)
		{
			Elf_Byte value = stoul(args[i]->get_value());
			sc->put(value);
			args[i]->second_pass();
			delete args[i];
		}

		parser->increment_LC(args.size() * sizeof(Elf_Byte));
	}
	else if (directive == ".align")
	{
		//TODO alling this shit LCLCLCL
	}
}