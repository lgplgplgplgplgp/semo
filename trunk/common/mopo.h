
/*

+	Semo C()mpiler Compiling-Render ,  part of SC mid-para

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

# ifndef __MOPO
# define __MOPO

enum {

	//	author : Jelo Wang
	//	since : 20100802
	//	(C)TOK

	MOPO_RGB565 ,
	MOPO_RGB888 ,	
	MOPO_RGB8888 ,	
	
} ;

typedef struct {

	//	author : Jelo Wang
	//	since : 20100802
	//	(C)TOK

	unsigned char* buffer ;
	int width ;
	int height ;
	int format ;

} MOPO ;

# endif

extern int MOPOCreatePanel ( int format , int width , int height ) ;
extern int MOPOOutputPanel ( char* path ) ;
extern int MOPODestroyPanel () ;

extern void MOPODrawPixel ( int x , int y , int red , int green , int blue ) ;
extern int MOPODrawLine ( int x1 , int y1 , int x2 , int y2 , int red , int green , int blue ) ;
extern int MOPODrawCircle ( int x , int y , int raduis , int red , int green , int blue ) ;
extern int MOPOFillCircle ( int x , int y , int raduis , int red , int green , int blue ) ;
extern void MOPODrawText ( int x , int y , const char* text , int red , int green , int blue ) ;
extern void MOPODrawRect ( int x , int y , int x_scale , int y_scale , int linewidth , int red , int green , int blue ) ;
extern void MOPOStretch ( int cx , int cy , float radius , int dx , int dy ) ;
extern void MOPODrawGrid ( int red , int green , int blue ) ;

extern void MOPOExpDFSRender ( int exp , int x , int y ) ;
extern int MOPOLgaExpRender ( int lgnosiaa , int anltype , int x , int y ) ;
extern int MOPOCFBFSRender ( int lgnosiaa , int x , int y ) ;
extern int MOPOCFDFSRender ( int lgnosiaa , int type , int x , int y , int fx , int fy , int deep ) ;
extern int MOPOExpBFSRender ( int exp , int x , int y ) ;	
extern void MOPOIGBFSRender ( SCClGraph* graph ) ;

