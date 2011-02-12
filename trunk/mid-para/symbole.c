
/*

+	SYMBOLE , between SC front-para and mid-para

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

//	author: Jelo Wang
//	since : 20081109
//	(c)TOK

# include "schal.h"
# include "sccl.h"
# include "symbole.h"
# include "lgnosia.h"

# if 0
{
//since : 20090321
//notes : for struct head trail chain debug
#define _LOOP_PRINT_STRUCT_HEAD_NAME(symbole) \
		while (symbole -> struct_head . next) { SCLog ("struct head : %s , chain : %d\n",symbole -> struct_head . next -> name,symbole -> struct_head . next -> number) ; \
		symbole -> struct_head . next = symbole -> struct_head . next  ->  next ; }


//since : 20090321
//notes : for struct head trail chain debug
#define _LOOP_WALK_STRUCT_HEAD_TRAIL(shead) int count = 0 ;\
			for( ;shead ;shead = shead  ->  next,count++) {\
				if (shead  ->  trail . next) {\
					SCLog ("father struct head : %s , on layer : %d\n",shead -> name,shead  ->  on_layer) ;\
					for( ;shead  ->  trail . next ;shead  ->  trail . next  = shead  ->  trail . next  ->  trail . next,count++) {\
						if (shead  ->  trail . next  ->  trail . snode) {\
							SCLog ("struct head : %s , on layer : %d\n",shead  ->  trail . next  ->  trail . snode  ->  name,shead  ->  trail . next  ->  trail . snode  ->  on_layer) ;\
							_PRINT_S("\n") ;\
						} else if (shead  ->  trail . next  ->  trail . anode) {\
							SCLog ("struct context : %s \n",shead  ->  trail . next  ->  trail . anode  ->  name) ;\
							_PRINT_S("\n") ;\
						}\
					}\
				} else SCLog ("struct head : %s , on layer : %d\n\n",shead -> name,shead  ->  on_layer) ;\
			}\
			SCLog ("struct head totall %d\n",count) ;

//since : 20090321
//notes : for struct head trail chain debug
#define _LOOP_WALK_STRUCT_HEAD_AZONAL_TRAIL(shead) int count = 0 ;\
			for( ;shead ;shead = shead  ->  next,count++) {\
				if (shead  ->  trail . next) {\
					SCLog ("father struct head : %s , on layer : %d\n",shead -> name,shead  ->  on_layer) ;\
					for( ;shead  ->  trail . next ;shead  ->  trail . next  = shead  ->  trail . next  ->  trail . next,count++) {\
						if (shead  ->  trail . next&&shead  ->  trail . next  ->  trail . anode) {\
							SCLog ("azonal head : %s\n",shead  ->  trail . next  ->  trail . anode  ->  name) ;\
						} else if (shead  ->  trail . next&&shead  ->  trail . next  ->  trail . snode) {\
							SCLog ("struct head : %s\n",shead  ->  trail . next  ->  trail . snode  ->  name) ;\
						}\
					}\
				} else SCLog ("struct head : %s , on layer : %d\n\n",shead -> name,shead  ->  on_layer) ;}\
				SCLog ("struct head totall %d\n",count) ;


//since : 20090321
//notes : for struct head trail chain debug
#define _LOOP_PRINT_STRUCT_TRAIL(symbole) \
		while (symbole -> struct_head . next  ->  trail .  next) { SCLog ("trial node : %s\n",symbole -> struct_head . next  ->  trail .  next  ->  trail .  node  -> name) ; \
		symbole -> struct_head . next  ->  trail .  next = symbole -> struct_head . next  ->  trail .  next  ->  trail . next ; }


//since : 20090322
//notes : for struct symbols debug
#define _LOOP_PRINT_STRUCT_SYMBOL(symbol) \
		for( ;symbol ;symbol = symbol -> next) {\
			SCLog ("struct symbol : %s\n",symbol  ->  azonal  ->  name) ; \
			_LOOP_WALK_STRUCT_HEAD_AZONAL_TRAIL(symbol -> head) ;\
		}\
}
# endif

# define SYMBOLE_DRCDISTUB(x)\
	(x == AZONAL_DRC_IF_DISTURB||x == AZONAL_DRC_ELSEIF_DISTURB||x == AZONAL_DRC_ELSE_DISTURB)

SYMBOLE* symbole = 0 ;

int SymboleNew () {

	//	author : Jelo Wang
	//	input : symbol table installation
	//	(C)TOK
	
	SYMBOLE* symbole = (SYMBOLE* ) SCMalloc ( sizeof(SYMBOLE) ) ;

	if ( !symbole ) return 0 ;
	
	symbole->variable.next = 0 ;
	symbole->variable.head = 0 ;
	
	symbole->array.next = 0 ;
	symbole->array.head = 0 ;	
	
	symbole->constant.next = 0 ;
	symbole->constant.head = 0 ;
	
	symbole->function.next = 0 ;
	symbole->function.head = 0 ;	

	symbole->funccal.next = 0 ;
	symbole->funccal.head = 0 ;	
		
	symbole->controlflow.next = 0 ;
	symbole->controlflow.head = 0 ;
	
	symbole->vartotall = 0 ;
	symbole->arrtotall = 0 ;
	symbole->functotall = 0 ;
	symbole->exptotall = 0 ;
	symbole->logicalblocks = 0 ;

	return (int)symbole ;

}


void SymboleSetCurrent ( int symbol ) {

	//	author : Jelo Wang
	//	input : symbol table installation
	//	(C)TOK

	symbole = (SYMBOLE* ) symbol ;

}


AZONAL* SymboleAddVarAzonal ( 
	
	char* name ,
	int data_type , 
	int azonal_type , 
	int scope ,
	int is_param ,
	int line ,
	int belong ,
	int lgabelong 
) {

	//	author : Jelo Wang
	//	notes : add ISA_VARIABLE
	//	since : 20100418
	//	(C)TOK

	//	(1) param : SYMBOLE* 
	//	(2) param : name
	//	(3) param : data type
	//	(4) param : azonal type
	//	(5) param : scope of variable
	//	(6) param : is it parameter of function	
	//	(7) param : line
	//	(8) param : function belong
	
	AZONAL* azonal = 0 ;
	
	if ( !symbole ) return 0 ;
	
	azonal = (AZONAL* ) SCMalloc ( sizeof(AZONAL) ) ;

	azonal->azonaltype = azonal_type ;
	azonal->datatype = data_type ;
	azonal->isparam = is_param ;	
	azonal->scope = scope ;
	azonal->line = line ;
	azonal->belong = belong ;
	azonal->used = 0 ;
	azonal->head = 0 ;
	azonal->next = 0 ;
	azonal->lgabelong = lgabelong ;

	azonal->DRC.deftimes = 0 ;
	azonal->DRC.chain = 0 ;
	
	azonal->name = (char* ) SCMalloc ( sc_strlen ( name ) + 1 ) ;
	sc_strcpy ( azonal->name , name ) ;
	
	if ( 0 == symbole->variable.next ) {
		symbole->variable.next = azonal ;
		symbole->variable.head = azonal ;
	} else {
		symbole->variable.next->next = azonal ;
		symbole->variable.next = azonal ; 
	}

	symbole->vartotall ++ ;
	
	if ( azonal->isparam ) 
	{
		AZONAL* belonganl = (AZONAL*) belong ;
		if ( ISA_FUNCTION == belonganl->azonaltype ) {
			SymboleAddFunctionParameter ( belonganl , azonal ) ;
		}
	}

	return azonal ;
	
}


AZONAL* SymboleFindVarAzonal ( 

	char* name ,
	int scope ,
	int lgabelong 

) {

	//	author : Jelo Wang
	//	notes : add ISA_VARIABLE
	//	since : 20100421
	//	(C)TOK

	//	(1) param : SYMBOLE* 
	//	(2) param : name
	//	(3) param : scope of variable
	//	(4) param : function belong

	AZONAL* globalanl = 0 ;
	AZONAL* azonal = 0 ;
	
	if ( !symbole ) return 0 ;

	if ( 0 == scope ) {
		
		for ( azonal = symbole->variable.head ; azonal ; azonal = azonal->next ) {

			if ( (0 == azonal->scope) && !sc_strcmp ( azonal->name , name ) ) {
				return 1 ;
			}	
			
		}

	} else if ( scope ) {

		//	找到每一个名字为name的变量，取出其lga，在其lga中查找当前lgabelong。

		for ( azonal = symbole->variable.head ; azonal ; azonal = azonal->next ) {

			if ( sc_strcmp ( azonal->name , name ) ) continue ;

			if ( LgnosiaFindContext ( (LGNOSIA*)azonal->lgabelong , lgabelong ) ) {
				return azonal ;
			}
			
		}
		
	}

	return azonal ;
	
}


AZONAL* SymboleVarAzonalSavable ( 

	char* name ,
	int scope ,
	int lgabelong 

) {

	//	author : Jelo Wang
	//	since : 20100514
	//	(C)TOK

	//	(1) param : name
	//	(2) param : scope of variable
	//	(4) param : lga belong to 
	
	AZONAL* globalanl = 0 ;
	AZONAL* azonal = 0 ;
	
	if ( !symbole ) return 0 ;

	if ( 0 == scope ) {
		
		for ( azonal = symbole->variable.head ; azonal ; azonal = azonal->next ) {

			if ( (0 == azonal->scope) && !sc_strcmp ( azonal->name , name ) ) {
				return 0 ;
			}	
			
		}

	} else if ( scope ) {


		for ( azonal = symbole->variable.head ; azonal ; azonal = azonal->next ) {

			if ( sc_strcmp ( azonal->name , name ) ) continue ;
				
//			if ( LgnosiaFindContext ( (LGNOSIA*)azonal->lgabelong , lgabelong ) ) {
				
//				if ( scope == azonal->scope ) return 0 ;
				
//			}

			//	在相同LGA 中，只允许一次定义。
			if ( lgabelong == azonal->lgabelong ) {
				return 0 ;
			}
			
		}

		
	}


	return 1 ;
	
}


void SymboleAzonalRefInc ( AZONAL* azonal ) {

	//	author : Jelo Wang
	//	since : 20100516
	//	(C)TOK

	//	increase reference times of AZONAL .

	if ( azonal ) azonal->used ++ ;

	
}

AZONAL* SymboleAddFunction ( char* name , int azonaltype , int type , int line ) {

	//	author : Jelo Wang
	//	input : name,type of return value,line
	//	output: insert a function record into symbol table
	//	(C)TOK


	AZONAL* azonal = (AZONAL* ) SCMalloc ( sizeof(AZONAL) ) ;

	if ( !azonal || !symbole  ) return 0 ;
		
	azonal -> name = (char* ) SCMalloc ( sc_strlen (name) + 1 ) ; 
	sc_strcpy ( azonal -> name , name ) ;
		
	azonal->datatype = type ;
	azonal->azonaltype = azonaltype ;
	azonal->line = line ;

	azonal->DRC.deftimes = 0 ;
	azonal->DRC.chain = 0 ;

	azonal->tack.head = 0 ;
	azonal->tack.next = 0 ;
	azonal->tack.totall = 0 ;
	azonal->number = ++ symbole->functotall ;
	azonal->next = 0 ;

	if ( 0 == symbole->function.next ) {
		symbole->function.next = azonal ;
		symbole->function.head = azonal ;
	} else {
		symbole->function.next->next = azonal ;
		symbole->function.next = azonal ;
	}

	symbole->logicalblocks ++ ;
	
	return azonal ;

}

AZONAL* SymboleAddFunctionCall ( char* name , int azonaltype , int line ) {

	//	author : Jelo Wang
	//	20110212
	//	(C)TOK

	AZONAL* azonal = 0 ;
	
	ASSERT(name);

	azonal = SCMalloc ( sizeof(AZONAL) ) ;
	ASSERT(azonal) ;
	
	azonal->name = SCMalloc ( sc_strlen ( name ) + 1 ) ;
	ASSERT(azonal->name);
	sc_strcpy ( azonal->name , name ) ;

	azonal->azonaltype = azonaltype ;
	azonal->line = line ;
			
	if ( 0 == symbole->funccal.next ) {
		symbole->funccal.next = azonal ;
		symbole->funccal.head = azonal ;
	} else {
		symbole->funccal.next->next = azonal ;
		symbole->funccal.next = azonal ;
	}

	return azonal ;
	
}

int SymboleGetCurrentFuncNumber () {
	
	//	author : Jelo Wang
	//	since : 20100430
	//	(C)TOK

	if ( !symbole ) return -1;

	return symbole->function.next->number ;
	
}

int SymboleGetCurrentFunc() {
	
	//	author : Jelo Wang
	//	since : 20100508
	//	(C)TOK

	if ( !symbole ) return -1;

	return (int)symbole->function.next ;
	
}

AZONAL* SymboleFindFunction ( char* name ) {

	//	author : Jelo Wang	
	//	input : name
	//	notes: find a function
	//	(C)TOK

	AZONAL* walker = 0 ;
		 
	if ( !symbole )
		return 0 ;

	for ( walker = symbole->function.head ; walker && sc_strcmp ( walker -> name , name ) ; walker = walker->next ) ;

	return walker ? walker : 0 ;

}

int SymboleAddFunctionParameter ( AZONAL* function , AZONAL* azonal ) {

	//	author : Jelo Wang	
	//	input : AZONAL pointer
	//	notes: add parameters of a function
	//	(C)TOK
	
	ASSERT(symbole&&azonal&&function) ;
 	
	if ( 0 == function->tack.next ) {
		function->tack.next = azonal ;
		function->tack.head = azonal ;
	} else {
		function->tack.next->next = azonal ;
		function->tack.next = azonal ;
	}	

	function->tack.totall ++ ;

	return 1 ;

}

void SymboleSetFunctionParameters ( AZONAL* function , int parameters ) {

	//	author : Jelo Wang	
	//	input : amount
	//	notes: set amount of parameters of a function
	//	(C)TOK
	
	if ( function ) function->tack.totall = parameters ;

}

int SymboleGetFunctionNumber ( char* name ) {

	//	author : Jelo Wang
	//	input : function name
	//	output: function number
	//	since : 20081231
	//	(C)TOK

	AZONAL* azonal = 0 ;
	
	if ( !symbole )
		return 0 ;
	
	for ( azonal = symbole->function.head ; azonal && sc_strcmp ( azonal->name , name ); azonal = azonal->next ) ;
		
	
	return azonal ? azonal->number : 0 ;
	
}


AZONAL* SymboleAddControlAzonal ( 

	int azonaltype , 
	int line 
	
) {

	//	author : Jelo Wang
	//	input : pointer of entry,data type
	//	notes: add control flow
	//	since : 20100115
	// (C)TOK
	
	AZONAL* azonal = (AZONAL* ) SCMalloc ( sizeof(AZONAL) ) ;
	
	if ( !azonal || !symbole ) return 0 ;
	
	azonal->name = 0 ;
	azonal->azonaltype = azonaltype ;
	azonal->line = line ;
	azonal->number = ++symbole ->logicalblocks ;

	azonal->tack.head = 0 ;
	azonal->tack.next = 0 ;
	azonal->head = 0 ;
	azonal->next = 0 ;
	
	if ( 0 == symbole->controlflow.next ) {
		symbole->controlflow.next = azonal ;
		symbole->controlflow.head = azonal ;
	} else {
		symbole->controlflow.next->next = azonal ;
		symbole->controlflow.next = azonal ;
	}

	return azonal ;
	
}

void SymboleUninstall () {

	//	author : Jelo Wang
	//	since : 20100430
	//	(C)TOK

	AZONAL* azonal = 0 ;

	for ( azonal = symbole->variable.head ; azonal ; ) {
		
		symbole->variable.next = azonal->next ;
		if ( azonal->name ) SCFree ( azonal->name ) ;

		if ( azonal->DRC.chain ) SCClListDestroy ( azonal->DRC.chain ) ;

		SymboleDRCDropDRC ( azonal ) ;
		
		SCFree ( azonal ) ;
		azonal = symbole->variable.next ;
		
	}

	for ( azonal = symbole->function.head ; azonal ; ) {
		
		symbole->function.next = azonal->next ;
		if ( azonal->scope ) SCFree ( azonal->scope ) ;
		if ( azonal->name ) SCFree ( azonal->name ) ;
		SCFree ( azonal ) ;
		azonal = symbole->function.next ;
		
	}

	for ( azonal = symbole->controlflow.head ; azonal ; ) {
		
		symbole->controlflow.next =  azonal->next ;
		if ( azonal->scope ) SCFree ( azonal->scope ) ;
		if ( azonal->name ) SCFree ( azonal->name ) ;
		SCFree ( azonal ) ;
		azonal = symbole->controlflow.next ;
		
	}

	SCFree ( symbole ) ;
	
}


# if 0
int SymboleFindAzonalWithSCope ( SYMBOLE* symbole , char* name , int function , char* scope , int type ) {

	//	author : Jelo Wang
	//	input : name,function number,scope,ending flow
	//	notes : find variable or array with scope
	//	since : 20081227
	//	(C)TOK
	
	AZONAL* walker ;
	int find ;

	if ( ISA_VARIABLE == type )
		walker = symbole ->variable next ;
	else if ( ISA_ARRAY == type )
		walker = symbole -> array . next ;
	else if ( ISA_FUNCTION == type )
		walker = symbole -> function . next ;
	else if ( ISA_EXPR == type )
		walker = symbole -> EXPR . next ;
	else
		return 0 ;

	while ( walker ) {

		if ( !sc_strcmp ( name , walker -> name ) && ( !walker -> belong ) ) {
			walker -> used = 1 ;
			return 1 ;
		} 
		
		if ( !sc_strcmp ( name , walker -> name ) ) {
			if ( walker -> belong == function )
				if ( 0 >= sc_strcmp ( walker -> scope , scope ) ) {
					walker -> used = 1 ;
					return 1 ;
			}
		} 
		
		if ( !sc_strcmp ( name , walker -> name ) )
			find = -1 ;
			
			walker = walker  ->  next ;

	}	
	
	if ( -1 == find )
		return -1 ;
		return 0 ;

}


AZONAL* SymboleGetAzonal ( SYMBOLE* symbole , int type ) {

	//	author : Jelo Wang
	//	input : data class (variable or array or function)
	//	output: pointer of symbol table entry of data class
	//	(C)TOK
	
	if ( ISA_VARIABLE == type )
		return symbole -> variable . next ;
	else if ( ISA_ARRAY == type )
		return symbole -> array . next ;
	else if ( ISA_FUNCTION == type )
		return symbole -> function . next ;
	else if ( ISA_EXPR == type )
		return symbole -> EXPR . next ;
		return 0 ;
}

AZONAL* SymboleGetAzonalWithSCope ( SYMBOLE* symbole , int function , char* name , char* scope , int type ) {

	//	author : Jelo Wang
	//	input : function number,name,scope
	//	output: pointer of entry
	//	since : 20081231
	//	(C)TOK

	AZONAL* azonal = 0 ;	
		
	if ( !symbole || !sc_strlen ( name ) || !sc_strlen ( scope ) )
		return 0 ;

	if ( !symbole -> array . next && !symbole -> variable . next && !symbole -> numeral . next )
		return 0 ;
	
	if ( ISA_VARIABLE == type )
		azonal = symbole -> variable . next ;
	else if ( ISA_ARRAY == type )
		azonal = symbole -> array . next ;
	else if ( ISA_NUMERAL == type )
		azonal = symbole -> numeral . next ;
	else
		return 0 ;

	while ( azonal && ((azonal -> belong!=function)||(0!=sc_strcmp(azonal -> name,name))||(0!=sc_strcmp(azonal -> scope,scope))) )
		azonal = azonal  ->  next ;

	if (azonal)
		return azonal ;
		return 0 ;

}


AZONAL* SymboleGetAzonalWithName ( SYMBOLE* symbole , char* function , char* name , char* scope , int type ) {

	//	author : Jelo Wang
	//	input : function name,name,scope
	//	output: pointer of entry
	//	since : 20081231
	//	(C)TOK

	int function_num = -1 ;
	
	if (!symbole||!sc_strlen(function)||!sc_strlen(name)||!sc_strlen(scope) )
		return 0 ;

	function_num = SymboleGetFunctionNumber ( symbole -> function . next , function ) ;
	
	return SymboleGetAzonalWithSCope ( symbole , function_num , name , scope , type ) ;

}


int SymboleFindAzonalWithAzonal ( SYMBOLE* symbole , AZONAL* name , int type ) {

	//	author : Jelo Wang
	//	input : pointer of entry,data type
	//	output: 1 or 0
	//	(C)TOK
	
	AZONAL* walker ;

	switch ( type ) {
		case ISA_VARIABLE:
			walker = symbole -> variable . head ;
		break ;
		case ISA_ARRAY:
			walker = symbole -> array . head ;
		break ;
		case ISA_NUMERAL:
			return 0 ;

	}	

	while ( walker ) {
		
		if ( !sc_strcmp ( name -> name , walker -> name ) && ( name -> belong == walker -> belong ) && !sc_strcmp ( name -> scope , walker -> scope ) ) {
			//struct_trail_number != 0 means the symbol is a trail node of struct symbol
			if ( walker  ->  struct_context && walker  ->  struct_context == name  ->  struct_context )
				return 1 ;
			return 0 ;
		}

		walker = walker  ->  next ;

	}	

	return 0 ;



}


AZONAL* SymboleFindAzonal ( SYMBOLE* symbole , int type , char* name , int belong ) {

	//	author : Jelo Wang
	//	input : ending flow,name,belong
	//	notes : adjust property used of varible or array
	//	(C)TOK

	AZONAL* walker = 0 ;	

	if (!name)
		return 0 ;

	switch ( SymboleGetAzonalType ( type ) ) {
		case ISA_VARIABLE:
			walker = symbole -> variable . head ;
		break ;
		case ISA_ARRAY:
			walker = symbole -> array . head ;
		break ;
	}	

	while ( walker ) {
	
		if (!sc_strcmp(walker -> name,name)&&(belong==walker -> belong)) {
			walker -> used = 1 ;
			return walker ;
		} else if (!sc_strcmp(walker -> name,name)&&!sc_strcmp(walker -> scope,"0")) {
			walker -> used = 1 ;
			return walker ;
		}

		walker = walker  ->  next ;
		
	}	return 0 ;


}


int SymboleGetAzonalPoolLength ( AZONAL* azonal ) {

	//	author : Jelo Wang	
	//	input : AZONAL
	//	output: length of AZONAL Pool
	//	(C)TOK
	
	int length = 0 ;
	
	while ( azonal ) azonal = azonal -> next , length ++ ;
		
	return length ;
	
}


int SymboleSetArraySCale ( SYMBOLE* symbole , int size ) {

	//	author : Jelo Wang
	//	input : AZONAL
	//	notes : set contents amount of an array
	//	(C)TOK
	
	if ( !symbole -> array . next )
		return 0 ;
		
	symbole -> array . next -> size = size ;

	return 1 ;

}



AZONAL* SymboleAddControlAzonal ( SYMBOLE* symbole , char* name , int azonaltype , int endflow , char* scope , int line ) {

	//	author : Jelo Wang
	//	input : pointer of entry,data type
	//	notes: add control flow
	//	since : 20100115
	// (C)TOK
	
	AZONAL* azonal = (AZONAL* ) SCMalloc ( sizeof(AZONAL) ) ;
	
	if ( !azonal ) return 0 ;
	
	azonal -> name = (char* ) SCMalloc ( sc_strlen (name) + 1 ) ;
	
	if ( !azonal -> name ) return 0 ;
	
	sc_strcpy(azonal -> name,name) ;
	azonal -> azonaltype = azonaltype ;
	azonal -> datatype = endflow ;
	azonal -> line = line ;
	azonal -> number = ++symbole -> control_flow_amount ;

	azonal -> tack . head = 0 ;
	azonal -> tack . next = 0 ;
	
	if ( 0 == symbole -> controlflow . next ) {
		symbole -> controlflow . next = azonal ;
		symbole -> controlflow . head = azonal ;
	} else {
		symbole -> controlflow . next -> next = azonal ;
		symbole -> controlflow . next = azonal ;
	}

	return azonal ;
	
}


int SymboleAddStructHead ( SYMBOLE* symbole , char* name , char* scope , int belong , int type , int on_layer ) {

	//	author : Jelo Wang
	//	input : struct head name,scope,belong,and type,struct_trail_number
	//	output: sucess 1 or 0
	//	(C)TOK

	STRUCT_HEAD* stnode = 0 ;
	
	if (!name || !scope || !symbole)
		return 0 ;
	
	if ( SymboleFindStructHead ( symbole , name , scope , belong , on_layer ) )
		return 0 ;

	stnode = (STRUCT_HEAD* ) SCMalloc ( sizeof(STRUCT_HEAD) ) ;
	stnode  ->  layer = 0 ;
	
	stnode  ->  name = (char* ) SCMalloc ( sc_strlen(name) + 1 ) ;
	sc_strcpy ( stnode  ->  name , name ) ;
	
	stnode  ->  scope = (char* ) SCMalloc ( sc_strlen(scope) + 1 ) ;
	sc_strcpy ( stnode  ->  scope , scope ) ;
	
	stnode  ->  belong = belong ;
	stnode  ->  type = type ;
	stnode  ->  on_layer = on_layer ;
	stnode  ->  number = ++ symbole -> struct_head_amount ;
	stnode  ->  trail . next = 0 ;
	stnode  ->  next = 0 ;
	stnode  ->  front = symbole -> struct_head -> next ;

	symbole -> struct_head -> next = stnode ;
	symbole -> struct_head = stnode ;
	
	return 1 ;

}


int SymboleFindStructHead ( SYMBOLE* symbole , char* name , char* scope , int belong , int on_layer ) {

	//	author : Jelo Wang	
	//	input : struct head name,scope,belong
	//	output: sucess 1 or 0
	//	(C)TOK

	STRUCT_HEAD* walker = 0 ;
	
	if ( !name || !scope )
		return 0 ;

	if ( !symbole  ->  struct_head  ->  next )
		return 0 ;

	walker = symbole  ->  struct_head  ->  next ;

	while ( walker )
  		if ( sc_strcmp (walker  ->  name,name) || (walker -> belong != belong) || sc_strcmp ( walker  ->  scope , scope ) || (walker -> on_layer != on_layer) )
			walker = walker  ->  next ;
		else break ;

	return (walker) ? 1 : 0 ;


}



int SymboleFindStructTrial ( SYMBOLE* symbole , AZONAL* azonal , STRUCT_HEAD* shead ) {

	//	author : Jelo Wang	
	//	input : AZONAL
	//	output: sucess 1 or 0
	//	(C)TOK
	
	STRUCT_HEAD* anode = 0 ;

	if (!azonal)
		return 0 ;

	anode = (STRUCT_HEAD* ) SCMalloc ( sizeof(STRUCT_HEAD) ) ;
	anode -> trail . anode = azonal ;
	anode -> trail . snode = 0 ;
	anode -> trail . next = shead -> trail . next ;
	shead -> trail . next = anode ;
	azonal -> struct_context = shead -> number ;

	return 1 ;

}



int SymboleAddStructAzonal ( SYMBOLE* symbole , STRUCT_HEAD* shead , AZONAL* azonal ) {


	//	author : Jelo Wang
	//	input : STRUCT HEAD,AZONAL
	//	output: sucess 1 or 0
	//	(C)TOK

	STRUCT_SYMBOL* stru_sym = 0 ;
	
	if (!shead || !azonal)
		return 0 ;

	stru_sym = (STRUCT_SYMBOL* ) SCMalloc ( sizeof(STRUCT_SYMBOL) ) ;
	stru_sym  ->  azonal = azonal ;
	stru_sym  ->  head = shead ;
	stru_sym  ->  next = symbole -> struct_symbol . next ;
	symbole -> struct_symbol . next = stru_sym ;

	return 1 ;

}

void SymboleSetStructSCale ( SYMBOLE* symbole , int scale ) {

	//	author : Jelo Wang	
	//	input : amount
	//	notes : set amount of field of a struct azonal
	//	since : 20090320
	//	(C)TOK

	if ( symbole -> _struct_ . next )
		symbole -> _struct_ . next  ->  scale = scale ;


}
#endif

/*
static char* SymboleDRCConvertCFFSETtoSSA ( AZONAL* azonal , ANLDRC* cff ) {


	//	author : Jelo Wang
	//	since : 20100812
	//	(C)TOK

	//	notes : Convert CFFSET to SSA form
	
	char error [] = { "SymboleDRCConvertCFFSETtoSSA error" } ;

	SCClList* looper = 0 ;
	SCClString* cffset = 0 ;
	
	if ( !cff ) return error ;
	else if ( AZONAL_DRC_CFF != cff->cf ) return error ;
	
	if ( 1 == cff->cffset.type ) {

		cffset = SCClStringNew () ;
		
		if ( !cffset ) return error ;
		
		looper = (SCClList* ) cff->cffset.handle ;

		cff->cffset.type = 2 ;
		cff->cffset.handle = cffset ;
			
		SCClStringAddStr ( cffset , "%$SSA_MEG(" ) ;
		for ( looper = looper->head ; looper ; looper = looper->next ) {
			
			char* name = 0 ;
			
			name = sc_strcat ( azonal->name , SCClItoa (looper->element) ) ;
			SCClStringAddStr ( cffset , name ) ;
			if ( looper->next ) SCClStringAdd( cffset , ',' ) ;
		
			SCFree ( name ) ;
			
		}
		SCClStringAddStr ( cffset , ")" ) ;
		SCClStringAdd( cffset , '\0' ) ;		
		
	} else if ( 2 == cff->cffset.type ) {

		cffset = (SCClString* ) cff->cffset.handle ; 

		if ( !cffset ) return error ;
		
		return cffset->data ;

	}
	
	
	return error ;


}
*/

