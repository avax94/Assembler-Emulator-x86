#pragma once
#include "typies.h"

class reallocation_table;

class section
{ 
private:
	char* section_data = nullptr;
protected:
	Elf_Shdr section_info;
	reallocation_table* realloc_table;
	Elf_Word location_counter;
	std::string name;
public:
	section(){};
	
	void set_section_offset(Elf_Word offset);
	Elf_Word get_section_size() const;
	virtual void allocate_space(){}
	virtual void put(Elf_Word instr){}
	virtual void put(Elf_Double ll){}
	virtual void put(Elf_Byte c){}
	Elf_Word get_LC() const;
	reallocation_table* get_realloc_t() const;
	virtual std::string get_name() const;
	section(Elf_Shdr& e, string name, reallocation_table* r = nullptr);
	~section();
	section(char* data, Elf_Shdr shdr);

	virtual char* serialize() { return section_data; }

	friend class section_table;
};

