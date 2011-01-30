

/*

+	ARMv ASM lexer , part of SC arm-back-para

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

# ifndef __ARM_ASM_LEXER
# define __ARM_ASM_LEXER

# include "sccl.h"

enum {

	//	author : Jelo Wang
	//	since : 20110130
	//	(C)TOK

	//	normal mode without specical features
	LEXERARM_DEFAULT_MODE = 0x1	 ,
	//	normal mode with specical features , like blank fliter
	LEXERARM_FLITER_MODE = 0x2 ,	

} LEXERARM_MODE ;

typedef enum {

	//	author : Jelo Wang
	//	since : 20100723
	//	(C)TOK

	AASM_FEN = 0x1000,			//;
	AASM_EQU ,					//=
	AASM_ZKL , 					//[
	AASM_ZKR , 				   	//]
	AASM_DOU , 				   	//,
	AASM_XY ,  					//'
	AASM_DY ,  					//"
	AASM_CHA , 				   	//char
	AASM_STR , 				  	//string
	AASM_MAO ,					//:
	AASM_SHARP ,				//#	
	AASM_ENTER ,				
	AASM_CHROW ,
	AASM_VAR ,
	AASM_ARRAY ,
	AASM_VARDEF ,
	AASM_ARRAYDEF ,
	AASM_PROCDEF ,
	AASM_PROCCALL ,
	AASM_LABEL ,
	AASM_INTENUM ,
	AASM_FLTNUM ,
	AASM_FLTENUM ,
	

} ARMASM_SYMBOLS ;

typedef struct {

	//	author : Jelo Wang
	//	since : 20100723
	//	(C)TOK

	char c ;
	char* token ;

	int v ;
	int pv ;

	int set ;
	int stop ; 
	int mode ;
	
	SCClStack stack ;
	SCClString* code ;
	
} LEXERARM ;

# endif

extern LEXERARM* lexarm ;

extern LEXERARM* lexerarm_new ( char* asm , int set ) ;
extern void lexerarm_set ( LEXERARM* lexerarm ) ;
extern void lexerc_setmode ( int mode ) ;
extern int lexerarm_genv () ;
extern int lexerarm_head_genv ( int border )  ;
extern int lexerarm_destroy () ;