static ANLDRC* SymboleDRCSearchCFF ( AZONAL* azonal ) {

	//	author : Jelo Wang
	//	since : 20100812
	//	(C)TOK

	//	notes : Search CFF

	SCClList* looper = 0 ;
	ANLDRC* drc = 0 ;

	if ( !azonal->DRC.chain ) return 0 ;

	for ( looper = azonal->DRC.chain->head ; looper ; looper = looper->next ) {
		drc = (ANLDRC* ) looper->element ;
		if ( AZONAL_DRC_CFF == drc->cf ) {
			return drc ;	
		}
	}

	return 0 ;


}

static SCClList* SymboleDRCSearchCFFHead ( AZONAL* azonal , int scope , int lga ) {

	//	author : Jelo Wang
	//	since : 20100813
	//	(C)TOK

	//	notes : Search CFFHEAD , the first record front of CFF

	SCClList* looper = 0 ;
	ANLDRC* drc = 0 ;

	if ( !azonal->DRC.chain ) return 0 ;

	for ( looper = azonal->DRC.chain->head ; looper ; looper = looper->next ) {
		drc = (ANLDRC* ) looper->element ;
		if ( drc && scope != drc->scope ) {
			return looper ;	
		}
	}

	return 0 ;


}


static char* SymboleDRCConvertCFFToSSA ( AZONAL* azonal , ANLDRC* cff ) {

	//	author : Jelo Wang
	//	since : 20100812
	//	(C)TOK

	//	Convert CFF to SSA Form
	
	char error [] = { "SymboleDRCConvertCFFToSSA error" } ;
	
	SCClList* looper = 0 ;
	SCClString* cffstring = 0 ;
		
	cff->cffref_t ++ ;
	
	if ( 1 == cff->cffset.type ) {

		looper = (SCClList* ) cff->cffset.handle ;
		
		if ( !looper ) return error ;

		cff->cffset.type = 2 ;

		cffstring = SCClStringNew () ;

		cff->cffset.handle = (void* )cffstring ;

		if ( !cffstring ) return error ;
		
		SCClStringAddStr ( cffstring , "%$SSA_MEG(" ) ;
		for ( looper = looper->head ; looper ; looper = looper->next ) {
			
			char* name = 0 ;
			
			name = sc_strcat ( azonal->name , SCClItoa (looper->element) ) ;
			SCClStringAddStr ( cffstring , name ) ;
			if ( looper->next ) SCClStringAdd( cffstring , ',' ) ;
			SCFree ( name ) ;
			
		}
		SCClStringAddStr ( cffstring , ")" ) ;
		SCClStringAdd( cffstring , '\0' ) ;

		return cffstring->data ;
		
	} else if ( 2 == cff->cffset.type ) {
		
		cffstring = (SCClString* ) cff->cffset.handle ; 

		if ( !cffstring ) return error ;
		
		return cffstring->data ;
		
	}

	

}


