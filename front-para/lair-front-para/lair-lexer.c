
/*

+	LAC LEXER , part of SC mid-para

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

# include "schal.h"
# include "sccl.h"
# include "lair-lexer.h"
# include "lair-grammar.h"

# define LAC_TOK_DEFAULT_LENGTH 256
# define LAC_LANGUGE_KEYWORDS_TOTALL 10

SCClString LACTOK = { 0 , 0 , 0 , 0 , 0 } ;
LEXERLAC* lexac = 0 ;

typedef struct {
	
	//	author : Jelo Wang
	//	notes : key words of lac language
	//	since : 20100503
	//	(C)TOK

	char* name ;
	int   type ;

} LAC_KEYWORDS ;

LAC_KEYWORDS lac_key[ LAC_LANGUGE_KEYWORDS_TOTALL ] = {

	{ "else" , LAC_ELSE } ,
	{ "extern" , LAC_EXTERN } ,
	{ "for" , LAC_FOR } ,
	{ "goto" , LAC_GOTO } ,
	{ "if" , LAC_IF } ,
	{ "register" , LAC_REGISTER } ,
	{ "return" , LAC_RETURN } ,
	{ "static" , LAC_STATIC } ,
	{ "volatile" , LAC_VOLATILE } ,
	{ "while" , LAC_WHILE } ,

} ;

static int lexerac_digit_lex ( int v ) {
	
	//	author : Jelo Wang
	//	since : 20100412
	//	(C)TOK

	switch ( v ) {
		case LAC_INTNUM : 
		case LAC_FLTNUM :   
		case LAC_HEXNUM :   
		case LAC_FLTENUM :  
		case LAC_INTENUM : 
			return 1 ; 
		break ;
	}

	return 0 ;

}


LEXERLAC* lexerac_new ( unsigned char* data , int mmode ) {

	//	author : Jelo Wang
	//	since : 20090825
	//	(C)TOK

	//	return a new lexer

	LEXERLAC* lexer = (LEXERLAC* ) SCMalloc ( sizeof(LEXERLAC) ) ; 

	lexer->v = 0 ;
	lexer->pv = 0 ;
	lexer->ppv = 0;

	lexer->line = 1 ;
	lexer->stop = 0 ;
	lexer->file = 0 ;
	lexer->mode = mmode ;
	lexer->ios = 1 ;
	lexer->stack = 0 ;
	lexer->deep = 0 ;
	lexer->headbit = 0 ;
	lexer->scale = 0 ;

	lexer->code = SCClStringCreate ( data , sc_strlen ( data ) ) ;

	lexer->scstack = (SCClStack* ) SCMalloc ( sizeof ( SCClStack ) ) ;

	SCClStringInitEx ( &LACTOK , 256 ) ;	

	return lexer ;

}

void lexerac_set ( LEXERLAC* lexer ) {

	//	author : Jelo Wang
	//	since : 20090825
	//	(C)TOK

	//	set a runtime lexer

	lexac = lexer ;

}

void lexerac_clear_status () {

	//	author : Jelo Wang
	//	since : 20091127
	//	(C)TOK

	//	set value

	if ( !lexac ) return ;

	lexac->v = 0 ;
	lexac->c = 0 ;
	lexac->pv = 0 ;
	lexac->ppv = 0 ;
	lexac->token = 0 ;
	lexac->scale = 0 ;

}

void lexerac_set_file ( char* name ) {

	//	author : Jelo Wang
	//	since : 20090921
	//	(C)TOK

	//	set file name 

	if ( lexac -> file ) {
		SCFree ( lexac -> file ) ;
	}

	lexac -> file = (char* ) SCMalloc ( sc_strlen ( name ) + 1 ) ;
	sc_strcpy ( lexac -> file , name ) ;

}

void lexerac_destroy () {

	//	author : Jelo Wang
	//	since : 20090828
	//	(C)TOK

	//	notes : destroy the current lexer

	if ( ! lexac ) return ;

	SCClStringDestroy ( lexac->code ) ;

	if ( lexac->file ) SCFree ( lexac->file ) ;  

	if ( lexac->scstack ) {
		SCClStackDestroy ( lexac->scstack ) ;
		SCFree ( lexac->scstack ) ;
	}

	SCFreeEx (& lexac ) ;

	if ( LACTOK.data ) {
		SCFreeEx ( &LACTOK.data ) ;
	}

}


LEXERLAC* lexerac_get () {

	//	author : Jelo Wang
	//	since : 20090901
	//	(C)TOK

	return lexac ;

}

void lexerac_setmode ( int mmode ) {

	//	author : Jelo Wang
	//	since : 20100129
	//	(C)TOK

	//	set lexer runing mode

	if ( lexac )
		lexac->mode = mmode ;

}

static int lexerac_find_key ( char* token ) {

	//	author : Jelo Wang
	//	since : 2008
	//	updated : 20090816
	//	(C)TOK
	
	//	notes : look over keywords table

	int count = 0 ;


	while ( count < LAC_LANGUGE_KEYWORDS_TOTALL ) {

		if ( !sc_strcmp ( lac_key[ count ].name , token ) )
			return lac_key[ count ].type ;
		
		count ++ ;
		
	}

	return 0 ;

}


void lexerac_ready ()  {

	//	author : Jelo Wang
	//	notes : code_set_iterator
	//	since : 20090810
	//	(C)TOK
	
	( (SCClString*) lexac->code)->get_walker = 0 ;
	( (SCClString*) lexac->code)->last_walker = 0 ;

	lexac->v = 0 ;
	lexac->pv = 0 ;
	lexac->ppv = 0;

	lexac->line = 1 ;
	lexac->stop = 0 ;
	lexac->stack = 0 ;

	lexac->deep = 0 ;
	lexac->headbit = 0 ;	


}

int lexerac_overflowed ()  {

	//	author : Jelo Wang
	//	notes : code_overflowed
	//	since : 20090810
	//	(C)TOK
	
	if ( ( lexac->code -> get_walker >= lexac->code -> length ) ) {
		lexac->c = 0;
		lexac->token = 0;
		lexac->v = 0;
		lexac -> stop = 1;
		return 1 ;
	}

	lexac->c = lexac->code -> data [ lexac->code -> get_walker ] ;

	return 0;

}

int lexerac_next ()  {

	//	author : Jelo Wang
	//	notes : code_next
	//	since : 20090810
	//	(C)TOK
	
	if ( lexerac_overflowed () )
		return 1;

	lexac->c = lexac->code -> data [ lexac->code -> get_walker + 1 ] ;

	lexac->code -> last_walker = lexac->code -> get_walker ;
	lexac->code -> get_walker ++ ;
	

}

unsigned char lexerac_get_atom ()  {

	//	author : Jelo Wang
	//	notes : code_get
	//	since : 20090810
	//	(C)TOK
	
	if ( lexerac_overflowed () )
		return 0;
	
	lexac->c = lexac->code -> data [ lexac->code -> get_walker ] ;

	return lexac->code -> data [ lexac->code -> get_walker ] ;

}



void lexerac_put_atom ( unsigned char atom )  {

	//	author : Jelo Wang
	//	notes : code_get
	//	since : 20090926
	//	(C)TOK
	
	if ( lexerac_overflowed () )
		return 0;
	
	lexac->code -> data [ lexac->code -> get_walker ] = atom ;

}

void lexerac_rollback ()  {

	//	author : Jelo Wang
	//	notes : code_setback_walker
	//	since : 20090810
	//	(C)TOK
	
	lexac->code -> get_walker = SCClStackPop ( lexac -> scstack ) ; 

}



void lexerac_backup () { 

	//	author : Jelo Wang
	//	since : 20100308
	//	(C)TOK

	SCClStackPush ( lexac -> scstack , lexac->code -> get_walker ) ;
	
}

void lexerac_backup_clear () { 

	//	author : Jelo Wang
	//	since : 20100308
	//	(C)TOK

	SCClStackPop ( lexac->scstack ) ;
	
}


void lexerac_setback ( int step )  {

	//	author : Jelo Wang
	//	notes : code_setback_walker
	//	since : 20090810
	//	(C)TOK
	
	if ( 0 > lexac->code->get_walker - step )
		return 0 ;

	lexac->code->get_walker = lexac->code->get_walker - step ;


}

void lexerac_skip_blank () {

	//	author : Jelo Wang
	//	notes : is_blank_token
	//	since : 20090810
	//	(C)TOK
	
	while ( sc_is_blank ( lexerac_get_atom () ) ) {

		if ( '\r' == lexac->c ) lexac->line ++ ;	
		if ( '\n' == lexac->c ) lexac->line ++ ;
		
		lexerac_next () ;

	}


}

void lexerac_skip_space () {

	//	author : Jelo Wang
	//	notes : lexerac_skip_space
	//	since : 20090816
	//	(C)TOK
	
	while ( 0x20 == lexac->code -> data[ lexac->code -> get_walker ] || '\t' == lexac->code -> data[ lexac->code -> get_walker ] ) {
	
		lexerac_next () ;

	}


}

void lexerac_jump ( int step ) {

	//	author : Jelo Wang
	//	notes : code_setback_walker
	//	since : 20090813
	//	(C)TOK
	
	if ( lexac->code -> get_walker + step >= lexac->code -> length )
		return ;
	
	lexac->code -> last_walker = lexac->code -> get_walker ;
	lexac->code -> get_walker = lexac->code -> get_walker + step ;
	lexac->c = lexac->code -> data [ lexac->code -> get_walker ] ;	


}

int lexerac_look ( int step )  {

	//	author : Jelo Wang
	//	notes : lexerac_look
	//	since : 20090813
	//	(C)TOK
	
	if ( lexac->code -> get_walker + step >= lexac->code -> length )
		return 0 ;
	
	return lexac->code -> data [ lexac->code -> get_walker + step ] ; 


}

int lexerac_is_var () {
	
	//	author : Jelo Wang
	//	rex model : $VAR => alpha(alpha | _)*
	//	since : 2008
	//	(C)TOK
	
	if( sc_is_alpha(lexac->c)||'_' == lexac->c|| sc_is_digit(lexac->c) ) {
		SCClStringAdd ( &LACTOK , lexerac_get_atom () ) ;
		lexerac_next () ;
		return 1 ;
	}

	SCClStringAdd ( &LACTOK , LAC_EOS ) ;

	return 0 ;

}


int lexerac_is_num () {

	//	author : Jelo Wang
	//	rex model : $INTNUM
	//	since : 2008
	//	(C)TOK
	
	if( sc_is_digit(lexac->c) ) {
		SCClStringAdd ( &LACTOK , lexerac_get_atom () ) ;
		lexerac_next () ;
		return 1 ;
	}   
	
	SCClStringAdd ( &LACTOK , '\0' ) ;

	return 0 ;
	
}

# define LACHEX(x) (sc_is_digit(x)||('a'<= x && x<='f')||('A'<=x && x<='F'))
	
int lexerac_is_hex() {

	//	author : Jelo Wang
	//	rex model : $HEXNUM
	//	since : 2008
	//	(C)TOK
	
	if(LACHEX(lexac->c)) {
		SCClStringAdd ( &LACTOK , lexerac_get_atom () ) ;
		lexerac_next () ;
		return 1 ;
	}  

	SCClStringAdd ( &LACTOK , LAC_EOS ) ;
	
	return 0 ;
	
}


int lexerac_cluster ( int el ) {

	//0x22 is "
	//0x5c is \
	//identify the contents of character varible,char*gtkings="[[][[";
	//dismatching bracket in quotation marks varible is not solecism
	//since : 2008
	//	(C)TOK
	
	int state = 0 ;

	if ( '"' == el ) {



		while ( lexac->code->get_walker < lexac->code->length ) {

			
			switch ( state ) {

				case 0 :

					if ( '"' == lexac->c ) { state = 1 ; SCClStringAdd ( &LACTOK , lexac->c ) ; lexerac_next () ; }
					else { lexerac_next () ; return 0 ; }
						
				break ;

				case 1 :

					if ( '\\' == lexac->c ) { state = 2 ; lexerac_next () ; }
					else if ( '"' == lexac->c ) {

						SCClStringAdd ( &LACTOK , '"' ) ;
						SCClStringAdd ( &LACTOK , LAC_EOS ) ;
											
						lexac->token = LACTOK.data ;
						lexac->c = 0 ;
						lexac->pv = lexac->v ;
						lexac->v = LAC_STR ;

						lexerac_next () ;

						return 1 ;

						
					} else { SCClStringAdd ( &LACTOK , lexac->c ) ; state = 1 ; lexerac_next () ; }
				
					
						
				break ;		

				case 2 :

					SCClStringAdd ( &LACTOK , lexac->c ) ; 
					lexerac_next () ; 
					state = 1 ;
						
				break ;	
				
			}
		}

	
	} else if ( '\'' == el ) {

		int step = 0 ;
		
		while ( step < 3 ) {

			
			switch ( state ) {

				case 0 :

					if ( '\'' == lexac->c ) { state = 1 ; SCClStringAdd ( &LACTOK , lexac->c ) ; lexerac_next () ; step ++ ; }
					else { lexerac_next () ; return 0 ; }
						
				break ;

				case 1 :

					if ( '\\' == lexac->c ) { state = 2 ; lexerac_next () ; }
					else if ( '\'' == lexac->c ) {

						SCClStringAdd ( &LACTOK , '\'' ) ;
						SCClStringAdd ( &LACTOK , LAC_EOS ) ;

						lexac->token = LACTOK.data ;
						lexac->c = 0 ;
						lexac->pv = lexac->v ;
						lexac->v = LAC_CHA ;
						
						lexerac_next () ;
						
						return 1 ;
						
						
					} else { SCClStringAdd ( &LACTOK , lexac->c ) ; state = 1 ; step ++ ; lexerac_next () ; }
				
						
				break ;		

				case 2 :

					SCClStringAdd ( &LACTOK , lexac->c ) ; 
					lexerac_next () ; 
					state = 1 ;
					step ++ ; 
					
				break ;	
				
			}
		}
		
	}


	return 1 ;


}


int lexerac_drop_junk ( int el ) {

	//	author : Jelo Wang
	//	since : 2008
	//	updated : 20090828
	//	notes : drop_junk streams like //.... /*...*/
	//	(C)TOK
	
	int get_walker = 0 ;
	
	char escape = 0 ; 

	if ( '/' == el ) {

		while ( lexac->code->get_walker < lexac->code->length ) {

			if ( '\\' == lexac->code -> data[lexac->code->get_walker] ) {
				escape = '\\' ;
			} else if( 13 == lexac->code -> data[lexac->code->get_walker] && '\\' != escape ) {
				break ;
			} else escape = 0 ;
			
			lexac->code -> data [ lexac->code->get_walker ] = 0x20 ;

			lexerac_next () ; 

		}

	} else if( '*' == el ) {

		//	(exp) /*FASDF*//*asdfsdf****///*/
		//	(1)	/*FASDF*/
		//	(2)	/*asdfsdf****/
		//	(3)	//*/

		int state = 0 ;
		int atom = 0 ;


		while ( lexac->code->get_walker < lexac->code->length ) {

			atom = lexac->code -> data [ lexac->code->get_walker ] ;

			switch ( state ) {
				
				case 0 :
					
					if( '/' == atom ) {
						lexerac_next () ; 
						state = 1 ;
					} else if ( '*' == atom ) {
						lexerac_next () ; 
						state = 2 ;
					} else {
						lexac->code -> data [ lexac->code->get_walker ] = 0x20 ;
						lexerac_next () ; 
					}

				break ;
				
				case 1 :

					if ( '*' == atom ) {
						lexac->code -> data [ lexac->code->get_walker - 1 ] = 0x20 ;
						lexac->code -> data [ lexac->code->get_walker ] = 0x20 ;
						lexerac_next () ; 
					} else {
						lexac->code -> data [ lexac->code->get_walker - 1 ] = 0x20 ;
						lexac->code -> data [ lexac->code->get_walker ] = 0x20 ;
						lexerac_next () ; 
					}

					state = 0 ;

				break;

				case 2 :

					if ( '/' == atom ) {
						lexac->code -> data [ lexac->code->get_walker - 1 ] = 0x20 ;
						lexac->code -> data [ lexac->code->get_walker ] = 0x20 ;
						lexerac_next () ; 
						goto end_junk ;
					} 
					
					lexac->code -> data [ lexac->code->get_walker - 1 ] = 0x20 ;
					state = 0 ;

					
				break ;

			}

		}
	

	}
	
