#include <string>
#include "typies.h"
using namespace std;

string ADD = "add";
string SUB = "sub";
string MUL = "mul";
string DIV = "div";
string INT = "int";
string MOV = "mov";
string CMP = "cmp";
string AND = "and";
string OR = "or";
string NOT = "not";
string TEST = "test";
string LDR = "ldr";
string STR = "str";
string LDRIA = "ldria";
string STRIA = "stria";
string LDRIB = "ldrib";
string STRIB = "strib";
string STRDB = "strdb";
string LDRDA = "ldrda";
string STRDA = "strda";
string LDRDB = "ldrdb";
string CALL = "call";
string IN = "in";
string OUT = "out";
string SHR = "shr";
string SHL = "shl";
string LDCH = "ldch";
string LDCL = "ldcl";
//todo remove this
string L = "ldcl";

Elf_Word current_section_start_addr = 0;

Elf_Word abs_diff(Elf_Word a, Elf_Word b)
{
	return a > b ? a - b : b - a;
}

Elf_Word current_section;
bool active_command;

const string TEXT_SECTION = ".text";
const string BSS_SECTION = ".bss";
const string DATA_SECTION = ".data";

void _serialize(char* s, Elf_Ehdr& e)
{
	char* tmp = (char*)&e;

	int idx = 0;

	for (int i = 0; i < SIZE_OF_ELF_EHDR; i++)
	{
		s[idx++] = tmp[i];

		//skip 3 beacuase of padding: TODO check on linux
		//if (i == 0) { i += 3; }
	}
}