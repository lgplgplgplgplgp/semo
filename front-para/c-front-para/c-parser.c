
/*

+	PARSER , part of SC front-para

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

# include "c-errors.h"
# include "c-lexer.h"
# include "c-grammar.h"
# include "c-parser.h"
# include "evalor.h"
# include "sccl.h"
# include "schal.h"
# include "symbole.h"
# include "lgnosia.h"

PARSERC* parserc = 0 ;
SCClStack cfstack = { 0 , 0 , 0 , 0 , 0 } ;

# define NUMERIC_FLOW(x) ((x==C_INTNUM) ||(x==C_FLTNUM) ||(x==C_HEXNUM)||(x==C_INTENUM)||(x==C_FLTENUM))
# define OPERAND_FLOW(x) (NUMERIC_FLOW(x)||(x==C_VAR)||(x==C_ARRAY)||(x==C_FUNCCAL))

# define DUA_OPERATOR(x) (C_SUB <= x && x <= C_HE)
# define CMA_OPERATOR(x) ((x==C_DOU))

//	prior operator
# define PORL_OPERATOR(x) (x==C_XKL)
# define PORR_OPERATOR(x) (x==C_XKR)

# define RESET_PARSER()\
	parserc->head = -1 ;\
	parserc->form = -1 ;\
	parserc->scope = -1 ;\
	SCClStackDestroy ( &parserc->scopestack ) ;\

# define MATCH_PARSER_SCOPE(sscope)\
	(sscope==SCClStackGet( &parserc->scopestack ))

# define SET_PARSER_SCOPE(sscope)\
	SCClStackPush ( &parserc->scopestack , sscope ) ;\

# define UNSET_PARSER_SCOPE()\
	parserc->scope = SCClStackPop ( &parserc->scopestack ) ;\

# define SCOPER_PUSH()\
	parserc->stack ++ ;\

# define SCOPER_POP()\
	parserc->stack -- ;\

# define SCOPER_GET() parserc->stack

static void parserc_ready () {

	parserc = (PARSERC* ) SCMalloc ( sizeof(PARSERC) ) ;

	if ( !parserc ) {

		SCLog ("[sc][c-front-para][parserc] initialization has failed\n") ;
		SCTerminate () ;
		return 0 ;
		
	}
	
	parserc->form = -1 ;
	parserc->head = -1 ; 
	parserc->scope = 0 ;
	parserc->stop = 0 ;
	parserc->stack = 0 ;
	
	SCClStackInit ( &parserc->scopestack ) ;
	
}

static int parserc_genv () {

	//	author : Jelo Wang
	//	since : 20091128
	//	(C)TOK
	
	int state = 0 ;

	for ( ; !lexc->stop ; ) {

		switch ( state ) {
			
			case 0 :

				if ( is_define_head_flow ( lexc->v ) ) {
					parserc->head = lexc->v ;
					state = 0 ;
				} else if ( C_VAR == lexc->v ) {
					parserc->form = lexc->v ;
					return 1 ;
				} else if ( C_FUNCDEF == lexc -> v ) {
					parserc->form = lexc->v ;
					return 1 ;
				}

				lexerc_genv () ;
				
			break ;
			
		}
	}

}

static int parser_c_read_function () {

	//	author : Jelo Wang
	//	notes : read function 
	//	since : 20100116
	//	(C)TOK
	
	int stack = 0 ;

	AZONAL* azonal = 0 ;
	int lgnosia = 0 ;

	if ( C_FUNCDEF != lexc->v )
		return 0 ;

	azonal = SymboleFindFunction ( lexc->token ) ;
	
	if ( azonal ) cerror ( C_PARSER_MOD , IS_C_ERROR , "function '%s' multi-definited , line : %d\n" , lexc->token , lexc->line ) ;
	else azonal = SymboleAddFunction ( lexc->token , ISA_FUNCTION , parserc->head , lexc->line ) ;
	
	//	create a LGA
	lgnosia = LgnosiaNew ( (int) azonal , LGNOSIA_TOP_IDENT ) ;

	//	parsing parameter scope of function
	stack = 0 ;
 	SET_PARSER_SCOPE(PARSERC_SCOPE_PARAM);
	while ( !lexc->stop ) {
		
		lexerc_genv () ;
		
		if ( C_XKL == lexc->v )
			stack ++ ;
		if ( C_XKR == lexc->v )
			stack -- ;
		
		if ( 0 == stack ) 
			break ;

		parser_c_read_symbol_def () ;
		
	}
	UNSET_PARSER_SCOPE();
	
 	if ( lexc->stop || stack ) {
		cerror ( C_PARSER_MOD , IS_C_ERROR , "'()' is unmatched , line : %d\n" , lexc -> line ) ;
		RESET_PARSER () ;
		return 1 ;
	}
	
	LgnosiaStackPush ( (int) lgnosia ) ;
	
	//	parsing function body
	stack = 0 ;
	SCOPER_PUSH () ;
	while ( !lexc->stop ) {
		
		lexerc_genv () ;
		
		if ( C_DKL == lexc->v )
			stack ++ ;
		if ( C_DKR == lexc->v )
			stack -- ;
		
		if ( 0 == stack ) 
			break ;		
		
		parser_c_read_controlflow () ;
		parser_c_read_symbol_def () ;
		parser_c_read_symbol_inf () ;

	}

	LgnosiaStackPop () ;
	SCOPER_POP () ;
		
	if ( lexc->stop ) {
		cerror ( C_PARSER_MOD , IS_C_ERROR , "'{}' is unmatched , line : %d\n" , lexc->line ) ;
		RESET_PARSER () ;
		return 1 ;
	}

}


static int parser_c_read_if () {

	//	author : Jelo Wang
	//	since : 20100116
	//	(C)TOK

	int stack = 0 ;

	EVALOR* evalor = 0 ;
	AZONAL* azonal = 0 ;
	int lgnosia = 0 ;
	
	if ( C_IF != lexc->v ) 
		return 0 ;

	//	create control flow azonal
	azonal = SymboleAddControlAzonal ( ISA_IFCF , lexc->line ) ;
	
	//	create LGA
	lgnosia = LgnosiaNew ( (int)azonal , LGNOSIA_CP_IDENT ) ;
			
	LgnosiaAddContext ( LgnosiaStackTop () , (int) lgnosia , LGNOSIA_CP_IDENT ) ;
		
	SET_PARSER_SCOPE(PARSERC_SCOPE_PARAM);
	//	parsing parameter scope of controlflow
	//	Get an evalor
	evalor = EvalorCreate () ;
	EvalorSetCurrent ( evalor ) ;
	//	parsing EXPR flow
	parser_c_read_expr () ;
	EvalorFinish () ;
	
	//	add parameter for the LGA.
	LgnosiaAddParameter ( lgnosia , EvalorGetTree () , LGNOSIA_EXP_IDENT ) ;			
	UNSET_PARSER_SCOPE();

	//	Generate po_chain
	LgnosiaStackPush ( LgnosiaGenPOC ( lgnosia , 0 ) ) ;

	//	push if-flow
	//	we use this for ne_chain generating
	SCClStackPush ( &cfstack , lgnosia ) ;
	SCClStackPush ( &cfstack , parserc->stack ) ;

	//	parsing body
	stack = 1 ;
	SCOPER_PUSH () ;
	while ( !lexc->stop ) {

		lexerc_genv () ;

		if ( C_DKL == lexc->v )
			stack ++ ;
		if ( C_DKR == lexc->v )
			stack -- ;
		
		if ( 0 == stack ) 
			break ;		

		parser_c_read_controlflow () ;
		parser_c_read_symbol_def () ;
		parser_c_read_symbol_inf () ;
			

	}

	LgnosiaStackPop () ;
	SCOPER_POP () ;	
		
	if ( stack ) {
		cerror ( C_PARSER_MOD , IS_C_ERROR , "'{}' is unmatched , line : %d\n" , lexc->line ) ;
		RESET_PARSER () ;
		return 1 ;
	}	


}

static int parser_c_read_else () {

	//	author : Jelo Wang
	//	since : 20100502
	//	(C)TOK

	int stack = 0 ;
	int elseif = 0 ;
	
	EVALOR* evalor = 0 ;
	AZONAL* azonal = 0 ;
	int lgnosia = 0 ;
	int FatherLga = 0 ;
	int ne_chain = 0 ;
	
	if ( C_ELSE != lexc->v ) 
		return 0 ;

	if ( C_IF == lexerc_head_genv(1) ) {

		lexerc_genv () ;
		elseif = 1 ;
		//	create control flow azonal
		azonal = SymboleAddControlAzonal ( ISA_ELSEIFCF , lexc->line ) ;

		
	} else {
		elseif = 0 ;
		azonal = SymboleAddControlAzonal ( ISA_ELSECF , lexc->line ) ;
	}
	

	{
	
		//	generate ne_chain of LGNOSIA_CP_IDENT.
		
		int deep = 	SCClStackPop ( &cfstack ) ;
		int lga = SCClStackPop ( &cfstack ) ;

		while ( deep > parserc->stack ) {
			
			if ( SCClStackEmpty(&cfstack) ) break ; 

			deep = 	SCClStackPop ( &cfstack ) ;
			lga = SCClStackPop ( &cfstack ) ;

		}

		ne_chain = LgnosiaGenNEC ( lga , azonal ) ;
//		LgnosiaStackPush ( ne_chain ) ;
	
	}

	if ( 1 == elseif ) {

		//	create LGA for the else-if flow
		lgnosia = LgnosiaNew ( (int)azonal , LGNOSIA_CP_IDENT ) ;
		//	add context of ne_chain
		LgnosiaAddContext ( ne_chain, (int) lgnosia , LGNOSIA_CP_IDENT ) ;

		SET_PARSER_SCOPE(PARSERC_SCOPE_PARAM);
		//	parsing parameter scope of controlflow
		//	Get an evalor
		evalor = EvalorCreate () ;
		EvalorSetCurrent ( evalor ) ;
		//	parsing EXPR flow
		parser_c_read_expr () ;
		EvalorFinish () ;
		
		//	add parameter for the LGA.
		LgnosiaAddParameter ( lgnosia , EvalorGetTree () , LGNOSIA_EXP_IDENT ) ;			
		UNSET_PARSER_SCOPE();
		
		//	Generate po_chain
		LgnosiaStackPush ( LgnosiaGenPOC ( lgnosia , azonal ) ) ;

		//	push if-flow
		//	we use this for ne_chain generating
		SCClStackPush ( &cfstack , lgnosia ) ;
		SCClStackPush ( &cfstack , parserc->stack ) ;

		//	parsing body
		stack = 1 ;
	
	} else {		

		LgnosiaStackPush ( ne_chain ) ;
		stack = 0 ;
	}
	
	SCOPER_PUSH () ;
	while ( !lexc->stop ) {

		lexerc_genv () ;

		if ( C_DKL == lexc->v )
			stack ++ ;
		if ( C_DKR == lexc->v )
			stack -- ;
		
		if ( 0 == stack ) 
			break ;		

		parser_c_read_controlflow () ;
		parser_c_read_symbol_def () ;
		parser_c_read_symbol_inf () ;
			

	}
	LgnosiaStackPop () ;
	SCOPER_POP () ;	

	if ( 1 == elseif ) { 
		//	push else-if flow
		LgnosiaQueueEnter ( lgnosia ) ;
	}

	if ( stack ) {
		cerror ( C_PARSER_MOD , IS_C_ERROR , "'{}' is unmatched , line : %d\n" , lexc->line ) ;
		RESET_PARSER () ;
		return 1 ;
	}	


}

static int parser_c_read_while () {

	//	author : Jelo Wang
	//	since : 20100502
	//	(C)TOK

	int stack = 0 ;

	EVALOR* evalor = 0 ;
	AZONAL* azonal = 0 ;
	int lgnosia = 0 ;
	
	if ( C_WHILE != lexc->v ) 
		return 0 ;

	//	create control flow azonal
	azonal = SymboleAddControlAzonal ( ISA_WHILECF , lexc->line ) ;
		
	//	create LGA
	lgnosia = LgnosiaNew ( (int)azonal , LGNOSIA_CP_IDENT ) ;

	LgnosiaAddContext ( LgnosiaStackTop () , (int) lgnosia , LGNOSIA_CP_IDENT ) ;
	LgnosiaStackPush ( lgnosia ) ;
	
	SET_PARSER_SCOPE(PARSERC_SCOPE_PARAM);
	//	parsing parameter scope of controlflow
	//	Get an evalor
	evalor = EvalorCreate () ;
	EvalorSetCurrent ( evalor ) ;
	//	parsing EXPR flow
	parser_c_read_expr () ;
	EvalorFinish () ;
	
	//	add parameter for the LGA.
	LgnosiaAddParameter ( lgnosia , EvalorGetTree () , LGNOSIA_EXP_IDENT ) ;			
	UNSET_PARSER_SCOPE();

	//	parsing body
	stack = 1 ;
	SCOPER_PUSH () ;
	while ( !lexc->stop ) {

		lexerc_genv () ;

		if ( C_DKL == lexc->v )
			stack ++ ;
		if ( C_DKR == lexc->v )
			stack -- ;
		
		if ( 0 == stack ) 
			break ;		

		parser_c_read_controlflow () ;
		parser_c_read_symbol_def () ;
		parser_c_read_symbol_inf () ;
			

	}

	LgnosiaStackPop () ;
	SCOPER_POP () ;	
	
	if ( stack ) {
		cerror ( C_PARSER_MOD , IS_C_ERROR , "'{}' is unmatched , line : %d\n" , lexc->line ) ;
		RESET_PARSER () ;
		return 1 ;
	}	


}


static int parser_c_read_controlflow () {

	//	author : Jelo Wang
	//	since : 20100116
	//	(C)TOK

	switch ( lexc -> v ) {

		case C_IF :
			parser_c_read_if () ;
		break ;

		case C_ELSE :
			parser_c_read_else () ;
		break ;

		case C_FOR :
		break ;

		case C_WHILE :
			parser_c_read_while () ;
		break ;

		case C_DO :
		break ;

		case C_SWITCH :
		break ;

		default :
			return 0 ;

	}

}



static int parser_c_is_controlflow ( int head ) {

	//	author : Jelo Wang
	//	since : 20100116
	//	(C)TOK

	switch ( head ) {

		case C_IF :
		case C_FOR :
		case C_WHILE :
		case C_DO :
		case C_SWITCH :
			return 1 ;

		default :
			return 0 ;

	}


}

static int parser_c_read_symbol_inf () {

	//	author : Jelo Wang
	//	since : 20100202
	//	(C)TOK


	switch ( lexc->v ) {
		case C_VAR :
			return parser_c_read_variable_inf () ;
		break ;
		case C_INTNUM :
			return parser_c_read_intnum_inf () ;
		break ;
		default : return 0 ;
	}

	return 1 ;
	
}

static int parser_c_read_symbol_def () {

	//	author : Jelo Wang
	//	since : 20100202
	//	(C)TOK


	switch ( lexc->v ) {
		case C_VARDEF :
			parser_c_read_variable_def () ;
		break ;

		default : return 0 ;
	}

	return 1 ;
	
}


static int parser_c_read_variable_def () {

	//	author : Jelo Wang
	//	since : 20100202
	//	(C)TOK

	int scope = 0 ;

	AZONAL* azonal = 0 ;
	int lgnosia = 0 ;
	EVALOR* evalor = 0 ;

	if ( C_VARDEF != lexc->v )
		return 0 ;

	//	gen scope value
	scope = SCOPER_GET () ;

	if ( 0 == SymboleVarAzonalSavable ( lexc->token , scope , LgnosiaStackTop () ) ) {
		cerror ( C_PARSER_MOD , IS_C_ERROR , "variable is multi-defined: '%s' , line : %d\n" , lexc->token , lexc->line ) ;
	}
	
	azonal = SymboleAddVarAzonal (
		lexc->token ,
		lexc->headbit ,
		ISA_VARIABLE ,
		scope ,
		( parserc->scope == PARSERC_SCOPE_PARAM ) ? (1) : (0) ,
		lexc->line ,
		SymboleGetCurrentFunc () ,
		(int) LgnosiaStackTop ()
	) ;

	LEXERC_HEADBIT_RESET();

	//	create a LGNOSIA_SYM_IDENT
	lgnosia = LgnosiaNew ( (int)azonal , LGNOSIA_SYM_IDENT ) ;

	//	and context to the top LGA.
	LgnosiaAddContext ( LgnosiaStackTop () , (int)lgnosia , LGNOSIA_SYM_IDENT ) ;

	if ( C_EQU == lexerc_head_genv(1) ) {
		
		SET_PARSER_SCOPE(PARSERC_SCOPE_PARAM);
		lexerc_genv () ;
		
		if ( OPERAND_FLOW(lexerc_head_genv(1) )) {
			lexerc_genv () ;
		}

		//	Get an evalor
		evalor = EvalorCreate () ;
		EvalorSetCurrent ( evalor ) ;
		//	parsing EXPR flow
		parser_c_read_expr () ;
		EvalorFinish () ;
	
		//	add parameter for the LGNOSIA.
		LgnosiaAddParameter ( lgnosia , EvalorGetTree () , 0 ) ;	
		UNSET_PARSER_SCOPE();
		
	}

	return 1 ;

}

static int parser_c_read_intnum_inf () {

	//	author : Jelo Wang
	//	since : 20110125
	//	(C)TOK

	if ( C_INTNUM != lexc->v )
		return 0 ;

	return (int)SymboleAndNumeric ( lexc->token , ISA_INTEGER ) ;

}


static int parser_c_read_variable_inf () {

	//	author : Jelo Wang
	//	since : 20100202
	//	(C)TOK

	int scope = 0 ;

	AZONAL* azonal = 0 ;
	EVALOR* evalor = 0 ;
	int lgnosia = 0 ;
	
	if ( C_VAR != lexc->v )
		return 0 ;

	//	gen scope value
	scope = SCOPER_GET () ;

	azonal = SymboleFindVarAzonal ( lexc->token , scope , LgnosiaStackTop () ) ;

	if ( !azonal ) {
		cerror ( C_PARSER_MOD , IS_C_ERROR , "undefined variable : '%s' , line : %d\n" , lexc->token , lexc->line ) ;
	}

	SymboleAzonalRefInc ( azonal ) ;
	
	if ( 0 == MATCH_PARSER_SCOPE(PARSERC_SCOPE_PARAM) ) {

		//	and context to the top LGA.
		//	create a LGNOSIA_SYM_IDENT
		lgnosia = LgnosiaNew ( (int)azonal , LGNOSIA_SYM_IDENT ) ;
		LgnosiaAddContext ( LgnosiaStackTop () , (int)lgnosia , LGNOSIA_SYM_IDENT ) ;

	}
	
	if ( C_EQU == lexerc_head_genv (1) ) {	
		SET_PARSER_SCOPE(PARSERC_SCOPE_PARAM);
		lexerc_genv () ;
		if ( OPERAND_FLOW(lexerc_head_genv(1) )) {
			lexerc_genv () ;
		}

		//	Get an evalor
		evalor = EvalorCreate () ;
		EvalorSetCurrent ( evalor ) ;		
		//	parsing EXPR flow
		parser_c_read_expr () ;
		EvalorFinish () ;

		//	add parameter for the LGNOSIA.
		LgnosiaAddParameter ( lgnosia , EvalorGetTree () , 0 ) ;	
		UNSET_PARSER_SCOPE();
	
	}
		
	return (int)azonal ;


}

static int parser_c_read_expr () {

	//	author : Jelo Wang
	//	since : 20100202
	//	(C)TOK

	//	(1) a1+a2*a3 ;

	int symbol = 0 ;

	if ( PORL_OPERATOR(lexerc_head_genv (1)) ) {
		lexerc_genv () ;
		parser_c_read_poroperator ( 0 , 1 ) ;
	}

	while (!lexc->stop) {
			
		//	read prior operator
		parser_c_read_poroperator ( 0 , 1 ) ;

		//	parsing symbol and return its handle
		symbol = parser_c_read_symbol_inf () ;

		//	if the handle is zero , break the processing.
		if ( 0 == symbol ) break ;
			
		//	push the symbol in evalor.
		EvalorOperandPush ( symbol ) ;

		lexerc_genv () ;

reoperator :
		if ( DUA_OPERATOR(lexc->v) ) {
			//	push the operator in evalor
			EvalorOperatorPush ( lexc->v ) ;
			lexerc_genv () ;
		}
		
		//	read prior operator
		parser_c_read_poroperator ( 1 , 1 ) ;

		if ( DUA_OPERATOR(lexc->v) )
			goto reoperator ;	

	}

	return 1 ;

		
}

static void parser_c_read_poroperator ( int side , int evalor_enable ) {

	//	author : Jelo Wang
	//	since : 20091124
	//	(C)TOK
	
	//	(1) side == 0 , left bracket
	//	(2) side == 1 , right bracket
	//	(3) if evalor_enable == 1 , push bracket in it.	

	int key = 0 ;
	
	if ( 0 == side ) key = C_XKL ;
	else if ( 1 == side ) key = C_XKR ;
	
	if ( key != lexc->v ) return ;
	
	while ( !lexc->stop ) {	
		if ( key != lexc->v ) break ;
		if ( 1 == evalor_enable ) EvalorOperatorPush ( lexc->v ) ;
		if ( key != lexerc_head_genv (1) ) {
			lexerc_genv () ;
			break ;
		} else lexerc_genv () ;
	}

}

int parser_c_run ( int* lines ) {

	//	author : Jelo Wang
	//	since : 20091124
	//	(C)TOK

	int success = 1 ;
	int state = 0 ;
	
	int symbole = SymboleNew () ;
	SymboleSetCurrent ( symbole ) ;
	
# if 0

	//	these codes bellow just for debug
	parserc_ready () ;
	lexerc_ready () ;
	lexerc_setmode ( LEXERC_FLITER_MODE ) ;
	
	for ( lexerc_genv () ; !lexc->stop ; lexerc_genv () ) {
re : 				
		if ( C_VARDEF == lexc->v ) {
		} else if ( C_ARRAYDEF == lexc->v ) {
		} else if ( C_FUNCDEF == lexc->v ) {
			while ( !lexc->stop ) {
				lexerc_genv () ;
				if ( C_FUNCDEF == lexc->v )
					goto re ;
			}
		} else if ( C_AUTO <= lexc->v &&  lexc->v <= C_UNION ) {
		} else {			
			success = 0 ;
			cerror ( C_PARSER_MOD , IS_C_ERROR , "undefined grammar : %x , line : %d\n" , lexc->v , lexc->line ) ;
		}

	}
	
	*lines = *lines +  lexc->line ;
	
	if ( 0 == success ) return 0 ;

	*lines = 0 ;

# endif

	parserc_ready () ;
	lexerc_ready () ;
	lexerc_setmode ( LEXERC_FLITER_MODE | LEXERC_HEADBIT_MODE ) ;
	
	for ( lexerc_genv () ; !lexc->stop ; lexerc_genv () ) {
				
		if ( C_VARDEF == lexc->v ) {
		} else if ( C_ARRAYDEF == lexc->v ) {
		} else if ( C_FUNCDEF == lexc->v ) {
			parser_c_read_function () ;
		} else {
			cerror ( C_PARSER_MOD , IS_C_ERROR , "undefined grammar : %x , line : %d\n" , lexc->v , lexc->line ) ;
		}

	}

	*lines = *lines +  lexc->line ;
		
	lexerc_destroy () ;
	SCClStackDestroy ( &parserc->scopestack ) ;
	SCFreeEx ( &parserc ) ;

	return 1 ;
	
}

