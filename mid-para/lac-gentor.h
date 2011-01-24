
/*

+	LAC Form Generator , part of SC mid-para

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


# ifndef __LAC_GENTOR__
# define __LAC_GENTOR__
	
# include "symbole.h"
# include "lgnosia.h"
# include "sccl.h"

typedef struct {

	//	author : Jelo Wang
	//	since : 20100107
	//	(C)TOK
	
	int form ;
	int stop ;
	int delt ;
	int label ;
	int lgnosia ;
	SCClList* tree ;
	SCClStack identor ;
	SCClStack deltstack ;
	SCClStack scopestack ;
	SCClStack lgastack ;	
	
} LAC_GENTOR ;

# endif

extern void LACIdentorPush () ;
extern void LACIdentorPop () ;
extern char* gentorlac_get_identor () ;
extern void gentorlac_ready () ;
extern void gentorlac_next () ;
extern void gentorlac_genv () ;
extern void genterlac_gen_funcdef () ;
extern void gentorlac_gen_ifcf ( LGNOSIA* lgnosia , AZONAL* azonal ) ;
extern void lacgentor_gen_else ( LGNOSIA* lgnosia , AZONAL* azonal ) ;
extern void lacgentor_gen_while ( LGNOSIA* lgnosia , AZONAL* azonal ) ;
extern void gentorlac_switcher ( LGNOSIA* lgnosia ) ;
extern void gentorlac_gen_variable ( AZONAL* azonal ) ;
extern int gentorlac_gen_expr ( EXPR* expression , int drop ) ;
extern void gentorlac_disp_expr (  EXPR* expression ) ;
extern char* gentor_lac_run ( char* lacfile ) ;
extern void lacgentor_switcher ( LGNOSIA* lgnosia ) ;
extern void lacgentor_gen_variable ( AZONAL* azonal ) ;
extern void lacgentor_gen_pochain ( LGNOSIA* po_chain ) ;
extern void lacgentor_gen_nechain ( LGNOSIA* ne_chain ) ;

