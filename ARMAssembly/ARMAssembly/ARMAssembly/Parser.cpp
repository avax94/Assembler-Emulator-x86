#include "Parser.h"
#include "File.h"
#include "Label.h"
#include "Command.h"
#include <iostream>
#include <regex>
#include "global_directive.h"
#include "section_directive.h"
#include "type_directive.h"
#include "prog_section.h"
#include "command_int.h"
#include "command_call.h"
#include "reg_imm_ind.h"
#include <fstream>
#include "command_shift.h"
#include "reg_ind_argument.h"
#include "command_io.h"
#include "command_ldch.h"
#include "command_jmp.h"
#include "invalid_com_dir_exception.h"
#include "command_log.h"
#include "command_mov.h"
#include "command_ldstr.h"
#include "register_argument.h"
#include "immediate_argument.h"
#include "command_op.h"
#include "symbol_argument.h"
#include "Command.h"
#include "Directives.h"
#include "symbol_table.h"
#include "reallocation_table.h"
#include "typies.h"
#include "invalid_syntax_exception.h"
#include "section.h"

using namespace std;

Parser * parser;


Parser::Parser(File* file)
{
	line = "";
	this->file = file;
	line_number = 0;
	sec_table = new section_table();
	
	Elf_Shdr dummy_sec;
	dummy_sec.sh_addr = 0;
	dummy_sec.sh_entsize = 0;
	dummy_sec.sh_flags = 0;
	dummy_sec.sh_info = 0;
	dummy_sec.sh_link = 0;
	dummy_sec.sh_name = 0;
	dummy_sec.sh_offset = 0;
	dummy_sec.sh_size = 0;
	dummy_sec.sh_type = 0;
	section* sec = new prog_section(dummy_sec, ".none", nullptr);
	sec_table->put(sec, ".none");
	sec_table->put_str_tbl();
	sym_table = new symbol_table(sec_table);
	
}


Parser::~Parser()
{
	delete sec_table;
	delete realloc_table;
	delete sym_table;
}

section_table* Parser::get_section_table() const
{
	return sec_table;
}

void Parser::getLine()
{
	regex tabs("[,\t]");
	regex empty_line("^[ \t]*$");
	regex replace_sufficent_spaces(" +(?=( |:))| +(?=\\+)| +(?=\\])");
	regex replace_bracket("\\[ ");
	regex replace_plus("\\+ ");
	regex replace_beginning("^ +| $");
	
	line = file->readLine();
	++line_number;

	while ((regex_match(line, empty_line) || line=="") && !file->eof())
	{
		line = file->readLine();
		++line_number;
	}

	//tabs to spaces
	line = regex_replace(line, tabs, " ");
	//Lets format this nicely - only single spaces no tabs and no space after label name and :
	line = regex_replace(line, replace_sufficent_spaces, "");
	line = regex_replace(line, replace_bracket, "[");
	line = regex_replace(line, replace_plus, "+");
	line = regex_replace(line, replace_beginning, "");
	//everything to lower;
	for (unsigned int i = 0; i < line.size(); i++)
	{
		if (line[i] >= 'A' && line[i] <= 'Z')
		{
			line[i] += -'A' + 'a';
		}
	}

	index = 0;
}
//TODO edit to be much fasterd
Entity* Parser::get_next()
{
	string entity = "";

	if (index >= line.size())
	{
		getLine();
	}

	while (index != line.size() && line[index] != ' ')
	{
		entity += line[index++];
	}
	
	if (entity == "" && file->eof())
	{
		return nullptr;
	}

	index++;

	return generate_entity(entity);
}
//TODO finish this
Command* Parser::generate_command(string command)
{
	string message = "Expected command/directive/label but " + command + " found, at line: " + std::to_string(line_number);
	char * m = new char[message.size() + 1];
	strcpy(m, message.c_str());
	bool flag = false;

	if (command[0] == 'f')
	{
		flag = true;
		command = command.substr(1);
	}

	char condition = calculate_command_condition(command.substr(command.size() - 2, 2));
	
	//TODO throw
	if (condition == UD) { throw invalid_com_dir_exception(m); }

	command = command.substr(0, command.size() - 2);

	if (command == INT)
	{
		return new command_int(command, flag, condition);
	}
	else if (command == ADD || command == SUB || command == MUL || command == DIV || command == CMP)
	{
		return new command_op(command, flag, condition);
	}
	else if (command.find(LDR) != std::string::npos || command.find(STR) != std::string::npos)
	{
		int f = generate_incdec(command.substr(command.size() - 2, 2));
		
		command = command.substr(0, command.size() - 2);

		if (f == UD)
		{
			throw invalid_com_dir_exception(m);
		}

		return new command_ldstr(command, flag, condition, f);
	}
	else if (command == CALL)
	{
		return new command_call(command, flag, condition);
	}
	else if (command == IN || command == OUT)
	{
		return new command_io(command, flag, condition);
	}
	else if (command == MOV)
	{
		return new command_mov(command, flag, condition);
	}
	else if (command == SHR || command == SHL)
	{
		return new command_shift(command, flag, condition);
	}
	//TODO fix
	else if (command == LDCH || command == LDCL || command == "ldc")
	{
		return new command_ldch(command, flag, condition);
	}
	else if (command == JMP)
	{
		return new command_jmp(command, flag, condition);
	}
	else
	{
		throw invalid_com_dir_exception(m);
	}
}

