
/*

+	Register Allocation , part of SC mid-para

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

# include "sccl.h"
# include "schal.h"
# include "regoc.h"
# include "lac-grammar.h"
# include "lac-lexer.h"

//	register allocator
static REGOC regoc = { 0 , 0 , 0 } ;
//	register's livescope monitor , we need this information for the interference-graph generative process
static LIVESCOPE** lsmonitor = 0 ;
static int lslooper = 0 ;
static int lslength = 0 ;
//	default amouts of lsmonitor that means the totall livescopes apeared in one scope
# define LSDEFAULT_LENGTH 256
//	interference-graph
SCClGraph* iG = 0 ;

int RegocRegPoolCreate ( char** regs , int totall ) {

	//	author : Jelo Wang
	//	since : 20100730
	//	(C)TOK

	//	notes : create a register pool

	int looper = 0 ;
	
	regoc.reg = (char** ) SCMalloc ( sizeof(char*)*totall ) ;

	if ( !regoc.reg ) return 0 ;

		
	regoc.regtoll = totall ;

	for ( looper = 0 ; looper < regoc.regtoll ; looper ++ )
		sc_strcpy_ex ( &regoc.reg[looper] , regs[looper] ) ;
		
	return 1 ;

}

void RegocBuildRefGraph ( char* code , int position ) {

	//	author : Jelo Wang
	//	since : 20100831
	//	(C)TOK
	
	return ;

	//	create reference graph
	regoc.graph = SCClGraphCreate () ;

	//	we need a lac lexer to analysis live-range
	//	build a reference graph
//	lexerac_set ( lexerac_new ( codes , LEXLAC_FLITER_MODE ) ) ; 

	for ( lexerac_ready () , lexerac_genv () ; !lexac->stop ; lexerac_genv () ) {
		if ( LAC_VAR == lexac->v ) {
			SCClGraphAddNode ( (SCClGraph* )regoc.graph , lexac->token ) ;
		} 
	}

	//	find reference range
	for ( lexerac_ready () , lexerac_genv () ; !lexac->stop ; lexerac_genv () ) {
		if ( LAC_VAR == lexac->v ) {
			SCClGraphAddNode ( (SCClGraph* )regoc.graph , lexac->token ) ;
		} 
	}	

	//	coloring

	lexerac_destroy () ;

}
int RegocSetSwapper ( SCClString* scclstr ) {

	//	author : Jelo Wang
	//	since : 20100730
	//	(C)TOK

	//	When a alloc-problem needs SWAP(Rn),the engine can swap it to low-level codes with SCClString*
	regoc.swapper = (int) scclstr ;

	return 1 ;
	
}

char* RegocAlloc ( int lr ) {

	//	author : Jelo Wang
	//	since : 20100730
	//	(C)TOK

	//	Alloc a register for the live range
	//	If there are no remaining registers , the engine would swap a symbol to memory
	
	char* reg = regoc.reg[0] ;
	
	return reg ;
	
}

void RegocDestroy () {

	//	author : Jelo Wang
	//	since : 20100730
	//	(C)TOK

	int looper = 0 ;

	for ( looper = 0 ; looper < regoc.regtoll ; looper ++ ) 
		SCFree(regoc.reg[looper]) ;

	SCFreeEx(&regoc.reg) ;

}

static int RegocCheckLiveScope ( LIVESCOPE* ls_1 , LIVESCOPE* ls_2 ) {

	//	author : Jelo Wang
	//	since : 20110107
	//	(C)TOK

	//	if ls_2 is has interference relation with ls_1 return 1 else return 0

	if ( !ls_1 || !ls_2 ) return 0 ;

	if ( -1 == ls_1->end_line || -1 == ls_2->end_line ) return 0 ;
	
	if ( (ls_1->start_line == ls_2->start_line) && (ls_1->end_line == ls_2->end_line ) )
		return 1 ;
	else if ( (ls_1->start_line >= ls_2->start_line) && (ls_1->end_line >= ls_2->end_line ) )
		return 1 ;
	else if ( (ls_1->start_line >= ls_1->start_line) && (ls_1->end_line <= ls_2->end_line ) )
		return 1 ;
	else if ( (ls_2->start_line >= ls_1->start_line) && (ls_2->end_line >= ls_1->end_line ) )
		return 1 ;
	else if ( (ls_2->start_line >= ls_1->start_line) && (ls_2->end_line <= ls_1->end_line ) )
		return 1 ;
	
	return 0 ;
	
}


void RegocLiveScopeMoiCreate () {

	//	author : Jelo Wang
	//	since : 20110110
	//	(C)TOK
	
	//	create live scope monitor

	lsmonitor = (LIVESCOPE** ) SCMalloc ( sizeof (LIVESCOPE*)*LSDEFAULT_LENGTH ) ;
	__SCASSERT ( !lsmonitor ) ;
	lslength = LSDEFAULT_LENGTH ;
	lslooper = 0 ;

	//	create interference-graph
	iG = SCClGraphCreate () ;
	__SCASSERT ( !iG ) ;
	
}

void RegocLiveScopeAdd ( char* live , int line ) {

	//	author : Jelo Wang
	//	since : 20110107
	//	(C)TOK
	
	//	generate livescope since lac code generating process begin
	//	build interferene-graph , and run Yorktown-Allocator

	int looper = 0 ;

	//	search the live
	for ( looper = lslooper ; looper > -1 ; looper -- ) {
		if ( !sc_strcmp (live , lsmonitor[looper]->live) ) {
			break ;
		}
	}

	//	if it isnt found , insert it into lsmonitor
	if ( looper == -1 ) {

		//	if there have not enough pool for saving the live , we needs realloc
		if ( lslooper == lslength ) {
			lsmonitor = (LIVESCOPE** ) SCRemalloc ( lsmonitor , \
				(lslooper-1)*lslength*sizeof (LIVESCOPE*) , \
				((lslooper-1)*lslength*sizeof (LIVESCOPE*))+LSDEFAULT_LENGTH*sizeof (LIVESCOPE*) ) ;
			__SCASSERT ( !lsmonitor ) ;
			//	new length
			lslength = ((lslooper-1)*lslength)+LSDEFAULT_LENGTH ;
		} 

		lsmonitor[looper] = (LIVESCOPE* ) SCMalloc ( sizeof (LIVESCOPE) ) ;
		__SCASSERT (!lsmonitor[looper]) ;
		
		sc_strcpy ( lsmonitor[looper]->live , live ) ;
		lsmonitor[looper]->start_line = line ;
		lsmonitor[looper]->end_line = -1 ;
		//	plus lslooper
		lslooper ++ ;
		
	} else if ( looper > -1 ) {
	//	 if the register is already eixts which indicate the livescope completed
		lsmonitor[looper]->end_line = line ;
	}
	
	if ( 0 < lslooper ) {
		if ( RegocCheckLiveScope ( (LIVESCOPE*)lsmonitor[lslooper-1] , (LIVESCOPE*)lsmonitor[lslooper]) ) {
			//	create interference-graph			
		}
	}
	
} 


void RegocLiveScopeClear () {

	//	author : Jelo Wang
	//	since : 20110107
	//	(C)TOK
	
	//	clear livescope monitor

	lslooper = 0 ;
	
} 

