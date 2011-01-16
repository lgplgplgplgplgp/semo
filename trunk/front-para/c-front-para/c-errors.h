/*
+	error mssage & mssage manager of SC

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

# ifndef __ERRORS__
# define __ERRORS__

# define IS_C_ERROR 0
# define IS_C_WARRING 1

# define C_PRESOR_MOD 0
# define C_LEXER_MOD 1
# define C_PARSER_MOD 2

typedef struct errorc {
	
	//	author : Jelo Wang
	//	since : 20090813

	//	(1) : SCLog 
	//	(2) : type of SCLog , error or warning
	//	(3) : line
	//	(4) : mode from , persor , lexer or parser

	char* SCLog ;
	char* file ;

	int type ;
	int line ;
	
	unsigned short mode ;
	
	struct errorc* next ;  

} ERRORC ;


extern char ERR_C_FRONT[120][120] ;

int cerror ( int module , int type , const   char* SCLog , ... ) ;

# endif
