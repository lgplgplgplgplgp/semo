
/*

+	HAL of SC

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

# ifndef __SCHAL
# define __SCHAL

# define SEEK_HEAD SEEK_SET

//# define MEMORY_MONITOR_ENABLE

extern void* SCNormalloc ( long int length ) ;

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
extern int sc_hal_file_puts ( const char* string , int file ) ;
extern int sc_hal_file_putc ( int string , int file ) ;
extern void SCMemcpy ( void* target_memory , void* source_memory , int length ) ;
extern void SCTerminate () ;