end_junk :
	
	return 1 ; 

}


int lexerac_lookahead ( char element , int step ) {

	//	author : Jelo Wang
	//	since : 20100111
	//	(C)TOK


	//	do nothing
	if ( -1 == step ) return -1 ;

	//	search with element
	if ( -2 == step ) {

		int step_orignal = lexac->code->get_walker ;

		for ( ; lexac->code->get_walker < lexac->code->length && lexac->code->data[lexac->code->get_walker] != element; ) {
			lexac->code->get_walker  ++ ;
		}

		if ( element  == lexac->code->data[lexac->code->get_walker] ) {
			lexac->code->get_walker  = step_orignal ;
			return 1 ;
		} else {
			return -1 ;
		}

	//	search with step
	} else {

		int step_append =  lexac->code->get_walker + step ;

		if ( step_append < lexac->code->length ) {
			if ( element  == lexac->code->data[step_append] )
				return 1 ;
			else
				return 1 ;
		} else return 0 ;
	}
	
}

int lexerac_search_function () {

	//	author : Jelo Wang
	//	since : 20100116
	//	(C)TOK

	int step_orignal = lexac->code->get_walker ;

	for ( ; lexac->code->get_walker < lexac->code->length && lexac->code->data[lexac->code->get_walker] != '{'; ) {
		lexac->code->get_walker  ++ ;
	}

	if (  '{'  == lexac->code->data[lexac->code->get_walker] ) {

		for ( ; lexac->code->get_walker < lexac->code->length && lexac->code->data[lexac->code->get_walker] != ')'; )
			lexac->code->get_walker  -- ;

		if (  ')'  == lexac->code->data[lexac->code->get_walker] ) {

			lexac->code->get_walker  = step_orignal ;
			return 1 ;

		} else {

			lexac->code->get_walker  = step_orignal ;
			return 0 ;

		}

	} else {
		lexac->code->get_walker  = step_orignal ;
		return 0 ;
	}

	
}

