//#ifndef _TYPES_H
//#define _TYPES_H
////Types to be used by this assembly
//typedef char Elf_Half;
//typedef unsigned int Elf_Addr;
//typedef unsigned int Elf_Word;
//typedef unsigned int Elf_Off;
//typedef char Elf_Byte;
//
//typedef struct
//{
//	Elf_Half e_type;
//	Elf_Half e_machine;
//	Elf_Word e_version;
//	Elf_Addr e_entry;
//	Elf_Off  e_phoff;
//	Elf_Off  e_shoff;
//	Elf_Word e_flags;
//	Elf_Half e_ehsize;
//	Elf_Half e_phentsize;
//	Elf_Half e_phnum;
//	Elf_Half e_shentsize;
//	Elf_Half e_shnum;
//	Elf_Half e_shstrndx;
//} Elf_Ehdr;
//
//typedef struct
//{
//	Elf_Word sh_name;
//	Elf_Word sh_type;
//	Elf_Word sh_flags;
//	Elf_Addr sh_addr;
//	Elf_Off  sh_offset;
//	Elf_Word sh_size;
//	Elf_Word sh_link;
//	Elf_Word sh_info;
//	Elf_Word sh_addralign;
//	Elf_Word sh_entsize;
//} Elf_Shdr;
//
//typedef struct
//{
//	Elf_Word st_name;
//	Elf_Addr st_value;
//	Elf_Word st_size;
//	Elf_Byte st_info;
//	Elf_Half st_shndx;
//} Elf_Sym;
//
//typedef struct
//{
//	Elf_Addr rt_offset;
//	Elf_Word rt_info;
//} Elf_Rel;
//
//#define SYMBOL_ENTRY_SIZE sizeof(Elf_Word)*2 + sizeof(Elf_Byte) + sizeof(Elf_Addr) + sizeof(Elf_Half)
//
//#define ADD "add"
//#define SUB "sub"
//#define MUL "mul"
//#define DIV "div"
//#define INT "int"
//#define MOV "mov"
//#define CMP "cmp"
//#define AND "and"
//#define OR "or"
//#define NOT "not"
//#define TEST "test"
//#define LDR "ldr"
//#define STR "str"
//#define CALL "call"
//#define IN "in"
//#define OUT "out"
//#define SHR "shr"
//#define SHL "shl"
//#define LDCH "ldch"
//#define L "l"
//
////conditions for commands
//#define EQ 0
//#define NE 1
//#define GT 2
//#define GE 3
//#define LT 4
//#define LE 5
//#define HI 6
//#define HS 7
//#define UD 8
//
////flag for commands
//#define F 0
//#define NF 1
//
////binding for symbols
//#define STB_LOCAL  0x00
//#define STB_GLOBAL 0x10
//
////types for sybols
//#define STT_OBJECT  0x00
//#define STT_SECTION 0x01
//
//extern Elf_Half current_section;
//
//#endif