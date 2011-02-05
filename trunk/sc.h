
/*

+	Semo Compiler 0.3.0

+	'Semo Compiler' is a multi-objective compiler which developing under the terms of the 
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

# ifndef __SMART_COMPILER
# define __SMART_COMPILER

//# define SEMO_DEBUG 

enum {

	//	author : Jelo Wang
	//	since : 20100817
	//	(c)TOK

	//	notes : parameters of the framework

	//	compile for c language based on C99 
	SC_C99 = 0x1 ,
	//	compile for c plus plus language
	SC_CPP = 0x2 ,
	//	compile for java language	
	SC_JAVA = 0x4 ,
	//	export pre-compiling results to external file
	SC_PO = 0x8 ,
	//	export IR to external file
	SC_LAC = 0xa ,
	//	export semo assembly code to external file
	SC_SASM = 0x20 ,
	//	enable compiling-render and export results
	SC_CR = 0x40 ,
	//	generate object-file with ELF format by default
	SC_ELF = 0x80 ,
	//	compile and link
	SC_LINK = 0x100 ,
	//	compile only by default
	SC_NLINK = 0x200 ,
	//	lga lgnosia graph	
	SC_LGA = 0x400 ,
	//	exp expression graph
	SC_EXP = 0x800 ,
	//	delete comments	
	SC_DC = 0x1000 ,
	//	marco substitution
	SC_MS = 0x2000 ,	
	//	compile for the ARM architecture by default
	SC_ARM = 0x4000 ,
	//	compile for the x80386 architecture
	SC_X80386 = 0x8000 ,
	//	both dc and ms features by default , = 2^17
	SC_DM = 0x10000 ,				
	//	interference graph of live scope , = 2^18
	SC_IG = 0x20000	
	
} ;

enum {

	//	author : Jelo Wang
	//	since : 20091127
	//	(c)TOK

	//	An binary IMAGE that can runing straightly
	SC_DYNAMIC_RUNING ,
	//	An object file or something like that fashion
	SC_EXECUTABLE_UNIT ,
		
} ;

typedef struct COMPILER_NODE {

	//	author : Jelo Wang
	//	notes : initialize compiler parameters
	//	since : 20090809
	//	(c)TOK

	//	input files list , a handle to SCCllist*
	int il  ;
	//	object files list , a handle to SCCllist*	
	int ol ;
	int parameter ;	
	//	register allocation costs
	int regoccosts ;
	//	lives spliting times
	int lssplits ;	
	int lines ;
	int codes ;

	int (*PRESOR)( unsigned char* ) ;
	int (*PARSER)( int* ) ;
	char* (*GENTOR)( char* ) ;
	char* (*ASMOR)( char* , char* ) ;
	void (*ASSEMER)( char* , char* , int* ) ;
	void (*LINKER ) ( int ) ;
	void (*RELEASE)() ;

	long stime ; 
	long etime ; 
	
} COMPILER ;

extern COMPILER* compiler ;

# endif

extern int SCCompile ( int argc , char** argv , int type ) ;

