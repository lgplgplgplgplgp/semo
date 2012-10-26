
/*

+	LAC (LGNOSIA Code) Generator , part of SC mid-para

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
# include "lac.h"
# include "regoc.h"
# include "corenr.h"

static LAC* lac = 0 ;
static LAC* lacswaper = 0 ;
static LABELMOI* labelmoi = 0 ;

void LACMemoryFrameInit ( int totall ) {

	//	author : Jelo Wang
	//	since : 20110224
	//	(C)TOK

	//	stack_append is the totall memory needs of parameters of a function



}

void LACMemoryFrameAdd ( void* handle , char* frame ) {

	//	author : Jelo Wang
	//	since : 20100505
	//	(C)TOK


}

char* LACMemoryFrameGet ( char* var ) {

	//	author : Jelo Wang
	//	since : 20100505
	//	(C)TOK


	return 0 ;

}

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

void LACSwapIn () {

	//	author : Jelo Wang
	//	since : 20110122
	//	(C)TOK
	
	lacswaper = lac ;
}

void LACSwapBack () {

	//	author : Jelo Wang
	//	since : 20110122
	//	(C)TOK
	
	lac = lacswaper ;
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

	if ( LAC_PROC == lacn->type )	
		lac->proctotall ++ ;
	
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

static void LACRefChainInsert ( int head , int chain ) {

	//	author : Jelo Wang
	//	since : 20110121
	//	(C)TOK

	LAC* lac = (LAC* )head ;

	SCClListInsert ( &lac->refchain , chain ) ;

}

static void LACLiveScopeSplit ( int laca ) {

	//	author : Jelo Wang
	//	since : 20110121
	//	(C)TOK

	//	将lac 引用链上的节点分裂

	LAC* lac = laca ;
	
	SCClList* looper = 0 ;
	
	compiler->lssplits ++ ;

	lac->type = LAC_L_MEM ;

	for ( looper = lac->refchain.head ; looper ; looper = looper->next ) {
		LAC* lacnode = looper->element ;		
		lacnode->type =LAC_R_MEM ;		
	}

		
}

void LACLiveScopeGenerate ( int degreesmax ) {
	
	//	author : Jelo Wang
	//	since : 20110118
	//	(C)TOK

	//	1. locate live scopes of LAC per procedure 
	//	2. build interference graphs with live scopes
	//	3. color the i-graphs
	//	4. alloc register for the live scope based on colored i-graphs

	SCClGraph* iG = 0 ;
	int totall_colors = degreesmax ;	
	int laclsnumber = 0 ;

	int stime = 0 ;
	int etime = 0 ;
	
	LAC* llooper = 0 ;
	LAC* newlac = 0 ;
	LAC* orignallac = lac->head ;

	stime = clock () ;

	RegocRegisterReady ( lac->proctotall ) ;
	//	backup the orignal LAC context
	LACSwapIn () ;
	
restart :

	newlac = LACNew () ;
	LACSetContext ( newlac ) ;
	
	for ( llooper = orignallac ; llooper ; ) {

		LAC* lacnode = 0 ;

		lacnode = LACAdd ( llooper->code.data , llooper->type , llooper->scope ) ;
				
readproc :

		if ( LAC_PROC != llooper->type ) {
			
			llooper = llooper->next ;
			
		} else {

			laclsnumber = 0 ;
						
			RegocLiveScopeMoiCreate () ;

			//	get next looper
			for ( llooper = llooper->next ; llooper ; llooper = llooper->next ) {			

				lacnode = LACAdd ( llooper->code.data , llooper->type , llooper->scope ) ; 
				
				if ( LAC_L_DELT == lacnode->type ) {		

					//	添加生命域
					//	将原始的llooper加入LiveScopeMonitor，因为生命域分列时需要操作它
					int lsn = RegocLiveScopeAdd ( llooper->code.data , llooper->scope , llooper->line , (void* )llooper ) ;
					//	生命域编号
					char* value = sc_strcat ( "." , SCClItoa (lsn) ) ;		
					SCClStringAddStr ( &lacnode->code , value ) ;
					SCClStringAdd ( &lacnode->code , 0 ) ;
					SCFree ( value ) ;	

					laclsnumber ++ ;
					
				} else if ( LAC_R_DELT == lacnode->type ) {

					//	生命域引用
					//	获取其编号
					int lsn = RegocCheckLiveScope ( lacnode->code.data , lacnode->scope , lacnode->line ) ;
					char* value = 0 ; 
					
					if ( -1 != lsn ) {
						value = sc_strcat ( "." , SCClItoa (lsn) ) ;					
						SCClStringAddStr ( &lacnode->code , value ) ;
						SCClStringAdd ( &lacnode->code , '\0' ) ;
						SCFree ( value ) ;		
					} else if ( -1 == lsn ) {
						//	set register 0 as default if the ls hasnt defined
						SCClStringAddStr ( &lacnode->code , ".0" ) ;
						SCClStringAdd ( &lacnode->code , '\0' ) ;
					}
					
					if ( -1 < lsn ) {
						//	将lac 添加到其引用链
						int lac = 0 ;					
						lac = RegocLiveScopeGetLAC () ;
						LACRefChainInsert ( lac , lacnode ) ;
					}
					

				} else if ( LAC_PROC == llooper->type || 0 == llooper->next ) {

					if ( 0 < laclsnumber ) {
						//	get another lac proc
						//	coloring the graph of the last proc
						int lac = 0 ;
						
						iG = RegocIGraphCreate () ;
						lac = SCClGraphColoring ( iG , degreesmax ) ;	
						
						if ( -1 != lac && lac ) {						
							//	graph coloring is failed if lac itsnt equal -1
							//	split the problem
							LACLiveScopeSplit ( lac ) ;
							RegocLiveScopeMoiDestroy () ;
							SCClGraphDestroy ( iG ) ;							
							//LACClear () ;
							
							goto restart ;
							
						}
						
						if ( SC_IG & compiler->parameter ) {
							CORENRIGBFSRender ( (SCClGraph* ) iG , 0 , 0 ) ;
						}

						//	start register allocation based on interference-graph					
						RegocRegisterAlloc ( iG , laclsnumber ) ;
						RegocLiveScopeMoiDestroy () ;	

					}
					
					if ( 0 == llooper->next ) {
						goto success ;
					}

					//	end of LAC flow
					goto readproc ;
					
				}
				
				
			}

			
		}

		
	}

success :

	//	destroy the orginal LAC flow
	LACClearEx ( orignallac ) ;

	etime = clock () ;
	
	compiler->regoccosts = etime - stime ;
	
	return ;
	
			
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

void LACClearEx ( LAC* walker ) {

	//	author : Jelo Wang
	//	since : 20110122
	//	(C)TOK

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

