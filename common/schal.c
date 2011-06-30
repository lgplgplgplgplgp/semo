
/*

+	Semo Compiler HAL 

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

# define MALLOC malloc
# define FREE free

# ifdef MEMORY_MONITOR_ENABLE
MEMORY_MONITOR mem_monitor = { 0 , 0 , 0 , 0 , 0 , 0 } ;
int MMTInit ( MEMORY_MONITOR* mem_monitor ) {

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


void MMTAdd ( MEMORY_MONITOR* mem_monitor , char* file , int line , int length , int address ) {
	
	//	author : Jelo Wang
	//	since : 20100418
	//	(C)TOK

	unsigned int mask_tail = 0xabcd12ef ;
	MEMORY_MONITOR* newmem = 0 ;
	
	if ( !mem_monitor ) return ;

	newmem	= (MEMORY_MONITOR* ) malloc ( sizeof(MEMORY_MONITOR) ) ;

	SCMemcpy ( (void*)((unsigned int)address+length) , &mask_tail , sizeof(unsigned int) ) ;

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

void MMTCheckOverflow ( MEMORY_MONITOR* mem_monitor ) {

	//	author : Jelo
	//	since : 2011.4.10
	//	(C)TOK
	
	//	notes : 识别溢出内存块

	MEMORY_MONITOR* looper = 0 ;
		
	for ( looper = mem_monitor->head ; looper ; looper=looper->next ) 
	{

		unsigned int mask_tail = 0 ;

		SCMemcpy ( &mask_tail , (void*)((unsigned int )looper->address+looper->length) , sizeof (unsigned int ) ) ;

		if ( 0xabcd12ef != mask_tail )
		{
			SCLog ( "Fatal Error : Memory Overflowed in file : '%s' , line : '%d\n'" , looper->file , looper->line ) ;
		}

	}

	
}

void MMTFree ( MEMORY_MONITOR* mem_monitor , int address ) {
	
	//	author : Jelo Wang
	//	since : 20100418
	//	(C)TOK

	MEMORY_MONITOR* walker = mem_monitor->head ;
	MEMORY_MONITOR* pre = mem_monitor ;
	
	if ( !address ) return ;

	for ( ; walker ;pre = walker , walker = walker->next ) {
		if ( walker->address == address ) {
			if ( walker == mem_monitor->head ) {
				mem_monitor->head = walker->next ;
			}
			break ;
		}
	}

	if ( walker ) {
		
		pre->next = walker->next ; 		
		FREE ( walker ) ;
		
	}
	
}


void MMTDestroy ( MEMORY_MONITOR* mem_monitor ) {
	
	//	author : Jelo Wang
	//	since : 20100418
	//	(C)TOK

	MEMORY_MONITOR* walker = 0 ;
	
	if ( !mem_monitor ) return ;

	for ( walker = mem_monitor->head ; walker ; ) {
		mem_monitor->next = walker->next ;
		if ( walker->file ) FREE ( walker->file ) ;
		FREE ( walker ) ;
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


void* SCNormalloc ( unsigned int length , char* file , int line ) {

	//	author : Jelo Wang
	//	notes : malloc
	//	since : 20090809
	
	void* buffer = 0 ;

	# ifdef MEMORY_MONITOR_ENABLE
		buffer = (void*) MALLOC ( length + sizeof(unsigned int) ) ;
		MMTAdd ( &mem_monitor , file , line , length , (int)buffer ) ;
		SCHalMemoryOverflowed () ;
	# else
		buffer = (void*) MALLOC ( length ) ;
	# endif

	if ( buffer ) memset ( buffer , 0 , length ) ;
	
	return buffer ;

}

void* SCRealloc ( void* buffer , long int length ) {

	//	author : Jelo Wang
	//	notes : remalloc
	//	since : 20090809

	void* address = 0 ;
	
	# ifdef MEMORY_MONITOR_ENABLE
		MMTFree ( &mem_monitor , (int)buffer ) ;
		address = realloc ( buffer , length+sizeof(unsigned int) ) ;
		MMTAdd ( &mem_monitor , "SCRealloc" , -1 , length , (int)address ) ;
		SCHalMemoryOverflowed () ;
	# else
		address = realloc ( buffer , length ) ;
	# endif
	
 	return address ;


}


void* SCRemalloc ( void* buffer , long int bufferlen , long int length ) {

	//	author : Jelo Wang
	//	notes : SCRemalloc
	//	since : 20090831
	
	unsigned char* memory = 0 ;

	# ifdef MEMORY_MONITOR_ENABLE
		MMTFree ( &mem_monitor ,(int)buffer ) ;
		memory = (unsigned char* ) MALLOC ( length + sizeof(unsigned int) ) ; 
		MMTAdd ( &mem_monitor , "SCRemalloc" , -1 , length , (int)memory ) ;
		SCHalMemoryOverflowed () ;
	# else
		memory = (unsigned char* ) MALLOC ( length ) ; 
	# endif

	if ( memory ) memcpy ( memory , buffer , bufferlen ) ;
	
	FREE ( buffer ) ;

	return memory ;


}

int SCFree ( void* buffer ) {

	//	author : Jelo Wang
	//	notes : free
	//	since : 20090809

	# ifdef MEMORY_MONITOR_ENABLE
		MMTCheckOverflow ( &mem_monitor ) ;
		MMTFree ( &mem_monitor , (int)buffer ) ;
	# endif

	FREE ( buffer ) ;

	return 1 ;

}


int SCFreeEx ( void** buffer ) {

	//	author : Jelo Wang
	//	notes : free
	//	since : 20090809

	# ifdef MEMORY_MONITOR_ENABLE
		MMTCheckOverflow ( &mem_monitor ) ;
		MMTFree ( &mem_monitor , (int)*buffer ) ;
	# endif

 	FREE ( *buffer ) ;

	*buffer = 0 ;

	return 1 ;

}

void SCHalMemoryOverflowed ()
{

	//	author : Jelo Wang
	//	since : 2011630
	//	(C)TOK

	# ifdef MEMORY_MONITOR_ENABLE
		MMTCheckOverflow ( &mem_monitor ) ;
	# endif

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

		MMTDestroy ( &mem_monitor ) ;

	//	SCLog ("%1.3f kb memory is leaked.\n",(float)(totall/1024)) ;
		SCLog ( "Memory leaked : %d times\n" , leakedtimes ) ;

	# endif

	
	return 0 ;
}

void* SCHalFileOpen ( char* path , char* flag ) {

	//	author : Jelo Wang
	//	notes : fopen
	//	since : 20090809

 	return (void* ) fopen ( path , flag ) ;

}

int SCHalFileSeek ( void* file , int offset , int direct ) {

	//	author : Jelo Wang
	//	notes : fseek
	//	since : 20090809

	return fseek ( (FILE*)file , offset , direct ) ;

}

int SCHalFileRead ( void* file , void* buffer , int size , int counter ) {

	//	author : Jelo Wang
	//	notes : fread
	//	since : 20090809

	return fread ( buffer , size , counter , (FILE*)file ) ;

}

int SCHalFileWrite ( void* file , void* buffer , int size , int counter ) {
	
	//	author : Jelo Wang
	//	notes : fwrite
	//	since : 20090809

	return fwrite ( buffer , size , counter , (FILE*)file ) ;

}

short int SCHalFileGetc ( void* file ) {
	
	//	author : Jelo Wang
	//	notes : fwrite
	//	since : 20090809

	return fgetc ( (FILE*)file ) ;

}

int SCHalFileEnd ( void* file ) {
	
	//	author : Jelo Wang
	//	notes : fwrite
	//	since : 20090809

	return feof ( (FILE*)file ) ;

}


int SCHalFileLength ( void* file ) {

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



int SCHalFileClose ( void* file ) {
	
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

char* SCHalGetFilePath ( char* name ) {

	//	author : Jelo Wang
	//	notes : GetFilePath
	//	since : 20090831


	extern int sc_strlen ( char* ) ;
	extern void sc_back_insert ( char* , char* , int ) ;

	static char path [ 1024 ] ; 

	int slash = 0 ;

	GetModuleFileName ( NULL , path , 1024 ) ;
	
	slash = sc_strlen ( path ) ;

	for ( ; slash >= 0 && '\\' != path [ slash ] ; slash -- ) ;

	sc_back_insert ( path , name , slash + 1 ) ;
		
	return path ;

}
# endif

