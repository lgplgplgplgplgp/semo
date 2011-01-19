
/*

+	SEMO C()MPILER 0.2.0

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

# include "c-lexer.h"
# include "c-presor.h"
# include "c-parser.h"
# include "arm-asmor.h"
# include "arm-assemer.h"
# include "lac-gentor.h"
# include "schal.h"
# include "sccl.h"
# include "sc.h"

COMPILER* compiler = 0 ;

static void help () {
	
	//	author : Jelo Wang
	//	since : 20091125
	//	(c)TOK	
 
	SCLog ( 	"\
\t - Semo C()mpiler Frameworks 0.2.0 -\n\
\t - Techniques of Knowledge since 2008 -\n\n\
Format : \n\
\t sc option1-option2...-optionn <SET module=para> file1 file2...filen \n\
Options :\n\
\t -c \t\t compile for c language based on C99\n\
\t -cpp \t\t compile for c plus plus language\n\
\t -java \t\t compile for java language\n\
\t -po \t\t export pre-compiling results to external file\n\
\t -lac \t\t export IR to external file\n\
\t -sasm \t\t export smart assembly code to external file\n\
\t -cr \t\t enable compiling-render and export results\n\
\t -elf \t\t generate object-file with ELF format by default\n\
\t -link \t\t compile and link\n\
\t -nlink \t compile only by default\n\
\t -arm \t\t compile for the ARM architecture by default\n\
\t -x80386 \t compile for the x80386 architecture\n\
\n\
SET :\n\
\t module : po pre-compiling\n\
\t parameter : dc delete comments\n\
\t parameter : ms marco substitution\n\
\t parameter : dm both dc and ms features by default\n\
\n\
\t module : cr compiling-render\n\
\t parameter : lga lgnosia graph\n\
\t parameter : exp expression graph\n\
\n\
\t module : linker\n\
\t parameter : file1 file2...filen\n\
\n\
Example :\n\
\t sc test.c -c -x80386\n\
\t sc -c -lac -sasm test.c test2.c test3.c\n\
\t sc -c -lac -sasm -cr -arm SET po=dc,ms SET cr=lga test.c test2.c\n\
\t sc test.c SET linker=test2.lib test3.lib\n\
\n\
More :\n\
\t For further more info you can visit the offical site\n\
\t http://www.tok.cc \n\
" ) ;
	
}

static int sc_command_parser ( COMPILER* compiler , int argc , char** argv ) {

	//	author : Jelo Wang
	//	since : 20091125
	//	(c)TOK

	int walker = 0 ;
	int state = 0 ;

	SCClString str = {0} ;
	
	for ( walker = 0 ; walker < argc-1 ; walker ++ ) {
		
		SCClStringAddStr ( &str , argv[walker + 1] ) ;
		SCClStringAdd ( &str , 0x20 ) ;	

	}	

	SCClStringAdd ( &str , '\0' ) ;

	//	Use lexer of c for the commands analysis
	lexerc_set ( lexerc_new ( str.data , LEXERC_DEFAULT_MODE ) ) ; 

	SCClStringReset ( &str ) ;

	while ( !lexc->stop ) {

		lexerc_genv () ;

		switch ( state ) {
			
			case 0 :
				if ( !sc_strcmp ( lexc->token , "-" ) ) state = 1 ;
				else if ( lexc->token && !sc_strcmp ( lexc->token , "SET" ) ) {				
					if ( '.' != lexerc_look (0) ) {						
						state = 3 ;
						lexerc_skip_blank () ;
					} else {			
						//	save file name wit str			
						SCClStringAddStr ( &str , "SET" ) ;
						SCClStringAddStr ( &str , "." ) ;
						lexerc_next () ;
						state = 2 ;
					}
				} else {
		
					SCClStringAddStr ( &str , lexc->token ) ;

					if ( '.' == lexerc_look (0) ) {
						SCClStringAddStr ( &str , "." ) ;
						lexerc_next () ;
					}
					
					state = 2 ;

				}
			break ;
			
			case 1 :
				
				if ( !sc_strcmp ( lexc->token , "c" ) ) {
					compiler->parameter |= SC_C99 ;
					if ( compiler->parameter & SC_CPP ) {
						SCLog ( "Language '-cpp' has already specified\n" ) ;
						goto ErrorRelease ;						
					}	
					if ( compiler->parameter & SC_JAVA ) {
						SCLog ( "Language '-java' has already specified\n" ) ;
						goto ErrorRelease ;						
					}							
				} else if ( !sc_strcmp ( lexc->token , "cpp" ) ) {
					compiler->parameter |= SC_CPP ;				
					SCLog ( "The compiler is not supports CPP yet \n" ) ;						
					if ( compiler->parameter & SC_C99 ) {
						SCLog ( "Language '-c' has already specified\n" ) ;
						goto ErrorRelease ;						
					}	
					if ( compiler->parameter & SC_JAVA ) {
						SCLog ( "Language '-java' has already specified\n" ) ;
						goto ErrorRelease ;						
					}							
					goto ErrorRelease ;
				} else if ( !sc_strcmp ( lexc->token , "java" ) ) {
					compiler->parameter |= SC_JAVA ;			
					SCLog ( "The compiler is not supports JAVA yet \n" ) ;
					if ( compiler->parameter & SC_C99 ) {
						SCLog ( "Language '-c' has already specified\n" ) ;
						goto ErrorRelease ;						
					}	
					if ( compiler->parameter & SC_CPP ) {
						SCLog ( "Language '-cpp' has already specified\n" ) ;
						goto ErrorRelease ;						
					}						
					goto ErrorRelease ;
				} else if ( !sc_strcmp ( lexc->token , "po" ) ) {
					compiler->parameter |= SC_PO ;						

				} else if ( !sc_strcmp ( lexc->token , "lac" ) ) {
					compiler->parameter |= SC_LAC ;						

				} else if ( !sc_strcmp ( lexc->token , "sasm" ) ) {
					compiler->parameter |= SC_SASM ;						

				} else if ( !sc_strcmp ( lexc->token , "cr" ) ) {
					compiler->parameter |= SC_CR ;						

				} else if ( !sc_strcmp ( lexc->token , "elf" ) ) {
					compiler->parameter |= SC_ELF ;						

				} else if ( !sc_strcmp ( lexc->token , "link" ) ) {
					compiler->parameter |= SC_LINK ;			
					if ( compiler->parameter & SC_NLINK ) {
						SCLog ( "Linker is multi-defined with '-nlink'\n" ) ;
						goto ErrorRelease ;						
					}	
				} else if ( !sc_strcmp ( lexc->token , "nlink" ) ) {
					compiler->parameter |= SC_NLINK ;	
					if ( compiler->parameter & SC_LINK ) {
						SCLog ( "Linker is multi-defined with '-link'\n" ) ;
						goto ErrorRelease ;						
					}						

				} else if ( !sc_strcmp ( lexc->token , "arm" ) ) {
					compiler->parameter |= SC_ARM ;		
					if ( compiler->parameter & SC_X80386 ) {
						SCLog ( "Architecture '-x80386' has already specified\n" ) ;
						goto ErrorRelease ;						
					}							

				} else if ( !sc_strcmp ( lexc->token , "x80386" ) ) {
					compiler->parameter |= SC_X80386 ;		
					if ( compiler->parameter & SC_ARM ) {
						SCLog ( "Architecture '-arm' has already specified\n" ) ;
						SCLog ( "The compiler is not supports x80386 yet\n" ) ;
						goto ErrorRelease ;						
					}							
				}  else {
					SCLog ( "Undefined parameter\n" ) ;
					goto ErrorRelease ;						
				}

				lexerc_skip_blank () ;
				state = 0 ;
				
			break ;

			case 2 :

				if ( lexc->token && sc_is_alpha (lexc->token[0]) ) 
					SCClStringAddStr ( &str , lexc->token ) ;

				SCClStringAdd ( &str , '\0' ) ;
				SCClStringReset ( &str ) ;

				SCClListInsert ( compiler->il , (int)sc_strnew ( str.data ) ) ;

				lexerc_skip_blank () ;
				
				state = 0 ;
			
			break ;
			
			case 3 :				

				if ( !sc_strcmp ( lexc->token , "po" ) ) {
					lexerc_skip_blank () ;
					lexerc_genv () ;
					if ( sc_strcmp ( lexc->token , "=" ) ) {
						SCLog ( "Incorrect 'SET' form\n" ) ;
						goto ErrorRelease ;						
					}
					lexerc_skip_blank () ;
					lexerc_genv () ;
					if ( !sc_strcmp ( lexc->token , "dc" ) ) {
						compiler->parameter |= SC_DC ;		
					} else if ( !sc_strcmp ( lexc->token , "ms" ) ) {
						compiler->parameter |= SC_MS ;		
					} else if ( !sc_strcmp ( lexc->token , "dm" ) ) {
						compiler->parameter |= SC_DM ;		
					}  else {
						SCLog ( "Incorrect 'SET' form\n" ) ;
						goto ErrorRelease ;						
					}
				} else if ( !sc_strcmp ( lexc->token , "cr" ) ) {
					lexerc_skip_blank () ;
 					lexerc_genv () ;
					if ( sc_strcmp ( lexc->token , "=" ) ) {
						SCLog ( "Incorrect 'SET' form\n" ) ;
						goto ErrorRelease ;						
					}
					lexerc_skip_blank () ;
					lexerc_genv () ;
					if ( !sc_strcmp ( lexc->token , "lga" ) ) {
						compiler->parameter |= SC_LGA ;		
					} else if ( !sc_strcmp ( lexc->token , "exp" ) ) {
						compiler->parameter |= SC_EXP ;		
					} else {
						SCLog ( "Incorrect 'SET' form\n" ) ;
						goto ErrorRelease ;						
					}				
				} else {
					SCLog ( "Incorrect 'SET' form\n" ) ;
					goto ErrorRelease ;						
				}

				state = 0 ;
				
			break ;
		}
		
	}

	//	parameters of default 
	compiler->parameter |= SC_C99 ;
	compiler->parameter |= SC_ARM ;
	compiler->parameter |= SC_NLINK ;

	if ( SC_CR & compiler->parameter )
		if ( !(SC_EXP & compiler->parameter ) && !(SC_LGA & compiler->parameter )  )
			compiler->parameter |= SC_EXP ;
	
	lexerc_destroy () ;

	return 1 ;
	
ErrorRelease :	

	lexerc_destroy () ;
	
	return 0 ;

}


COMPILER* SCCompilerCreate () {

	//	author : WANG QUANWE
	//	since : 20100430
	//	(c)TOK	
	
	COMPILER* compiler = (COMPILER* ) SCMalloc ( sizeof(COMPILER) ) ;

	if ( !compiler ) return 0 ;

	compiler->il = (SCClList* ) SCMalloc ( sizeof(SCClList) ) ;

	if ( !compiler->il ) {
		SCFree ( compiler ) ;
		return 0 ;
	}

	compiler->ol = (SCClList* ) SCMalloc ( sizeof(SCClList) ) ;

	if ( !compiler->ol ) {

		SCFree ( compiler->il ) ;
		SCFree ( compiler ) ;
		return 0 ;
		
	}

	compiler->start_time = 0 ;
	compiler->end_time = 0 ;
	compiler->lines = 0 ;
	compiler->codes = 0 ;
	
	compiler->PRESOR = 0 ;
	compiler->PARSER = 0 ;
	compiler->GENTOR = 0 ;
	compiler->ASMOR = 0 ;
	compiler->LINKER = 0 ;
	compiler->ASSEMER = 0 ;

	return compiler ;
	

}

static void SCCompilerDestroy () {

	//	author : WANG QUANWE
	//	since : 20091127
	//	(c)TOK	

	int walker = 0 ;
	
	if ( !compiler ) return ;

	SCClListDestroy ( compiler->il ) ;
	
	if ( compiler->il ) 
		SCFree ( compiler->il ) ;

	SCFreeEx ( &compiler ) ;

	# ifdef MEMORY_MONITOR_ENABLE
		SCHalMemoryLeaked() ;
	# endif
		  
}

static int SCCompilerReady ( int argc , char** argv  ) {

	//	author : Jelo Wang
	//	notes : initialize compiler parameters
	//	since : 20090809
	//	(c)TOK
	
	int walker = 0 ;
	int file_length = 0;   

	int inputfile = 0 ;

	unsigned char* buffer = 0;

	if ( !argc || argc < 2 ) {
		help () ;
 		SCTerminate ( 1 ) ;
		return 0 ;
		
	}
	
	compiler = SCCompilerCreate () ;

	if ( !compiler ) return 0 ;
	
	if ( !sc_command_parser ( compiler , argc , argv ) ) {
		SCFreeEx ( &compiler ) ;
		return 0 ;
	}
	
	if ( SC_C99 & compiler->parameter ) {
		compiler->PRESOR = presor_c_run ;
		compiler->PARSER = parser_c_run ;
	}

	if ( SC_ARM & compiler->parameter ) {
		compiler->ASMOR = asmor_arm_run ;
		compiler->ASSEMER = assemer_arm_run ;
	}
	
	compiler->GENTOR = gentor_lac_run ;
	compiler->RELEASE = SCCompilerDestroy ;
	
	return 1 ;

}

int SCCompile ( int argc , char** argv , int type ) {

	//	author : Jelo Wang
	//	since : 20091127
	//	(c)TOK

	float preline = 0 ;

	LEXERC* lexer = 0 ;

 	SCCompilerReady ( argc , argv ) ;

	if ( !compiler ) return -1 ;
	
	compiler->start_time = clock () ;

	SCClListSetIterator ( compiler->il , SCCLLISTSEEK_HEAD ) ;

	for ( ; SCClListIteratorPermit ( compiler->il ) ; SCClListListIteratorNext ( compiler->il ) ) {
		
		char* lac = 0 ;
		char* asm = 0 ;
		char* buffer = 0 ;
		char* file = (char* ) SCClListIteratorGetElement ( compiler->il ) ;
		char* o = 0 ;

		int filen = 0 ;
		//int inputfile = SCHalFileOpen ( "C:\\Projects\\sc\\Debug\\ssa1.txt" , "rb" ) ;
		
		int inputfile = SCHalFileOpen ( "G:\\workspace\\semo\\Debug\\ssa1.txt" , "rb" ) ;
		
		if ( !inputfile ) {
			SCLog ("Can not open the file '%s'\n" , file ) ;
			continue ;
		}
		
		filen = SCHalFileLength ( inputfile ) ;
		buffer = (char* ) SCMalloc ( filen ) ;

		if ( !buffer ) {
			SCLog ("Not enough memory\n") ;
			continue ;
		}
		
		SCHalFileSeek ( inputfile , 0 , SEEK_HEAD ) ;
		SCHalFileRead ( inputfile , buffer , 1 , filen ) ;

		lexerc_set ( lexerc_new ( buffer , LEXERC_DEFAULT_MODE ) ) ; 		
		
		compiler->PRESOR ( sc_strcat (file,".po") ) ;	
 	 	compiler->PARSER ( &compiler->codes , &compiler->lines ) ; 
		lac = compiler->GENTOR ( sc_strcat (file,".lac") ) ;
		asm = compiler->ASMOR ( lac , sc_strcat (file,".sasm") ) ;
		
		o = sc_strcat (file,".elf") ;
		SCClListInsert  ( compiler->ol , o ) ;
		compiler->ASSEMER ( asm , o ) ;

		SCHalFileClose ( inputfile ) ;
		SCFree ( buffer ) ;
		SCFree ( file ) ;
		
	}

	if ( SC_LINK & compiler->parameter ) {
		if ( SC_ELF & compiler->parameter ) {
			compiler->LINKER = 0 ;
		}
		compiler->LINKER ( compiler->ol ) ;
	}
	
	compiler->end_time = clock () ;
	
	if ( compiler->lines && compiler->end_time - compiler->start_time )
		preline = (float)(compiler->lines / (compiler->end_time - compiler->start_time)) ;

	SCLog ("Cost : %1.3f sec\n" , (float)(compiler->end_time - compiler->start_time)/1000 ) ;
	SCLog ("Lines : %d \n" , compiler->lines ) ;
	SCLog ("Lines/MS : %1.3f \n" , preline ) ;
	SCLog ("Codes : %1.3fkb\n" , (float)compiler->codes / 1024 ) ;
	SCLog ("\n" ) ;

	compiler->RELEASE () ;

	return 0 ;
	
}
