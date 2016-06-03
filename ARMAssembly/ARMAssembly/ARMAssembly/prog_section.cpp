#include "prog_section.h"
#include "typies.h"
#include "File.h"

////TODO DELETE
//struct HexCharStruct
//{
//	unsigned char c;
//	HexCharStruct(unsigned char _c) : c(_c) { }
//};
//
//inline std::ostream& operator<<(std::ostream& o, const HexCharStruct& hs)
//{
//	return (o << std::hex << (int)hs.c);
//}
//
//inline string to_hex(char c)
//{
//	switch (c){
//	case 0:
//		return "0";
//	case 1:
//		return "1";
//	case 2:
//		return "2";
//	case 3:
//		return "3";
//	case 4:
//		return "4";
//	case 5:
//		return "5";
//	case 6:
//		return "6";
//	case 7:
//		return "7";
//	case 8:
//		return "8";
//	case 9:
//		return "9";
//	case 10:
//		return "A";
//	case 11:
//		return "B";
//	case 12:
//		return "C";
//	case 13:
//		return "D";
//	case 14:
//		return "E";
//	case 15:
//		return "F";
//	}
//}
//
//inline string hex(unsigned char _c)
//{
//	char f = _c % 16;
//	char s = _c / 16;
//	if (s > 15)
//	{
//		cout << "HEHE";
//	}
//
//	return "" + to_hex(s) + to_hex(f);
//}



prog_section::prog_section(Elf_Shdr sc, string name, reallocation_table* r_tbl) : section(sc, name, r_tbl)
{
	
}


prog_section::~prog_section()
{
}

void prog_section::put(Elf_Word ins)
{
	instructions.push_back(ins & F_BYTE_MASK);
	instructions.push_back((ins & S_BYTE_MASK) >> 8);
	instructions.push_back((ins & T_BYTE_MASK) >> 16);
	instructions.push_back(ins >> 24);

	section_info.sh_size += 4 * sizeof(Elf_Byte);
}

void prog_section::put(Elf_Double ins)
{
	Elf_Word h = ins >> sizeof(Elf_Word) * 8;
	Elf_Word l = ins & (~((1 << (sizeof(Elf_Word) * 8) - 1)));
	put(l); put(h);
}

void prog_section::put(Elf_Byte s)
{
	section_info.sh_size += sizeof(Elf_Byte);
	instructions.push_back(s);
}

void prog_section::print(File* f)
{
	for (int i = 0; i < instructions.size(); i++)
	{
		//f->write(hex(instructions[i]) + " ");
	}
}

char* prog_section::serialize()
{
	int sz = instructions.size();
	char* result = new char[sz];
	int idx = 0;

	for (int i = 0; i < instructions.size(); i++)
	{
		char* tmp = (char*) &instructions[i];
		
		for (int j = 0; j < sizeof(Elf_Byte); j++)
		{
			result[idx++] = tmp[j];
		}
	}

	return result;
}
