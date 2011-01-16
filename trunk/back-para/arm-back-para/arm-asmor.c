
/*

+	ARMv ASM generator , part of SC arm-back-para

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
# include "lac-grammar.h"
# include "lac-lexer.h"
# include "Regoc.h"
# include "arm-asm.h"
# include "arm-asmor.h"

SCClString ArmAsm = { 0 , 0 , 0 , 0 , 0 } ;

# define DUA_OPERATOR(x) (LAC_SUB <= x && x <= LAC_HE)

static void asmor_arm_gen_exp ( SCClString* lac ) {

	//	author : Jelo Wang
	//	since : 20100723
	//	(C)TOK	

	//	generate the form like '%$R.n = An-1+An' into ARM codes
	//	results ADD Rn , An , An-1

	char Rd [32] = {0} ;
	char Rn [32] = {0} ;
	char Rm [32] = {0} ;

	int state = 0 ;

	//	backup the orignal lexer
	int lexerac = lexerac_get () ;
	
	//	generate a new laxer
	lexerac_set ( lexerac_new ( lac->data , LEXLAC_FLITER_MODE ) ) ; 
	lexerac_ready () ;
		
	while ( !lexac->stop ) {

		lexerac_genv () ;

		switch ( state ) {
			case 0 :
				if ( LAC_VREG == lexac->v ) {
					state = 1  ;
					sc_strcpy ( Rd , lexac->token ) ;
				}
			break ;
			case 1 :
				if ( LAC_VAR == lexac->v ) {
					state = 2  ;
					sc_strcpy ( Rn , lexac->token ) ;
				}
			break ;
			case 2 :
				if ( DUA_OPERATOR(lexac->v) ) {
					state = 3  ;
				}
			break ;
			case 3 :
				if ( LAC_VAR == lexac->v ) {
					lexac->stop = 1 ;
					sc_strcpy ( Rm , lexac->token ) ;
					THUMB_ADD_RdRnRm (RegocAlloc(Rd),RegocAlloc(Rn),RegocAlloc(Rm)) ;
				}
			break ;
		}
			
	}

	//	set the orignal lexer
	lexerac_set ( lexerac ) ;
		
}

static int asmor_arm_read_variable_inf () {

	
	//	author : Jelo Wang
	//	since : 20100722
	//	(C)TOK

	if ( LAC_VAR != lexac->v ) return 0 ;
	
	return 1 ;

}

static int asmor_arm_read_virtual_reg () {

	
	//	author : Jelo Wang
	//	since : 20100722
	//	(C)TOK

	SCClString lac = { 0 , 0 , 0 , 0 , 0 } ;

	if ( LAC_VREG != lexac->v ) return 0 ;

	SCClStringInit ( &lac ) ;
	SCClStringAddStr ( &lac , lexac->token ) ;
	
	while ( !lexac->stop ) {

		lexerac_genv () ;
		
		if ( LAC_FEN == lexac->v ) break ;

		SCClStringAddStr ( &lac , lexac->token ) ;
		
	}

	SCClStringAdd ( &lac , '\0' ) ;
	
	//	generate arm-exp form
	asmor_arm_gen_exp ( &lac ) ;

	return 1 ;

}


static int asmor_arm_read_symbol_inf () {

	//	author : Jelo Wang
	//	since : 20100722
	//	(C)TOK

	switch ( lexac->v ) {
		case LAC_VAR :
			return asmor_arm_read_variable_inf () ;
		break ;
		case LAC_VREG :
			return asmor_arm_read_virtual_reg () ;
		break ;
		default : return 0 ;
	}

	return 1 ;
	
}

static int asmor_arm_read_symbol_def () {

	//	author : Jelo Wang
	//	since : 20100722
	//	(C)TOK

	switch ( lexac->v ) {

		default : return 0 ;
	}

	return 1 ;
	
}

int asmor_arm_funcdef () {

	//	author : Jelo Wang
	//	since : 20100505
	//	(C)TOK

	int stack = 0 ;
	
	if ( LAC_FUNCDEF != lexac->v ) return 0 ;

	SCClStringAddStr ( &ArmAsm , "proc " ) ;
	SCClStringAddStr ( &ArmAsm , lexac->token ) ;
	SCClStringAddStr ( &ArmAsm , "\r\n" ) ;

	//	parsing parameter scope of function
	stack = 0 ;
	while ( !lexac->stop ) {
		
		lexerac_genv () ;
		
		if ( LAC_XKL == lexac->v )
			stack ++ ;
		if ( LAC_XKR == lexac->v )
			stack -- ;
		
		if ( 0 == stack ) 
			break ;
		
	}

	//	parsing function body
	stack = 0 ;
	while ( !lexac->stop ) {
		
		lexerac_genv () ;
		
		if ( LAC_DKL == lexac->v )
			stack ++ ;
		if ( LAC_DKR == lexac->v )
			stack -- ;
		
		if ( 0 == stack ) 
			break ;		

		if ( LAC_LABEL == lexac->v ) {
			SCClStringAddStr ( &ArmAsm , sc_strcat ( lexac->token , ":\r\n")  ) ;
			continue ;
		}

		asmor_arm_read_symbol_def () ;
		asmor_arm_read_symbol_inf () ;
		
	}	
	
	SCClStringAddStr ( &ArmAsm , "end\r\n" ) ;
	
	
}

char* asmor_arm_run ( char* lac , char* asmout ) {

	//	author : Jelo Wang
	//	since : 20100505
	//	(C)TOK
	
	char* reg[15] = {

		//	Normal registers that can be programed on ARM
		{"R1"} , {"R2"} , {"R3"} ,
		{"R4"} , {"R5"} , {"R6"} ,
		{"R7"} , 	
		
	} ;

	if ( !lac ) return 0 ;

	//	create reg-pool of register allocator 
	//	only 7 registers that can be used in ARM
	RegocRegPoolCreate ( reg , 7 ) ;
	//	create ref graph
//	RegocBuildRefGraph ( 0 )  ;
	//	set a swapper to Regoc
//	RegocSetSwapper ( &ArmAsm ) ;
	
	lexerac_set ( lexerac_new ( lac , LEXLAC_FLITER_MODE ) ) ; 
	lexerac_ready () ;

	SCClStringInit ( &ArmAsm ) ;
	
	for ( lexerac_genv () ; !lexac->stop ; lexerac_genv () ) {
		if ( LAC_FUNCDEF == lexac->v ) {
			asmor_arm_funcdef () ;
		} 
	}

	SCClStringAdd ( &ArmAsm , '\0' ) ;

	if ( SC_SASM & compiler->parameter ) {
		int file = SCHalFileOpen ( asmout , "wb+" ) ;
		SCHalFileWrite ( file , ArmAsm.data , 1 , sc_strlen (ArmAsm.data) ) ;
		SCHalFileClose ( file ) ;
		SCFree ( asmout ) ;
	}	
	
	RegocDestroy () ;
	
	return ArmAsm.data ;

}