static ANLDRC* SymboleDRCGetTheCurrentDRC ( AZONAL* azonal ) {

	//	author : Jelo Wang
	//	since : 20100812
	//	(C)TOK

	//	notes : return the current DRC record 
	
	if ( !azonal->DRC.chain ) return 0 ;
	else if ( !azonal->DRC.chain->head ) return 0 ;
	return (ANLDRC* ) azonal->DRC.chain->next->element ; 


}

char* SymboleDRCAdd ( AZONAL* azonal , int scope , int cf , int lga ) {

	//	author : Jelo Wang
	//	since : 20100812
	//	(C)TOK

	//	Add a DRC record
	
	char error [] = { "SymboleDRCAdd error" } ;

	ANLDRC* drc = 0 ;	
	ANLDRC* anldrc = SymboleDRCGetTheCurrentDRC ( azonal ) ;
	
	if ( anldrc && scope < anldrc->scope ) {
		SymboleDRCGenCFF ( azonal , scope , lga  ) ;
	}

	drc = (ANLDRC* ) SCMalloc ( sizeof(ANLDRC) ) ;

	if ( !drc ) return error ;

	drc->lga = lga ;
	drc->alias = ++azonal->DRC.deftimes ;
	drc->scope = scope ;
		
	if ( ISA_IFCF == cf ) drc->cf = AZONAL_DRC_IF_DISTURB ;
	else if ( ISA_ELSEIFCF == cf ) drc->cf = AZONAL_DRC_ELSEIF_DISTURB ;
	else if ( ISA_ELSECF == cf ) drc->cf = AZONAL_DRC_ELSE_DISTURB ;
	else drc->cf = AZONAL_DRC_DEFAULT ;

	if ( !azonal->DRC.chain ) azonal->DRC.chain = SCClListNew () ;
	if ( azonal->DRC.chain ) SCClListInsert ( azonal->DRC.chain , (int)drc ) ;

	# if 0
	{
		SCClList* walker = 0 ;
		if ( azonal->DRC.chain ) walker = azonal->DRC.chain->head ;
		printf("___________________________________\n") ;
		for ( ; walker ; walker = walker->next ) {
			ANLDRC* drc = 0 ;
			drc = (ANLDRC* ) walker->element ;
			printf("DRC as : %d , sp : %d , cf : %d , lga : %d , cffe : %d\n",drc->alias,drc->scope,drc->cf,drc->lga,drc->cffset.handle) ;
		}
	}
	# endif

	//	release the memory outside
	return sc_strcat ( azonal->name , SCClItoa (drc->alias) ) ; 	
	
}

