#include "linker.h"
#include <fstream>
#include <string>

using namespace std;

linker::linker()
{
}


linker::~linker()
{
}

void read(char*, char*, int, int);
void linker::first_pass(string file_path)
{
	ifstream file(file_path, std::ifstream::ate | std::ifstream::binary);
	Elf_Ehdr e_hdr;

	int size = file.tellg();
	file.seekg(0);
	char* _file = new char[size];
	file.read(_file, size);
	read(_file, (char*)&e_hdr, SIZE_OF_ELF_EHDR, 0);

	int offset = e_hdr.e_shoff + e_hdr.e_shstrndx * e_hdr.e_shentsize;
	//find symtable entry
	Elf_Shdr strtbl;
	read(_file, (char*)&strtbl, e_hdr.e_shentsize, offset);
	string symtbl_entry = "";
	offset = strtbl.sh_offset;
	int idx = 0;
	int i = 0;
	while (true)
	{
		char c;
		read(_file, &c, 1, offset + i);
		i++;

		if (c == '\0')
		{
			if (symtbl_entry == ".symtab")
			{
				break;
			}

			idx++;
			symtbl_entry = "";
		}
		else
		{
			symtbl_entry += c;
		}
	}

	Elf_Word symtab_address; Elf_Word symstrtbl_add;
	offset = e_hdr.e_shoff + idx * e_hdr.e_shentsize + 4 * sizeof(Elf_Word);
	read(_file, (char*)&symtab_address, sizeof(Elf_Word), offset);
	offset = e_hdr.e_shoff + (idx - 1) * e_hdr.e_shentsize + 4 * sizeof(Elf_Word);
	read(_file, (char*)&symstrtbl_add, sizeof(Elf_Word), offset);

	cout << "HEHE";
}

int main()
{
	linker l;
	l.first_pass("C:\\Users\\stefa\\Desktop\\bdv.o");
}


void read(char* source, char* dst, int size, int offset)
{
	for (int i = 0; i < size; i++)
	{
		dst[i] = source[offset + i];
	}
}