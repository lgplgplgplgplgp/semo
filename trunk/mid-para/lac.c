
/*

+	LAC , part of SC mid-para

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

# include "schal.h"
# include "sccl.h"
# include "lac.h"
# include "regoc.h"

LAC* lac = 0 ;
LABELMOI* labelmoi = 0 ;

int LACNew () {

	//	author : Jelo Wang
	//	since : 20100505
	//	(C)TOK

	LAC* lac = (LAC* ) SCMalloc ( sizeof(LAC) ) ;
	
	SCClStringInit ( &lac->code ) ;
	lac->number = 0 ;
	lac->length = 0 ; 
	lac->head = 0 ;
	lac->next = 0 ;

	return (int) lac ;

} 

void LACSetContext ( int context ) {

	//	author : Jelo Wang
	//	since : 20100505
	//	(C)TOK

	lac = (LAC* ) context ;

} 

int LACAdd ( char* string , LAC_ATOM type , int scope ) {

	//	author : Jelo Wang
	//	since : 20100505
	//	(C)TOK
	
	LAC* lacn = (LAC* ) SCMalloc ( sizeof(LAC) ) ;
	
	SCClStringInitEx ( &lacn->code , sc_strlen ( string ) ) ;
 	SCClStringAddStr ( &lacn->code , string ) ;
	
	SCClStringAdd ( &lacn->code , '\0' ) ;
	
	lacn->havelabel = 0 ;
	lacn->length = 0 ;
	lacn->type = type ;
	lacn->scope = scope ;
	//	number of lac node
	lacn->number = lac->length ; 
	lacn->line = lac->line ;
	lacn->head = 0 ;
	lacn->next = 0 ;

	if ( LAC_CR == lacn->type )
		lac->line ++ ;
	
	if ( 0 == lac->head ) {
		lac->head = lacn ;
		lac->next = lacn ;
	} else {
		lac->next->next = lacn ;
		lac->next = lacn ;
	}

	lac->length ++ ;
	lac->colen = lac->colen + lacn->code.length ;
	
	return lacn ;

}

LAC* LACGet ( int number ) {

	//	author : Jelo Wang
	//	since : 20100505
	//	(C)TOK
	
	LAC* walker = lac->head ; 
	
	for ( ; walker && number != walker->number ; walker = walker->next ) ;

	return walker ;

}

void LACAddLabel ( int lacn , char* label ) {

	//	author : Jelo Wang
	//	since : 20100505
	//	(C)TOK
	
	LAC* lachandle = (LAC* ) lacn ;
		
	if ( !lachandle ) return ;
	if ( 1 == lachandle->havelabel ) return ; 
	
	SCClStringAddStr ( &lachandle->code , label ) ;	
	SCClStringAdd ( &lachandle->code , '\0' ) ;	

	lachandle->havelabel = 1 ;
	
}

char* LACGetContent () {

	//	author : Jelo Wang
	//	since : 20100505
	//	(C)TOK
	
	//	convert LAC to char*
	
	LAC* walker = lac->head ; 
	SCClString string = { 0 , 0 , 0 , 0 , 0 } ;

	SCClStringInitEx ( &string , lac->colen ) ;
	
	for ( ; walker ; walker = walker->next ) {

		SCClStringAddStr ( &string , walker->code.data ) ;

	}	

	SCClStringAdd ( &string , '\0' ) ;

	return string.data ;

	
}

void LACLiveScopeGenerate () {
	
	//	author : Jelo Wang
	//	since : 20110118
	//	(C)TOK

	LAC* llooper = lac ;

	if ( !llooper ) return ;

	RegocLiveScopeMoiCreate () ;

	for ( llooper = llooper->head ; llooper ; llooper = llooper->next ) {

		if ( LAC_L_DELT == llooper->type ) {			
			int lsn = RegocLiveScopeAdd ( llooper->code.data , llooper->scope , llooper->line ) ;
			char* value = sc_strcat ( "." , SCClItoa (lsn) ) ;		
			SCClStringAddStr ( &llooper->code , value ) ;
			SCClStringAdd ( &llooper->code , 0 ) ;			
			SCFree ( value ) ;			
		} else if ( LAC_R_DELT == llooper->type ) {
			int lsn = RegocCheckLiveScope ( llooper->code.data , llooper->scope , llooper->line ) ;
			char* value = sc_strcat ( "." , SCClItoa (lsn) ) ;
			SCClStringAddStr ( &llooper->code , value ) ;
			SCClStringAdd ( &llooper->code , 0 ) ;
			SCFree ( value ) ;			
		}
	}
	
	RegocLiveScopeCreateRefGraph () ;
		
}

void LACClear () {

	//	author : Jelo Wang
	//	since : 20100505
	//	(C)TOK


	LAC* walker = lac->head ; 
 
	for ( ; walker ; ) {
		lac->next = walker->next ; 
		SCFree ( walker->code.data ) ;
		SCFree ( walker ) ;
		walker = lac->next ;
	}	

} 


int LACLabelMoiNew () {

	//	author : Jelo Wang
	//	since : 20100505
	//	(C)TOK

	//	控制流监视器
	
	LABELMOI* monitor = (LABELMOI* ) SCMalloc ( sizeof(LABELMOI) ) ;

	monitor->lac = 0 ;
	monitor->head = 0 ;
	monitor->next = 0 ;

	return (int) monitor ;

} 

void LACLabelMoiSetContext ( int monitor ) {

	//	author : Jelo Wang
	//	since : 20100505
	//	(C)TOK
	
	labelmoi = (LABELMOI* ) monitor ;

} 

void LACLabelMoiAdd ( int lac ) {

	//	author : Jelo Wang
	//	since : 20100505
	//	(C)TOK

	//	将某LAC 节点加入监控队列
	
	LABELMOI* la = (LABELMOI* ) SCMalloc ( sizeof(LABELMOI) ) ;
		
	la->lac = (LAC* ) lac ;
	la->head = 0 ;
	la->next = 0 ;
	
	if ( 0 == labelmoi->head ) {
		labelmoi->head = la ;
		labelmoi->next = la ;
	} else {
		labelmoi->next->next = la ;
		labelmoi->next = la ;
	}

}

void LACLabelMoiSetLabel ( char* label ) {

	//	author : Jelo Wang
	//	since : 20100505
	//	(C)TOK

	//	为所监控的LAC节点添加label
	
	LABELMOI* walker = labelmoi->head ;

	while ( walker ) {
		
		labelmoi->next = walker->next ;
		LACAddLabel ( walker->lac , label ) ;
		SCFree ( walker ) ;
		walker = labelmoi->next ;

	}

	labelmoi->head = 0 ;

}

void LACLabelMoiClear () {

	//	author : Jelo Wang
	//	since : 20100505
	//	(C)TOK

}

