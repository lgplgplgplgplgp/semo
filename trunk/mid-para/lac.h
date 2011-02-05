
/*

+	LGNOSIA Code , part of SC mid-para

+	'Semo Compiler' is a multi-objective compiler which developing under the terms of the 
+	GNU general public license as published by the Free Software Foundation.
+	The project lunched by Jelo Wang since 02.Jan.2008 from 'Techniques of Knowledge' community. 

+	You can redistribute it and/or modify it under the terms of the gnu general public version 3 of 
+	the license as published by the free software foundation.this program is distributed in the hope 
+	that it will be useful,but without any warranty.without even the implied warranty of merchantability 
+	or fitness for a particular purpose.																					
																												
+	(C)	突壳开源Techniques of Knowledge
+		an open source community since 2008
+		Community : http://www.tok.cc
+		Contact Us : jelo.wang@gmail.com

+		-Thanks to Our Committers and Friends
+		-Best Wish to all whose Contributes and Inspires
+		-Techniques of Knowledge 
+		-致伙伴们最美好祝愿
+		-突壳开源社区

*/ 

# ifndef __LAC_
# define __LAC_

# include "sccl.h"

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
	
	LAC_PROC ,	
	LAC_IF ,
	LAC_L_DELT ,
	LAC_R_DELT ,
	LAC_L_MEM ,
	LAC_R_MEM ,
	
	//	change row
	LAC_CR ,
	
} LAC_ATOM ;

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

	//	lac type
	int type ;
	//	a handle to another datas
	int handle ;
	//	scope of lac atom
	int scope ;
	//	line position
	int line ;
	//	totall of lac-procedure
	int proctotall ;
	int havelabel ;
	//	number of the lac atom in the whole chain
	int number ;
	int length ;
	//	lac codes length in bytes
	int colen ;
	
	struct LACA* head ;
	struct LACA* next ;

	SCClString code ;

	//	reference chain
	SCClList refchain ;

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
extern LAC* LACGet ( int number ) ;
extern int LACAdd ( char* string , LAC_ATOM type , int scope ) ;
extern void LACAddLabel ( int number , char* label ) ;
extern char* LACGetContent () ;
extern void LACLiveScopeGenerate ( int degreesmax ) ;
extern void LACClear () ;
extern void LACClearEx ( LAC* walker ) ;

extern int LACLabelMoiNew () ;
extern void LACLabelMoiSetContext ( int monitor ) ;
extern void LACLabelMoiAdd ( int lac ) ;
extern void LACLabelMoiSetLabel ( char* label ) ;
extern void LACLabelMoiClear () ;
