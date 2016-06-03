#pragma once
#include <string>

using namespace std;

typedef unsigned short Elf_Half;
typedef unsigned int Elf_Addr;
typedef unsigned int Elf_Word;
typedef unsigned long long Elf_Double;
typedef unsigned int Elf_Off;
typedef unsigned char Elf_Byte;

typedef struct
{
	Elf_Addr e_entry;
	Elf_Off e_phoff;
	Elf_Off e_shoff;
	Elf_Word e_flags;
	Elf_Half e_ehsize;
	Elf_Half e_phentsize;
	Elf_Half e_phnum;
	Elf_Half e_shentsize;
	Elf_Half e_shnum;
	Elf_Half e_shstrndx;
	Elf_Byte e_type;
} Elf_Ehdr;

#define SIZE_OF_ELF_EHDR (sizeof(Elf_Byte) + 4 * sizeof(Elf_Word) + 6 * sizeof(Elf_Half))
void _serialize(char*, Elf_Ehdr&);

typedef struct
{
	Elf_Word sh_name;
	Elf_Word sh_type;
	Elf_Word sh_flags;
	Elf_Addr sh_addr;
	Elf_Off sh_offset;
	Elf_Word sh_size;
	Elf_Word sh_link;
	Elf_Word sh_info;
	Elf_Word sh_entsize;
} Elf_Shdr;

#define SIZE_OF_ELF_SHDR (9 * sizeof(Elf_Word))

typedef struct
{
	Elf_Word st_name;
	Elf_Addr st_value;
	Elf_Word st_size;
	Elf_Half st_shndx;
	Elf_Byte st_info;
} Elf_Sym;

#define SYMBOL_ENTRY_SIZE (3 * sizeof(Elf_Word) + sizeof(Elf_Byte) + sizeof(Elf_Half))

typedef struct
{
	Elf_Addr rt_offset;
	Elf_Half rt_info;
	Elf_Byte rt_carry_dependent;
} Elf_Rel;

#define SIZE_OF_ELF_REL (sizeof(Elf_Addr) + sizeof(Elf_Byte) + sizeof(Elf_Half))

extern string ADD;
extern string SUB;
extern string MUL;
extern string DIV;
extern string INT;
extern string MOV;
extern string CMP;
extern string AND;
extern string OR;
extern string NOT;
extern string TEST;
extern string LDR;
extern string STR;
extern string LDRIA;
extern string STRIA;
extern string LDRIB;
extern string STRIB;
extern string STRDB;
extern string LDRDA;
extern string STRDA;
extern string LDRDB;
extern string CALL;
extern string IN;
extern string OUT;
extern string SHR;
extern string SHL;
extern string LDCH;
extern string LDCL;
//todo remove this
extern string L;

#define EQ 0
#define NE 1
#define GT 2
#define GE 3
#define LT 4
#define LE 5
#define HI 6
#define HS 7
#define UD 8

#define F 0
#define NF 1

#define STB_LOCAL 0x00
#define STB_GLOBAL 0x10

#define STT_UNDEFINED
#define STT_OBJECT 0x00
#define STT_SECTION 0x01

#define SHF_WRITE 0x01
#define SHF_ALLOC 0x02
#define SHF_EXECINSTR 0x04
#define SHF_NONE 0x00

#define SHT_PROGBITS 0
#define SHT_SYMTAB 1
#define SHT_STRTAB 2
#define SHT_RELA 3
#define SHT_REL 4

#define R_386_16_H 0x1000
#define R_386_16_L 0x2000
#define R_386_16_PC

#define RT_SECTION 0x0100
#define RT_SYMBOL 0x0200

#define R_SUBSTRACT
#define R_ADD

#define CLEAR_MASH_H 0x0000FFFF
#define CLEAR_MASK_L 0xFFFF0000
#define CLEAR_MASK_14_H 0x0003FFFF
#define CLEAR_MASK_18_L 0xFFFC0000
#define F_BYTE_MASK 0x000000FF
#define S_BYTE_MASK 0x0000FF00
#define T_BYTE_MASK 0x00FF0000

#define INT_C 0x0
#define ADD_C 0x1
#define SUB_C 0x2
#define MUL_C 0x3
#define DIV_C 0x4
#define CMP_C 0x5
#define AND_C 0x6
#define OR_C 0x7
#define NOT_C 0x8
#define TEST_C 0x9
#define LDR_C 0xA
#define STR_C 0xB
#define CALL_C 0xC
#define IN_OUT_C 0xD
#define MOV_SHR_SHL_C 0xE
#define LDCH_L_C 0xF

#define IA 2
#define DA 3
#define IB 4
#define DB 5

extern const string TEXT_SECTION;
extern const string BSS_SECTION;
extern const string DATA_SECTION;
extern Elf_Word current_section_start_addr;
extern bool active_command;
extern Elf_Word current_section;

Elf_Word abs_diff(Elf_Word a, Elf_Word b);

#define ET_REL 0
#define ET_EXEC 1