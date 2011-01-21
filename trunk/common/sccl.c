
/*

+	Semo C()mpiler Common Library

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

# include "schal.h"
# include "sccl.h"

int sc_randex ( int bottom , int upper ) {

	//	author : Jelo Wang
	//	since : 20110120
	//	(C)TOK

	int value = rand () % upper ;
 
	while ( value < bottom ) {
		value = rand () % upper ;
	}

	return value ;
	
}

int sc_is_blank ( unsigned char element ) {

	//	author : Jelo Wang
	//	notes : is_blank_token
	//	since : 20090810
	//	(C)TOK

	if ( ' ' == element || '\t'== element ||'\r'== element || '\n'== element )
		return 1;
		return 0;

}

int sc_is_space ( unsigned char element ) {

	//	author : Jelo Wang
	//	notes : is_blank_token
	//	since : 20090828
	//	(C)TOK

	if ( ' ' == element || '\t'== element )
		return 1;
		return 0;

}

int sc_is_alpha ( char el ) {

	//	author : Jelo Wang
	//	since : 20090812
	//	(C)TOK

	return ( ('a' <= el && el <= 'z' ) || ('A' <= el && el <= 'Z' ) ) ? 1 : 0 ;

}

int sc_is_digit ( char el ) {

	//	author : Jelo Wang
	//	since : 20090812
	//	(C)TOK

	return ( ('0' <= el && el <= '9' ) ) ? 1 : 0 ;

}

int sc_is_symbol ( char el ) {

	//	author : Jelo Wang
	//	since : 20090828
	//	(C)TOK

	return ( ( 0x21 <= el && el <= 0x7e ) ) ? 1 : 0 ;

}

int sc_strlen ( char* str ) {

	//	author : Jelo Wang
	//	since : 20091120
	//	(C)TOK
	
	int length = 0 ;

	if ( !str ) return length ;

	for ( ; *str++ ; length ++ ) ;

	return length  ;
	

}

int sc_strcpy ( char* T , char* S ) {

	//	author : Jelo Wang
	//	since : 20091122
	//	(C)TOK

	int walker = 0 ;
	
	if ( !T || !S ) return 0 ;
	
	while ( *S ) *T++ = *S++;

	*T = *S ;
	
	return 1 ;
	

}

int sc_strcpy_ex ( char** T , char* S ) {

	//	author : Jelo Wang
	//	since : 20091122
	//	(C)TOK

	int walker = 0 ;
	char* R = 0 ;
	
	if ( !*T ) *T = (char* ) SCMalloc ( sc_strlen (S)+1 ) ;
	if ( !*T ) return 0 ;
	
	R = *T ;

	while ( *S ) *R++ = *S++;

	*R = *S ;
	
	return 1 ;
	

}

int sc_strcpy_withlen ( char* T , char* S , int length ) {

	//	author : Jelo Wang
	//	since : 20091122
	//	(C)TOK

	int walker = 0 ;

	for ( ; *S && walker < length ; ) {
		*T  ++ = *S ++ ; 
		walker ++ ;
	}

	*T = *S ;
		
	return 1 ;
	

}

int sc_strcmp ( char* T , char* S ) {

	//	author : Jelo Wang
	//	since : 20091128
	//	(C)TOK

	int tlen = sc_strlen (T) ;
	int slen = sc_strlen (S) ;

	if ( tlen != slen ) 
		return 1 ;

	while ( *T && *S ) 
		if ( *T ++ != *S ++ ) 
			return 1 ;
	
	return 0 ;
	

}

int sc_strcmplen ( char* T , char* S ) {

	//	author : Jelo Wang
	//	since : 20100421
	//	(C)TOK

	//	return 0 if T == S
	//	return 1 if len(T) > len(S)
	//	return -1 if len(T) < len(S)

	int tlen = sc_strlen (T) ;
	int slen = sc_strlen (S) ;
	
	if ( tlen < slen ) return -1 ;
	else if ( tlen == slen ) return 0 ;
	else if ( tlen > slen ) return 1 ;
	

}

int sc_memset ( void* memory , int value , int length ) {

	//	author : Jelo Wang
	//	since : 20091128
	//	(C)TOK
	
	unsigned char* buffer = (unsigned char*) memory ; 

	int walker = 0 ;

	for ( walker = 0 ; walker < length ; walker ++ ) {
		buffer [ walker ] = value ;
	}

	return 1 ;
	
}

char* sc_strnew ( char* S ) {


	//	author : Jelo Wang
	//	since : 20100811
	//	(C)TOK

	char* T = 0 ;

	if ( !S ) return 0 ;

	T = (char* ) SCMalloc ( sc_strlen (S)+1 ) ;

	if ( !T ) return 0 ;
	
	sc_strcpy ( T , S ) ;

	return T ;
	
}

char* sc_substr_with_pos ( char* A , char* B , int pos ) {

	//	author : Jelo Wang
	//	since : 20090816
	//	(C)TOK

	//	notes : substr from A at pos , save the results in B

	int len = sc_strlen (A) ;
	int walker = 0 ;
	
	if ( pos >= len )
		return 0 ;

	B = (char* ) SCMalloc ( len ) ; 

	if ( !B ) return 0 ;

	for ( walker = 0  ; pos < len && ( sc_is_alpha(A [ pos ]) || sc_is_digit(A [ pos ])||'_'==A [ pos ]) ; pos ++ , walker ++ ) {

		B [ walker ] = A [ pos ]  ;

	}

	B [ walker ] =  '\0' ;

	return B ;


}
	
	
char* sc_substr ( char* A , int start , int end ) {

	//	author : Jelo Wang
	//	since : 20090824
	//	(C)TOK

	int walker = start;
	int counter = 0 ;

	char* results = (char* ) SCMalloc ( end ) ;

	if ( !results ) return 0 ;

	for ( ; walker < sc_strlen(A) && walker < start + end ; walker ++ , counter ++ )
		results [ counter ] = A[walker];
	
	results [counter] = 0;
	return results ;


}

char* sc_strcat ( char* A , char* B ) {

	//	author : Jelo Wang
	//	since : 20100130
	//	(C)TOK
	
	char* C = 0 ;
	int walker = 0 ;
	int length_of_A = sc_strlen (A) ;
	int length_of_B = sc_strlen (B) ;

	if ( !A || !B ) return 0 ;

	C = (char* ) SCMalloc ( length_of_A + length_of_B + 1 ) ;

	for ( walker = 0 ; walker < length_of_A ; walker ++ ) 
		C [ walker ] = A [ walker ] ;

	for ( ; walker < length_of_A + length_of_B ; walker ++ ) 
		C [ walker ] = B [ walker-length_of_A ] ;
	
	C [ walker ] = '\0' ;

	return C ;
}

void sc_strtrim ( char* S ) {

	// author : Jelo Wang
	// since : 20100311
	// (C)TOK
 
	int len_s = sc_strlen (S) ;
	int s_walker = 0 ;
	int a_walker = 0 ;
	char* A = (char* ) SCMalloc ( len_s + 1 ) ;

	for ( s_walker = 0 ; s_walker < len_s ; s_walker ++ ) {
  
 		if ( 0x20 != S [ s_walker ] && '\t' != S [ s_walker ] ) {
			A [ a_walker ] = S [ s_walker ] ;
			a_walker ++ ;
		}
 
 
	} 
 
	A [ a_walker ] = '\0' ;

	sc_strcpy ( S , A ) ;

}

void sc_substrcpy ( char* A , char* B , int start ) {

	//	author : Jelo Wang
	//	since : 20090812
	// (C)TOK
	
	int walker = start;
	int counter = 0 ;

	for ( ; counter < sc_strlen(B); walker ++ , counter ++ )
		A [ walker ] = B [ counter ] ;


}

int sc_substrcmp ( char* A , char* B , int start ) {

	//	author : Jelo Wang
	//	since : 20090816
	// (C)TOK
	
	///	notes : whole words macthing

	int walker = start;
	int counter = 0 ;
	int length_of_B = sc_strlen (B) ;
	int lenth_of_A = sc_strlen (A) ;

	for ( ; counter < length_of_B; walker ++ , counter ++ ) {
		if ( A [ walker ] != B [ counter ] ) 
			return 1;
	}

	if ( lenth_of_A == length_of_B )
		return 0 ;

	if ( counter < length_of_B )
		return 1 ;

	if ( walker < lenth_of_A ) {
		if ( sc_is_alpha(A [ walker ]) )
			return 1 ;
		if ( sc_is_alpha(A[walker-length_of_B-1]) )
			return 1 ;
	} else if ( walker == lenth_of_A ) {
		
		if ( sc_is_alpha(A[walker-length_of_B-1]) )
			return 1 ;

	}

	return 0;


}

int sc_strfuzcmp ( char* A , char* B ) {

	//	author : Jelo Wang
	//	since : 20100505
	// 	(C)TOK
	
	///	notes : fuzzy compare

	int len_b = sc_strlen (B) ;
	int len_a = sc_strlen (B) ;
	int walker = 0 ;

	if ( len_b > len_a ) return 0 ;

	for ( walker = 0 ; walker < len_b ; ) {
		if ( A[walker] == B[walker] ) walker ++ ;
		else break ;
	}

	if ( walker >= len_b ) return 1 ;
	
	return 0 ;


}

int sc_substr_search ( char* A , char* B ) {

	// AUTHOR : Jelo Wang
 	// since : 20100323
	// (C)TOK
	
 	// search substring B in A

 	int walker_A = 0 ;
 	int walker_B = 0 ; 
 	int len_of_A = sc_strlen (A) ;
 	int len_of_B = sc_strlen (B) ;

 	for ( walker_A = 0 ; walker_A < len_of_A && walker_B < len_of_B ; ) {
		if ( A [ walker_A ] == B [ walker_B ] ) {
   			walker_A ++ ;
   			walker_B ++ ;
  		} else {
   			walker_A ++ ;
   			walker_B = 0 ;
  		}
 	}

 	if ( walker_B == len_of_B ) return 1 ;
 	return 0 ;

}

void sc_submemset ( unsigned char* buffer , int data , int offset , int end ) {

	//	author : Jelo Wang
	//	since : 20090818
	// (C)TOK
	
	//	set buffer with data from offset to end

	int walker ;

	for( walker = offset ; walker < offset + end ; walker ++ )
		buffer [ walker ] = data ;

}

void sc_strinsert ( char* A , char* S , int start ) {

	//	author : Jelo Wang
	//	since : 20110119
	//	(C)TOK

	//	insert S into A from start to sc_strlen(S)
	
	int walker = 0 ;
	int counter = 0 ;
	int move_step = 0 ;
	int move_border = 0 ;

	int len_s = sc_strlen (S) ;
	int A_len = sc_strlen (A) ;

	move_step = len_s ;
	move_border = start ;

	for ( walker = A_len - 1 ; walker >= move_border ; walker -- ) {
		A [ move_step + walker ] = A [ walker ] ;
	}

	for ( counter = start ; counter < start + len_s ; counter ++ ) {
		A [ counter ] = S [ counter - start ] ;
	}

	A [ A_len+len_s ] = '\0' ;

}

void sc_back_insert ( char* path , char* name , int pos ) {

	//	author : Jelo Wang
	//	notes : sc_back_insert
	//	since : 20090831
	//	(C)TOK

	int walker = 0 ;

	for ( walker = pos ; '\0' != name [ walker - pos ]  ; walker ++ ) {
		path [ walker ] = name [ walker - pos ] ;
	}

	path [ walker ] = '\0' ;

}

SCClString* SCClStringNew () {

	//	author : Jelo Wang
	//	since : 20100430
	//	(C)TOK

	return (SCClString* ) SCMalloc ( sizeof(SCClString ) ) ;
	
}

SCClString* SCClStringCreate ( char* el , int len ) {

	//	author : Jelo Wang
	//	since : 20091128
	//	(C)TOK

 	SCClString* string = (SCClString* ) SCMalloc ( sizeof(SCClString) ) ;

	int walker = 0 ;
	
	if ( !string )
		return 0 ;
 
	string->data = (char* ) SCMalloc ( len + 1 ) ; 

	if ( !string->data )
		return 0 ;

	for ( walker = 0 ; walker < len ; walker ++ ) {

		string->data [ walker ] = el [ walker ] ;
			
	}
	
	string->add_walker = len ;
	string->get_walker = 0 ;
	string->last_walker = 0 ;
	string->length = len ;

	return string ;


}

void SCClStringInit ( SCClString* string ) {

	//	author : Jelo Wang
	//	since : 20090816
	//	(C)TOK
	
	if ( 0 < string->data ) SCFree ( string->data ) ;

	string->data = 0 ;
	string->add_walker = 0 ;
	string->get_walker = 0 ;
	string->last_walker = 0 ;
	string->length = 0 ;

}

void SCClStringInitEx ( SCClString* string , int len ) {

	//	author : Jelo Wang
	//	since : 20090816
	//	(C)TOK
	
	if ( 0 < string->data ) SCFree ( string->data ) ;
	
	string->data = (char* ) SCMalloc ( len + 1 ) ; 
	string->add_walker = 0 ;
	string->get_walker = 0 ;
	string->last_walker = 0 ;
	string->length = len ;

}

void SCClStringReset ( SCClString* string ) {

	//	author : Jelo Wang
	//	since : 20091128
	//	(C)TOK

	string->add_walker = 0 ;
	string->get_walker = 0 ;
	string->last_walker = 0 ;
	
}

void SCClStringAdd ( SCClString* string , char el ) {

	//	author : Jelo Wang
	//	since : 20090816
	//	(C)TOK

	if ( 0 < string->add_walker && '\0' == string->data [string->add_walker-1] ) {
		
		string->add_walker = string->add_walker - 1 ;
		string->length = string->length - 1 ;

	} 

	if ( string->add_walker >= string->length ) {

		string->length ++ ;
		
		string->data = (char* ) SCRealloc ( string->data , string->length + 1 ) ;

	}

	string->data [ string->add_walker ] = el ;
	string->add_walker ++ ;

}

int SCClStringAddStr ( SCClString* string , char* el ) {

	//	author : Jelo Wang
	//	since : 20090816
	//	(C)TOK

	int counter = 0 ;
	int ellen = 0 ;
	
	if ( !el ) return 0 ;

	ellen = sc_strlen (el) ;

	if ( 0 == string->length && ellen ) {

		string->data = (char* ) SCMalloc ( ellen + 1 ) ;
		string->length = ellen ;
		string->add_walker = string->length ;
		sc_strcpy ( string->data , el );

		return 1 ;

	} 

	if ( 0 < string->add_walker && '\0' == string->data [string->add_walker-1] ) {
		
		string->add_walker = string->add_walker - 1 ;
		string->length = string->length - 1 ;

	} 


	if ( string->add_walker >= string->length ) {

		string->data = (char* ) SCRealloc ( string->data , string->length + ellen ) ;

		string->length += ellen ;

	}

	for ( counter = 0 ; counter < ellen ; counter ++ ) {
	 
		string->data [ string->add_walker ] = el [ counter ] ;
		string->add_walker ++ ;

	}

}

void SCClStringInsert ( SCClString* A , char* S , int start ) {

	//	author : Jelo Wang
	//	since : 20090830
	//	(C)TOK

	int walker = 0 ;
	int counter = 0 ;
	int move_step = 0 ;
	int move_border = 0 ;

	int len_s = sc_strlen (S) ;
	int A_len = A->length ;

	A->length = A->length + len_s ;
	A->add_walker = A->length ;

	A->data = (char* ) SCRealloc ( A->data ,  A->length + 1 ) ;	

	move_step = len_s ;
	move_border = start ;

	for ( walker = A_len - 1 ; walker >= move_border ; walker -- ) {
		A->data [ move_step + walker ] = A->data [ walker ] ;
	}

	for ( counter = start ; counter < start + len_s ; counter ++ ) {
		A->data [ counter ] = S [ counter - start ] ;
	}

	A->data [ A->add_walker ] = '\0' ;

}

void SCClStringInsertAtom ( SCClString* A , char atom , int start , int end ) {

	//	author : Jelo Wang
	//	since : 20090921
	//	(C)TOK

	int walker = 0 ;
	int counter = 0 ;

	for ( counter = start ; counter < start + end && counter < A->length ; counter ++ ) {
		A->data [ counter ] = atom ;
	}


}

char* SCClStringGetStr ( SCClString* string ) {

	//	author : Jelo Wang
	//	since : 20090816
	//	(C)TOK

	return string->data ;

}
	
void SCClStringRepStr ( SCClString* A , char* S , int start , int len_d ) {

	//	author : Jelo Wang
	//	since : 20090816
	//	updated : 20090827
	//	(C)TOK

	//	notes : replace a substr of B from A with C
	//	notes : if start > sc_strlen(A) the length of A would be realloc 

	int walker = 0 ;
	int counter = 0 ;
	int move_step = 0 ;
	int move_border = 0 ;

	int len_s = sc_strlen ( S ) ;

	if ( len_s > A->length ) {
		
		A->length = A->length + (len_s - len_d) ;
		A->data = SCRealloc ( A->data , A->length ) ;
		
	} else if ( len_s > len_d ) {

		A->length = A->length + len_s - len_d ;
		A->data = SCRealloc ( A->data , A->length + 1 ) ;		

	}

	if ( len_s == len_d ) {
	//	len_s == len_d
	//	step = 1 , border = A -> length (no need to move anything)
		move_step = 0 ;
		move_border = A->length ;
	} else if ( len_s > len_d ) {

	//	len_s > len_d
	//	step = len_s - len_d , border = start + len_d
		move_step = len_s - len_d ;
		move_border = start + len_d ;	
		
		//	the last length of data = A->length-(len_s - len_d)-1
		for ( walker = A->length - (len_s - len_d)-1 ; walker >= move_border ; walker -- ) {
			A->data [ move_step + walker ] = A->data [ walker ] ;
		}
	
		A->add_walker = A->add_walker + move_step ;

		A->data [ A->add_walker ] = '\0' ;	
		
	} else if ( len_d > len_s ) {
	//	len_d > len_s
	//	step = 1 , border = start + len_d	
		move_step = 0 ;
		move_border = start + len_d ;	
	}
	
	if ( len_s == len_d ) {
		for ( counter = start ; counter < start + len_s ; counter ++ ) {
			A->data [ counter ] = S [ counter - start ] ;
		}
	} else if ( len_s > len_d ) {
		for ( counter = start ; counter < start + len_s ; counter ++ ) {
			A->data [ counter ] = S [ counter - start ] ;
		}	
	} else if ( len_d > len_s ) {
		for ( counter = start ; counter < start + len_d ; counter ++ ) {
			if ( counter < start + len_s )
				A->data [ counter ] = S [ counter - start ] ;
			else
				A->data [ counter ] = 0x20 ;
		}	
	}

}

void SCClStringRepStrMulti ( SCClString* A , char* B , char* C ) {

	//	author : Jelo Wang
	//	since : 20090816
	//	(C)TOK

	//	notes : lookup all substr B in A , then replace B with C
	//	input : A = "asdf b sadf asdf bbb sdf b"
	//	input : B = b
	//	input : C = B

	//	output: A = "asdf B sadf asdf bbb sdf B"

	int walker = 0 ;
	int counter = 0 ;

	int len_d = sc_strlen(B) ;

	if ( !B || !C ) return ;

	for ( walker = 0 ; walker < A->length ; walker ++ ) {
		
		if ( !sc_substrcmp ( A->data , B , walker ) ) {

			SCClStringRepStr ( A , C , walker , len_d ) ;

		}

	}


}

void SCClStringDestroy ( SCClString* string ) {

	//	author : Jelo Wang
	//	since : 20090816
	//	(C)TOK

	if ( !string ) return ;
	
  	if ( string->data ) SCFree ( string->data ) ;

	SCFree ( string ) ;

	
}

void SCClStringDestroyKernal ( SCClString* string ) {

	//	author : Jelo Wang
	//	since : 20090816
	//	(C)TOK

	if ( !string ) return ;
	
  	if ( string->data ) SCFree ( string->data ) ;

	
}

void SCClListInit ( SCClList* list ) {

	//	author : Jelo Wang
	//	since : 20091123
	//	(C)TOK

	list->element = 0 ;
	list->totall = 0 ;
	list->front = 0 ;
	list->head = 0 ;
	list->next = 0 ;

}

SCClList* SCClListNew () {

	//	author : Jelo Wang
	//	since : 20102806
	//	(C)TOK

	SCClList* list = (SCClList* ) SCMalloc ( sizeof(SCClList) ) ;

	if ( !list ) return 0 ;
	
	list->element = 0 ;
	list->totall = 0 ;
	list->front = 0 ;
	list->head = 0 ;
	list->next = 0 ;

	return list ;

}

void SCClListInsert ( SCClList* list , int el ) {

	//	author : Jelo Wang
	//	since : 20091123
	//	(C)TOK

	SCClList* listnew = 0 ;

	if ( !list ) return ;

	listnew = (SCClList* ) SCMalloc ( sizeof(SCClList) ) ;

	if( !listnew ) return ;
	
	listnew->element = el ;
	listnew->next = 0 ;
	
	if ( 0 == list->head ) {
		list->head = listnew ;
		listnew->front = list ;
		list->next = listnew ;		
		list->totall = 1 ;
	} else {
		list->next->next = listnew ;
		listnew->front = list->next ;
		list->next = listnew ;		
		list->totall ++ ;
 	}
	
}

void SCClListInsertEx ( SCClList* list , int el , int eltype ) {

	//	author : Jelo Wang
	//	since : 20100718
	//	(C)TOK

	SCClList* listnew = (SCClList* ) SCMalloc ( sizeof(SCClList) ) ;

	if ( !list ) return ;
	else if( !listnew ) return ;

	listnew->eltype = eltype ;	
	listnew->element = el ;
	listnew->next = 0 ;
	
	if ( 0 == list->head ) {
		list->head = listnew ;
		listnew->front = list ; 
		list->next = listnew ;		
		list->totall = 1 ;		
	} else {
		list->next->next = listnew ;
		listnew->front = list->next ;
		list->next = listnew ;
		list->totall ++ ;		
	}
	
}

int SCClListEmpty ( SCClList* list ) {

	//	author : Jelo Wang
	//	since : 20100811
	//	(C)TOK

	if ( !list ) return 1 ;

	if ( !list->head ) return 1 ;

	return 0 ;
	
}

int SCClListSetIterator ( int lt , int position ) {

	//	author : Jelo Wang
	//	since : 20100609
	//	(C)TOK

	//	notes : set an iterator

	SCClList* list = (SCClList* ) lt ;
	SCClList* walker = 0 ;

	if ( 0 == list ) return 0 ;
	
	if ( SCCLLISTSEEK_HEAD == position ) {
		//	将迭代器指向头节点
		list->next = list->head ; 
	} else if ( SCCLLISTSEEK_TAIL == position ) {
		//	将迭代器指向尾节点
		for ( walker = list->head ; walker && walker->next ; walker = walker->next ) ;
		if ( walker ) list->next = walker ;
	}

	return 1 ; 

}

int SCClListIteratorPermit ( int lt ) {

	//	author : Jelo Wang
	//	since : 20100609
	//	(C)TOK

	//	notes : 判断一个迭代器是否可以继续运行

	SCClList* list = (SCClList* ) lt ;
	SCClList* walker = 0 ;

	if ( 0 == list ) return 0 ;
	if ( 0 == list->next ) return 0 ;

	return 1 ; 

}

int SCClListIteratorGetElement ( int lt ) {

	//	author : Jelo Wang
	//	since : 20100609
	//	(C)TOK

	//	notes : get an element

	SCClList* list = (SCClList* ) lt ;

	if ( 0 == list ) return 0 ;
	if ( 0 == list->next ) return 0 ;

	return list->next->element ;

}

int SCClListListIteratorNext ( int lt ) {

	//	author : Jelo Wang
	//	since : 20100609
	//	(C)TOK

	//	notes : iterator next

	SCClList* list = (SCClList* ) lt ;

	if ( 0 == list ) return 0 ;
	if ( 0 == list->next ) return 0 ;

	list->next = list->next->next ; 

	return 1 ;

}

int SCClListSearchBigestElement ( int lt ) {

	//	author : Jelo Wang
	//	since : 20100805
	//	(C)TOK

	int cv = 0 ;
	int lv = 0 ;
	
	SCClList* looper = 0 ;
	
	for ( looper = ((SCClList* )lt)->head ; looper ; looper = looper->next ) {
		cv = looper->element ;
		if ( cv > lv ) lv = cv ;
	}

	return lv ;

}

void SCClListDeleteBetweenTwo ( int N1 , int N3 ) {

	//	author : Jelo Wang
	//	since : 20100811
	//	(C)TOK

	//	notes : delete all of the nodes between N1 to N3
	
	SCClList* N1A = (SCClList* )N1 ;
	SCClList* N3A = (SCClList* )N3 ;

	SCClList* walker = 0 ;
	
	if ( !N1A ) return ;

	for ( walker = N1A->next ; walker && (int)walker != (int)N3A ; ) {	
		N1A->next = walker->next ;
		SCFree ( walker ) ;
		walker = N1A->next ;
	}

	N1A->next = N3A ;
	if ( N3A ) N3A->front = N1A ;	
	
}

void SCClListConect ( int N1 , int element , int N3  ) {

	//	author : Jelo Wang
	//	since : 20100806
	//	(C)TOK

	//	notes : create N2 width element , contect N1 with N2 an N3 like : N1->N2->N3
	//	delete all of the nodes between N1 to N3
	
	SCClList* N1A = (SCClList* )N1 ;
	SCClList* N2A = 0 ;
	SCClList* N3A = (SCClList* )N3 ;

	if ( !N1A ) return ;

	N2A = (SCClList* ) SCMalloc ( sizeof(SCClList) ) ;
	
	if ( !N2A ) return ;
	
	N2A->element = element ;
	N2A->next = N3A ;
	N2A->front = N1A ;

	N1A->next = N2A ;

	if ( N3A ) N3A->front = N2A ;
	
	
}

void SCClListDestroy ( SCClList* list ) {

	//	author : Jelo Wang
	//	since : 20100430
	//	(C)TOK

	SCClList* walker = list->head ;

	while ( walker ) {
		list->next = walker->next ;
		SCFree ( walker ) ;
		walker = list->next ;
	}
	
}

char* SCClItoa ( int number ) {

	//	author : Jelo Wang
	//	since : 20100425
	//	(C)TOK
	
	static char buffer [ 32 ] ;

	itoa ( number , buffer , 10 ) ;

	return buffer ; 
	
}

int SCClAtoi ( char* number ) {

	//	author : Jelo Wang
	//	since : 20100726
	//	(C)TOK
	
	return atoi ( number ) ; 
	
}

void SCClStackInit ( SCClStack* istack ) {

	//	author : Jelo Wang
	//	since : 20090819

	istack->element = 0 ;
	istack->next = 0 ;
	istack->base = 0 ;
	istack->deep = 0 ;
	istack->number = 0 ;
 
}

int SCClStackGet ( SCClStack* istack  ) {

	//	author : Jelo Wang
	//	since : 20090819
	
	if ( istack->next )
		return istack->next->element ;
		return 0 ;
	
}

int SCClStackEqual ( SCClStack* istack  , int element ) {

	//	author : Jelo Wang
	//	since : 20091123
	
	return (istack->next) ? (istack->next->element == element ) : 0 ;
	
}

long int SCClStackEmpty ( SCClStack* istack ) {

	//	author : Jelo Wang
	//	since : 20091123
	
	return (istack->next) ?  0  : 1 ;
	
}

void SCClStackPush ( SCClStack* istack , int eelement ) {

	//	author	: Jelo Wang
	//	since	: 20090819
	
	SCClStack* nnode = 0 ;

	nnode = (SCClStack* ) SCMalloc ( sizeof(SCClStack)  ) ;
	nnode->element = eelement ;
	nnode->next = istack->next  ;
	istack->next = nnode ;
	istack->deep ++ ;
	nnode->number = istack->deep ;

	if ( 0 == istack->base )
		istack->base = nnode ;	
	
	
}

int SCClStackPop ( SCClStack* istack ) {

	//	author	: Jelo Wang
	//	since	: 20090819
	
	long int el = 0 ;
	SCClStack* deln = 0 ;
	
	if ( !istack->next )
		return 0 ;

	el = istack->next->element ;

	deln = istack->next ;

	istack->next = deln->next ;

	SCFree ( deln ) ;

	istack->deep -- ;

	return el ;
	
	
}

int SCClStackLook ( SCClStack* istack , int eelement ) {

	//	author	: Jelo Wang
	//	since	: 20090819
	
	SCClStack* nnode = istack->next ; 

	for ( ; nnode && nnode->element != eelement ; nnode = nnode->next ) ;

	return nnode ? 1 : 0 ;	

}

int SCClStackGetDeep ( SCClStack* istack ) {

	//	author	: Jelo Wang
	//	since	: 20100425

	return istack->deep ;
	
}

void SCClStackDestroy ( SCClStack* istack ) {

	//	author	: Jelo Wang
	//	since	: 20100425

	while ( !SCClStackEmpty (istack) )
		SCClStackPop ( istack ) ;

	istack->base = 0 ;

}

void SCClQueueInit ( SCClQueue* queue ) {

	//	author : Jelo Wang
	//	since : 20100425
	//	(C)TOK

	SCClListInit ( &queue->data ) ;

	queue->front = queue->data.head ;
	queue->rear = queue->data.next ;
	
}

int SCClQueueEmpty ( SCClQueue* queue ) {

	//	author : Jelo Wang
	//	since : 20110119
	//	(C)TOK

	if ( !queue ) return 1 ;
	else if ( !queue->front ) return 1 ;
	else return 0 ;
	
}

void SCClQueueEnter ( SCClQueue* queue , int element ) {
	
	//	author : Jelo Wang
	//	since : 20100425
	//	(C)TOK

	SCClListInsert ( &queue->data , element ) ;

	queue->front = queue->data.head ;
	queue->rear = queue->data.next ;
	
}

int SCClQueueOut ( SCClQueue* queue ) {
	
	//	author : Jelo Wang
	//	since : 20100425
	//	(C)TOK
	
	int node = queue->data.head ;
	int element = 0 ;
	
	if ( !queue->front ) return 0 ;

	element = queue->front->element ;

	queue->data.head = queue->data.head->next ;
	queue->front = queue->data.head ; 	

	SCFree ( (void* )node ) ;
  
	return element ;

}

void SCClQueueDestroy ( SCClQueue* queue ) {
	
	//	author : Jelo Wang
	//	since : 20100425
	//	(C)TOK

	SCClListDestroy ( &queue->data ) ;
	
}

void SCClGraphInit ( SCClGraph* graph ) {

	//	author : Jelo Wang
	//	since : 20100831
	//	(C)TOK

	if ( !graph ) return ;

	SCClListInit ( &graph->nl ) ;

}

SCClGraph* SCClGraphCreate () {

	//	author : Jelo Wang
	//	since : 20100831
	//	(C)TOK
	
	return (SCClGraph* ) SCMalloc ( sizeof( SCClGraph ) ) ; 
	
}

SCClGraphNode* SCClGraphAddNode ( SCClGraph* graph , int N , int handle ) {

	//	author : Jelo Wang
	//	since : 20100831
	//	(C)TOK
	
	SCClGraphNode* node = 0 ;

	if ( !graph ) return 0 ;

	node = SCClGraphSearchNode ( graph , N ) ;

	//	this node is already exits
	if ( node ) return node ;
		
	node = (SCClGraphNode* ) SCMalloc ( sizeof(SCClGraphNode) ) ;
	
	if ( !node ) return 0 ;

	SCClListInit ( &node->nei ) ;

	node->degree = 0 ;
	node->id = N ;
	node->color = -1 ;
	node->handle = handle ;

	SCClListInsert ( &graph->nl , (int)node  ) ;
	graph->totall ++ ;

	return node ;

}

SCClGraphNode* SCClGraphSearchNode ( SCClGraph* graph , int N ) {

	//	author : Jelo Wang
	//	since : 20100831
	//	(C)TOK

	SCClList* llooper = 0 ;
	
	if ( !graph ) return 0 ;

	for ( llooper = graph->nl.head ; llooper ; llooper = llooper->next ) {

		SCClGraphNode* node = (SCClGraphNode* ) llooper->element ;

		if ( !node ) continue ;
		if ( N == node->id ) return node ;
		
	}

	return 0 ;
	
}

void SCClGraphAddEdge (  SCClGraph* graph , int N1 , int N2 ) {

	//	author : Jelo Wang
	//	since : 20100831
	//	(C)TOK

	SCClGraphNode* node1 = 0 ;
	SCClGraphNode* node2 = 0 ;
	
	if ( !graph ) return ;

	node1 = SCClGraphSearchNode ( graph , N1 ) ;

	if ( !node1 ) return ;
	
	node2 = SCClGraphSearchNode ( graph , N2 ) ;

	if ( !node2 ) return ;

	SCClListInsert ( &node1->nei , (int)node2 ) ;
	SCClListInsert ( &node2->nei , (int)node1 ) ;

	node1->degree ++ ;
	node2->degree ++ ;
	
}

int SCClGraphHaveEdge ( SCClGraph* graph , int N1 , int N2 ) {

	//	author : Jelo Wang
	//	since : 20100831
	//	(C)TOK
	
	SCClGraphNode* node1 = 0 ;
	SCClGraphNode* node2 = 0 ;
	
	if ( !graph ) return ;

	node1 = SCClGraphSearchNode ( graph , N1 ) ;

	if ( !node1 ) return ;
	
	node2 = SCClGraphSearchNode ( graph , N2 ) ;

	if ( !node2 ) return ;
	
	SCClListSetIterator ( &node1->nei , SCCLLISTSEEK_HEAD ) ;

	for ( ; SCClListIteratorPermit ( &node1->nei ) ; SCClListListIteratorNext ( &node1->nei ) ) {
		
		SCClGraphNode* node = (SCClGraphNode* )SCClListIteratorGetElement ( &node1->nei ) ;
			
		if ( !node ) continue ;
		if (  N2 == node->id ) return 1 ;
		
	}	

	return 0 ;

}

int SCClGraphGetDegree ( SCClGraph* graph , int N ) {

	//	author : Jelo Wang
	//	since : 20100831
	//	(C)TOK

	SCClGraphNode* node = 0 ;
	
	if ( !graph ) return -1 ;

	node = SCClGraphSearchNode ( graph , N ) ;	

	if ( !node ) return -1 ;

	return node->degree ;
	
}

void SCClGraphDelete ( SCClGraph* graph , int N ) {

	//	author : Jelo Wang
	//	since : 20100831
	//	(C)TOK	

	SCClList* llooper = 0 ;
	
	SCClGraphNode* node = 0 ;
	
	if ( !graph ) return  ;
	
	for ( llooper = graph->nl.head ; llooper ; llooper = llooper->next ) {

		node = (SCClGraphNode* ) llooper->element ;

		if ( !node ) continue ;
		if ( N == node->id ) continue ;
		
	}

	if ( !llooper ) return ;

	//
	//	delete node on graph list
	llooper->front->next = llooper->next ; 
	llooper->next->front = llooper->front ;
	SCFree ( llooper ) ;
	//
	
	SCClListDestroy ( &node->nei ) ;

	SCFree ( node ) ;
	
	
}

static int SCClGraphGetColor ( SCClGraphNode* node , int totall_colors ) {

	//	author : Jelo Wang
	//	since : 20110118
	//	(C)TOK	

	SCClList* looper = 0 ;
	
	//	value of colors bettwen [0,totall_colors)
	int* colors = (int* ) SCMalloc ( sizeof(int)*totall_colors ) ; 
	
	for ( looper = node->nei.head ; looper ; looper = looper->next ) {

		node = (SCClGraphNode* ) looper->element ;

		//	记录邻接点的颜色
		if ( -1 != node->color ) {	
			//	邻接点颜色已被占用
			colors [ node->color ] = 1 ;
		}
		
	}

	{	
		int looper = 0 ;
		for ( looper = 0 ; looper < totall_colors ; looper ++ ) {
			//	顺序取一个没有占用的颜色
			if ( 0 == colors [looper] ) {	
				SCFree ( colors ) ;
				//	return color
				return looper ;
			}
		}
	}

	SCFree ( colors ) ;

	return -1 ;

	
}

int SCClGraphColoring ( SCClGraph* graph , int totall_colors ) {

	//	author : Jelo Wang
	//	since : 20110118
	//	(C)TOK	

	//	当无法染色时返回LAC handle
	//	当然色成功是返回-1

	SCClList* llooper = 0 ;
	SCClList* inlooper = 0 ;
	SCClGraphNode* node = 0 ;
	
	ASSERT(graph) ;
	
	for ( llooper = graph->nl.head ; llooper ; llooper = llooper->next ) {

		node = (SCClGraphNode* ) llooper->element ;

		//	node->handle is a handle of LAC
		if ( node->degree >= totall_colors ) return node->handle ;

		if ( -1 == node->color ) node->color = SCClGraphGetColor ( node , totall_colors ) ;

		//	邻接点
		for ( inlooper = node->nei.head ; inlooper ;  inlooper = inlooper->next ) {

			SCClGraphNode* innode = (SCClGraphNode* ) inlooper->element ;

			//	这个判定不妥，因为度大于等于颜色数不一定是没法染色的
			//	要看有没有回路，只有当所有邻接点颜色都取完后才无法染色
			//	需要改进
			//	node->handle is a handle of LAC
			if ( innode->degree >= totall_colors ) 
				return innode->handle ;
		
			if ( -1 == innode->color ) innode->color = SCClGraphGetColor ( innode , totall_colors ) ;
			
		}
	
						
	}	
	
	return -1 ;
	
}

void SCClGraphDestroy ( SCClGraph* graph ) {

	//	author : Jelo Wang
	//	since : 20100831
	//	(C)TOK	

	if ( !graph ) return ;

	SCClListSetIterator ( &graph->nl , SCCLLISTSEEK_HEAD ) ;

	for ( ; SCClListIteratorPermit ( &graph->nl ) ; SCClListListIteratorNext ( &graph->nl ) ) {
		
		SCClGraphNode* node = (SCClGraphNode* ) SCClListIteratorGetElement ( &graph->nl ) ;
		
		if ( !node ) continue ;

  		SCClListDestroy ( &node->nei ) ;
		SCFree ( node ) ;
		
	}
	
	SCClListDestroy ( &graph->nl ) ;
	
}

