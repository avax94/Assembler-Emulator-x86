#include "memory.h"

Elf_Byte* memory::rom = nullptr;
Elf_Byte* memory::cached_page = nullptr;
int memory::cached_page_num = 0;
hard_disk memory::hd;
bool memory::dirty_page = true;

memory::memory()
{
}


memory::~memory()
{
}

Elf_Byte memory::read_memory(Elf_Addr addr)
{
	Elf_Addr page = addr >> 25;
	
	if (page != cached_page_num)
	{
		if (dirty_page)
			hd.write(page, cached_page, page_size);

		cached_page = hd.read(page, page_size);
	}

	return cached_page[addr & 0x01FFFFFF];
}

Elf_Word memory::read_word(Elf_Addr addr)
{
	Elf_Word result = 0;
	
	char* tmp = (char*)&result;

	for (int i = 0; i < 4; i++)
	{
		*(tmp + i) = read_memory(addr + i);
	}

	return result;
}

void memory::write_word(Elf_Addr addr, Elf_Word word)
{
	char* tmp = (char*)&word;
	
	for (int i = 0; i < 4; i++)
	{
		write_memory(addr + i, *(tmp + i));
	}
}

void memory::write_memory(Elf_Addr addr, Elf_Byte b)
{
	Elf_Addr page = addr >> 25;

	if (page != cached_page_num)
	{
		if (dirty_page)
			hd.write(page, cached_page, page_size);

		cached_page = hd.read(page, page_size);
	}

	cached_page[addr & 0x01FFFFFF] = b;
}

void memory::initialize()
{
	cached_page = new Elf_Byte[page_size];

}

void memory::set_dirty()
{
	dirty_page = true;
}

void memory::finalize()
{
	delete[]cached_page;
}