unsigned int Parser::get_line_number() const
{
	return line_number;
}

vector<Entity*> Parser::get_arguments()
{
	vector<Entity*> result;

	//iterate till end of line index = 0 indicates new line
	while (index < line.size())
	{
		result.push_back(get_next());
	}

	return result;
}

Entity* Parser::generate_entity(string entity)
{
	regex command(".*add.*|.*sub.*|.*mul.*|.*div.*|.*cmp.*|.*int.*|.*and.*|.*or.*|.*not.*|.*test.*|.*ldr.*|.*str.*|.*call.*|.*in.*|.*out.*|.*mov.*|.*shr.*|.*shl.*|.*ldch.*|.*ldcl.*|.*ldc.*|.*jmp.*|.*push.*|.*pop.*");
	regex reg("^r([0-9]|1[0-5])$");
	regex reg_ind("\\[r([0-9]|1[0-5])\\]");
	regex reg_imm_ind_r("[0-9]+\\+\\[r([0-9]|1[0-5])\\]|\\[r([0-9]|1[0-5])\\]\\+[0-9]+|\\[r([0-9]|1[0-5])\\+[0-9]+\\]|\\[[0-9]+\\+r([0-9]|1[0-5])\\]");
	regex number("[0-9]+");
	if (active_command)
	{
		if (regex_match(entity, reg))
		{
			//TODO FINISH THIS
			return new register_argument(entity);
		}
		else if (regex_match(entity, reg_ind))
		{
			return new reg_ind_argument(entity);
		}
		else if (regex_match(entity, number))
		{
			return new immediate_argument(std::stoll(entity));
		}
		else if (regex_match(entity, reg_imm_ind_r))
		{
			return new reg_imm_ind(entity);
		}
		else if (!regex_match(entity, command) && entity[entity.size() - 1] != ':' && entity[0] != '.')
		{
			return new symbol_argument(entity);
		}
		else
		{
			string message = "Invalid syntax at line: " + std::to_string(line_number);
			char * m = new char[message.size() + 1];
			strcpy(m, message.c_str());
			throw invalid_com_dir_exception(m);
		}
	}
	else
	{
		if (regex_match(entity, command))
		{
			return generate_command(entity);
		}
		else if (entity[0] == '.')
		{
			return generate_directive(entity);
		}
		else if (entity[entity.size() - 1] == ':')
		{
			return new Label(entity, get_section_table()->get_current_section()->get_LC());
		}
		else
		{
			string message = "Expceted command\\label\\direcite at line: " + std::to_string(line_number) + " but " + entity + " found";
			char * m = new char[message.size() + 1];
			strcpy(m, message.c_str());
			throw invalid_com_dir_exception(m);
		}
	}
	return nullptr;
}

Directives* Parser::generate_directive(string directive)
{
	regex type_d(".char|.word|.long|.align|.skip");
	regex section_d("^.text.*|^.data.*|^.bss|^.end$");
	regex global_d("^.global$");

	if (regex_match(directive, type_d))
	{
		return new type_directive(directive);
	}
	else if (regex_match(directive, section_d))
	{
		return new section_directive(directive);
	}
	else if (regex_match(directive, global_d))
	{
		return new global_directive();
	}
	else
	{
		//error
		string message = "Invalid directive " + directive + " at line: "  + std::to_string(line_number);
		char * m = new char[message.size() + 1];
		strcpy(m, message.c_str());
		throw invalid_com_dir_exception(m);
	}
}

void Parser::initialize_for_second_pass()
{
	//initalize file
	const char* fname = file->get_name();
	delete file;
	file = new File(fname, true);

	//reset LC
	location_counter = 0;
	sec_table->reset_LCs();

	current_section_start_addr = 0;
	current_section = 0;

	line_number = 0;
}

bool Parser::seen_label(std::string label)
{
	return passed_labels.find(label) != passed_labels.end();
}

void Parser::insert_label(std::string label)
{
	passed_labels.insert(label);
}

char Parser::calculate_command_condition(string cd)
{
	if (cd == "eq")
	{
		return EQ;
	}
	else if (cd == "ne")
	{
		return NE;
	}
	else if (cd == "gt")
	{
		return GT;
	}
	else if (cd == "ge")
	{
		return GE;
	}
	else if (cd == "lt")
	{
		return LT;
	}
	else if (cd == "le")
	{
		return LE;
	}
	else if (cd == "hi")
	{
		return HI;
	}
	else if (cd == "hs")
	{
		return HS;
	}
	else
	{
		return UD;
	}
}

int Parser::generate_incdec(string cd)
{
	if (cd == "ia")
	{
		return IA;
	}
	else if (cd == "ib")
	{
		return IB;
	}
	else if (cd == "db")
	{
		return DB;
	}
	else if (cd == "da")
	{
		return DA;
	}
	else
	{
		return UD;
	}
}

