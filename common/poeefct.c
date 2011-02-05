
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

#include <stdio.h>
#include <time.h>
#include "poeefct.h"

POEE_FCT poee_fct [ MAX_FCT_NODE_SUM ] ;
STRTAB** poee_strtab ;
int poee_stack ;
int poee_fct_counter ;
int poee_fct_border_time ; 
int poee_fct_index ;

void POEEFCT_INIT ( int time ) {

	//	author : Jelo Wang
	//	(C)TOK
		
	poee_fct_counter = 0 ;
	poee_stack = 0 ;
	poee_fct_border_time = time ;
}

void POEEFCT_START ( unsigned char* module ) {

	//	author : Jelo Wang
	//	(C)TOK
		
	poee_stack ++ ;
	if ( poee_fct_counter < MAX_FCT_NODE_SUM ) {
		poee_fct [poee_fct_counter ] . id = ecoder_mask_h_unsetbit & (int)module;
		poee_fct [poee_fct_counter ] . time = clock () ;
		poee_fct_index = poee_fct_counter  ;
		poee_fct_counter ++ ;
	}
}

void POEEFCT_END ( unsigned char* module ) {

	//	author : Jelo Wang
	//	(C)TOK
		
	poee_stack -- ;
	if ( poee_fct_counter >= 0 ) {
		poee_fct [poee_fct_counter ] . id = ecoder_mask_h_setbit | (int)module;
		poee_fct [poee_fct_counter ] . time = clock () ;
		if ( poee_fct_border_time <= (poee_fct [poee_fct_counter ] . time - poee_fct [poee_fct_index] . time ) ) {
			poee_fct_counter ++ ;
			poee_fct_index -- ;
		} else {
			poee_fct_counter = poee_fct_index ;
		}
	}
}


void POEEFCT_OUT( char* filename ) {
		
	//	author : Jelo Wang
	//	notes : gen fct file
	//	(C)TOK
			
	int file = fopen( filename , "wb" ) ;
		
	int file_length = 0 ;
	int strtab_offset = 0 ;
	int fct_offset = 0 ;
	int strtab_sum = 0 ;
	int walker = 0 ;

	if ( poee_stack ) {
		printf("POEE_FCT_PROFILING_OUT :: fatal error stack is unbalanced\n");
	}

	if ( !poee_fct_counter ) 
		return ; 
		
	fputc ( 'F' , file ) ; 
	fputc ( 'C' , file ) ; 
	fputc ( 'T' , file ) ; 
	fputc ( 0x20 , file ) ; 

	file_length = file_length + 4 ;
		
	fwrite ( &fct_offset , sizeof(int) , 1 , file ) ;
	fwrite ( &poee_fct_counter , sizeof(int) , 1 , file ) ;
	fwrite ( &strtab_offset , sizeof(int) , 1 , file ) ;
	fwrite ( &strtab_sum , sizeof(int) , 1 , file ) ;

	file_length = file_length + 16 ;
		
	fct_offset = file_length ;

	poee_strtab = (STRTAB** ) malloc ( sizeof(STRTAB**) * poee_fct_counter ) ;
	
	for ( walker = 0 ; walker < poee_fct_counter ; walker ++ ) {

		fwrite ( &poee_fct[walker] . id , sizeof(long) , 1 , file ) ;
		file_length = file_length + sizeof(long) ;
			
		fwrite ( &poee_fct[walker] . time , sizeof(long) , 1 , file ) ;
		file_length = file_length + sizeof(long) ;

		if ( 0 == ( 0x80000000 & poee_fct[walker] . id ) ) {
			poee_strtab [strtab_sum] = (STRTAB* ) malloc ( sizeof(STRTAB) ) ;
			poee_strtab [strtab_sum] -> id = poee_fct[walker] . id ;
			poee_strtab [strtab_sum] -> data_len = strlen ( (char*)poee_fct[walker] . id ) ;
			poee_strtab [strtab_sum] -> data = (char* ) malloc ( poee_strtab [strtab_sum] -> data_len ) ;				strcpy ( poee_strtab [strtab_sum] -> data , (char* ) poee_fct[walker] . id ) ;
			strtab_sum ++ ;
		}


	}

	strtab_offset = file_length ;

	for ( walker = 0 ; walker < strtab_sum ; walker ++ ) {
			
		char* str = 0 ; 
			
		fwrite ( &poee_strtab [walker] -> id , sizeof(long) , 1 , file ) ;
		file_length = file_length + sizeof(long) ;
			
		fwrite ( &poee_strtab[walker] -> data_len , sizeof(long) , 1 , file ) ;
		file_length = file_length + sizeof(int) ;

		str = poee_strtab [walker] -> data ;
			
		fwrite ( str , 1 ,  poee_strtab[walker] -> data_len , file ) ;
		file_length = file_length + poee_strtab[walker] -> data_len ;
				
	}

	fseek ( file , 4 , SEEK_SET ) ;
	fwrite ( &fct_offset , sizeof(int) , 1 , file ) ;
	fwrite ( &poee_fct_counter , sizeof(int) , 1 , file ) ;
	fwrite ( &strtab_offset , sizeof(int) , 1 , file ) ;
	fwrite ( &strtab_sum , sizeof(int) , 1 , file ) ;	
		
	fclose ( file ) ;

	poee_strtab = 0 ;
	poee_stack = 0 ;
	poee_fct_counter = 0 ;
	poee_fct_border_time = 0 ; 
	poee_fct_index = 0 ;
		

}










