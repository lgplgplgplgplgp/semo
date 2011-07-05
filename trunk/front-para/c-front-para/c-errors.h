
/*

+	error mssage & mssage manager of SC

+	'Semo Compiler' is a multi-objective compiler which is developing under the terms of the 
+	GNU general public license as published by the Free Software Foundation.
+	The project lunched by Jelo Wang since 2.Feb.2008 from 'Techniques of Knowledge' community. 

+	You can redistribute it and/or modify it under the terms of the gnu general public version 3 of 
+	the license as published by the free software foundation.this program is distributed in the hope 
+	that it will be useful,but WITHOUT ANY WARRANTY.without even the implied warranty of merchantability 
+	or fitness for a particular purpose.																					
																												
+	(C)	突壳开源Techniques of Knowledge
+		an open source community since 2008
+		Community : http://www.tok.cc
+		Contact Us : jelo.wang@gmail.com

+		-Thanks to Our Committers and Friends
+		-Best Wish to all who Contributed and Inspired
+		-Techniques of Knowledge 
+		-致伙伴们最美好祝愿
+		-突壳开源社区

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

	//	(1) : SClog 
	//	(2) : type of SClog , error or warning
	//	(3) : line
	//	(4) : mode from , persor , lexer or parser

	char* SClog ;
	char* file ;

	int type ;
	int line ;
	
	unsigned short mode ;
	
	struct errorc* next ;  

} ERRORC ;


extern char ERR_C_FRONT[120][120] ;

int cerror ( int module , int type , const   char* SClog , ... ) ;

# endif
