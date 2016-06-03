#include "reg_imm_ind.h"
#include <regex>

using namespace std;

reg_imm_ind::reg_imm_ind(std::string reg) : register_argument("")
{
	std::regex brackets("\\[|\\]");
	reg = regex_replace(reg, brackets, "");
	int idx = reg.find("r");
	string r = "";
	r += reg[idx++];
	r += reg[idx++];
	
	if (idx < reg.size() && reg[idx] != '+')
	{
		r += reg[idx];
	}

	this->reg = r;
	value = regex_replace(reg, regex("\\+r([0-9]|1[0-5])|r([0-9]|1[0-5])\\+"), "");

	this->reg += " " + value;
}


reg_imm_ind::~reg_imm_ind()
{
}