int lexerac_expect ( int atom ) {

	//	author : Jelo Wang
	//	since : 20100430
	//	(C)TOK

	if ( atom != lexerac_head_genv (1) ) {
		return 0 ;
	} else {
		lexerac_genv () ;
		return 1 ;
	}
	
}

int lexerac_skip_bracket ( int bracket_type ) {

	//	author : Jelo Wang
	//	since : 20100116
	//	(C)TOK

	//	skip '(.....)'

	int stack = 0 ;
	
	char bracket_left = 0 ;
	char bracket_right = 0 ;

	if ( LEXLAC_STRONG_BRACKET == bracket_type ) {
		
		bracket_left = '{' ;
		bracket_right = '}' ;
		
	} else if ( LEXLAC_MID_BRACKET == bracket_type ) { 
	
		bracket_left = '[' ;
		bracket_right = ']' ;	
		
	} else if ( LEXLAC_SMART_BRACKET == bracket_type ) {
	
		bracket_left = '(' ;
		bracket_right = ')' ;	
		
	}
	
	if ( bracket_left != lexac->c ) return 0 ;

	while ( !lexac -> stop ) {

		if ( bracket_left == lexac->c ) stack ++ ;
		if ( bracket_right == lexac->c ) stack -- ;

		lexerac_next () ;

		if ( 0 == stack ) break ;
			
	}

	return 1 ;

	
	
}

