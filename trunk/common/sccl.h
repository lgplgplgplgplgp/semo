
/*

+	Semo C()mpiler Common Library

+	Semo C()mpiler is a free software created by (c)Techniques of Knowledge since 20080202.
+	(C)TOK Jelo Wang

+
+	You can redistribute it and/or modify it under the terms of the gnu general public license	
+	as published by the free software foundation, either version 3 of the license or any later 	
+	version.this program is distributed in the hope	that it will be useful,but without any 		
+	warranty.without even the implied warranty of merchantability or fitness for a particular 	
+	purpose.																					
																											
+	(c)	Techniques of Knowledge
+		an open source group since 2008
+		page : http://www.tok.cc
+		email : wqw85@sina.com

*/

# ifndef __SCCL
# define __SCCL

# include <assert.h>

# define ASSERT assert

typedef enum {

	//	author : Jelo Wang
	//	since : 20100609
 
	//	notes : seek

	SCCLLISTSEEK_HEAD ,
	SCCLLISTSEEK_TAIL ,

} SCCLLISTSEEK ;

typedef struct {

	//	author : Jelo Wang
	//	notes : SCClString
	//	since : 20090816
	//	(c)TOK

	char* data ;
	int length ;
	int last_walker ;
	int add_walker ;
	int get_walker ;

} SCClString ;

typedef struct ILN {

	//	author : Jelo Wang
	//	notes : SLIST
	//	since : 20091123
	//	(c)TOK

	//	data type of element
	int eltype ;
	int element ;
	int totall ;
	
	struct ILN* head ;
	struct ILN* front ;
	struct ILN* next ;
	
} SCClList ;

typedef struct ISTK {

	//	author : Jelo Wang
	//	since : 20090819
	//	(c)TOK
	
	int element ;
	int deep ;
	int number ;
	
	struct ISTK* base ;		
	struct ISTK* next ;

} SCClStack ;

typedef struct {

	//	author : Jelo Wang
	//	since : 20100802
	//	(C)TOK

	SCClList data ;
	SCClList* front ;
	SCClList* rear ;
	
} SCClQueue ;

typedef struct {

	//	author : Jelo Wang
	//	since : 20100831
	//	(C)TOK

	char* name; 
	int id ;
	int degree ;
	int color ;

	int x ; 
	int y ;

	//	neighbor
	SCClList nei ;		
	
} SCClGraphNode ;

typedef struct {

	//	author : Jelo Wang
	//	since : 20100831
	//	(C)TOK

	//	none-direction graph

	//	nodes list
	SCClList nl ;
	//	totall nodes
	int totall ;
	
} SCClGraph ;

# endif

//	generic strings func
extern int sc_is_blank ( unsigned char element ) ;
extern int sc_is_space ( unsigned char element ) ;
extern int sc_is_alpha ( char el ) ;
extern int sc_is_digit ( char el ) ;
extern int sc_is_symbol ( char el ) ;
extern int sc_strlen ( char* str ) ;
extern int sc_strcpy ( char* T , char* S ) ;
extern int sc_strcpy_withlen ( char* T , char* S , int length ) ;
extern int sc_strcmp ( char* T , char* S )  ;
extern int sc_strcmplen ( char* T , char* S ) ;
extern int sc_substrcmp ( char* A , char* B , int start ) ;
extern int sc_strfuzcmp ( char* A , char* B ) ;
extern char* sc_strnew ( char* S ) ;
extern char* sc_strcat ( char* A , char* B ) ;
extern char* sc_substr_with_pos ( char* A , char* B , int pos ) ;
extern char* sc_substr ( char* A , int start , int end ) ;
extern void sc_strtrim ( char* S ) ;
extern int sc_memset ( void* , int , int ) ;
extern void sc_substrcpy ( char* A , char* B , int start ) ;
extern void sc_submemset ( unsigned char* buffer , int data , int offset , int end ) ;
extern void sc_back_insert ( char* path , char* name , int pos ) ;
extern void sc_strinsert ( char* A , char* S , int start ) ;

