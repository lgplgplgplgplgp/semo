
/*

+	Grammar of LAC , SC mid-para

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


# ifndef __LAC_GRAMMAR_
# define __LAC_GRAMMAR_

# define LAC_FEN					2    //;
# define LAC_EQU					3    //=
# define LAC_ZKL 					4    //[
# define LAC_ZKR 				5    //]
# define LAC_XKL 					6    //(
# define LAC_XKR 				7    //)
# define LAC_DKL 				8    //{
# define LAC_DKR 				9    //}
# define LAC_DOU 				10   //,
# define LAC_XY  					11   //'
# define LAC_DY  					12   //"
# define LAC_ZSL 					13   ///*
# define LAC_ZSR 				14   //*/
# define LAC_CHA 				15   //char
# define LAC_STR 				16   //string
# define LAC_MAO 				17   //:
# define LAC_ASK 				18   //?

# define LAC_JAA 				19   //++
# define LAC_JNN 				20   //--
# define LAC_TAN 				21   //!
# define LAC_SUB 				22   //-
# define LAC_ADD 				23   //+
# define LAC_HU  					24   //|
# define LAC_MUL 				25   //*
# define LAC_DIV 					26   ///
# define LAC_MOD 				27   //%
# define LAC_SHR 				28	 //>>
# define LAC_SHL 				29   //<<
# define LAC_LT  					30   //<
# define LAC_LE  					31   //<=
# define LAC_EQ  					32   //==
# define LAC_NE  					33   //!=
# define LAC_GT  					34   //>
# define LAC_GE  					35   //>=
# define LAC_EE  					36   //=
# define LAC_AND 				37   //&&
# define LAC_OR  					38   //||
# define LAC_HE  					39   //&
# define LAC_EXO 				40   //~
# define LAC_BEY 				41   //^=
# define LAC_BEO 				42   //|=
# define LAC_BEA 				43   //&=
# define LAC_BEB 				44   //%=
# define LAC_SLD 				45   ///=
# define LAC_SLM 				46   //*=
# define LAC_SLS 					47   //-=
# define LAC_SLA 				48   //+=
# define LAC_SHLL 				49  //<<=
# define LAC_SHRR 				50  //>>=
# define LAC_PNT 				51   //->
# define LAC_FIL 					52   //.
# define LAC_PPT 				53   //**
# define LAC_YHH 				54   //^
# define LAC_ASC 				55   //ASCII

# define LAC_BREAK       			1001
# define LAC_CASE        			1002
# define LAC_FOR         			1003
# define LAC_CONTINUE    			1004
# define LAC_GOTO        			1005
# define LAC_DEFAULT     			1006
# define LAC_DO          			1007
# define LAC_WHILE      		 	1008
# define LAC_ELSE        			1009
# define LAC_SWITCH      			1010
# define LAC_IF          				1011
# define LAC_RVALUE				1012
# define LAC_REGISTER          		1013
# define LAC_EXTERN	          		1015
# define LAC_RETURN	          		1016
# define LAC_SIZEOF	          		1017
# define LAC_STATIC	          		1018
# define LAC_VOLATILE	          	1019

# define LAC_INTNUM      			2100 //integer
# define LAC_FLTNUM      			2101 //float
# define LAC_HEXNUM      			2102 //hex
# define LAC_FLTENUM     			2103 //float e
# define LAC_INTENUM     			2104 //int e
# define LAC_MUS_INTNUM      		2105 //integer
# define LAC_MUS_FLTNUM      		2106 //float
# define LAC_MUS_HEXNUM      		2107 //hex
# define LAC_MUS_FLTENUM     		2108 //float e
# define LAC_MUS_INTENUM     		2109 //int e
# define LAC_FUNCDEF				2033
# define LAC_VAR         				2034 //general variables name
# define LAC_VARDEF         				2035 //general variables name
# define LAC_ARRAY      		 		2036
# define LAC_FUNCCAL     			2037 //function call
# define LAC_LABEL     				2038 //label
# define LAC_VREG					2040
	
# define LAC_EOS         			'\0' //ending of string
# define LAC_ENTER				2515
# define LAC_ESCAPE				2516
# define LAC_CHROW				2517
# define LAC_SPACE				2518
# define LAC_TABLEK				2519

# endif