int lexerac_matchop ( int el ) {

	//	author : Jelo Wang
	//	since : 2008
	//	updated : 20090813
	//	notes : identify operator
	//	(C)TOK
	
	int STATE=0;
	int TYPE=0;

	static char buffer [ 4 ] ;

	if ( '=' == lexerac_look ( 1 ) ) {
		
		switch ( el ) {
			
			case '=':
				
				lexerac_jump ( 2 ) ;
				sc_strcpy ( buffer , "==" ) ;
				
				lexac->token = buffer ;
				lexac->c = 0 ;
				lexac->pv = lexac->v ;
				lexac->v = LAC_EQ ;

			break;
			
			case '<':
				lexerac_jump ( 2 ) ;
				sc_strcpy ( buffer , "<=" ) ;
				
				lexac->token = buffer ;
				lexac->c = 0 ;
				lexac->pv = lexac->v ;				
				lexac->v = LAC_LE ;	

			break;
			
			case '>':
				lexerac_jump ( 2 ) ;
				sc_strcpy ( buffer , ">=" ) ;
				
				lexac->token = buffer ;
				lexac->c = 0 ;
				lexac->pv = lexac->v ;				
				lexac->v = LAC_GE ;					
			break;
			
			case '!':
				lexerac_jump ( 2 ) ;
				sc_strcpy ( buffer , "!=" ) ;
				
				lexac->token = buffer ;
				lexac->c = 0 ;
				lexac->pv = lexac->v ;				
				lexac->v = LAC_NE ;						
			break;
			
			case '+':
				lexerac_jump ( 2 ) ;
				sc_strcpy ( buffer , "+=" ) ;
				
				lexac->token = buffer ;
				lexac->c = 0 ;
				lexac->pv = lexac->v ;				
				lexac->v = LAC_SLA ;							
			break;
			
			case '-':
				lexerac_jump ( 2 ) ;
				sc_strcpy ( buffer , "-=" ) ;
				
				lexac->token = buffer ;
				lexac->c = 0 ;
				lexac->pv = lexac->v ;				
				lexac->v = LAC_SLS ;				
			break;
			
			case '*':
				lexerac_jump ( 2 ) ;
				sc_strcpy ( buffer , "*=" ) ;
				
				lexac->token = buffer ;
				lexac->c = 0 ;
				lexac->pv = lexac->v ;				
				lexac->v = LAC_SLM ;							
			break;
			
			case '/':
				lexerac_jump ( 2 ) ;
				sc_strcpy ( buffer , "/=" ) ;
				
				lexac->token = buffer ;
				lexac->c = 0 ;
				lexac->pv = lexac->v ;				
				lexac->v = LAC_SLD ;						
			break;
			
			case '|':
				lexerac_jump ( 2 ) ;
				sc_strcpy ( buffer , "|=" ) ;
				
				lexac->token = buffer ;
				lexac->c = 0 ;
				lexac->pv = lexac->v ;				
				lexac->v = LAC_BEO ;						
			break;
			
			case '&':
				lexerac_jump ( 2 ) ;
				sc_strcpy ( buffer , "&=" ) ;
				
				lexac->token = buffer ;
				lexac->c = 0 ;
				lexac->pv = lexac->v ;				
				lexac->v = LAC_BEA ;						
			break;
			
			case '^':
				lexerac_jump ( 2 ) ;
				sc_strcpy ( buffer , "^=" ) ;
				
				lexac->token = buffer ;
				lexac->c = 0 ;
				lexac->pv = lexac->v ;				
				lexac->v = LAC_BEY ;						
			break;
			
			case '%':
				lexerac_jump ( 2 ) ;
				sc_strcpy ( buffer , "%=" ) ;
				
				lexac->token = buffer ;
				lexac->c = 0 ;
				lexac->pv = lexac->v ;				
				lexac->v = LAC_BEB ;						
			break;
			
		}
		
	} else {

		switch ( el ) {
			
			case '=':
				
				lexerac_jump ( 1 ) ;
				sc_strcpy ( buffer , "=" ) ;
				
				lexac->token = buffer ;
				lexac->c = 0 ;
				lexac->pv = lexac->v ;				
				lexac->v = LAC_EQU ;			
				
			break;
			
			case '<':

				if ( '<' == lexerac_look ( 1 ) && '=' == lexerac_look ( 2 ) ) {
					
					lexerac_jump(3) ;
					sc_strcpy ( buffer , "<<=" ) ;
					
					lexac->token = buffer ;
					lexac->c = 0 ;
					lexac->pv = lexac->v ;					
					lexac->v = LAC_SHLL ;	
				
				} else if ( '<' == lexerac_look ( 1 ) ) {

					lexerac_jump ( 2 ) ;
					sc_strcpy ( buffer , "<<" ) ;
					
					lexac->token = buffer ;
					lexac->c = 0 ;
					lexac->pv = lexac->v ;					
					lexac->v = LAC_SHL ;						

				} else {
				
					lexerac_jump ( 1 ) ;
					sc_strcpy ( buffer , "<" ) ;
					
					lexac->token = buffer ;
					lexac->c = 0 ;
					lexac->pv = lexac->v ;					 
					lexac->v = LAC_LT ;			

				}
				
			break;
			
			case '>':
				
				if ( '>' == lexerac_look ( 1 ) && '=' == lexerac_look ( 2 ) ) {

					lexerac_jump ( 3 )  ;
					sc_strcpy ( buffer , ">>=" ) ;
					
					lexac->token = buffer ;
					lexac->c = 0 ;
					lexac->pv = lexac->v ;					 
					lexac->v = LAC_SHRR ;
				
				} else if ( '>' == lexerac_look ( 1 ) ) {

					lexerac_jump ( 2 ) ;
					sc_strcpy ( buffer , ">>" ) ;
					
					lexac->token = buffer ;
					lexac->c = 0 ;
					lexac->pv = lexac->v ;	
				 	lexac->v = LAC_SHR ;
				
				} else {
			    	
					lexerac_jump ( 1 ) ;
					sc_strcpy ( buffer , ">" ) ;
					
					lexac->token = buffer ;
					lexac->c = 0 ;
					lexac->pv = lexac->v ;					
				 	lexac->v = LAC_GT ;		
				
				}
					
			break;
			
			case '!':
				
				lexerac_jump ( 1 ) ;
				sc_strcpy ( buffer , "!" ) ;
				
				lexac->token = buffer ;
				lexac->c = 0 ;
				lexac->pv = lexac->v ;				
				lexac->v = LAC_TAN ;						
				
			break;
			
			case '+':
				
				if ( '+' == lexerac_look ( 1 ) ) {

					lexerac_jump ( 2 ) ;
					sc_strcpy ( buffer , "++" ) ;
					
					lexac->token = buffer ;
					lexac->c = 0 ;
					lexac->pv = lexac->v ;					
					lexac->v = LAC_JAA ;	
					
				} else {
					
					int lexv = 0 ; 
					lexerac_jump ( 1 ) ;
					//lexv = lexerac_head_genv (1) ;

					//if ( lexerac_digit_lex (lexv) ) {
						
					//	lexerac_genv () ;

					//	return 1 ;

					//} else {

						sc_strcpy ( buffer , "+" ) ;
						
						lexac->token = buffer ;
						lexac->c = 0 ;
						lexac->pv = lexac->v ;					 
						lexac->v = LAC_ADD ;
						
					//}

					
				}
				
			break;
			
			case '-':
				
				if ( '-' == lexerac_look ( 1 ) ) {
					
					lexerac_jump ( 2 ) ;
					sc_strcpy ( buffer , "--" ) ;
					
					lexac->token = buffer ;
					lexac->c = 0 ;
					lexac->pv = lexac->v ;					
					lexac->v = LAC_JNN ;
				
				} else if ( '>' == lexerac_look ( 1 ) ) {
			    	
					 lexerac_jump ( 2 ) ;
					sc_strcpy ( buffer , "->" ) ;
					
					lexac->token = buffer ;
					lexac->c = 0 ;
					lexac->pv = lexac->v ;					 
					lexac->v = LAC_PNT ;					 
					 
				} else {

					int lexv = 0 ; 
					lexerac_jump ( 1 ) ;
					lexv = lexerac_head_genv (1) ;
					
					if ( lexerac_digit_lex (lexv) ) {
						
						lexerac_genv () ;
						SCClStringInsert ( &LACTOK , "-" , 0 ) ;
						
						switch ( lexac->v ) {
							case LAC_INTNUM : 
								lexac->pv = lexac->v ;
								lexac->v = LAC_MUS_INTNUM ;
							break ;
							case LAC_FLTNUM :   
								lexac->pv = lexac->v ;
								lexac->v = LAC_MUS_FLTNUM ;
							break ;
							case LAC_HEXNUM :   
								lexac->pv = lexac->v ;
								lexac->v = LAC_MUS_HEXNUM ;
							break ;
							case LAC_FLTENUM :  
								lexac->pv = lexac->v ;
								lexac->v = LAC_MUS_FLTENUM ;
							break ;
							case LAC_INTENUM : 
								lexac->pv = lexac->v ;
								lexac->v = LAC_MUS_INTENUM ;
							break ;
						}

						return 1 ;

					} else {
						
						sc_strcpy ( buffer , "-" ) ;
						
						lexac->token = buffer ;
						lexac->c = 0 ;
						lexac->pv = lexac->v ;					 
						lexac->v = LAC_SUB ;			
					
					}
					 
				}
				
			break;
			
			//	int* gtkings;
			//	int a,a2,a3,*a4,a5;
			//	pointer gtkings;
			//	pointer a4;
			
			case '*':
				
				if ( '*' == lexerac_look ( 1 ) ) {

					lexerac_jump ( 2 ) ;
					sc_strcpy ( buffer , "**" ) ;
					
					lexac->token = buffer ;
					lexac->c = 0 ;
					lexac->pv = lexac->v ;					
					lexac->v = LAC_PPT ;							 

				} else {

					lexerac_jump ( 1 ) ;
					sc_strcpy ( buffer , "*" ) ;
					
					lexac->token = buffer ;
					lexac->c = 0 ;
					lexac->pv = lexac->v ;					
					lexac->v = LAC_MUL ;						 

				}
				
			break;
			
			case '/':
				

				if ( '/' == lexerac_look ( 1 ) ) {
					
					lexerac_drop_junk ('/') ;

					lexac->token = 0 ;
					lexac->c = 0 ;
					lexac->pv = 0 ;				
					lexac->v = 0 ;

				} else if( '*' == lexerac_look ( 1 ) ) {

					lexerac_drop_junk ('*') ;

					lexac->token = 0 ;
					lexac->c = 0 ;
					lexac->pv = 0 ;				
					lexac->v = 0 ;

				} else {

					lexerac_jump ( 1 ) ;
					sc_strcpy ( buffer , "/" ) ;
					
					lexac->token = buffer ;
					lexac->c = 0 ;
					lexac->pv = lexac->v ;				
					lexac->v = LAC_DIV ;
				
				}
				
			break;
			
			case '|':
				
				if ( '|' == lexerac_look ( 1 ) ) {

					lexerac_jump ( 2 ) ;
					sc_strcpy ( buffer , "||" ) ;
					
					lexac->token = buffer ;
					lexac->c = 0 ;
					lexac->pv = lexac->v ;					
					lexac->v = LAC_OR ;						 

				} else {

					lexerac_jump ( 1 ) ;
					sc_strcpy ( buffer , "|" ) ;
					
					lexac->token = buffer ;
					lexac->c = 0 ;
					lexac->pv = lexac->v ;					
					lexac->v = LAC_HU ;						 

				}
				
			break;
			
			case '&':
				
				if ( '&' == lexerac_look ( 1 ) ) {

					lexerac_jump ( 2 ) ;
					sc_strcpy ( buffer , "&&" ) ;
					
					lexac->token = buffer ;
					lexac->c = 0 ;
					lexac->pv = lexac->v ;					
					lexac->v = LAC_AND ;			 
					 
				} else {

					lexerac_jump ( 1 ) ;
					sc_strcpy ( buffer , "&" ) ;
					
					lexac->token = buffer ;
					lexac->c = 0 ;
					lexac->pv = lexac->v ;					
					lexac->v = LAC_HE ;		
					
				}
				
			break;
			
			case '^':
				
				lexerac_jump ( 1 ) ;
				sc_strcpy ( buffer , "^" ) ;
					
				lexac->token = buffer ;
				lexac->c = 0 ;
				lexac->pv = lexac->v ;				
				lexac->v = LAC_YHH ;					
				
			break;

/*

							{

								//	generate lsn
								SCClString Lsn = {0} ; 
								lexerac_next () ;
									
								while ( sc_is_digit(lexac->c) ) {
									SCClStringAdd ( &Lsn , lexerac_get_atom () ) ;
									lexerac_next () ;
								}   
								
								SCClStringAdd ( &Lsn , '\0' ) ;

								lexac->lsn = SCClAtoi ( Lsn.data ) ;								
								SCClStringDestroyEx ( &Lsn ) ;
	
							}
*/
			
			case '%':

				if (  '$' == lexerac_look ( 1 )  ) {
					
					lexerac_jump ( 2 ) ;
					
					//	skip these junk streams that we donnt needed
					lexerac_skip_space () ;
					
					while( lexerac_is_var () ) ;

					if ( 'V' == LACTOK.data[0] && '.' == lexerac_look ( 0 ) ) {
					
						//	virtual register detected 					
						SCClStringInsert ( &LACTOK , "%$" , 0 ) ;
						
						{

							//	generate lsn
							SCClString Lsn = {0} ; 
							lexerac_next () ;
									
							while ( sc_is_digit(lexac->c) ) {
								SCClStringAdd ( &Lsn , lexerac_get_atom () ) ;
								lexerac_next () ;
							}   

							SCClStringAdd ( &Lsn , '\0' ) ;

							lexac->lsn = SCClAtoi ( Lsn.data ) ;								
							SCClStringDestroyEx ( &Lsn ) ;

							SCClStringAdd ( &LACTOK , '.' ) ;										
							SCClStringAddStr ( &LACTOK , SCClItoa (lexac->lsn)) ;		
							SCClStringAdd ( &LACTOK , '\0' ) ;							

						}
							
						lexac->pv = lexac->v ;				
						lexac->v = LAC_VREG ;
						lexac->token = LACTOK.data ;
						
					} else if ( 'P' == LACTOK.data[0] && 'E' == LACTOK.data[1] ) {
						
						lexac->pv = lexac->v ;				
						lexac->v = LAC_PE ;
						lexac->token = LACTOK.data ;						

 					} else if ( 'C' == LACTOK.data[0] && 'A' == LACTOK.data[1] ) {

						lexac->pv = lexac->v ;				
						lexac->v = LAC_CA ;
						lexac->token = LACTOK.data ;						

					}
					
					return STATE;
					
				}
				
				lexerac_jump ( 1 ) ;
				sc_strcpy ( buffer , "%" ) ;
					
				lexac->token = buffer ;
				lexac->c = 0 ;
				lexac->pv = lexac->v ;				
				lexac->v = LAC_MOD ;						
				
			break;

		}
	
		
	}
	
	return STATE;

}

