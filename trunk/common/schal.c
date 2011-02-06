
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

# include <malloc.h>
# include <memory.h>
# include <stdarg.h> 
# include "schal.h"

# define SCMalloc malloc
# define SC_FREE free

# ifdef MEMORY_MONITOR_ENABLE
MEMORY_MONITOR mem_monitor = { 0 , 0 , 0 , 0 , 0 , 0 } ;
int MemoryMonitorInit ( MEMORY_MONITOR* mem_monitor ) {

	//	author : Jelo Wang
	//	since : 20100418
	//	(C)TOK

	if ( !mem_monitor ) return 0 ; 
	
	mem_monitor->file = 0 ;
	mem_monitor->line = 0 ;
	mem_monitor->length = 0 ;
	mem_monitor->address = 0 ;	
	mem_monitor->head = 0 ;		
	mem_monitor->next = 0 ;

	return (int)mem_monitor ;
		
}


void MemoryMonitorAdd ( MEMORY_MONITOR* mem_monitor , char* file , int line , int length , int address ) {
	
	//	author : Jelo Wang
	//	since : 20100418
	//	(C)TOK

	MEMORY_MONITOR* newmem = 0 ;
	
	if ( !mem_monitor ) return ;

	newmem	= (MEMORY_MONITOR* ) malloc ( sizeof(MEMORY_MONITOR) ) ;

	if ( file ) {
		newmem->file = (char* ) malloc ( sc_strlen ( file ) + 1 ) ;
		sc_strcpy ( newmem->file , file ) ;
	}

	newmem->line = line ; 
	newmem->length = length ;
	newmem->address = address ;
	newmem->head = 0 ;
	newmem->next = 0 ;

	if ( 0 == mem_monitor->head ) {
		mem_monitor->head = newmem ;
		mem_monitor->next = newmem ;
	} else {
		mem_monitor->next->next = newmem ;
		mem_monitor->next = newmem ;
	}

}


void MemoryMonitorFree ( MEMORY_MONITOR* mem_monitor , int address ) {
	
	//	author : Jelo Wang
	//	since : 20100418
	//	(C)TOK

	MEMORY_MONITOR* walker = mem_monitor->head ;
	MEMORY_MONITOR* pre = mem_monitor ;
	
	if ( !address ) return ;

	for ( ;walker && walker->address != address ;pre = walker , walker=walker->next) ;

	if ( walker ) {
		
		pre->next = walker->next ; 		
		if ( 0 == pre->next ) mem_monitor->next = pre ;
		free ( walker ) ;
		
	}
	
}


void MemoryMonitorDestroy ( MEMORY_MONITOR* mem_monitor ) {
	
	//	author : Jelo Wang
	//	since : 20100418
	//	(C)TOK

	MEMORY_MONITOR* walker = 0 ;
	
	if ( !mem_monitor ) return ;

	for ( walker = mem_monitor->head ; walker ; ) {
		mem_monitor->next = walker->next ;
		if ( walker->file ) free ( walker->file ) ;
		free ( walker ) ;
		walker = mem_monitor->next ;
	}
	
}

# endif

void SCLog ( const   char* SCLog , ... ) {

	//	author : Jelo Wang
	//	notes : printer
	//	since : 20090809
	
	va_list ap ;   
	va_start ( ap , SCLog ) ;   
	vprintf ( SCLog , ap ) ;
	va_end ( ap ) ;

} 


void* SCNormalloc ( long int length , char* file , int line ) {

	//	author : Jelo Wang
	//	notes : malloc
	//	since : 20090809
	
	void* buffer = 0 ;
	
	buffer = (void*) SCMalloc ( length ) ;

	if ( buffer ) memset ( buffer , 0 , length ) ;

	# ifdef MEMORY_MONITOR_ENABLE
		MemoryMonitorAdd ( &mem_monitor , file , line , length , (int)buffer ) ;
	# endif
	
	return buffer ;

}

void* SCRealloc ( void* buffer , long int length ) {

	//	author : Jelo Wang
	//	notes : remalloc
	//	since : 20090809

	void* address = 0 ;
	
	# ifdef MEMORY_MONITOR_ENABLE
		MemoryMonitorFree ( &mem_monitor , "SCRealloc" , -1 , length , (int)buffer ) ;
	# endif

	address = realloc ( buffer , length ) ;

	# ifdef MEMORY_MONITOR_ENABLE
		MemoryMonitorAdd ( &mem_monitor , "SCRealloc" , -1 , length , (int)address ) ;
	# endif
	
 	return address ;


}


