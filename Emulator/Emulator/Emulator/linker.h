#pragma once
#include <vector>
#include "reallocation_table.h"
#include "symbol_table.h"
#include "symbol.h"
#include <string>

using namespace std;
class linker
{
	//symbol_table undefined_symbols;
	//symbol_table symbols;
public:
	linker();
	~linker();

	void first_pass(std::string file);
};

