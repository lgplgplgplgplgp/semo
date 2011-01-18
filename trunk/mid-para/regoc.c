
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



void RegocLiveScopeMoiCreate () {

	//	author : Jelo Wang
	//	since : 20110110
	//	(C)TOK
	
	//	create live scope monitor

	lsmonitor = (LIVESCOPE** ) SCMalloc ( sizeof (LIVESCOPE*)*LSDEFAULT_LENGTH ) ;
	ASSERT ( lsmonitor ) ;
	lslength = LSDEFAULT_LENGTH ;
	lslooper = 0 ;

	//	create interference-graph
	iG = SCClGraphCreate () ;
	ASSERT ( iG ) ;
	
}


static int RegocLiveScopeFind ( char* live , int scope ) {

	//	author : Jelo Wang
	//	since : 20110107
	//	(C)TOK

	int looper = -1 ;

	//	search the live
	for ( looper = lslooper-1 ; looper > -1 ; looper -- ) {
		
		if ( scope == lsmonitor[looper]->scope ) {			
			if ( !sc_strcmp (live , lsmonitor[looper]->live) ) {				
				break ;
			}
		}
		
	}

	return looper ;
	
}


int RegocCheckLiveScope ( char* live , int scope , int line ) {

	//	author : Jelo Wang
	//	since : 20110107
	//	(C)TOK

	LIVESCOPE* ls = 0 ;
	
	int offset = RegocLiveScopeFind ( live , scope ) ;

	if ( -1 == offset ) return -1 ;
 
	ls = lsmonitor[offset] ;

	ls->end_line = line ;

	return ls->number ;

}

static int RegocLiveScopeRefCheck ( LIVESCOPE* ls_1 , LIVESCOPE* ls_2 ) {

	//	if ls_2 is has interference relation with ls_1 return 1 else return 0

	if ( !ls_1 || !ls_2 ) return 0 ;

	if ( -1 == ls_1->end_line || -1 == ls_2->end_line ) return 0 ;

	if ( ls_1->start_line > ls_2->end_line ) 
		return 0 ;
	else if ( ls_2->start_line > ls_1->end_line ) 
		return 0 ;
	else if ( (ls_1->start_line == ls_2->start_line) && (ls_1->end_line == ls_2->end_line ) )
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


int RegocLiveScopeAdd ( char* live , int scope , int line ) {

	//	author : Jelo Wang
	//	since : 20110107
	//	(C)TOK
	
	//	generate live scope , and return its number

	//	if there have not enough pool for saving the live , we needs realloc
	if ( lslooper == lslength ) {	
		lsmonitor = (LIVESCOPE** ) SCRemalloc ( lsmonitor , \
			(lslooper-1)*lslength*sizeof (LIVESCOPE*) , \
			((lslooper-1)*lslength*sizeof (LIVESCOPE*))+LSDEFAULT_LENGTH*sizeof (LIVESCOPE*) ) ;

		ASSERT ( lsmonitor ) ;
		//	new length
		lslength = ((lslooper-1)*lslength)+LSDEFAULT_LENGTH ;
	} 

	lsmonitor[lslooper] = (LIVESCOPE* ) SCMalloc ( sizeof (LIVESCOPE) ) ;
	
	ASSERT (lsmonitor[lslooper]) ;
	
	sc_strcpy ( lsmonitor[lslooper]->live , live ) ;

	lsmonitor[lslooper]->scope = scope ;
	lsmonitor[lslooper]->number = lslooper ;
	lsmonitor[lslooper]->start_line = line ;
	lsmonitor[lslooper]->end_line = -1 ;
	//	plus lslooper
	lslooper ++ ;

	return lslooper - 1 ;
		
} 


void RegocLiveScopeCreateRefGraph () {

	//	author : Jelo Wang
	//	since : 20110107
	//	(C)TOK

	int looper = 0 ;
	int inlooper = 0 ;

	for ( looper = 0 ; looper < lslooper ; looper ++ ) {

		LIVESCOPE* ls_1 = lsmonitor[looper] ;

		//	add a node into ref graph
		SCClGraphAddNode ( iG , ls_1->number ) ;
		
		for ( inlooper = looper + 1 ; inlooper < lslooper ; inlooper ++ ) {

			LIVESCOPE* ls_2 = lsmonitor[inlooper] ;

			if ( RegocLiveScopeRefCheck ( ls_1 , ls_2 ) ) {
				
				SCClGraphAddNode ( iG , ls_2->number ) ;
				//	add ref edge bettwen ls_1 and ls_2
				SCClGraphAddEdge ( iG , ls_1->number , ls_2->number ) ;			
		
			}
			
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

