
/*

+	SYMBOLE , between SC front-para and mid-para

+	'Semo Compiler' is a multi-objective compiler which is developing under the terms of the 
+	GNU general public license as published by the Free Software Foundation.
+	The project lunched by Jelo Wang since 2.Feb.2008 from 'Techniques of Knowledge' community. 

+	You can redistribute it and/or modify it under the terms of the gnu general public version 3 of 
+	the license as published by the free software foundation.this program is distributed in the hope 
+	that it will be useful,but without any warranty.without even the implied warranty of merchantability 
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

//	author: Jelo Wang
//	since : 20081109
//	(c)TOK

# ifndef __SYMBOLE
# define __SYMBOLE

typedef enum {

	//	author : Jelo Wang
	//	since : 2008
	//	(C)TOK

	ISA_INTEGER ,
	ISA_VARIABLE ,
	ISA_ARRAY ,		
	ISA_FUNCTION ,
	ISA_FUNCCAL ,
	ISA_CONST ,
	ISA_STRUCTHEAD ,	
	ISA_VARSTRUCT ,
	ISA_ARRSTRUCT ,
	ISA_VARUNION , 
	ISA_ARRUNION ,
	ISA_TYPEDEF ,
	ISA_IFCF ,	
	ISA_ELSECF ,
	ISA_ELSEIFCF ,
	ISA_FORCF ,	
	ISA_WHILECF ,	
	ISA_DOWHILECF ,
	ISA_SWITCHCF ,

} AZONALTYPE ;

typedef enum {

	//	author : Jelo Wang
	//	since : 20100423
	//	(C)TOK

	EXP_OPERAND ,
	EXP_OPERATOR 
	
} EXPATOM ;

typedef enum {

	//	author : Jelo Wang
	//	since : 20100502
	//	(C)TOK
	
	EXP_DELT_DEFAULT ,
	EXP_DELT_ANLDATA ,
	EXP_DELT_ANLNUMERIC ,	

} EXPDELTTYPE ;

typedef enum {

	//	author : Jelo Wang
	//	since : 20100806
	//	(C)TOK
	
	AZONAL_DRC_DEFAULT ,
	AZONAL_DRC_CFF ,
	AZONAL_DRC_GRAY_CFF ,
	AZONAL_DRC_IF_DISTURB ,	
	AZONAL_DRC_ELSEIF_DISTURB ,
	AZONAL_DRC_ELSE_DISTURB , 
	AZONAL_DRC_WHILE_DISTURB ,
	
} DRCTYPE ;

typedef struct EXPRE {

	//	author : Jelo Wang
	//	since : 20100202
	//	(C)TOK

	//	field (1) type of an EXPR
	//	field (2) linked to an azonal or a functer
	
	char* delt ;

	int delttype ;	
	int type ;
	int handle ; 
	int deep ;

	//	for compiling-render 
	int x ;
	int y ;
			
	struct EXPRE* father ;
	struct EXPRE* left ;
	struct EXPRE* right ;
	struct EXPRE* brother ;

} EXPR ;

typedef struct {

	//	author : Jelo Wang
	//	since : 20100806
	//	(C)TOK

	//	notes : AZONAL Define-Reference Chain

	int alias ;
	int scope ;
	int cf ;
	int lga ;
	
	//	cff reference times
	int cffref_t ;
	
	//	for AZONAL_DRC_CFF
	//	all of alias in a CFF set are saved here
	struct {
		//	type == 1 is a handle to a SCClList*
		//	type == 2 is a handle to a SCClString*		
		int type ;
		void* handle ;
	} cffset ;
	
} ANLDRC ;

typedef struct ANLLSNN {

	//	author : Jelo Wang
	//	since : 20100826
	//	(C)TOK

	//	notes : life span of ANL

	char* name ;
	int  start_pn;
	int end_pn ;

	struct ANLLSNN* head ;
	struct ANLLSNN* next ;
	
} ANLLSN ;

typedef struct ANODE {

	//	author : Jelo Wang
	//	since : 20090321	
	
	//	notes : ANL , the meaings of it depends on its scope.Identify an array , a variable or a structed symbol.

	char* name ;
	//	azonaltype , value between ISA_VARIABLE , ISA_ARRAY , ISA_FUNCTION , ISA_EXPR , ISA_CONTROLFLOW , ISA_NUMERAL
	int azonaltype ;		
	//	datatype , data type of an AZONAL
	int datatype ;	
	//	which symbol is belong to , symbol between function or structed symbol. 
	int belong ;		
	//	defination scope
	int scope ;
	//	a handle to LGA
	//	representing  the AZONAL which field is belong to
	int lgabelong ;
	int used ;
	int line ;
	int isparam ;			
	int number ;			
	int layer ;			
	int size ;
	int scale ;

//	struct {

		//	tack , SC use tack to handling additional information about a ANL , like a function which parameters are handled here

//		int totall ;
		
//		struct ANODE* head ;
//		struct ANODE* next ;
		
//	} tack ;

	//	tack , SC use tack to handling additional information about a ANL , like a function which parameters are handled here
	SCClList tack ;

	struct {

		//	identify defination times of ANL , SC use this value for alias generating.
		int deftimes ; 
		//	ANLDRC nodes are saved here.used for SSA transform. 
		SCClList* chain ;

	} DRC ;
	
	struct ANODE* head ;
	struct ANODE* next ;

} AZONAL ;

typedef struct SNODE {

	//	author : Jelo Wang
	//	notes : symbol table

	AZONAL constant ;	
	AZONAL variable ;
	AZONAL array ;
	AZONAL function ;
	AZONAL funccal ;
	AZONAL controlflow ;
		
	int vartotall ;
	int arrtotall ;
	int functotall ;
	int exptotall ;
	int logicalblocks ;

} SYMBOLE ;

/*
typedef struct STNODE {

	//	author : Jelo Wang
	//	since : 20090321

	//	field (1) : name of a struct head
	//	field (2): scope
	//	field (3) : belong to function
	//	field (4) : normal struct or union or enum or typdef type
	//	field (5) : struct head number we can use it for a trail chain number
	//	field (6) : layer position of a recursion struct
	//	field (7) : layers of a struct

	//	trail is a scope filed of a structs
	//	like some flow bellow , a is in the scope field of GTKINGS
	//	struct {
	//		int a ;
	//	} GTKINGS  ;
	//	field (1) : its a linked node to AZONAL . SC use it to linking a trail chain node of a struct symbol
	//	field (2) : for struct trail chain node
	//	field (3) : next node

	//	field (8) : next node
	//	field (9) : front node	
		
	char* name ;
	char* scope ;
	int belong ;			
	int type ;			
	int number ;			
	int on_layer ;		
	int layer ;			

	struct {
		struct AZONAL* anode ;	
		struct STNODE* snode ;	
		struct STNODE* next ;
	} trail ;
	
	struct STNODE* next ;
	struct STNODE* front ;

} STRUCTHEAD  ;


typedef struct STRU_SYM {

	//	author : Jelo Wang
	//	since : 20090322

	//	field (1) : its a linked node to AZONAL . 
	//	field (2) : its a linked node to STRUCT_HEAD . 
	//	field (3) : next node
	
	struct AZONAL* azonal ;
	STRUCT_HEAD* head ;
	struct STRU_SYM* next ;
	struct STRU_SYM* front ;	
	
} STRUCTS  ;
*/