char* SymboleDRCGetDRC ( AZONAL* azonal , int scope , int lga ) {
	
	//	author : Jelo Wang
	//	since : 20100812
	//	(C)TOK

	//	notes : Get DRC record
	
	char error [] = { "SymboleDRCGetDRC error" } ;

	ANLDRC* drc = 0 ;	
	ANLDRC* drcpre = 0 ;
	ANLDRC* anldrc = SymboleDRCGetTheCurrentDRC ( azonal ) ;
	ANLDRC* drcfront = 0 ;	

	SCClList* looper = 0 ;
	SCClList cfflist = {0} ;
	SCClString cffset = {0} ;

	if ( anldrc && scope < anldrc->scope ) {
		
		ANLDRC* cff = SymboleDRCGenCFF ( azonal , scope , lga ) ;				
		if ( cff ) return SymboleDRCConvertCFFToSSA ( azonal , cff ) ;
		
	} else if ( anldrc && scope == anldrc->scope ) {
			
		int cffhead = SymboleDRCSearchCFFHead ( azonal , scope , lga ) ;			
					
		//	if anldrc->lga is equal the current LGA , return anldrc->alias
		if ( lga == anldrc->lga ) {
			//	release the memory outside
			return sc_strcat ( azonal->name , SCClItoa (anldrc->alias) ) ; 
				
		}
		
		# if 0
		else if ( 0 == cffhead ) {
								
			//	并且与当前记录不属一个LGA，循环直到作用域不相等，
			//	取的同一层各个不同LGA的最后一个记录的别名，当循环直到作用域不相等时，
			//	取得记录别名。生成SSA FORM返回。
			
			for ( looper = azonal->DRC.chain->next ; looper ; looper = looper->front ) {
		
				drcpre = drc ;
				drc = (ANLDRC* ) looper->element ; 
						
				if ( !drc ) goto SSADEF ;
						
				if ( drcpre && drcpre->lga != drc->lga ) {
					SCClListInsert ( &cfflist , drc->alias ) ;
				} else if ( !drcpre ){
					SCClListInsert ( &cfflist , drc->alias ) ;
				}					
		
			}		
				
			SCClStringAddStr ( &cffset , "%$SSA_MEG(" ) ;
			for ( looper = cfflist.head ; looper ; looper = looper->next ) {
						
				char* name = 0 ;
						
				name = sc_strcat ( azonal->name , SCClItoa (looper->element) ) ;
				SCClStringAddStr ( &cffset , name ) ;
				if ( looper->next ) SCClStringAdd( &cffset , ',' ) ;
		
				SCFree ( name ) ;
						
			}
			SCClStringAddStr ( &cffset , ")" ) ;
			SCClStringAdd( &cffset , '\0' ) ;		
		
			{
				char* name = sc_strnew ( cffset.data ) ;	
				if ( !name ) return error ;
				return name ;
			}	
		
		} 
		# endif

		else if ( cffhead ) {

			//	（5）如果搜到了CFF，将CFF HEAD为始点S，到当前记录为终点P，
			//	取区间[S,P]中所有“有效别名”，生成SSA FORM返回。

			ANLDRC* drcnext = 0 ;		

			for ( looper = (SCClList* )cffhead ; looper ; looper = looper->next ) {

				drc = (ANLDRC* ) looper->element ; 

				if ( !drc ) goto SSADEF ;

				if ( AZONAL_DRC_CFF == drc->cf ) {
					//SymboleDRCConvertCFFSETtoSSA ( azonal , drc ) ;
					continue ;
				}
		
				if ( looper->next ) {
					drcnext = (ANLDRC* ) looper->next->element ; 					
					//	不属于同一个lga
					if ( drcnext && drcnext->lga != drc->lga ) {
						SCClListInsert ( &cfflist , drc->alias ) ;
					} //else SCClListInsert ( &cfflist , drc->alias ) ;

				} else {
					SCClListInsert ( &cfflist , drc->alias ) ;
				}

			}			

			SCClStringAddStr ( &cffset , "%$SSA_MEG(" ) ;
			for ( looper = cfflist.head ; looper ; looper = looper->next ) {
				
				char* name = 0 ;
				
				name = sc_strcat ( azonal->name , SCClItoa (looper->element) ) ;
				SCClStringAddStr ( &cffset , name ) ;
				if ( looper->next ) SCClStringAdd( &cffset , ',' ) ;			
				SCFree ( name ) ;
				
			}
			SCClStringAddStr ( &cffset , ")" ) ;
			SCClStringAdd( &cffset , '\0' ) ;		
			
			{
				char* name = sc_strnew ( cffset.data ) ;	
				if ( !name ) return error ;
				return name ;
			}
			
		}		

		
	} else if ( 0 == anldrc ) {

SSADEF :	
		//	release the memory outside
		return sc_strcat ( azonal->name , SCClItoa (azonal->DRC.deftimes) ) ; 	
						
	} else if ( anldrc && scope > anldrc->scope && !SYMBOLE_DRCDISTUB(anldrc->cf ) ) {

		//	release the memory outside
		return sc_strcat ( azonal->name , SCClItoa (anldrc->alias) ) ; 		

	} else if ( anldrc && scope > anldrc->scope && SYMBOLE_DRCDISTUB(anldrc->cf) ) {
		
		for ( looper = azonal->DRC.chain->next ; looper ; looper = looper->front ) {
		
			drcpre = drc ;
			drc = (ANLDRC* ) looper->element ; 
						
			if ( !drc ) break ;
						
			if ( drcpre && drcpre->lga != drc->lga ) {
				SCClListInsert ( &cfflist , drc->alias ) ;
			} else if ( !drcpre ){
				SCClListInsert ( &cfflist , drc->alias ) ;
			}					
		
		}
		
		SCClStringAddStr ( &cffset , "%$SSA_MEG(" ) ;
		for ( looper = cfflist.head ; looper ; looper = looper->next ) {
						
			char* name = 0 ;
						
			name = sc_strcat ( azonal->name , SCClItoa (looper->element) ) ;
			SCClStringAddStr ( &cffset , name ) ;
			if ( looper->next ) SCClStringAdd( &cffset , ',' ) ;
			SCFree ( name ) ;
						
		}
		SCClStringAddStr ( &cffset , ")" ) ;
		SCClStringAdd( &cffset , '\0' ) ;		
		
		{
			char* name = sc_strnew ( cffset.data ) ;	
			if ( !name ) return error ;
			//	release the memory outside
			return name ;
		}			

	}

	return error ;
	

}

