#include "Label.h"
#include "Parser.h"
#include "symbol_table.h"

Label::Label(std::string label_name, Elf_Addr value)
{
	label_name = label_name.substr(0, label_name.size() - 1);
	this->label_name = label_name;
	this->value = value;
}

Label::~Label()
{
}

void Label::first_pass()
{
	Elf_Sym symbol;
	symbol.st_info = STT_OBJECT | STB_LOCAL;
	symbol.st_shndx = current_section;
	symbol.st_size = sizeof(Elf_Addr);
	symbol.st_value = value;
	parser->get_symbol_table()->put(symbol, label_name, true);
}

void Label::second_pass()
{
	parser->insert_label(label_name);
}

std::string Label::get_label_name() const
{
	return label_name;
}