int lexerac_genv ()  {

	//	author : Jelo Wang
	//	since : 2008
	//	(C)TOK
	
	int state = 0;

	char suffix = 0 ;

	SCClStringReset ( &LACTOK ) ;

	while ( !lexac->stop ) {
		
		lexerac_get_atom () ;

		switch ( state ) {

			case 0:

				if ( sc_is_alpha (lexac->c) || '_' == lexac->c ) state = 1 ;
				
				else if ( sc_is_digit (lexac->c) ) state = 3 ;

				else if ( 0x20 == lexac->c ) {

					lexerac_next () ;
					lexerac_get_atom () ;
					
					lexac->token = 0 ;

					lexac->c = ' ';

					lexac->ppv = lexac->pv ;
					lexac->pv = lexac->v ;				
					lexac->v = LAC_SPACE ;

					if ( !(lexac->mode & LEXLAC_FLITER_MODE) )
						return 1 ;

				} else if ( '\n' == lexac->c  ) {

					lexac->line ++;
					lexerac_next () ;

					lexac->token = 0 ;
					
					lexac->c = '\n';
					lexac->ppv = lexac->pv ;
					lexac->pv = lexac->v ;				
					lexac->v = LAC_ENTER ;

					if ( !(lexac->mode & LEXLAC_FLITER_MODE) )
						return 1 ;
					

				} 

				else if ( 13 == lexac->c ) {
					
					//	dont return a changing row token to above modules
					//	lexac->line ++;
					//	lexerac_next () ;
					//	lexac->ppv = lexac->pv ;
					//	lexac->pv = lexac->v ;				
					//	lexac->v = CHROW ;
					
					lexerac_next () ;
					
					lexac->token = 0 ;
					
					lexac->c = 0 ;

					lexac->ppv = lexac->pv ;
					lexac->pv = lexac->v ;				
					lexac->v = LAC_CHROW ;

					if ( !(lexac->mode & LEXLAC_FLITER_MODE) )
						return 1 ;
					
				}

				else if ( '\\' == lexac->c ) {

					lexerac_next () ;

					lexac->token = 0 ;
					
					lexac->c = '\\';
					lexac->ppv = lexac->pv ;
					lexac->pv = lexac->v ;
					lexac->v = LAC_ESCAPE ;
 
					return 1 ;

				}				
				
				else if ( '\t' == lexac->c ) {

					lexerac_next () ;
					lexerac_get_atom () ;

					lexac->token = 0 ;
					
					lexac->c = '\t';
					lexac->ppv = lexac->pv ;
					lexac->pv = lexac->v ;
					lexac->v = LAC_TABLEK ;
					
					//return 0 ;

				}
				
				else if ( '=' == lexac->c ) state = 5 ;
				else if ( '+' == lexac->c ) state = 5 ;
				else if ( '-' == lexac->c ) state = 5 ;
				else if ( '<' == lexac->c ) state = 5 ;
				else if ( '>' == lexac->c ) state = 5 ;
				else if ( '|' == lexac->c ) state = 5 ;
				else if ( '&' == lexac->c ) state = 5 ;
				else if ( '!' == lexac->c ) state = 5 ;
				else if ( '*' == lexac->c ) state = 5 ;
				else if ( '/' == lexac->c ) state = 5 ;
				else if ( '%' == lexac->c ) state = 5 ;
				else if ( '^' == lexac->c ) state = 5 ;

				else if( '\''== lexac->c ) state = 6;
				else if( '"'== lexac->c ) state = 6;

				else if ( '{' == lexac->c ) { 

					lexerac_next () ;
					lexac->token = 0 ;
					lexac->c = '{' ;
					lexac->pv = lexac->v ;				
					lexac->v = LAC_DKL ;
					
					return 1 ;

				} else if ( '}' == lexac->c ) { 

					lexerac_next () ;
					lexac->token = 0 ;
					lexac->c = '}' ;
					lexac->pv = lexac->v ;				
					lexac->v = LAC_DKR ;	

					return 1 ;

				} else if ( '[' == lexac->c ) { 

					lexerac_next () ;
					lexac->token = 0 ;
					lexac->c = '[' ;
					lexac->pv = lexac->v ;				
					lexac->v = LAC_ZKL ;	

					return 1 ;

				} else if ( ']' == lexac->c ) { 

					lexerac_next () ;
					lexac->token = 0 ;
					lexac->c = ']' ;
					lexac->pv = lexac->v ;				
					lexac->v = LAC_ZKR ;	

					return 1 ;

				} else if ( '(' == lexac->c ) { 

					lexerac_next () ;
					lexac->token = 0 ;
					lexac->c = '(' ;
					lexac->ppv = lexac->pv ;
					lexac->pv = lexac->v ;				
					lexac->v = LAC_XKL ;	

					return 1 ;

				} else if ( ')' == lexac->c ) { 

					lexerac_next () ;
					lexac->token = 0 ;
					lexac->c = ')' ;
					lexac->pv = lexac->v ;				
					lexac->v = LAC_XKR ;	
					return 1 ;

				} else if ( ';' == lexac->c ) { 

					lexerac_next () ;
					lexac->token = 0 ;
					lexac->c = ';' ;
					lexac->pv = lexac->v ;				
					lexac->v = LAC_FEN ;	
					return 1 ;

				} else if ( ',' == lexac->c ) { 

					lexerac_next () ;
					lexac->token = 0 ;
					lexac->c = ',' ;
					lexac->pv = lexac->v ;				
					lexac->v = LAC_DOU ;	
					return 1 ;

				} else if ( '.' == lexac->c ) { 

					lexerac_next () ;
					lexac->token = 0 ;
					lexac->c = '.' ;
					lexac->pv = lexac->v ;				
					lexac->v = LAC_FIL ;	
					return 1 ;

				} else {
					
					if ( 0 == lexac -> ios ) {
						
						lexac->token = 0 ;
						lexac->pv = lexac->v ;				
						lexac->v = 0 ;	

						SClog ( "unrecognizable symbol : %x detected , on line : %d\n" , lexac->c , lexac->line ) ;

						lexerac_next () ;
						
						return 0 ;

					} 

					lexerac_next () ;

				}

				
			break;

			case 1:
				
				while( lexerac_is_var () ) ;

				lexac->token = LACTOK.data ;					
				
				lexac->pv = lexac->v;

				lexac->v = lexerac_find_key ( lexac->token ) ;

				if ( 0 == lexac->v ) {

					//	backup lexer walker
					lexerac_backup () ;										
					lexerac_skip_space () ;
						
					if ( '(' == lexerac_look ( 0 ) ) {

						lexerac_skip_bracket ( LEXLAC_SMART_BRACKET ) ;
						lexerac_skip_blank () ;
						
						if ( '{' == lexerac_look ( 0 ) ) {

							lexac->v = LAC_FUNCDEF ;	
							lexac->c = 0 ;
						
						} else {
						
							lexac->v = LAC_FUNCCAL ;	
							lexac->c = 0 ;
							
						}
						
					} else if ( '[' == lexac->c ) {

						lexac->v = LAC_ARRAY ;	
						lexac->c = 0 ;		
												
					} else if ( ':' == lexac->c ) {

						lexac->v = LAC_LABEL ;	
						lexac->c = 0 ;									

					} else {	

						//	variable-name.number
						if ( '.' == lexac->c && sc_is_digit (lexerac_look (1)) ) {

							{

								//	generate lsn
								SCClString Lsn = {0} ; 
								lexerac_next () ;
									
								while ( sc_is_digit(lexac->c) ) {
									SCClStringAdd ( &Lsn , lexerac_get_atom () ) ;
									lexerac_next () ;
								}   
								
								SCClStringAdd ( &Lsn , '\0' ) ;

								lexac->lsn = SCClAtoi ( Lsn.data ) ;
 								SCClStringDestroyEx ( &Lsn ) ;

								SCClStringAdd ( &LACTOK , '.' ) ;
								SCClStringAddStr ( &LACTOK , SCClItoa (lexac->lsn)) ;
								SCClStringAdd ( &LACTOK , '\0' ) ;
						
							}
								
							if ( 0 == lexac->scale ) {
								lexac->v = LAC_VAR ;	
								lexac->c = 0 ;		
							} else {

								lexac->v = LAC_VARDEF ;	
								lexac->c = 0 ;	
							}

							return 1 ;
							
						}
						
					}

					//	rollback walker of lexer
					lexerac_rollback () ;
				
				} else {
		
					lexac->c = 0 ;

				}
				
				return 1 ;

			break;

			case 2:

				lexac -> pc = lexac->c ;
				
				//	skip these junk streams that we donnt needed
				lexerac_skip_space () ;
				
 				while( lexerac_is_var () ) ;

				lexac->token = LACTOK.data ;
				
				lexac->c = 0 ;
				lexac->v = 0 ;
					
				lexac->pv = lexac->v;

				lexac->v = LAC_VAR ;	
											
				return 1 ;

			break;


			case 3:
				
				while( lexerac_is_num () ) ;

				if( '.' == lexac->c ) {

					SCClStringAdd ( &LACTOK , lexac->c  ) ;
					lexerac_jump ( 1 ) ;
					state = 4 ;

				} else if( 'e' == lexac->c || 'E' == lexac->c ) {

					SCClStringAdd ( &LACTOK , lexac->c  ) ;
					lexerac_jump ( 1 ) ;
					
					if ( '-' == lexac->c || '+' == lexac->c  ) {
						SCClStringAdd ( &LACTOK , lexac->c  ) ;
						lexerac_jump ( 1 ) ;
					}

					while( lexerac_is_num () ) ;

					SCClStringAdd ( &LACTOK , LAC_EOS ) ;

					lexac->token = LACTOK.data ;
					lexac->pv = lexac->v ;
					lexac->v = LAC_INTENUM ;

					return 1 ;

				} else {

					suffix = lexac->code -> data [ lexac->code -> get_walker ] ;
					
					SCClStringAdd ( &LACTOK , LAC_EOS ) ;

					lexac->token = LACTOK.data ;
					lexac->pv = lexac->v ;
					lexac->v = LAC_INTNUM ;

					return 1 ;	
					
				}

				
			break;


			case 4:

				if( 'e' == lexac->c ) {
					
					SCClStringAdd ( &LACTOK , lexac->c ) ;
					lexerac_jump ( 1 ) ;

					if ( '-' == lexac->c || '+' == lexac->c  ) {
						SCClStringAdd ( &LACTOK , lexac->c ) ;
						lexerac_jump ( 1 ) ;
					}
					
					while( lexerac_is_num () ) ;
					
					SCClStringAdd ( &LACTOK , LAC_EOS ) ;

					lexac->token = LACTOK.data ;
					lexac->pv = lexac->v ;
					lexac->v = LAC_FLTENUM ;

					return 1 ;

				} else {
			
					while( lexerac_is_num () ) ;

					if( 'e' == lexac->c ) {

						state = 4 ;

					} else {
					
						SCClStringAdd ( &LACTOK , LAC_EOS ) ;

						lexac->token = LACTOK.data ;
							
						lexac->pv = lexac->v ;
						lexac->v = LAC_FLTNUM ;
						
						return 1 ;

					}

				

				}
			
			break;

			case 5 :

				lexerac_matchop ( lexac->c ) ;

				return 1 ;
				
			break;

			case 6 :

				lexerac_cluster ( lexac->c ) ;

				return 1 ;
				
			break ;

			default :
				return 1 ;

		}

	}

	return 0 ;

}