ANLDRC* SymboleDRCGenCFF ( AZONAL* azonal , int scope , int lga ) {

	//	author : Jelo Wang
	//	since : 20100812
	//	(C)TOK
	
	int currscope = scope ;

	int cff_tail = 0 ;
	int cff_head = 0 ;

	ANLDRC* drc = 0 ;
	ANLDRC* drcfront = 0 ;
	ANLDRC* drcnext = 0 ;
	ANLDRC* drcpre = 0 ;
	ANLDRC* cff = 0 ;
	
	SCClList* looper = (SCClList* )azonal->DRC.chain->next ;
	SCClList* cffset = SCClListNew () ;

	if ( !cffset ) return 0 ;

	{

		cff = SymboleDRCSearchCFF ( azonal ) ;
		if ( cff ) {
			return cff ;
		}
		
	}

	cff_tail = (int) looper ;
	
	for ( ; looper ; looper = looper->front ) {

		drcpre = drc ;
		drc = (ANLDRC* ) looper->element ; 

		if ( !drc ) {
			cff_head = looper->next ;
			break ;			
		}

		//	get the next DRC node
		//	and check its LGA value 		
		if ( drcpre && drcpre->lga != drc->lga ) {
			SCClListInsert ( cffset , drc->alias ) ;
		} else if ( !drcpre ){
			SCClListInsert ( cffset , drc->alias ) ;
		}		
		
		if ( looper->front ) drcfront = (ANLDRC* ) looper->front->element ; 
		else drcfront = 0 ;
		
		if ( !drcfront ) {
			//	save CFF HEAD
			//	the first DRC front of CFF		
			if ( drc ) SCClListInsert ( cffset , drc->alias ) ;
			cff_head = looper->next ;
			break ;
		} else if ( drcfront->scope != drc->scope && !SYMBOLE_DRCDISTUB(drcfront->cf) ) {
			//	save CFF HEAD
			//	the first DRC front of CFF		
			if ( drcfront ) SCClListInsert ( cffset , drcfront->alias ) ;
			cff_head = looper->next ;
			break ;
		}
		
	}

	//	将所有属于CF干扰区间内的节点变换为CFF
	cff = (ANLDRC* ) SCMalloc ( sizeof(ANLDRC) ) ;	
	
	if ( !cff ) {
		SCClListDestroy ( cffset ) ;
		SCFree ( cffset ) ;
		return 0 ;
	};

	cff->cf = AZONAL_DRC_CFF ;
	cff->cffset.type = 1 ;
	cff->cffset.handle = (void*)cffset ;	

	SCClListDeleteBetweenTwo ( cff_head , cff_tail ) ;
	SCClListConect ( cff_head , cff , cff_tail ) ;	
		
	return (int)cff ;
	

}	


