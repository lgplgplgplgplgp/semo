
/*

+	Semo Compiler Common Library 

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

# include <stdio.h>

# ifndef __SCHAL
# define __SCHAL

# define SEEK_HEAD SEEK_SET

//# define MEMORY_MONITOR_ENABLE

extern void* SCNormalloc ( unsigned int length , char* , int ) ;

# define SCMalloc(length) SCNormalloc(length,__FILE__,__LINE__)

typedef struct MMN {

	//	author : Jelo Wang
	//	since : 20100417
	//	(C)TOK
	
	char* file ;
	int line ;
	int length ;
	int address ;
	
	struct MMN* head ;
	struct MMN* next ;
	
} MEMORY_MONITOR ;

# endif


extern void* SCRealloc ( void* buffer , long int length ) ;
extern int SCFree ( void* buffer ) ;
extern int SCFreeEx ( void** buffer ) ;
extern int SCHalMemoryLeaked () ;
extern int SCHalFileOpen ( char* path , char* flag ) ;
extern int SCHalFileSeek( int file , int offset , int direct ) ;
extern int SCHalFileRead ( int file , void* buffer , int size , int counter ) ;
extern int SCHalFileWrite ( int file , void* buffer , int size , int counter ) ;
extern short int SCHalFileGetc ( int file ) ;
extern int SCHalFileEnd ( int file ) ;
extern int SCHalFileLength( int file ) ;
extern int SCHalFileClose ( int file ) ;
extern char* SCHalGetFilePath ( char* name ) ;
extern void SCMemcpy ( void* target_memory , void* source_memory , int length ) ;
extern void SCLog ( const	 char* SCLog , ... ) ;
extern void SCTerminate () ;

