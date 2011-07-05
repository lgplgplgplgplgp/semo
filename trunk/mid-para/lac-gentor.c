
/*

+	LAC (Lgnosia Code) Form Generator , part of SC mid-para

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

# include "sc.h"
# include "schal.h"
# include "sccl.h"
# include "c-grammar.h"
# include "symbole.h"
# include "evalor.h"
# include "lgnosia.h"
# include "lac-gentor.h"
# include "lac.h"
# include "corenr.h"

static SCClStack LabelStack = { 0 , 0 , 0 , 0 , 0 } ;
static SCClString* lacgentor_results = 0 ;
static LAC_GENTOR lacgentor = { -1 , 0 , 0 , 0 , 0 , 0 , 0 } ;

int corenr = 0 ;

# define LACGENTOR_STATUS_RESET()\
	lacgentor.form = -1;\

# define SET_LACGENTOR_DELT(deltn)\
	SCClStackPush ( &lacgentor.deltstack, (void* )lacgentor.delt );\
	lacgentor.delt = deltn;\

# define UNSET_LACGENTOR_DELT()\
	lacgentor.delt = SCClStackPop ( &lacgentor.deltstack ) ;\

# define GET_LACGENTOR_SCOPE() SCClStackGet( &lacgentor.scopestack )

# define SET_LACGENTOR_SCOPE(sscope)\
	SCClStackPush ( &lacgentor.scopestack , (void* )sscope ) ;\

# define UNSET_LACGENTOR_SCOPE()\
	SCClStackPop ( &lacgentor.scopestack ) ;\

# define PUSH_LACGENTOR_LGA(lga)\
	SCClStackPush ( &lacgentor.lgastack  , (void* )lga ) ;\
	
# define GET_LACGENTOR_LGA() SCClStackGet( &lacgentor.lgastack )

# define POP_LACGENTOR_LGA()\
	SCClStackPop ( &lacgentor.lgastack ) ;\

char* lacgentor_gen_operator ( int operator ) {

	//	author : Jelo Wang
	//	since : 20100428
	//	(C)TOK

	switch ( operator ) {

		case C_MUL : return "*" ;
		case C_DIV : return "/" ;
		case C_MOD : return "%" ;

		case C_ADD : return "+" ;
		case C_SUB : return "-" ;

		case C_SHL : return "<<" ;
		case C_SHR : return ">>" ;

		case C_GE :	return ">=" ;
		case C_GT :	return ">" ;
		case C_LT :	return "<" ;
		case C_LE :	return "<=" ;

		case C_EQ :	return "==" ;
		case C_NE :	return "!=" ;

		case C_YHH : return "^" ;

		case C_HE :	return "&" ;

		case C_HU :	return "|" ;

		case C_AND : return "&&" ;

		case C_OR :	return "||" ;

	}

}

static void LACIdentorPush () {

	//	author : Jelo Wang
	//	since : 20100130
	//	(C)TOK

	SCClStackPush ( &lacgentor.identor , (void* )1 ) ;
	
}

static void LACIdentorPop () {

	//	author : Jelo Wang
	//	since : 20100130
	//	(C)TOK

	SCClStackPop ( &lacgentor.identor ) ;
	
}

char* lacgentor_get_register ( int index ) {

	//	author : Jelo Wang
	//	since : 20100425

	static char tempv [64] = {"%$V"} ;
	
	return sc_strcat ( tempv , SCClItoa (index) ) ;
	
}


char* lacgentor_get_label () {

	//	author : Jelo Wang
	//	since : 20100503

	static char tempv [64] = {"L"} ;
	static char result [64] = {0} ;

	char* A = sc_strcat ( tempv , SCClItoa (lacgentor.label++) ) ;
	char* B = sc_strcat ( A , ":" ) ;

	SCFree ( A ) ;

	sc_strcpy ( result , B ) ;

	SCFree ( B ) ;

	return result ;
	
}


char* lacgentor_head_label ( int label ) {

	//	author : Jelo Wang
	//	since : 20100503

	static char tempv [64] = {"L"} ;
	static char result [64] = {0} ;
		
	char* A = sc_strcat ( tempv , SCClItoa (label) ) ;
	char* B = sc_strcat ( A , ":" ) ;
	
	SCFree ( A ) ;

	sc_strcpy ( result , B ) ;

	SCFree ( B ) ;

	return result ;
	
}

static char* lacgentor_get_identor () {

	//	author : Jelo Wang
	//	since : 20100130
	//	(C)TOK

	//	generate a label
	
	int walker = 0 ;
	char* scope = 0 ;
	
	if ( 0 == lacgentor.identor.deep ) return 0 ;
	
	scope = (char* ) SCMalloc ( lacgentor.identor.deep + 1 ) ;

	for ( walker = 0 ; walker < lacgentor.identor.deep ; walker ++ )
		scope [ walker ] = '\t' ;
	
	scope [ walker ] = '\0' ;

	return scope ;

}

void lacgentor_ready () {
	
	//	author : Jelo Wang
	//	since : 20100107
	//	(C)TOK
	
	lacgentor.form =-1 ;
	lacgentor.stop = 0  ;
	lacgentor.delt = 0 ;
	lacgentor.label = 0 ;
	lacgentor.tree = 0 ;
	lacgentor.lgnosia = 0 ;
	
	SCClStackInit ( &lacgentor.identor ) ;
	SCClStackInit ( &lacgentor.deltstack ) ;

	//	get LGA tree
	lacgentor.tree = (SCClList* ) LgnosiaGet () ;
	
	if ( lacgentor.tree ) 
		lacgentor.lgnosia = lacgentor.tree->element ;

} 

void lacgentor_next () {
	
	//	author : Jelo Wang
	//	since : 20100107
	//	(C)TOK

	if ( lacgentor.tree ) lacgentor.tree = lacgentor.tree->next ;
	
	if ( !lacgentor.tree ) {
		lacgentor.stop = 1 ; 
		return ;
	}

	lacgentor.lgnosia = lacgentor.tree->element ;

}

void lacgentor_genv () {
	
	//	author : Jelo Wang
	//	since : 20100107
	//	(C)TOK
	
	if ( !lacgentor.tree ) {
		lacgentor.form = -1 ;
		return ;
	}

	if ( !lacgentor.stop ) {
		
		if ( !lacgentor.lgnosia ) return ;
		
		lacgentor.form = ((LGNOSIA*)(lacgentor.lgnosia))->type ;
		
	}
	
}


void lacgentor_gen_funcdef () {
	
	//	author : Jelo Wang
	//	since : 20100107
	//	(C)TOK

	AZONAL* azonal = 0 ;	
	LGNOSIA* lgnosia = 0 ;
	SCClList* listlooper = 0 ;

	if ( LGNOSIA_TOP_IDENT != lacgentor.form ) {
		LACGENTOR_STATUS_RESET() ;
		return ;
	}

	lgnosia = (LGNOSIA* ) lacgentor.lgnosia ;
	azonal = (AZONAL* ) lgnosia->azonal ;

	LACAdd ( azonal->name , LAC_PROC , lacgentor.identor.deep ) ;
	LACAdd ( "(" , -1 , -1 ) ;

	//	gen __armcall para	 pattern
	//	根据__armcall 调用规范生成参数列表
	if ( 0 < azonal->tack.totall ) {
		
		int counter = 0 ;
		char* vn [ 4 ] = { "%$V1" , "%$V2" , "%$V3" , "%$V4" } ;

		AZONAL* anl = 0 ;
		SCClList* looper = 0 ;

		//	get parameters
		for ( listlooper = azonal->tack.head ; listlooper ; listlooper = listlooper->next , counter ++ ) {

			anl = (AZONAL* ) listlooper->element ;

			if ( counter < 4 ) {
				LACMemoryFrameAdd ( (void*)anl->name , vn[counter] ) ;
				LACAdd ( vn[counter] , LAC_L_DELT , -1 ) ;
			} else {
				LACMemoryFrameAdd ( (void*)anl->name , vn[counter] ) ;
			}
				
		}
		
	}
	
	LACAdd ( ")\r\n" , LAC_CR , -1 ) ;
	LACAdd ( "{\r\n" , LAC_CR , -1 ) ;
	
	PUSH_LACGENTOR_LGA(lgnosia) ;
	SET_LACGENTOR_SCOPE(ISA_FUNCTION);
	SET_LACGENTOR_DELT(0);
	LACIdentorPush () ;
	//	if this function has local variables
	//	we initialize the MF here
	if ( 0 < azonal->layer ) {
		LACMemoryFrameInit ( azonal->layer ) ;
		LACAdd ( lacgentor_get_identor () , -1 , -1 ) ;
		LACAdd ( "%$MEMORY." , -1 , -1 ) ;
		LACAdd ( SCClItoa (azonal->layer) , -1 , -1 ) ;
		LACAdd ("\r\n",LAC_CR,-1);
	}
	for ( listlooper = lgnosia->context.head ; listlooper ; listlooper = listlooper->next ) {
		lacgentor_switcher ( (LGNOSIA*)listlooper->element ) ;		
	}
	LACIdentorPop () ;
	UNSET_LACGENTOR_DELT();
	UNSET_LACGENTOR_SCOPE();
	POP_LACGENTOR_LGA() ;
	
	LACAdd ( "}" , LAC_CR , -1 ) ;
	LACAdd ("\r\n",LAC_CR,-1);
	
	SCFree ( (void* )lacgentor.lgnosia ) ;

}

void lacgentor_gen_ifcf ( LGNOSIA* lgnosia , AZONAL* azonal ) {
	
	//	author : Jelo Wang
	//	since : 20100130
	//	(C)TOK
	
	int lac = 0 ;
	static int ifflow_deep = 0 ;
	LGNOSIA* lgnosiawalker = 0 ;
	SCClList* parameter = 0 ;

	PUSH_LACGENTOR_LGA(lgnosia) ;
	SET_LACGENTOR_SCOPE(ISA_IFCF);

	SET_LACGENTOR_DELT(0);
	LACIdentorPush () ;
	parameter = lgnosia->parameter.head ;
	lacgentor_gen_expr ( (EXPR*)parameter->element , 1 ) ;
	LACIdentorPop () ;
	UNSET_LACGENTOR_DELT();

	LACAdd ( sc_strcat ( lacgentor_get_identor () , "if ( " ) , LAC_IF , -1 ) ;
	LACAdd ( ((EXPR*)parameter->element)->delt , LAC_R_DELT , lacgentor.identor.deep ) ;

	lac = LACAdd ( " ) FALSE goto " , -1 , -1 ) ;
	LACAdd ( "\r\n" , LAC_CR , -1 );

	//	push lga and lac 
	//	for if-flow label generating
	SCClStackPush ( &LabelStack , (void* )lac ) ;
	SCClStackPush ( &LabelStack , (void* )lgnosia ) ;
	
	ifflow_deep ++ ;

	SET_LACGENTOR_DELT(0);
	LACIdentorPush () ;
	//	generate po_chain
	lacgentor_gen_pochain ( lgnosia->po_chain ) ;
	LACIdentorPop () ;
	UNSET_LACGENTOR_DELT();
	
	{

		//	checking NEC
		//	if NEC exits make a label for it
		LGNOSIA* lga = (LGNOSIA* ) SCClStackPop ( &LabelStack ) ;
		int lacc = SCClStackPop ( &LabelStack ) ;
		
		if ( lga && lga->ne_chain ) {
			LACAddLabel ( lac , lacgentor_head_label (lacgentor.label) ) ;
		} else {
			LACLabelMoiAdd ( lacc ) ;
		}
		
	}

	LACLabelMoiAdd ( LACAdd ( sc_strcat ( lacgentor_get_identor () , "goto ") , -1 , -1 ) ) ;
	LACAdd ( "\r\n" , LAC_CR , -1 );
	
	SET_LACGENTOR_DELT(0);
	LACIdentorPush () ;		
	//	generate ne_chain	
	lacgentor_gen_nechain ( lgnosia->ne_chain ) ;
	LACIdentorPop () ;
	UNSET_LACGENTOR_DELT();

	ifflow_deep -- ;
	//	确保当前节点为控制链最后一个节点
	if ( 0 == ifflow_deep ) {
		//	set label
		LACLabelMoiSetLabel ( lacgentor_head_label (lacgentor.label) ) ;
	}

	UNSET_LACGENTOR_SCOPE();
	POP_LACGENTOR_LGA() ;

	SCFree ( lgnosia ) ;



}


void lacgentor_gen_elseifcf ( LGNOSIA* lgnosia , AZONAL* azonal ) {
	
	//	author : Jelo Wang
	//	since : 20100811
	//	(C)TOK
	
	int lac = 0 ;
	static int ifflow_deep = 0 ;
	LGNOSIA* lgnosiawalker = 0 ;
	SCClList* parameter = 0 ;

	PUSH_LACGENTOR_LGA(lgnosia) ;
	SET_LACGENTOR_SCOPE(ISA_ELSEIFCF);

	parameter = lgnosia->parameter.head ;
	lacgentor_gen_expr ( (EXPR*)parameter->element , 1 ) ;
		
	LACAdd ( sc_strcat ( lacgentor_get_identor () , "if ( " ) , LAC_IF , -1  ) ;
	LACAdd ( ((EXPR*)parameter->element)->delt , LAC_R_DELT , lacgentor.identor.deep ) ;

	lac = LACAdd ( " ) FALSE goto " , -1 , -1 ) ;
	LACAdd ( "\r\n" , LAC_CR , -1 );

	//	push lga and lac 
	//	for if-flow label generating
	SCClStackPush ( &LabelStack , (void* )lac ) ;
	SCClStackPush ( &LabelStack , (void* )lgnosia ) ;

//	SET_LACGENTOR_DELT(0);
//	LACIdentorPush () ;			
	//	generate po_chain
	lacgentor_gen_pochain ( lgnosia->po_chain ) ;
//	LACIdentorPop () ;
//	UNSET_LACGENTOR_DELT();

	{

		LGNOSIA* lga = (LGNOSIA* ) SCClStackPop ( &LabelStack ) ;
		int lacc = SCClStackPop ( &LabelStack ) ;
		
		if ( lga && lga->ne_chain ) {
			
			LACAddLabel ( lac , lacgentor_head_label (lacgentor.label) ) ;

		} else {
			LACLabelMoiAdd ( lacc ) ;
		}
		
	}

	LACLabelMoiAdd ( LACAdd ( sc_strcat ( lacgentor_get_identor () , "goto ") , -1 , -1 ) ) ;
	LACAdd ( "\r\n" , LAC_CR , -1 );
	
//	SET_LACGENTOR_DELT(0);
//	LACIdentorPush () ;		
	//	generate ne_chain	
	lacgentor_gen_nechain ( lgnosia->ne_chain ) ;
//	LACIdentorPop () ;
//	UNSET_LACGENTOR_DELT();

	//	确保当前节点为控制链最后一个节点
	if ( 0 == ifflow_deep ) {
		//	set label
		LACLabelMoiSetLabel ( lacgentor_head_label (lacgentor.label) ) ;
	}

	UNSET_LACGENTOR_SCOPE();
	POP_LACGENTOR_LGA() ;

	SCFree ( lgnosia ) ;
	
	
}


void lacgentor_gen_else ( LGNOSIA* lgnosia , AZONAL* azonal ) {
	
	//	author : Jelo Wang
	//	since : 20100502
	//	(C)TOK
	
	int lac = 0 ;

	SCClList* listlooper = 0 ;

	PUSH_LACGENTOR_LGA(lgnosia) ;
	SET_LACGENTOR_SCOPE(ISA_ELSECF);
	
	LACAdd ( sc_strcat ( lacgentor_get_identor () , "// else-flow\r\n" ) , LAC_CR , -1 ) ;
		
	for ( listlooper = lgnosia->context.head ; listlooper ; listlooper = listlooper->next ) {
			
		lacgentor_switcher ( (LGNOSIA*)listlooper->element ) ;
			
	}

	LACLabelMoiAdd ( LACAdd ( sc_strcat ( lacgentor_get_identor () , "goto ") , -1 , -1 ) ) ;
	LACAdd ( "\r\n" , LAC_CR , -1 );

	//	release the LGA
	SCFree ( lgnosia ) ;	

	UNSET_LACGENTOR_SCOPE();
	POP_LACGENTOR_LGA() ;

	SCFree ( lgnosia ) ;


}


void lacgentor_gen_while ( LGNOSIA* lgnosia , AZONAL* azonal ) {
	
	//	author : Jelo Wang
	//	since : 20100502
	//	(C)TOK

	char* entry_label = 0 ;
	SCClList* listlooper = 0 ;
	SCClList* parameter = 0 ;
	int lac = 0 ;

	PUSH_LACGENTOR_LGA(lgnosia) ;
	SET_LACGENTOR_DELT(0);
	parameter = lgnosia->parameter.head ;
	lacgentor_gen_expr ( (EXPR*)parameter->element , 0 ) ;
	UNSET_LACGENTOR_DELT();

	entry_label = lacgentor_get_label () ;
	LACAdd ( entry_label , -1 , -1 );
	LACAdd ( "\r\n" , LAC_CR , -1 );		

	LACAdd ( sc_strcat ( lacgentor_get_identor () , "while ( " ) , -1 , -1 ) ;
	LACAdd ( ((EXPR*)parameter->element)->delt , LAC_R_DELT , lacgentor.identor.deep ) ;
	lac = LACAdd ( " ) FALSE goto " , -1 , -1 ) ;
	LACAdd ( "\r\n" , LAC_CR , -1 );
	
	LACLabelMoiAdd ( lac ) ;

	SET_LACGENTOR_DELT(0);
	LACIdentorPush () ;

	for ( listlooper = lgnosia->context.head ; listlooper ; listlooper = listlooper->next ) {	
		lacgentor_switcher ( (LGNOSIA*)listlooper->element ) ;
	}

	lacgentor_gen_expr ( (EXPR*)parameter->head , 1 ) ;
	LACAdd ( sc_strcat ( lacgentor_get_identor () , "goto " ) , -1 , -1 ) ;
	LACAdd ( entry_label , -1 , -1 ) ;
	LACAdd ( "\r\n" , LAC_CR , -1 );

	SCFreeEx ( &((EXPR*)parameter->element)->delt ) ;
	SCFreeEx ( (void**)&parameter->element ) ;	
	
	LACIdentorPop () ;
	UNSET_LACGENTOR_DELT();
	
	LACLabelMoiSetLabel ( lacgentor_head_label (lacgentor.label) ) ;
	POP_LACGENTOR_LGA() ;

	SCFree ( lgnosia ) ;


}



void lacgentor_switcher ( LGNOSIA* lgnosia ) {

	//	author : Jelo Wang
	//	since : 20100130
	//	(C)TOK

	AZONAL* azonal = (AZONAL* ) lgnosia->azonal ;
	
	if ( !azonal ) return ;
		
	LACAdd ( lacgentor_get_label() , -1 , -1 );
	LACAdd ( "\r\n" , LAC_CR , -1 ) ;

	switch ( azonal->azonaltype ) {
		
		case ISA_IFCF :
			lacgentor_gen_ifcf ( lgnosia , azonal ) ;
		break ;
		case ISA_ELSEIFCF :
			lacgentor_gen_elseifcf ( lgnosia , azonal ) ;
		break ;
		case ISA_ELSECF :
			lacgentor_gen_else ( lgnosia , azonal ) ;
		break ;
		case ISA_WHILECF :
			lacgentor_gen_while ( lgnosia , azonal ) ;
		break ;

		case ISA_VARIABLE :
			lacgentor_gen_variable ( lgnosia , azonal ) ;
		break ;
		case ISA_FUNCCAL :
			lacgentor_gen_funccal ( lgnosia , azonal ) ;			
		break ;	

	}


}

void lacgentor_gen_variable ( LGNOSIA* lgnosia , AZONAL* azonal ) {

	//	author : Jelo Wang
	//	since : 20100425
	//	(C)TOK
	
	EXPR* expression = 0 ;

	if ( !lgnosia->parameter.head ) {

		char* name = SymboleDRCAdd ( azonal , lacgentor.identor.deep , GET_LACGENTOR_SCOPE() , GET_LACGENTOR_LGA() ) ;

//		LACAdd ( lacgentor_get_identor () ) ;
//		LACAdd ( "%$STACK 4 " ) ;	
//		LACAdd ( name ) ;		
//		LACAdd ( " = 0 ;\r\n" ) ;		
		SCFree ( (void*)name ) ;

	} else {

		char* name = 0 ;
			
		expression = (EXPR*)lgnosia->parameter.head->element ;

		lacgentor_gen_expr ( (EXPR*)expression , 1 ) ;

		name = SymboleDRCAdd ( azonal , lacgentor.identor.deep , GET_LACGENTOR_SCOPE() , GET_LACGENTOR_LGA() ) ;		
		SymboleDRCDropCFF ( azonal ) ;
		
		LACAdd ( lacgentor_get_identor () , -1 , -1 ) ;

		LACAdd ( name , LAC_L_DELT , lacgentor.identor.deep ) ;
		LACAdd ( " = " , -1 , -1 ) ;

		if ( expression->delttype == EXP_DELT_ANLNUMERIC ) 
			LACAdd ( ((EXPR*)expression)->delt , -1 , -1 ) ;
		else 
			LACAdd ( ((EXPR*)expression)->delt , LAC_R_DELT , lacgentor.identor.deep ) ;
		
		LACAdd ( " ;\r\n" , LAC_CR , -1 ) ;

		SCFreeEx ( &((EXPR*)expression)->delt ) ;
		SCFreeEx ( (void**)&expression ) ;
		SCFree ( (void*)name ) ;
		
	}	

	SCFree ( lgnosia ) ;

}

static void lacgentor_gen_calltransfer ( AZONAL* azonal , int type ) {

	//	author : Jelo Wang
	//	since : 20110223
	//	(C)TOK

	//	generate __stdcall,__cdecl,__fastcall,__armcall pattern

	int counter = 0 ;
	
	//	before calling
	if ( 0 == type ) {

		if ( SC_C0 & compiler->parameter ) {

			//	for __stdcall type			
			LACAdd ( lacgentor_get_identor () , -1 , -1 ) ;
			LACAdd ( "%$STACK INIT " , -1 , -1 ) ;
			LACAdd ( SCClItoa ( azonal->tack.totall ) , -1 , -1 ) ; 
			LACAdd ( " ;\r\n" , LAC_CR , -1 ) ;
		
		} else if ( SC_C3 & compiler->parameter ) {
		
			//	for __armcall type						
			LACAdd ( lacgentor_get_identor () , -1 , -1 ) ;
//			LACAdd ( "%$STACK INIT " , -1 , -1 ) ;
//			LACAdd ( SCClItoa ( azonal->tack.totall ) , -1 , -1 ) ; 
//			LACAdd ( " ;\r\n" , LAC_CR , -1 ) ;

			if ( 5 > azonal->tack.totall ) {
				AZONAL* looper = azonal->tack.head ;
				while ( looper ) {
					LACAdd ( "%$V1 = " , -1 , -1 ) ;
					LACAdd ( looper->name , -1 , -1 ) ;
					LACAdd ( " ;\r\n" , LAC_CR , -1 ) ;
					looper = looper->next ;
				}
			}
		
		}

		
 	} else if ( 1 == type ) {

		//	after calling
		if ( SC_C3 & compiler->parameter ) {
		
			//	for __armcall type						
			LACAdd ( lacgentor_get_identor () , -1 , -1 ) ;
			LACAdd ( "%$STACK UNIT " , -1 , -1 ) ;
			LACAdd ( SCClItoa ( azonal->tack.totall ) , -1 , -1 ) ; 
			LACAdd ( " ;\r\n" , LAC_CR , -1 ) ;
		
		}
		
	}

}

void lacgentor_gen_funccal ( LGNOSIA* lgnosia , AZONAL* azonal ) {

	//	author : Jelo Wang
	//	since : 20110212
	//	(C)TOK

	char* name = 0 ;
	char* vn [ 4 ] = { "%$V1" , "%$V2" , "%$V3" , "%$V4" } ;

	int walker = 0 ;
	int counter = 0 ;

	SCClList* looper = 0 ;

	//	backup V1,V2,V3,V4
	for ( walker = 0 ; walker < azonal->tack.totall && 0 != walker-4 ; walker ++ ) {
		LACAdd ( lacgentor_get_identor () , -1 , -1 ) ; 											
		LACAdd ( "%$STACK IN " , -1 , -1 ) ;				
		LACAdd ( vn[walker] , -1 , -1 ) ;				
		LACAdd ( " ;\r\n" , LAC_CR , -1 ) ; 						
	}
	
	//	generate __armcall pattern
	if ( SC_C3 & compiler->parameter ) {
		
		//	totall parameters is <= 4
		if ( 4 >= azonal->tack.totall ) {			
			
			for ( walker = 0 , counter = 0 , looper = lgnosia->context.head ; looper ; looper = looper->next , counter ++ ) {

				LGNOSIA* lga = (LGNOSIA* ) looper->element ;
				AZONAL* anl = (AZONAL* ) lga->azonal ;
				
				LACAdd ( lacgentor_get_identor () , -1 , -1 ) ;	

				LACAdd ( vn[walker] , -1 , -1 ) ;				
				LACAdd ( " = " , -1 , -1 ) ;

				name = SymboleDRCGetDRC ( anl , lacgentor.identor.deep , GET_LACGENTOR_LGA() ) ;
				LACAdd ( name , LAC_R_DELT , lacgentor.identor.deep ) ;		
				SCFree ( name ) ;
				LACAdd ( " ;\r\n" , LAC_CR , -1 ) ;			

				
			}

			LACAdd ( lacgentor_get_identor () , -1 , -1 ) ; 
			LACAdd ( "%$CA " , -1 , -1 ) ;
			LACAdd ( azonal->name , LAC_P_CALL , lacgentor.identor.deep ) ;
			LACAdd ( " ;\r\n" , LAC_CR , -1 ) ;
			
		} else if ( 4 < azonal->tack.totall ) {

			//	skip REGSITER usage
			for ( counter = 0 , looper = lgnosia->context.head ; counter < 4 ; counter ++ ) {
				looper = looper->next ;
			}

			//	generate STACK usage
			LACAdd ( lacgentor_get_identor () , -1 , -1 ) ; 
			LACAdd ( "%$STACK INIT " , -1 , -1 ) ;
			LACAdd ( SCClItoa ( azonal->tack.totall - counter ) , -1 , -1 ) ;
			LACAdd ( " ;\r\n" , LAC_CR , -1 ) ;
			
			for ( ; looper ; looper = looper->next ) {
			
				LGNOSIA* lga = (LGNOSIA* ) looper->element ;
				AZONAL* anl = (AZONAL* ) lga->azonal ;				
			
				LACAdd ( lacgentor_get_identor () , -1 , -1 ) ; 								
				LACAdd ( "%$STACK IN " , -1 , -1 ) ;
				name = SymboleDRCGetDRC ( anl , lacgentor.identor.deep , GET_LACGENTOR_LGA() ) ;
				LACAdd ( name , LAC_R_DELT , lacgentor.identor.deep ) ; 	
				LACAdd ( " ;\r\n" , LAC_CR , -1 ) ; 		
				SCFree ( name ) ;
				
			}
			//	end her			

			for ( counter = 0 , looper = lgnosia->context.head ; looper && counter < 4 ; looper = looper->next , counter ++ ) {

				LGNOSIA* lga = (LGNOSIA* ) looper->element ;
				AZONAL* anl = (AZONAL* ) lga->azonal ;
				
				LACAdd ( lacgentor_get_identor () , -1 , -1 ) ;	
				LACAdd ( vn[counter] , -1 , -1 ) ;				
				LACAdd ( " = " , -1 , -1 ) ;

				name = SymboleDRCGetDRC ( anl , lacgentor.identor.deep , GET_LACGENTOR_LGA() ) ;
				LACAdd ( name , LAC_R_DELT , lacgentor.identor.deep ) ;		
				LACAdd ( " ;\r\n" , LAC_CR , -1 ) ;			
//				SCFree ( name ) ;
				
			}
			
			LACAdd ( lacgentor_get_identor () , -1 , -1 ) ; 
			LACAdd ( "%$CA " , -1 , -1 ) ;
			LACAdd ( azonal->name , LAC_P_CALL , lacgentor.identor.deep ) ;
			LACAdd ( " ;\r\n" , LAC_CR , -1 ) ;
			
			LACAdd ( lacgentor_get_identor () , -1 , -1 ) ; 								
			LACAdd ( "%$STACK UNIT " , -1 , -1 ) ;
			LACAdd ( SCClItoa ( azonal->tack.totall - counter ) , -1 , -1 ) ;
			LACAdd ( " ;\r\n" , LAC_CR , -1 ) ;
			
		}

	}

	for ( walker = 0 ; walker < azonal->tack.totall && 0 != walker-4 ; walker ++ ) {
		LACAdd ( lacgentor_get_identor () , -1 , -1 ) ; 							
		LACAdd ( "%$STACK OUT " , -1 , -1 ) ; 	 		
		LACAdd ( vn[walker] , -1 , -1 ) ;				
		LACAdd ( " ;\r\n" , LAC_CR , -1 ) ; 						
	}
	

}

int lacgentor_get_exp_deep ( EXPR* expression , EXPR* father , int deep , int* totall ) {

	//	author : Jelo Wang
	//	since : 20100802
	//	(C)TOK

	int ld = 0 ;
	int rd = 0 ;
	
	if ( !expression ) {
		return deep ;
	}

	*totall = *totall + 1 ;

	expression->deep = deep ;
	expression->father = father ;
	
	ld = lacgentor_get_exp_deep ( expression->left , expression , deep+1 , totall ) ; 
	rd = lacgentor_get_exp_deep ( expression->right , expression , deep+1 , totall ) ;
	
	return ld  > rd ? ld  : rd ;
	
}


int lacgentor_gen_expr ( EXPR* expression , int drop ) {

	//	author : Jelo Wang
	//	since : 20100425
	//	(C)TOK

	int recursived = 0 ;
	
	if ( !expression ) {
		return 0 ;
	}
	
	if ( expression->left ) {
		recursived = 1 ;
		lacgentor_gen_expr ( expression->left , drop ) ;
	}

	if ( expression->right ) {
		recursived = 1 ;		
		lacgentor_gen_expr ( expression->right , drop ) ;
	}

	if ( EXP_OPERAND == expression->type ) {

		char* name = 0 ; 
		AZONAL* azonal = expression->handle ;

		//	if this azonal is not a parameter
		//	else it would be in LAC-CALL-FRAME
		if ( 0 == azonal->isparam ) {
			
	 		if ( ISA_INTEGER != azonal->azonaltype ) {
				name = SymboleDRCGetDRC ( azonal , lacgentor.identor.deep , GET_LACGENTOR_LGA() ) ;
				expression->delt = (char* ) SCMalloc ( sc_strlen (name) + 1 ) ;
				sc_strcpy ( expression->delt  , name ) ;
				SCFree ( name ) ;
			} else {		
				expression->delt = (char* ) SCMalloc ( sc_strlen (azonal->name) + 1 ) ;
				sc_strcpy ( expression->delt  , azonal->name ) ;
			}

			if ( ISA_INTEGER == azonal->azonaltype )
				expression->delttype = EXP_DELT_ANLNUMERIC ;
			else 
				expression->delttype = EXP_DELT_ANLDATA ;

		} else {
//			char* frame = LACMemoryFrameGet ( azonal->name ) ;
//			//	get LAC-CALL-FRAME
//			expression->delt = (char* ) SCMalloc ( sc_strlen (frame) + 1 ) ;
//			sc_strcpy ( expression->delt  , frame ) ;		
//			expression->delttype = EXP_DELT_ANLDATA ;			
		}
		
		
	} else if ( EXP_OPERATOR == expression->type ) {

		char* tvalue = 0 ;
		
		LACAdd ( lacgentor_get_identor () , -1 , -1 ) ;

		tvalue = lacgentor_get_register (lacgentor.delt+1) ;
		expression->delt = (char* ) SCMalloc ( sc_strlen(tvalue) + 1 ) ;
		sc_strcpy ( expression->delt  , tvalue ) ;

		expression->delttype = EXP_DELT_DEFAULT ;
		
		LACAdd ( expression->delt , LAC_L_DELT , lacgentor.identor.deep ) ;
		LACAdd ( " = " , -1 , -1 ) ;
		LACAdd ( expression->left->delt , LAC_R_DELT , lacgentor.identor.deep ) ;
		LACAdd ( lacgentor_gen_operator(expression->handle) , -1 , -1 ) ;
		LACAdd ( expression->right->delt , LAC_R_DELT , lacgentor.identor.deep ) ;
		LACAdd ( ";\r\n" , LAC_CR , -1 ) ;

		lacgentor.delt = lacgentor.delt + 1 ;
		
		if ( (EXP_DELT_DEFAULT == expression->left->delttype) && (EXP_DELT_DEFAULT == expression->right->delttype) ) {
			lacgentor.delt = 0 ;
		}

		if ( drop ) {
			SCFreeEx ( &expression->left->delt ) ;
			SCFreeEx ( &expression->right->delt ) ;
			SCFreeEx ( &expression->left ) ;
			SCFreeEx ( &expression->right ) ;
		}
		
	}

	return 1 ;
	
}

void lacgentor_gen_pochain ( LGNOSIA* po_chain ) {

	//	author : Jelo Wang
	//	since : 20100719
	//	(C)TOK

	//	gen po_chain		

	if ( po_chain ) {
	
		SCClList* listlooper = 0 ;

		for ( listlooper = po_chain->context.head ; listlooper ; listlooper = listlooper->next ) {
	
			lacgentor_switcher ( (LGNOSIA* )listlooper->element ) ;
	
		}

	} 

}

void lacgentor_gen_nechain ( LGNOSIA* ne_chain ) {

	//	author : Jelo Wang
	//	since : 20100719
	//	(C)TOK

	//	gen ne_chain

	if ( ne_chain ) {
		
		SCClList* listlooper = 0 ;
	
		for ( listlooper = ne_chain->context.head ; listlooper ; listlooper = listlooper->next ) {
		
			lacgentor_switcher ( (LGNOSIA* )listlooper->element ) ;
		
		}

	} 


}

char* gentor_lac_run ( char* lacfile ) {

	//	author : Jelo Wang
	//	since : 20100107
	//	(C)TOK

	static int runtimes = 0 ;

	char* lac = 0 ;
	//	Create LAC Pool
	int lactex = LACNew () ;
	int LabelMoiGlobal = 0 ;

	int lgay = 120 ;

	LACSetContext ( lactex ) ;
	//	generate Global LabelMoi
	LabelMoiGlobal = LACLabelMoiNew () ;
	//	Set Current Monitor
	LACLabelMoiSetContext ( LabelMoiGlobal ) ;

	//	Initialize Lac Gentor
	lacgentor_ready () ; 

	LACAdd ( "# Codes Generated As Semo Compiler 0.3.0\r\n" , -1 , -1 ) ;
	LACAdd ( "# Techniques of Knowledge\r\n" , -1 , -1 ) ;
	LACAdd ( "# 突壳开源\r\n\r\n" , -1 , -1 ) ;	
	
	//	use compiling-render
	if ( SC_CR & compiler->parameter ) {

		CORENRCreatePanel ( CORENR_RGB565 , 2048 , 2048 ) ;
		CORENRDrawText ( 15 , 10 , "Semo C()mpiler Compiling-Render" , 200 , 200 , 200 ) ;	
		CORENRDrawText ( 15 , 10+14 , "(C)Techniques of Knowledge" , 200 , 200 , 200 ) ;	
		CORENRDrawText ( 15 , 10+14+14 , "   http://www.tok.cc" , 200 , 200 , 200 ) ;	

	}
	
	while ( !lacgentor.stop ) {
		
		lacgentor_genv () ;

		switch ( lacgentor.form ) {
			
			case LGNOSIA_TOP_IDENT :
				
				if ( SC_CR & compiler->parameter ) {		

					if ( SC_EXP & compiler->parameter )
						lgay = CORENRLgaExpRender ( lacgentor.lgnosia , LGNOSIA_TOP_IDENT , 200 , lgay ) ;	
					if ( SC_LGA & compiler->parameter )
						CORENRCFDFSRender ( lacgentor.lgnosia , 0 , 1024 , lgay , 1024 , lgay , 1 ) ;
	 				//CORENRCFBFSRender ( corenr , lacgentor.lgnosia , 200 , lgay ) ;
				}
				
				lacgentor_gen_funcdef () ; 
				
			break ;
			
		}

		LgnosiaDestroy ( lacgentor.lgnosia ) ;		
		lacgentor_next () ;

	}

	//	generate live-scope and start register allocation
	//if ( SC_ARM & compiler->parameter ) 
	//	LACLiveScopeGenerate ( 8 ) ;
	
	//	Get Lac Codes
	lac = LACGetContent () ;
	LACClear () ;

	if ( SC_CR & compiler->parameter ) {
		char* file = sc_strcat ( lacfile , ".cr.bmp" ) ;		
		CORENROutputPanel ( file ) ;
		CORENRDestroyPanel () ;
		SCFree ( file ) ;
	}

	if ( SC_LAC & compiler->parameter ) {
		void* file = SCHalFileOpen ( lacfile , "wb+" ) ;
		SCHalFileWrite ( file , lac , 1 , sc_strlen (lac) ) ;
		SCHalFileClose ( file ) ;
		//	release it here
		SCFree ( lacfile ) ;
	}

	SymboleUninstall () ;

	return lac ;

}