//	SCClString
extern SCClString* SCClStringNew () ;
extern SCClString* SCClStringCreate ( char* el , int len ) ;
extern void SCClStringRepStr ( SCClString* A , char* S , int start , int len_d ) ;
extern void SCClStringRepStrMulti ( SCClString* A , char* B , char* C ) ;
extern void SCClStringInit ( SCClString* string ) ;
extern void SCClStringInitEx ( SCClString* string , int len ) ;
extern void SCClStringReset ( SCClString* string ) ;
extern void SCClStringAdd ( SCClString* string , char el ) ;
extern int SCClStringAddStr ( SCClString* string , char* el ) ;
extern void SCClStringInsert ( SCClString* A , char* S , int start ) ;
extern void SCClStringInsertAtom ( SCClString* A , char atom , int start , int end ) ;
extern char* SCClStringGetStr ( SCClString* string ) ;
extern void SCClStringDestroy ( SCClString* string ) ;
extern void SCClStringDestroyKernal ( SCClString* string ) ;

//	SCClList
extern void SCClListInit ( SCClList* list ) ;
extern SCClList* SCClListNew () ;
extern void SCClListInsert ( SCClList* listwalker , int el ) ;
extern void SCClListInsertEx ( SCClList* list , int el , int eltype ) ;
extern int SCClListEmpty ( SCClList* list ) ;
extern void SCClListConect ( int N1 , int element , int N3  ) ;
extern int SCClListSetIterator ( int lt , int position ) ;
extern int SCClListIteratorPermit ( int lt ) ;
extern int SCClListIteratorGetElement ( int lt ) ;
extern int SCClListListIteratorNext ( int lt ) ;
extern int SCClListSearchBigestElement ( int lt ) ;
extern void SCClListDeleteBetweenTwo ( int N1 , int N3 ) ;
extern void SCClListDestroy ( SCClList* list ) ;
extern char* SCClItoa ( int number ) ;

//	SCClStack
extern void SCClStackInit ( SCClStack* stack ) ;
extern int SCClStackGet ( SCClStack* stack  ) ;
extern int SCClStackEqual ( SCClStack* stack  , int element ) ;
extern int SCClStackEmpty ( SCClStack* stack ) ;
extern void SCClStackPush ( SCClStack* stack , int element ) ;
extern int SCClStackPop ( SCClStack* stack ) ;
extern int SCClStack_look ( SCClStack* stack , int element ) ;
extern int SCClStackGetDeep ( SCClStack* stack ) ;
extern void SCClStackDestroy ( SCClStack* stack ) ;

//	SCClQueue
extern void SCClQueueInit ( SCClQueue* queue ) ;
extern void SCClQueueEnter ( SCClQueue* queue , int element ) ;
extern int SCClQueueOut ( SCClQueue* queue ) ;
extern int SCClQueueEmpty ( SCClQueue* queue ) ;
extern void SCClQueueDestroy ( SCClQueue* queue ) ;

//	SCClGraph
extern void SCClGraphInit ( SCClGraph* graph ) ;
extern SCClGraph* SCClGraphCreate () ;
extern SCClGraphNode* SCClGraphAddNode ( SCClGraph* graph , int N ) ;
extern SCClGraphNode* SCClGraphSearchNode ( SCClGraph* graph , int N ) ;
extern void SCClGraphAddEdge (  SCClGraph* graph , int N1 , int N2 ) ;
extern int SCClGraphHaveEdge ( SCClGraph* graph , int N1 , int N2 ) ; 
extern int SCClGraphGetDegree ( SCClGraph* graph , int N ) ; 
extern int SCClGraphColoring ( SCClGraph* graph , int totall_colors ) ; 
extern void SCClGraphDelete ( SCClGraph* graph , int N ) ; 
extern void SCClGraphDestroy ( SCClGraph* graph ) ; 
