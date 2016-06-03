#pragma once
#include "typies.h"
#include "hard_disk.h"

class memory
{
	memory();
	~memory();
	static hard_disk hd;
	static Elf_Byte * rom;
	static Elf_Byte* cached_page;
	static int cached_page_num;
	static const int page_size = 128 * 1024 * 1024; //128MB
	static bool dirty_page;

public:
	static Elf_Byte read_memory(Elf_Addr);
	static void write_memory(Elf_Word,Elf_Byte);
	static Elf_Word read_word(Elf_Addr);
	static void write_word(Elf_Addr, Elf_Word);
	static void set_dirty();
	static void initialize();
	static void finalize();
};

