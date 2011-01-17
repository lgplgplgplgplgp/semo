
/*

+	LAC Form Generator , part of SC mid-para

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
# include "sc.h"
# include "schal.h"
# include "sccl.h"
# include "c-grammar.h"
# include "symbole.h"
# include "evalor.h"
# include "lgnosia.h"
# include "lac-gentor.h"
# include "lac.h"
# include "mopo.h"

SCClStack LabelStack = { 0 , 0 , 0 , 0 , 0 } ;
SCClString* lacgentor_results = 0 ;
LAC_GENTOR lacgentor = { -1 , 0 , 0 , 0 , 0 , 0 , 0 } ;

int mopo = 0 ;

# define LACGENTOR_STATUS_RESET()\
	lacgentor.form = -1;\

# define SET_LACGENTOR_DELT(deltn)\
	SCClStackPush ( &lacgentor.deltstack, lacgentor.delt );\
	lacgentor.delt = deltn;\

# define UNSET_LACGENTOR_DELT()\
	lacgentor.delt = SCClStackPop ( &lacgentor.deltstack ) ;\

# define GET_LACGENTOR_SCOPE() SCClStackGet( &lacgentor.scopestack )

# define SET_LACGENTOR_SCOPE(sscope)\
	SCClStackPush ( &lacgentor.scopestack , sscope ) ;\

# define UNSET_LACGENTOR_SCOPE()\
	SCClStackPop ( &lacgentor.scopestack ) ;\

# define PUSH_LACGENTOR_LGA(lga)\
	SCClStackPush ( &lacgentor.lgastack  , lga ) ;\
	
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
		return 6 ;

		case C_HU :	return "|" ;

		case C_AND : return "&&" ;

		case C_OR :	return "||" ;

	}

}

static void LACIdentorPush () {

	//	author : Jelo Wang
	//	since : 20100130
	//	(C)TOK

	SCClStackPush ( &lacgentor.identor , 1 ) ;
	
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

	static char tempv [64] = {"%$R."} ;
	
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
	
	SCClStackInit ( &lacgentor.identor ) ;
	SCClStackInit ( &lacgentor.deltstack ) ;

	//	get LGA tree
	lacgentor.tree = (SCClList* ) LgnosiaGet () ;

	lacgentor.lgnosia = (LGNOSIA* ) lacgentor.tree->element ;

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

	lacgentor.lgnosia = (LGNOSIA* ) lacgentor.tree->element ;

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

	LACAdd ( azonal->name ) ;
	LACAdd ( "(" ) ;

	//	gen para
	
	LACAdd ( ")\r\n" ) ;
	LACAdd ( "{\r\n" ) ;

	PUSH_LACGENTOR_LGA(lgnosia) ;
	SET_LACGENTOR_SCOPE(ISA_FUNCTION);
	SET_LACGENTOR_DELT(0);
	LACIdentorPush () ;
	for ( listlooper = lgnosia->context.head ; listlooper ; listlooper = listlooper->next ) {

		lacgentor_switcher ( (LGNOSIA*)listlooper->element ) ;
		
	}
	LACIdentorPop () ;
	UNSET_LACGENTOR_DELT();
	UNSET_LACGENTOR_SCOPE();
	POP_LACGENTOR_LGA() ;
	
	LACAdd ( "}\r\n" ) ;

	SCFree ( lacgentor.lgnosia ) ;

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

	//	scope header
	LACAdd ( "#>\r\n" ) ;	
	LACAdd ( sc_strcat ( lacgentor_get_identor () , "if ( " ) ) ;
	LACAdd ( ((EXPR*)parameter->element)->delt ) ;

	lac = LACAdd ( " ) FALSE goto " ) ;
	LACAdd ( "\r\n" );

	//	push lga and lac 
	//	for if-flow label generating
	SCClStackPush ( &LabelStack , lac ) ;
	SCClStackPush ( &LabelStack , lgnosia ) ;
	
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

	LACLabelMoiAdd ( LACAdd ( sc_strcat ( lacgentor_get_identor () , "goto ") ) ) ;
	LACAdd ( "\r\n" );
	
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

	//	scope footer	
	LACAdd ( "#<\r\n" ) ;


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
		
	LACAdd ( sc_strcat ( lacgentor_get_identor () , "if ( " ) ) ;
	LACAdd ( ((EXPR*)parameter->element)->delt ) ;

	lac = LACAdd ( " ) FALSE goto " ) ;
	LACAdd ( "\r\n" );

	//	push lga and lac 
	//	for if-flow label generating
	SCClStackPush ( &LabelStack , lac ) ;
	SCClStackPush ( &LabelStack , lgnosia ) ;

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

	LACLabelMoiAdd ( LACAdd ( sc_strcat ( lacgentor_get_identor () , "goto ") ) ) ;
	LACAdd ( "\r\n" );
	
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
	
	LACAdd ( sc_strcat ( lacgentor_get_identor () , "// else-flow\r\n" ) ) ;
		
	for ( listlooper = lgnosia->context.head ; listlooper ; listlooper = listlooper->next ) {
			
		lacgentor_switcher ( (LGNOSIA*)listlooper->element ) ;
			
	}

	LACLabelMoiAdd ( LACAdd ( sc_strcat ( lacgentor_get_identor () , "goto ") ) ) ;
	LACAdd ( "\r\n" );

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
	LACAdd ( entry_label );
	LACAdd ( "\r\n" );		

	LACAdd ( sc_strcat ( lacgentor_get_identor () , "while ( " ) ) ;
	LACAdd ( ((EXPR*)parameter->element)->delt ) ;
	lac = LACAdd ( " ) FALSE goto " ) ;
	LACAdd ( "\r\n" );
	
	LACLabelMoiAdd ( lac ) ;

	SET_LACGENTOR_DELT(0);
	LACIdentorPush () ;

	for ( listlooper = lgnosia->context.head ; listlooper ; listlooper = listlooper->next ) {	
		lacgentor_switcher ( (LGNOSIA*)listlooper->element ) ;
	}

	lacgentor_gen_expr ( (EXPR*)parameter->head , 1 ) ;
	LACAdd ( sc_strcat ( lacgentor_get_identor () , "goto " ) ) ;
	LACAdd ( entry_label ) ;
	LACAdd ( "\r\n" );

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
		
	LACAdd ( lacgentor_get_label() );
	LACAdd ( "\r\n" ) ;

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
		
		LACAdd ( lacgentor_get_identor () ) ;

		LACAdd ( name ) ;
		LACAdd ( " = " ) ;
		LACAdd ( ((EXPR*)expression)->delt ) ;
		LACAdd ( " ;\r\n" ) ;

		SCFreeEx ( &((EXPR*)expression)->delt ) ;
		SCFreeEx ( (void**)&expression ) ;
		SCFree ( (void*)name ) ;
		
	}	

	SCFree ( lgnosia ) ;

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
	
	if ( !expression ) {
		return 0 ;
	}
	
	if ( expression->left ) {
		lacgentor_gen_expr ( expression->left , drop ) ;
	}

	if ( expression->right ) {
		lacgentor_gen_expr ( expression->right , drop ) ;
	}

	if ( EXP_OPERAND == expression->type ) {

		AZONAL* azonal = expression->handle ;
		
		char* name = SymboleDRCGetDRC ( azonal , lacgentor.identor.deep , GET_LACGENTOR_LGA() ) ;
		
		expression->delt = (char* ) SCMalloc ( name + 1 ) ;
		sc_strcpy ( expression->delt  , name ) ;
		expression->delttype = EXP_DELT_AZONAL ;

//		SCFree ( name ) ;
	
		
	} else if ( EXP_OPERATOR == expression->type ) {

		char* tvalue = 0 ;
		
		LACAdd ( lacgentor_get_identor () ) ;

		tvalue = lacgentor_get_register (lacgentor.delt+1) ;
		expression->delt = (char* ) SCMalloc ( sc_strlen(tvalue) + 1 ) ;
		sc_strcpy ( expression->delt  , tvalue ) ;

		expression->delttype = EXP_DELT_DEFAULT ;
		
		LACAdd ( expression->delt  ) ;
		LACAdd ( " = ") ;
		LACAdd ( expression->left->delt ) ;
		LACAdd ( lacgentor_gen_operator(expression->handle) ) ;
		LACAdd ( expression->right->delt ) ;
		LACAdd ( ";\r\n" ) ;

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
	
	//	use compiling-render
	if ( SC_CR & compiler->parameter ) {

		mopo = MOPOCreatePanel ( MOPO_RGB565 , 2048 , 2048 ) ;
		MOPODrawText ( mopo , 15 , 10 , "Sema Compiler Compiling-Render" , 200 , 200 , 200 ) ;	
		MOPODrawText ( mopo , 15 , 10+14 , "(C)Techniques of Knowledge" , 200 , 200 , 200 ) ;	
		MOPODrawText ( mopo , 15 , 10+14+14 , "   an open source group since 2008" , 200 , 200 , 200 ) ;	

	}
	
	while ( !lacgentor.stop ) {
		
		lacgentor_genv () ;

		switch ( lacgentor.form ) {
			
			case LGNOSIA_TOP_IDENT :
				
				if ( SC_CR & compiler->parameter ) {		

					if ( SC_EXP & compiler->parameter )
						lgay = MOPOLgaExpRender ( mopo , lacgentor.lgnosia , LGNOSIA_TOP_IDENT , 200 , lgay ) ;	
					if ( SC_LGA & compiler->parameter )
						MOPOCFDFSRender ( mopo , lacgentor.lgnosia , 0 , 1024 , lgay , 1024 , lgay , 1 ) ;
					//MOPOCFBFSRender ( mopo , lacgentor.lgnosia , 200 , lgay ) ;
				}
				
				lacgentor_gen_funcdef () ; 
				
			break ;
			
		}

		LgnosiaDestroy ( lacgentor.lgnosia ) ;
		
		lacgentor_next () ;

	}

	//	Get Lac Codes
	lac = LACGetContent () ;
	LACClear () ;

	if ( SC_CR & compiler->parameter ) {
		char* file = sc_strcat ( lacfile , ".cr.bmp" ) ;	
		MOPOOutputPanel ( mopo , file ) ;
		MOPODestroyPanel ( mopo ) ;
		SCFree ( file ) ;
	}
	
	if ( SC_LAC & compiler->parameter ) {
		int file = SCHalFileOpen ( lacfile , "wb+" ) ;
		SCHalFileWrite ( file , lac , 1 , sc_strlen (lac) ) ;
		SCHalFileClose ( file ) ;
		//	release it here
		SCFree ( lacfile ) ;
	}

	SymboleUninstall () ;

	return lac ;

}

