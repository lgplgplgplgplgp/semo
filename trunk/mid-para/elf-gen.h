
/*

+	ELF generator , part of SC common library

+	Semo C()mpiler is a free software created by (c)Techniques of Knowledge since 20080202.
+	(C)TOK Jelo Wang

+	You can redistribute it and/or modify it under the terms of the gnu general public license	
+	as published by the free software foundation, either version 3 of the license or any later 	
+	version.this program is distributed in the hope that it will be useful,but without any 		
+	warranty.without even the implied warranty of merchantability or fitness for a particular 	
+	purpose.																					
																											
+	(c)	Techniques of Knowledge
+		an open source group since 2008
+		page : http://www.tok.cc
+		email : wqw85@sina.com

*/

# ifndef __ELF_GEN
# define __ELF_GEN

# include "elf.h"
# include "sccl.h"

enum {

	//	author : Jelo Wang	
	//	since : 20100728
	//	(C)TOK

	ELF_UNDEF ,
	ELF_SHSTRTAB ,
	ELF_TEXT ,
	ELF_DATA ,
	ELF_RODATA ,
	ELF_CONST ,
	ELF_BSS ,

	ELF_SYMTAB ,
	ELF_STRTAB ,
	
	ELF_TEXT_REL ,
	ELF_TEXT_RELA ,
	ELF_DATA_REL ,
	ELF_DATA_RELA ,

	ELF_SECTIONS = 13 , 

} ;

typedef struct {

	//	author : Jelo Wang	
	//	since : 20100727
	//	(C)TOK

	Elf32_Ehdr header ;

	//	sections
	Elf32_Shdr sect[ELF_SECTIONS] ;

	//	items of text.rel,text.rel	,data.rel,data.rela	
	SCClList TextRel ;
	SCClList TextRela ;
	SCClList DataRel ;
	SCClList DataRela ;

	//	items of symtab
	//	and contents of strtab
	
	SCClList Shstrtab ;
	SCClList Symtab ;
	SCClList Strtab ;

	//	save data of '.text','.data','.rodata','.constdata','bss'
	SCClList Text ;
	SCClList Data ;
	SCClList ConstData ;
	SCClList Rodata ;

	struct {
		int length ;
	} ShstrtabMonior ;

	struct {
		int length ;
	} StrtabMonior ;	

} ELFGENTOR ;

# endif

extern int ElfGenCreateTextRel ( Elf32_Rel elf32_rel ) ;
extern int ElfGenCreateTextRela ( Elf32_Rela elf32_rela ) ;
extern int ElfGenCreateDataRel ( Elf32_Rel elf32_rel ) ;
extern int ElfGenCreateDataRela ( Elf32_Rela elf32_rela ) ;

extern int ElfGenInsertTextRelItem ( Elf32_Rel elf32_rel ) ;
extern int ElfGenInsertTextRelaItem ( Elf32_Rela elf32_rela ) ;
extern int ElfGenInsertDataRelItem ( Elf32_Rel elf32_rel ) ;
extern int ElfGenInsertDataRelaItem ( Elf32_Rela elf32_rela ) ;

extern int ElfGenAddTextValue ( int value ) ; 
extern int ElfGenGetTextScale ( int mul ) ;

extern int ElfGenSymtabGetSymbolInfo () ;
extern int ElfGenSymtabInsertSymbol ( Elf32_Sym sym ) ;
extern int ElfGenStrtabInsertString ( char* string ) ;
extern int ElfGenStrtabGetEntry () ;
extern int ElfGenGetShndx ( char* sec ) ;

extern int ElfGen ( char* out , int setscale ) ;