void* SCRemalloc ( void* buffer , long int bufferlen , long int length ) {

	//	author : Jelo Wang
	//	notes : SCRemalloc
	//	since : 20090831
	
	unsigned char* memory = (unsigned char* ) SCMalloc ( length  ) ;

	if ( memory ) memcpy ( memory , buffer , bufferlen ) ;
	
	SCFree ( buffer ) ;

	return memory ;


}

int SCFree ( void* buffer ) {

	//	author : Jelo Wang
	//	notes : free
	//	since : 20090809

	# ifdef MEMORY_MONITOR_ENABLE
		MemoryMonitorFree ( &mem_monitor , (int)buffer ) ;
	# endif

 	SC_FREE ( buffer ) ;

	return 1 ;

}


int SCFreeEx ( void** buffer ) {

	//	author : Jelo Wang
	//	notes : free
	//	since : 20090809

	# ifdef MEMORY_MONITOR_ENABLE
		MemoryMonitorFree ( &mem_monitor , (int)*buffer ) ;
	# endif

 	SC_FREE ( *buffer ) ;

	*buffer = 0 ;

	return 1 ;

}

int SCHalMemoryLeaked () {

	//	author : Jelo Wang
	//	since : 20091129

	# ifdef MEMORY_MONITOR_ENABLE
		
		int totall = 0 ;
		int leakedtimes = 0 ;
		
		MEMORY_MONITOR* walker = mem_monitor.head ;
	
		for ( ;walker;walker=walker->next) {

			SCLog ("address:%x,length:%d,file:'%s',line:%d - leaked\n",
				walker->address,\
				walker->length,\
				walker->file,\
				walker->line\
			) ;\
			
			totall = totall + walker->length ;

			leakedtimes ++ ;
			
		}

		MemoryMonitorDestroy ( &mem_monitor ) ;

	//	SCLog ("%1.3f kb memory is leaked.\n",(float)(totall/1024)) ;
		SCLog ( "Memory leaked : %d times\n" , leakedtimes ) ;

	# endif

	
	return 0 ;
}

int SCHalFileOpen ( char* path , char* flag ) {

	//	author : Jelo Wang
	//	notes : fopen
	//	since : 20090809

 	return fopen ( path , flag ) ;

}

int SCHalFileSeek ( int file , int offset , int direct ) {

	//	author : Jelo Wang
	//	notes : fseek
	//	since : 20090809

	return fseek ( (FILE*)file , offset , direct ) ;

}

int SCHalFileRead ( int file , void* buffer , int size , int counter ) {

	//	author : Jelo Wang
	//	notes : fread
	//	since : 20090809

	return fread ( buffer , size , counter , (FILE*)file ) ;

}

int SCHalFileWrite ( int file , void* buffer , int size , int counter ) {
	
	//	author : Jelo Wang
	//	notes : fwrite
	//	since : 20090809

	return fwrite ( buffer , size , counter , (FILE*)file ) ;

}

short int SCHalFileGetc ( int file ) {
	
	//	author : Jelo Wang
	//	notes : fwrite
	//	since : 20090809

	return fgetc ( (FILE*)file ) ;

}

int SCHalFileEnd ( int file ) {
	
	//	author : Jelo Wang
	//	notes : fwrite
	//	since : 20090809

	return feof ( (FILE*)file ) ;

}


int SCHalFileLength ( int file ) {

	//	author : Jelo Wang
	//	notes : fseek
	//	since : 20090809
	
	int length = 0;

	SCHalFileSeek ( file , 0 , SEEK_SET ) ;

	while( !SCHalFileEnd(file) ) {

		SCHalFileGetc ( file ) ;
		length ++ ;

	}

	SCHalFileSeek ( file , 0 , SEEK_SET ) ;
	
	return length ;

}



int SCHalFileClose ( int file ) {
	
	//	author : Jelo Wang
	//	notes : fclose
	//	since : 20090809
	
	return fclose ( (FILE*)file ) ;

}

void SCMemcpy ( void* target_memory , void* source_memory , int length ) {
	
	//	author : Jelo Wang
	//	since : 20100107

	memcpy ( target_memory , source_memory , length ) ; 

}

# ifdef WIN32

# include "windows.h"

void SCTerminate () {

	//	author : Jelo Wang
	//	since : 20091125

	exit ( 1 ) ;
	
	
}

char* GetFilePath ( char* name ) {

	//	author : Jelo Wang
	//	notes : GetFilePath
	//	since : 20090831
		
	static char path [ 1024 ] ; 

	int slash = 0 ;

	GetModuleFileName ( NULL , path , 1024 ) ;
	
	slash = sc_strlen ( path ) ;

	for ( ; slash >= 0 && '\\' != path [ slash ] ; slash -- ) ;

	sc_back_insert ( path , name , slash + 1 ) ;
		
	return path ;

}
# endif

