
/*

+	constant evaluator of SC c-front-para

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


# ifndef __EVALOR_
# define __EVALOR_

# include "sccl.h"

//# define __EVALOR_DEBUG

typedef struct {

	//	author : Jelo Wang
	//	since : 20100427
	//	(C)TOK

	//	EXPR evalutor

	//	(1) suffix  
	//	(2) operator stack
	//	(3) EXPR tree
	
	SCClList suffix ;
	SCClStack operator ;
	int tree ;

} EVALOR ;

# endif

extern long int c_evalor () ;
extern EVALOR* EvalorCreate () ;
extern void EvalorSetCurrent ( EVALOR* evalor );
extern void EvalorUnsetCurrent () ;
extern void EvalorOperandPush ( int operand ) ;
extern void EvalorOperatorPush (int operator ) ;
extern void EvalorFinish () ;
extern void EvalorClear () ;
extern int EvalorGetTree () ;

