
/*

+	PRESOR , part of SC c-front-para

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

# ifndef __C_PRESOR_
# define __C_PRESOR_

# include "sccl.h"

enum {
	
	//	author : Jelo Wang
	//	notes : there are only two macros type in c language , function or constant
	//	since : 20090809

	MACRO_FUNC ,
	MACRO_CONST ,

} MACRO_TYPE ;

typedef struct c_macro {

	//	author : Jelo Wang
	//	notes : MACRO 
	//	since : 20090809
	
	//	(1) name of macro
	//	(2) file name that the macro belong to .
	//	(3) macro body saved here , we used SCClString to handle it .
	//	(4) type of macro : MACRO_FUNC or MACRO_CONST
	//	(5) params of macro are saved here
	
	char* name ;
	char* file ;
	SCClString body ;

	unsigned short type ;

	struct {
		
		//	params of macro would saved by the follow format :
		//	xxx xxx xxx xx , the gaps between param's name is a blank

		int totall ;
		
		SCClString token ;

	} param ;

	struct c_macro* front ;	
	struct c_macro* next ;

} MACRO ; 

# endif

extern int presor_c_run ( char* ) ;