# endif


extern int SymboleNew () ;
extern void SymboleSetCurrent ( int ) ;
extern AZONAL* SymboleAddVarAzonal ( 

	char* name ,
	int data_type , 
	int azonal_type , 
	int scope ,
	int is_param ,
	int line ,
	int belong ,
	int lgabelong 
	
) ;

extern  int SymboleAddFunctionParameter ( AZONAL* function , AZONAL* azonal ) ;
extern AZONAL* SymboleFindVarAzonal ( char* name ,int scope ,int belong ) ;
extern AZONAL* SymboleVarAzonalSavable ( char* name ,int scope ,int lgabelong ) ;
extern void SymboleAzonalRefInc ( AZONAL* azonal ) ;
extern AZONAL* SymboleAddFunction ( char* name , int azonaltype , int type , int line ) ;
extern AZONAL* SymboleAddFunctionCall ( char* name , int azonaltype , int tacktotall , int line ) ;
extern AZONAL* SymboleFindFunction ( char* name ) ;
extern int  SymboleAddFunctionParameter ( AZONAL* function , AZONAL* para ) ;
extern void SymboleSetFunctionParameters ( AZONAL* function , int parameters ) ;
extern int SymboleGetFunctionNumber ( char* name ) ;
extern AZONAL* SymboleAddControlAzonal ( int azonaltype , int line ) ; 
extern void SymboleUninstall () ;
extern int SymboleGetCurrentFuncNumber () ;
extern void* SymboleGetCurrentFunc () ; 
extern void* SymboleGetFuncParameter ( void* azonal , char* name ) ;

extern char* SymboleDRCAdd ( AZONAL* azonal , int scope , int cf , int lga ) ;
extern char* SymboleDRCGetDRC ( AZONAL* azonal , int scope , int lga ) ;
extern ANLDRC* SymboleDRCGenCFF ( AZONAL* azonal , int scope , int lga ) ;
extern void SymboleDRCDropCFF ( AZONAL* azonal ) ;
extern void SymboleDRCDropDRC ( AZONAL* azonal ) ;
extern AZONAL* SymboleAndNumeric ( char* name , int type ) ;

