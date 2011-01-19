
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

# ifndef __REGOC
# define __REGOC

# define LIVE_NAME_LENGTH 32

typedef struct {

	//	author : Jelo Wang
	//	since : 20110107
	//	(C)TOK

	//	register name 
	char live [ LIVE_NAME_LENGTH ] ;


	//	number of live scope
	int number ; 
	
	int scope ;
	
	//	start position of a register's live scope
	int start_line ;
	//	end position of a register's live scope	
	int end_line ;
	
} LIVESCOPE ;

# endif

extern int RegocRegPoolCreate ( char** regs , int totall ) ;
extern void RegocBuildRefGraph ( char* code , int position ) ;
extern char* RegocAlloc ( int lr ) ;
extern void RegocLiveScopeMoiCreate () ;
extern int RegocLiveScopeAdd ( char* live , int scope , int line ) ; 
extern int RegocCheckLiveScope ( char* live , int scope , int line ) ;
extern int RegocIGraphCreate () ;
extern void RegocLiveScopeClear () ;
extern int RegocGetRegister ( int pn , int lsn ) ;
extern void RegocRegisterAlloc ( int graphp ) ;