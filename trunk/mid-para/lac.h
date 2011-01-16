
/*

+	LAC , part of SC mid-para

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

# ifndef __LAC_
# define __LAC_

# include "sccl.h"

typedef enum {

	//	author : Jelo Wang
	//	since : 20100722
	//	(C)TOK

	//	lac EXPR atoms
	
	LAC_SYMBOL_VARIABLE ,
	LAC_SYMBOL_ARRAY ,

} LAC_SYMBOL_ATOM ;

typedef enum {

	//	author : Jelo Wang
	//	since : 20100722
	//	(C)TOK

	//	lac EXPR opr
	
	LAC_EXP_OPRAND1 ,
	LAC_EXP_OPERATOR ,
	LAC_EXP_OPRAND2 ,
	LAC_EXP_LVALUE ,

} LAC_EXP_OPR ;

typedef enum {

	//	author : Jelo Wang
	//	since : 20100722
	//	(C)TOK

	//	lac EXPR atoms

	LAC_EXP_DATA ,
	LAC_EXP_ADD ,
	LAC_EXP_SUB ,
	LAC_EXP_MUL ,
	LAC_EXP_DIV ,


} LAC_EXP_ATOM ;

typedef enum {

	//	author : Jelo Wang
	//	since : 20100722
	//	(C)TOK

	//	lac EXPR atoms
	
	LAC_FLOW_LOGICAL ,
	LAC_FLOW_LOOP ,

} LAC_FLOW_ATOM ;

typedef enum {

	//	author : Jelo Wang
	//	since : 20100722
	//	(C)TOK

	//	lac EXPR atoms

	LAC_REG_MEMPORT ,	
	LAC_STATIC_MEMPORT ,
	LAC_STACK_MEMPORT ,
	LAC_HEAP_MEMPORT ,

} LAC_MEMPORT ;

typedef enum {


	//	author : Jelo Wang
	//	since : 20100722
	//	(C)TOK

	LAC_ATOM_PROC ,
	LAC_ATOM_FLOW ,
	LAC_ATOM_SYMBOL ,
	LAC_ATOM_EXP ,
	
} LAC_ATOM ;

typedef struct {

	//	author : Jelo Wang
	//	since : 20100722
	//	(C)TOK

	//	a LAC Procedure
	
	int azonal ;
	
} LAC_PROC ;

typedef struct {

	//	author : Jelo Wang
	//	since : 20100722
	//	(C)TOK

	//	LAC Control-flow 
	
	LAC_FLOW_ATOM type ;
	
	char* label ;
	char* expr ;
		
} LAC_FLOW ;

typedef struct {

	//	author : Jelo Wang
	//	since : 20100722
	//	(C)TOK

	//	LAC symbol 

	LAC_SYMBOL_ATOM type ;
	
	char* name ;
	char* label ;

	LAC_MEMPORT memport ;
	int based ;
	int size ;	
	
} LAC_SYMBOL ;

typedef struct {

	//	author : Jelo Wang
	//	since : 20100722
	//	(C)TOK

	//	LAC EXPR

	char* lvalue ;
	
	struct {
		int type ;
		char* name ;
	} oprand1 ;

	struct {
		int type ;
		char* name ;
	} operator ;

	struct {
		int type ;
		char* name ;
	} oprand2 ;

} LAC_EXP ;

typedef struct LACA {

	//	author : Jelo Wang
	//	since : 20100505
	//	(C)TOK

	//	a LAC flow consits two part
	//	oprand1 op oprand2
	//	and a label if necessary

	SCClString code ;
	
	int type ;
	int handle ;
	
	int havelabel ;
	int number ;
	int length ;
	int colen ;

	struct LACA* head ;
	struct LACA* next ;

} LAC ;

typedef struct LMON {

	//	author : Jelo Wang
	//	since : 20100505
	//	(C)TOK
	
	LAC* lac ;
	struct LMON* head ; 
	struct LMON* next ; 

} LABELMOI ;

# endif

extern int LACNew () ;
extern void LACSetContext ( int context ) ;

extern int LACAddProcDef ( int azonal ) ;
extern int LACAddSymbolDef ( LAC_SYMBOL_ATOM type , char* name , char* label , LAC_MEMPORT memport , int based , int size ) ;
extern int LACAddExp ( LAC_EXP_OPR opr , char* name , LAC_EXP_ATOM atom  ) ;
extern int LACExpAppend ( int lac , LAC_EXP_OPR opr , char* name , LAC_EXP_ATOM atom  ) ;

extern LAC* LACGet ( int number ) ;
extern void LACAddLabel ( int number , char* label ) ;
extern char* LACGetContent () ;
extern void LACClear () ;

extern int LACLabelMoiNew () ;
extern void LACLabelMoiSetContext ( int monitor ) ;
extern void LACLabelMoiAdd ( int lac ) ;
extern void LACLabelMoiSetLabel ( char* label ) ;
extern void LACLabelMoiClear () ;
