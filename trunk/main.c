
# include "sc.h"

int main ( int argc , char* argv [] ) {
	
	# ifdef SEMO_DEBUG
	{

		int count = 4 ;
		 
		char* value [] = {
			
			"sc",
			"ssa1.txt" ,
			" -cr" ,
			" -lac" ,							

		} ;
   
   		SCCompile ( count , value , SC_EXECUTABLE_UNIT ) ;

		return 0 ;
	}
	# else 	
	{
		
	   	SCCompile ( argc , argv , SC_EXECUTABLE_UNIT ) ;
	  
 		return 0 ;
	}
	# endif

 
}   
