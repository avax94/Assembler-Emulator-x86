#pragma once
#include <iostream>
#include <vector>
#include <string>
#include "section_table.h"
#include <set>

using namespace std;

class Command;
class File;
class Directives;
class Entity;
class symbol_table;
class reallocation_table;

class Parser
{
private:
	set<std::string> passed_labels;
	File* file;
	symbol_table * sym_table;
	reallocation_table * realloc_table;
	int line_number;
	unsigned int index;
	string line;
	unsigned int location_counter;
	section_table* sec_table;

	Directives * generate_directive(string directive);
	char calculate_command_condition(string cd);
	Entity* generate_entity(string entity);
	Command* generate_command(string command);
	int generate_incdec(string command);
public:
	void insert_label(std::string);
	bool seen_label(std::string);
	void initialize_for_second_pass();
	void increment_LC(unsigned int num);
	unsigned int get_LC() const;
	symbol_table* get_symbol_table() const;
	section_table* get_section_table() const;
	unsigned int get_line_number() const;
	Parser(File* file);
	~Parser();
	Entity* get_next();
	std::vector<Entity*> get_arguments();
	void getLine();
	
	void generate_output(File * f, bool exec);
	void generate_output(string f);
};

extern Parser* parser;