
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

# include <stdarg.h> 
# include "c-errors.h"

extern int strlen( char* ) ;
extern int strcpy( char* , char* ) ;
extern void* SCMalloc ( int ) ;

ERRORC errorc ;
ERRORC* errorc_walker = &errorc;

unsigned short ERROR_ENABLE = 1;
unsigned short WARNING_ENABLE = 1;

unsigned short C_PRESOR_ERROR_ENABLE = 1 ;
unsigned short C_LEXER_ERROR_ENABLE = 1 ;
unsigned short C_PARSER_ERROR_ENABLE = 1 ;
unsigned short C_SEMAN_ERROR_ENABLE = 1 ;

unsigned short C_PRESOR_WARNING_ENABLE = 1 ;
unsigned short C_LEXER_WARNING_ENABLE = 1 ;
unsigned short C_PARSER_WARNING_ENABLE = 1 ;
unsigned short C_SEMAN_WARNING_ENABLE = 1 ;

int cerror ( int module , int type , const   char* SCLog , ... ) {

	//	author : Jelo Wang
	//	notes : printer
	//	since : 20091124
	
	va_list ap ;   
	va_start ( ap , SCLog ) ;   
	vprintf ( SCLog , ap ) ;
	va_end ( ap ) ;
	
	return 1 ;

}
