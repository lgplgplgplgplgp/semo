# include "sccl.h"

# include "sc.h"

int main ( int argc , char* argv [] ) {

	# ifdef SEMO_DEBUG
	{

		int count = 4 ;
		 
		char* value [] = {
			
			"sc",
			"ca.txt" ,
			" -cr" ,
			" -lac" ,

		} ;
   
   		SCCompile ( count , value , SC_STATIC_COMPILING ) ;

		return 0 ;
	}
	# else 	
	{
		
	   	SCCompile ( argc , argv , SC_STATIC_COMPILING ) ;
	  
 		return 0 ;
	}
	# endif

	return 0 ;

 
}   
