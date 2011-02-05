
/*
+	Runtime Callings Profilling
+	(C)TOK Jelo Wang

+	You can redistribute it and/or modify it under the terms of the gnu general public license	
+	as published by the free software foundation, either version 3 of the license or any later 	
+	version.this program is distributed in the hope that it will be useful,but without any 		
+	warranty.without even the implied warranty of merchantability or fitness for a particular 	
+	purpose.																				
																												
+	(c)	Techniques of Knowledge
+		an open source group since 2008		
+		page : http://www.tok.cc
+		email : jelo.wang@gmail.com

*/

# ifndef __POEEFCT_
# define __POEEFCT_

#define MAX_FCT_NODE_SUM 120
#define ecoder_mask_h_unsetbit 0x0fffffff
#define ecoder_mask_h_setbit 0x80000000

typedef struct {

	//	author : Jelo Wang
	//	(C)TOK
	
	char key[4] ;
	int fct_offset ; 
	int fct_sum ;
	int strtab_offset ;
	int strtab_sum ;
	
} FCT_HEADER ;

typedef struct {

	//	author : Jelo Wang
	//	(C)TOK
	
	long id ;
	long time ;
	
} POEE_FCT ;

typedef struct {

	//	author : Jelo Wang
	//	(C)TOK
	
	long id ;
	long data_len ;
	unsigned char* data ;
	
} STRTAB ;

# endif

extern POEE_FCT poee_fct [ MAX_FCT_NODE_SUM ] ;
extern STRTAB** poee_strtab ;
extern int poee_stack ;
extern int poee_fct_counter ;
extern int poee_fct_border_time ; 
extern int poee_fct_index ;
