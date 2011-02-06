
/*

+	LAC Form Generator , part of SC mid-para

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