void SymboleDRCDropCFF ( AZONAL* azonal ) {

	//	author : Jelo Wang
	//	since : 20100810
	//	(C)TOK

	//	Delete CFF

	SCClList* looper = 0 ;
	ANLDRC* drc = 0 ;

	if ( !azonal->DRC.chain ) return ;

	for ( looper = azonal->DRC.chain->head ; looper ; looper = looper->next ) {
		drc = (ANLDRC* ) looper->element ;
		if ( drc && AZONAL_DRC_CFF == drc->cf ) {
			break ;	
		}
	}

	if ( drc && drc->cffref_t >= 2 ) {

		if ( 1 == drc->cffset.type ) {
			SCClListDestroy ( (SCClList* ) drc->cffset.handle ) ;
			SCFreeEx ( &drc->cffset.handle ) ;
		} else if ( 2 == drc->cffset.type ) {
			SCClStringDestroy ( (SCClString* ) drc->cffset.handle ) ;	
			drc->cffset.handle = 0 ;
		}		

	}

	if ( looper ) {
		looper->front->next = looper->next ; 
		if ( looper->next ) looper->next->front = looper->front ; 
		SCFree ( looper ) ;
	}

	
}

void SymboleDRCDropDRC ( AZONAL* azonal ) {

	//	author : Jelo Wang
	//	since : 20100810
	//	(C)TOK

	//	Destroy DRC

	SCClList* looper = 0 ;
	ANLDRC* drc = 0 ;
	
	if ( !azonal->DRC.chain ) return ;

return ;

	for ( looper = azonal->DRC.chain->head ; looper ; ) {

		azonal->DRC.chain->next = looper->next ;
		
		drc = (ANLDRC* ) looper->element ;
		
		if ( drc && AZONAL_DRC_CFF == drc->cf ) {
	
			if ( 1 == drc->cffset.type ) {
				SCClListDestroy ( (SCClList* ) drc->cffset.handle ) ;
				SCFreeEx ( &drc->cffset.handle ) ;
			} else if ( 2 == drc->cffset.type ) {
				SCClStringDestroy ( (SCClString* ) drc->cffset.handle ) ;	
				drc->cffset.handle = 0 ;
			}

			SCFree ( drc ) ;
		
		} 

		SCFree ( looper ) ;
		
		looper = azonal->DRC.chain->next ;

		
	}


}

AZONAL* SymboleAndNumeric ( char* name , int type ) {

	//	author : Jelo Wang
	//	since : 20110125
	//	(C)TOK

	AZONAL* azonal = (AZONAL* ) SCMalloc ( sizeof(AZONAL) ) ;

	azonal->name = (char* ) SCMalloc ( sc_strlen (name)+1 ) ;

	ASSERT(azonal->name) ;

	sc_strcpy ( azonal->name , name ) ;

	azonal->azonaltype = type ;

	return azonal ;
	
}

