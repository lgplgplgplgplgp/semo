
/*

+	PARSER , part of SC front-para

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


# ifndef __C_PARSER_
# define __C_PARSER_

# include "sccl.h"

enum {

	//	author : Jelo Wang
	//	since : 20100418
	//	(C)TOK
	
	PARSERC_SCOPE_PARAM = 1 ,
	//	scope of symbol evaluation
	PARSERC_SCOPE_EVA ,
		
} PARSERC_SCOPE ;

typedef struct {

	//	author : Jelo Wang
	//	since : 20091128
	//	(C)TOK
	
	short int head ;
	short int form ;
	short int stop ;
	short int scope ;
	short int stack ;
	SCClStack scopestack ;

} PARSERC ;

# endif

extern void read_define_head_flow () ;
extern void parserc_ready () ;
extern int parserc_genv () ;
extern int parser_c_read_function () ;
extern int parser_c_read_if () ;
extern int parser_c_read_else () ;
extern int parser_c_read_while () ;
extern int parser_c_read_controlflow () ;
extern int parser_c_is_controlflow ( int head ) ;
extern int parser_c_read_symbol_inf () ;
extern int parser_c_read_variable_def () ;
extern int parser_c_read_variable_inf () ;
extern int parser_c_read_symbol_def () ;
extern int parser_c_read_expr () ;
extern void parser_c_read_poroperator ( int side , int evalor_enable ) ;
int parser_c_run ( int* line , int * codes ) ;