int main()
{
 
	File* file = new File("C:\\Users\\stefa\\Desktop\\test.as", true);
	parser = new Parser(file);
	Entity* tmp = nullptr;

	while (true)
	{
		try{
			tmp = parser->get_next();
			if (tmp == nullptr)
				break;
			tmp->first_pass();
			delete tmp;
		}
		catch (invalid_com_dir_exception& e)
		{
			cout << e.what() << endl;
		}
		catch (invalid_syntax_exception& e)
		{
			delete tmp;
			cout << e.what() << endl;
		}
	}

	parser->initialize_for_second_pass();

	while (true)
	{
		try{
			tmp = parser->get_next();
			if (tmp == nullptr)
				break;
			tmp->second_pass();
			delete tmp;
		}
		catch (invalid_com_dir_exception& e)
		{
			cout << e.what() << endl;
		}
		catch (invalid_syntax_exception& e)
		{
			delete tmp;
			cout << e.what() << endl;
		}
	}
	parser->get_symbol_table()->erase_locals();
	File* f = new File("C:\\Users\\stefa\\Desktop\\dv.o", false);
	/*prog_section* p = (prog_section*) parser->get_section_table()->get_section(".text");
	p->print(f);
	delete f;
	*/
	//
	parser->generate_output(f, true);
	parser->generate_output("C:\\Users\\stefa\\Desktop\\bdv.o");

	system("pause");
}

symbol_table* Parser::get_symbol_table() const
{
	return sym_table;
}

void Parser::increment_LC(unsigned int inc)
{
	sec_table->increment_LC(inc);
	location_counter += inc;
}

unsigned int Parser::get_LC() const
{
	return location_counter;
}

void Parser::generate_output(File* f, bool execution)
{
	Elf_Ehdr e_hdr;
	e_hdr.e_type = ET_REL;
	e_hdr.e_entry = 0;
	e_hdr.e_phoff = 0;
	e_hdr.e_flags = 0;
	e_hdr.e_ehsize = SIZE_OF_ELF_EHDR;
	e_hdr.e_phentsize = 0;
	e_hdr.e_phnum = 0;
	e_hdr.e_shoff = 0; //TODO later
	e_hdr.e_shentsize = SIZE_OF_ELF_SHDR;
	e_hdr.e_shnum = sec_table->get_section_number();
	e_hdr.e_shstrndx = sec_table->get_section_id(".shstrtab");
	
	Elf_Word size = SIZE_OF_ELF_EHDR;
	
	
	for (int i = 0; i < sec_table->get_section_number(); i++)
	{
		section * s = sec_table->get_section(i);
		s->set_section_offset(size);
		size += s->get_section_size();
	}

	e_hdr.e_shoff = size;
	
	char* tmp = new char[SIZE_OF_ELF_EHDR];
	_serialize(tmp, e_hdr);
	f->write(tmp, SIZE_OF_ELF_EHDR);
	delete[]tmp;
	f->write('\n');

	for (int i = 0; i < sec_table->get_section_number(); i++)
	{
		section * s = sec_table->get_section(i);
		char* t = s->serialize();
		f->write(s->get_name() + " ");
		f->write(t, s->get_section_size());
		delete[]t;
		f->write('\n');
	}

	tmp = sec_table->serialize();
	f->write(tmp, sec_table->get_section_number() * SIZE_OF_ELF_SHDR);
	f->write('\n');
	delete f;
}

void Parser::generate_output(string f)
{
	ofstream file(f, std::ios::out | std::ios::binary);

	Elf_Ehdr e_hdr;
	e_hdr.e_type = ET_REL;
	e_hdr.e_entry = 0;
	e_hdr.e_phoff = 0;
	e_hdr.e_flags = 0;
	e_hdr.e_ehsize = SIZE_OF_ELF_EHDR;
	e_hdr.e_phentsize = 0;
	e_hdr.e_phnum = 0;
	e_hdr.e_shoff = 0; //TODO later
	e_hdr.e_shentsize = SIZE_OF_ELF_SHDR;
	e_hdr.e_shnum = sec_table->get_section_number();
	e_hdr.e_shstrndx = sec_table->get_section_id(".shstrtab");

	Elf_Word size = SIZE_OF_ELF_EHDR;


	for (int i = 0; i < sec_table->get_section_number(); i++)
	{
		section * s = sec_table->get_section(i);
		s->set_section_offset(size);
		size += s->get_section_size();
	}

	e_hdr.e_shoff = size;

	char* tmp = new char[SIZE_OF_ELF_EHDR];
	_serialize(tmp, e_hdr);
	file.write(tmp, SIZE_OF_ELF_EHDR);
	delete[]tmp;
	
	for (int i = 0; i < sec_table->get_section_number(); i++)
	{
		section * s = sec_table->get_section(i);
		char* t = s->serialize();
		file.write(t, s->get_section_size());
		delete[]t;
	}

	tmp = sec_table->serialize();
	file.write(tmp, sec_table->get_section_number() * SIZE_OF_ELF_SHDR);
}

