
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

# include <stdio.h>
# include <ctype.h>
# include "c-lexer.h"
# include "schal.h"
# include "sccl.h"
# include "evalor.h"
# include "c-grammar.h"
# include "symbole.h"

//	author: Jelo Wang
//	since : 20090102
//	in arithmetic EXPR
# define ARITHMETIC_SCOPE  3 
//	in bit EXPR
# define BITAND_SCOPE  4 

extern void SCLog ( char* , ... ) ;

SCClStack suffix_stack = { 0 , 0 , 0 } ;
SCClStack evalor_stack = { 0 , 0 , 0 } ;
EVALOR* evalor = 0 ;

# ifdef __EVALOR_DEBUG
static char* evalor_disp_operator ( int operator ) {

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
# endif

static int is_functer ( int functer ) {

	//	author: Jelo Wang
	//	since : 20091123
	//	(c)TOK


	if ( C_JAA <= functer && functer <= C_SHRR ) 
		return 1 ;
		return 0 ;

}

static int functer_priority ( int functer , int scope ) {

	//	author: Jelo Wang
	//	since : 20081229
	//	(c)TOK

	switch ( functer ) {

		case C_MUL :
		case C_DIV :
		case C_MOD :
		return 13 ;

		case C_ADD :
		case C_SUB :
		return 12 ;

		case C_SHL :
		case C_SHR :
		return 11 ;

		case C_GE :
		case C_GT :
		case C_LT :
		case C_LE :
		return 10 ;

		case C_EQ :
		case C_NE :
		return 9 ;

		case C_YHH :
		return 7 ;

		case C_HE :
		return 6 ;

		case C_HU :
		return 6 ;
		
		case C_AND :
		return 5 ;

		case C_OR :
		return 4 ;
		
		default : return -1 ;

	}

}


static int have_priority ( int priority , int object ) {

	//	author: Jelo Wang
	//	since : 20090102

	//	…¢”µ£ºß\Ëã·ûƒžÏÈ¼‰

	if( priority < object )
		return -1 ;
	if( priority == object )
		return 0 ;
		return 1 ;

}

static long int value_calac ( int op_1 , int op_2 , int functer ) {
	
	//	author: Jelo Wang
	//	since : 20091123
	
	switch ( functer ) {

		case C_SUB : return op_1 - op_2 ;
		case C_ADD : return op_1 + op_2 ;

		default : return 0 ;
		
	}
	
}

static void c_suffix_exp_build ( SCClStack* SCClStack , int value , int scope ) {

	//	author : Jelo Wang
	//	since : 20081229
	//	(c)TOK

	switch( value ) {

		case C_XKL : SCClStackPush ( SCClStack , C_XKL ) ; break ;

		case C_XKR :
			
			while( !SCClStackEqual ( SCClStack , C_XKL ) && !SCClStackEmpty ( SCClStack ) ) {
				SCClStackPush ( &suffix_stack , SCClStackPop( SCClStack ) ) ;
			}
			
			SCClStackPop ( SCClStack ) ;
			
		break ;
		
		default :

			if( SCClStackEmpty ( SCClStack ) ) {

				SCClStackPush ( SCClStack ,  value ) ;
				
			} else if( -1 == have_priority ( functer_priority ( value , scope ) , functer_priority ( SCClStackGet ( SCClStack ) , scope ) ) ) {
				
				while( !SCClStackEmpty ( SCClStack ) ) {
					
					int priority = have_priority (functer_priority ( value , scope ) , functer_priority ( SCClStackGet ( SCClStack ) , scope ) ) ;
					
					if( SCClStackEqual ( SCClStack , C_XKL ) ) {

						SCClStackPop ( SCClStack ) ;
						
						break ;

					} else if ( -1 == priority ) {
						SCClStackPush ( &suffix_stack , SCClStackPop( SCClStack ) ) ;
					} else if( 1 != priority ) {

						SCClStackPop ( SCClStack ) ;
						
						break ;
					}
					
				}
				
				SCClStackPush ( SCClStack ,  value ) ;
			 
			} else
				SCClStackPush ( SCClStack ,  value ) ;

	}

}


static void suffix_exp_done ( SCClStack* SCClStack ) {

	//author: Jelo Wang
	//since : 20090102

	while ( !SCClStackEmpty ( SCClStack ) )

		if ( !SCClStackEqual ( SCClStack , C_XKL ) )  {

			SCClStackPush ( &suffix_stack , SCClStackPop( SCClStack ) ) ;

		} else {

			SCClStackPop ( SCClStack )  ;
		
		}
		
}

static void c_gen_suffix_exp () {


	//	author : Jelo Wang
	//	since : 20091123
	//	(c)TOK

	SCClStack* ieval = (SCClStack* ) SCMalloc ( sizeof(SCClStack) ) ;
	
	if ( 0 == ieval ) {
		return 0 ;
	}

	SCClStackInit ( ieval ) ;
	SCClStackInit ( &suffix_stack ) ;
	
	while ( !lexc -> stop ) {
		
		lexerc_genv() ;

		if ( lexc -> v == C_ENTER  && lexc -> ppv != C_ESCAPE )
			break;

		if ( lexc -> v == C_CHROW  && lexc -> pv != C_ESCAPE )
			break;
		
		if ( is_functer  ( lexc -> v , ARITHMETIC_SCOPE ) )
			c_suffix_exp_build ( ieval , lexc -> v , ARITHMETIC_SCOPE ) ;
		else {
			SCClStackPush ( &suffix_stack , atoi ( lexc -> token ) ) ;
		}
	}

	suffix_exp_done ( ieval ) ;

}

static long int c_suffix_exp_evalor () {

	//	author : Jelo Wang
	//	since : 20091123
	//	(c)TOK

	int results = 0;
	
//	for ( ; !SCClStackEmpty ( &suffix_stack ) ;) {

		int functer = SCClStackPop ( &suffix_stack ) ;

		int op_2 = SCClStackPop ( &suffix_stack ) ;
		int op_1 = SCClStackPop ( &suffix_stack ) ;

		SCClStackPush ( &suffix_stack , value_calac ( op_1 , op_2 , functer ) ) ;
			
		
		
//	}

	results = SCClStackPop ( &suffix_stack ) ;

	return results ;


	
}

long int c_evalor () {

	//	author : Jelo Wang
	//	since : 20091123
	//	(c)TOK

	c_gen_suffix_exp () ;
	
	return c_suffix_exp_evalor () ;


}

EVALOR* EvalorCreate () {
	
	//	author : Jelo Wang
	//	since : 20100427
	//	(c)TOK

	EVALOR* evalor = (EVALOR* ) SCMalloc (sizeof(EVALOR)) ;

	evalor->suffix.element = 0 ;
	evalor->suffix.head = 0 ;
	evalor->suffix.next = 0 ;

	SCClStackInit ( &evalor->operator ) ;

	evalor->tree = 0 ;
	
	return evalor ;
	
}

void EvalorSetCurrent ( EVALOR* eva ) {

	//	author : Jelo Wang
	//	since : 20100427
	//	(c)TOK

	SCClStackPush ( &evalor_stack , (int) evalor ) ;
	evalor = eva ;
	
}

void EvalorUnsetCurrent () {

	//	author : Jelo Wang
	//	since : 20100427
	//	(c)TOK

	evalor = SCClStackPop ( &evalor_stack ) ;
	
}

void EvalorOperandPush ( int operand ) {
	
	//	author : Jelo Wang
	//	since : 20100427
	//	(c)TOK


	EXPR* expression = (EXPR* ) SCMalloc ( sizeof(EXPR) ) ;

	if ( !expression ) return ;
	if ( !evalor ) return ;

	expression->delt = 0 ;
	expression->type = EXP_OPERAND ;
	expression->delttype = EXP_DELT_DEFAULT ;
	expression->handle = operand ;
	expression->left = 0 ;
	expression->right = 0 ;
	expression->brother = 0 ;
	
	SCClListInsert ( &evalor->suffix , (int)expression ) ;

	
}

void EvalorOperatorPush ( int operator ) {
	
	//	author : Jelo Wang
	//	since : 20100427
	//	(c)TOK

	EXPR* expression = (EXPR* ) SCMalloc ( sizeof(EXPR) ) ;
	EXPR* stackexp = 0 ;
	EXPR* tree = 0 ;
	EXPR* operand1 = 0 ;
	EXPR* operand2 = 0 ;

	if ( !expression ) return 0 ;
	if ( !evalor ) return ;

	expression->delt = 0 ;
	expression->type = EXP_OPERATOR ;
	expression->delttype = EXP_DELT_DEFAULT ;	
	expression->handle = operator ;
	expression->left = 0 ;
	expression->right = 0 ;
	expression->brother = 0 ;
	
	switch ( expression->handle ) {

		case C_XKL :SCClStackPush ( &evalor->operator , expression ) ; break ;

		case C_XKR :
			
			while( !SCClStackEmpty ( &evalor->operator ) ) {
				
				tree = (EXPR* ) SCClStackPop ( &evalor->operator ) ;

				if ( C_XKL == tree->handle ) break ;

				SCClListInsert ( &evalor->suffix , (int)tree ) ; 

			}
			
		break ;

		default :

			if ( SCClStackEmpty ( &evalor->operator ) ) {

				SCClStackPush ( &evalor->operator ,  expression ) ;
				
			} else {

				int priority = 0 ;

				stackexp = SCClStackGet ( &evalor->operator ) ;

				priority = have_priority ( expression->handle , stackexp->handle ) ;

				if ( -1 != priority ) {
					SCClStackPush ( &evalor->operator , (int)expression ) ;
				} else {
					
					while( !SCClStackEmpty ( &evalor->operator ) ) {
						
						stackexp = SCClStackGet ( &evalor->operator ) ;

						priority = have_priority ( expression->handle , stackexp->handle ) ;
						
						if( C_XKL == stackexp->handle ) {
		
							SCClStackPop ( &evalor->operator ) ;
						
							break ;

						} else if ( -1 == priority ) {		
							
							SCClListInsert ( &evalor->suffix , (int)SCClStackPop ( &evalor->operator ) ) ; 

						} else if( -1 != priority ) {
		
							break ;
						
						}
					
					}
				
					SCClStackPush ( &evalor->operator , expression ) ;

				}
				
			}
			
	}
	
}


void EvalorFinish () {
	
	//	author : Jelo Wang
	//	since : 20100427
	//	(c)TOK

	int pri = 0 ;
	EXPR* tree = 0 ;
	SCClStack suffix_stack = { 0 , 0 , 0 } ;
	SCClList* suffix_walker = 0 ;


	# ifdef __EVALOR_DEBUG
	{
		static int times = 1 ;
		SCLog ( "Exp (%d) : " , times ) ;
		times ++ ;
	}
	# endif

				
	if ( !evalor ) return ;

   	while( !SCClStackEmpty (&evalor->operator) )
		SCClListInsert ( &evalor->suffix , SCClStackPop (&evalor->operator) ) ;
	
	for ( suffix_walker = evalor->suffix.head ; suffix_walker ; suffix_walker = suffix_walker->next ) {
		
		tree = (EXPR* ) suffix_walker->element ;
		tree->delt = 0 ;
		
		switch ( tree->type ) {

			case EXP_OPERATOR :
				
				# ifdef __EVALOR_DEBUG
					SCLog ( "%s " , evalor_disp_operator (tree->handle) ) ;
				# endif

				tree->right = SCClStackPop ( &suffix_stack );
				tree->left = SCClStackPop ( &suffix_stack );
				tree->right->brother = tree->left ;
				tree->left->brother = tree->right ;
				SCClStackPush ( &suffix_stack , (int)tree );
				
			break ;
			
			case EXP_OPERAND :
	
				# ifdef __EVALOR_DEBUG
				{
					AZONAL* azonal = tree->handle ;
					SCLog ( "%s " , azonal->name ) ;
				}
				# endif

				SCClStackPush ( &suffix_stack , (int)tree );
				
		}
	}

	evalor->tree = (int) SCClStackPop ( &suffix_stack ) ;
		
	EvalorClear () ;

	# ifdef __EVALOR_DEBUG
		SCLog ( "\n" ) ;
	# endif
	
}

void EvalorClear () {
	
	//	author : Jelo Wang
	//	since : 20100427
	//	(c)TOK

	SCClListDestroy ( &evalor->suffix ) ;
	SCClStackDestroy ( &evalor->operator ) ;

}


int EvalorGetTree () {
	
	//	author : Jelo Wang
	//	since : 20100427
	//	(c)TOK

	int tree = evalor->tree ;

	EvalorUnsetCurrent () ;
			
	return tree ;

}