void lexerac_clear_scale () {

	//	author : Jelo Wang
	//	since : 20100508
	//	(C)TOK

	if ( !lexac ) return ;

	lexac->scale = 0 ;
	
}


int lexerac_head_genv ( int border ) {

	//	author : Jelo Wang
	//	since : 20100119
	//	(C)TOK

	int value = -1 ;
	int walker = 0 ;

	int orgvalue = lexac->v ;	
	int orgline = lexac->line ;
	int orgpv = lexac->pv ;
	int orgppv = lexac->ppv ;
	int orgscale = lexac->scale ;

	
	char* orgtoken = 0 ;
	char orgchar = lexac->c ;

	lexerac_backup () ;
	
	if ( lexac->token ) {
		orgtoken = (char*) SCMalloc ( sc_strlen ( lexac->token ) + 1 ) ;
		sc_strcpy_withlen ( orgtoken , lexac->token , sc_strlen (lexac->token) ) ;
	}
	
	for ( walker = 0 ; walker < border ; walker ++ )  {
		lexac->deep ++ ;
		lexerac_genv () ;
		lexac->deep -- ;
	}

	if ( orgtoken ) {
		SCClStringReset ( &LACTOK ) ;
		SCClStringAddStr ( &LACTOK , orgtoken ) ;
		SCClStringAdd ( &LACTOK , '\0' ) ;
		SCFree ( orgtoken ) ;
	}
	
	value = lexac->v ;

	lexac->v = orgvalue ;
	lexac->token = LACTOK.data ;
	lexac->line = orgline ;
	lexac->pv = orgpv ;
	lexac->ppv = orgppv ;
	lexac->scale = orgscale ;
	
	lexerac_rollback () ;
	
	return value ; 
	
}

