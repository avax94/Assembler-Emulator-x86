#include "hard_disk.h"
#include <fstream>
#include <iostream>
#include <vector>
using namespace std;

hard_disk::hard_disk()
{
}


hard_disk::~hard_disk()
{
}

void hard_disk::write(Elf_Double page, const Elf_Byte* data, int N)
{
	const char* d = (const char*)data;
	string path = ".\\memory\\" + to_string(page) + ".memory";
	ofstream file(path, fstream::out | fstream::binary);
	file.write(d, N);
	file.close();
}

Elf_Byte* hard_disk::read(Elf_Double page, int N)
{
	char * data = new char[N];
	string path = ".\\memory\\" + to_string(page) + ".memory";
	ifstream file(path, fstream::binary | fstream::in);		
	file.read(data, N);
	file.close();
	return (Elf_Byte*)data;
}

