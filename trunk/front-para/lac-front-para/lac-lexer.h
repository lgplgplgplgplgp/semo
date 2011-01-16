
/*

+	LAC LEXER , part of SC mid-para

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

# ifndef __LAC_LEXER_
# define __LAC_LEXER_

# include "sccl.h"

# define lexerac_clearv() lexc->v = 0 ;

enum {

	//	author : Jelo Wang
	//	since : 20100503
	//	(C)TOK

	//	normal mode without specical features
	LEXLAC_DEFAULT_MODE = 0x1	 ,
	//	normal mode with specical features , like blank fliter
	LEXLAC_FLITER_MODE = 0x2 ,
	//	define-head bitmap generation process
	LEXLAC_HEADBIT_MODE = 0x4
 	

} LAC_LEXER_MODE ;

enum {

	//	author : Jelo Wang
	//	notes : bracket type , smart , mid , strong
	//	since : 20100503
	//	(C)TOK

	LEXLAC_SMART_BRACKET ,
	LEXLAC_MID_BRACKET ,
	LEXLAC_STRONG_BRACKET 
	
} LAC_BRACKET_TYPE ;

typedef struct {
	
	//	author : Jelo Wang
	//	notes : CODE
	//	since : 20090811
	//	(c)TOK
	
	//	(1) : lexical value 
	//	(2) : previous lexical value

	char* token ;
	char* file ;
	char c ;
	char pc ;
	
	int v ;
	int pv ;
	int ppv ;
	int line ;
	int stop ;
	int mode ;
	int deep ;	// if recursive occured , deep + 1
	int headbit ;
	
	//	ignore odd symbols
	int ios ;
	int stack ; 

	//	lac defination form STACK n 
	//	scale = n
	int scale ;
	
	SCClStack* scstack ;
	SCClString* code ;


} LEXERLAC ;


# endif


extern LEXERLAC* lexac ;

extern void lexerac_setmode ( int mode ) ;
extern void lexerac_ready () ;
extern int lexerac_overflowed () ;
extern int lexerac_next () ;
extern unsigned char lexerac_get_atom () ;
extern void lexerac_put_atom ( unsigned char atom ) ; 
extern void lexerac_rollback () ;
extern void lexerac_setback ( int step ) ;
extern void lexerac_skip_blank () ;
extern void lexerac_skip_space () ;
extern void lexerac_jump ( int step ) ;
extern int lexerac_look ( int step ) ;
extern void lexerac_genv () ;
extern LEXERLAC* lexerac_new ( unsigned char* data , int mmode ) ;
extern void lexerac_set ( LEXERLAC* lexer ) ;
extern void lexerac_clear_status () ;
extern void lexerac_set_file ( char* name ) ;
extern void lexerac_destroy () ;
extern LEXERLAC* lexerac_get () ;
extern void lexerc_clear_scale () ;
extern int lexerac_head_genv ( int border ) ;
extern int lexerac_expect ( int atom ) ;

