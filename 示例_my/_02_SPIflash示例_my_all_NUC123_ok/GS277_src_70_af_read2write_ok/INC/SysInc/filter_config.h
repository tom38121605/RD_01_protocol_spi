#ifndef __filter_config_h__
#define  __filter_config_h__
#if 0
// input over -6db will ouput error, and no any inc and dec
#define F_L_K11	0x3f13
#define F_L_K12	0xffff8322
#define F_L_K13	0x3ede
#define F_L_K14	0x7cde  
#define F_L_K15	0xffffc20f    



// SHIFT=14, F70_BH_5DB-.
#define F_L_K21	 0x2029     
#define F_L_K22	 0xffffc046 
#define F_L_K23	 0X1f92     
#define F_L_K24	 0x7f29     
#define F_L_K25	 0Xffffc0d6 


// SHIFT=14, F100_BL80_4DB+.
#define F_L_K31	 0x4034     
#define F_L_K32	 0xffff806c 
#define F_L_K33	 0X3f61     
#define F_L_K34	 0x7f94     
#define F_L_K35	 0Xffffc06b 


// SHIFT=14, F1700_BC1000_4DB-.
#define F_L_K41	 0x37FA     
#define F_L_K42	 0xffffA981 
#define F_L_K43	 0X20CD     
#define F_L_K44	 0x5673     
#define F_L_K45	 0XffffE72C 
                              
#define F_L_K51	0x282C
#define F_L_K52	0xffffB193
#define F_L_K53	0X2657
#define F_L_K54	0x3E48
#define F_L_K55	0XffffE1A2                              
                              
/*
// input over -6db will ouput error, and no any inc and dec
#define F_L_K21	0x3fac
#define F_L_K22	0xffff80a7
#define F_L_K23	0x3fac
#define F_L_K24	0x7f59
#define F_L_K25	0xffffC0a6


// input can't over -9db, and have a F55_B100_6DB+
#define F_L_K31	0x3fbf
#define F_L_K32	0xffff8081
#define F_L_K33	0X3fbf
#define F_L_K34	0x7f7e
#define F_L_K35	0Xffffc081


// input can't over -9db, and have a F2500_B3000_3DB+
#define F_L_K41	0x3ff6
#define F_L_K42	0xffff8038
#define F_L_K43	0X3fd6
#define F_L_K44	0x7fc8
#define F_L_K45	0Xffffc034


//F6000_B6000 high pass WHEN R_SHIFT 14
//no any inc or dec when r_shift 13
#define F_L_K51	0x282C
#define F_L_K52	0xffffB193
#define F_L_K53	0X2657
#define F_L_K54	0x3E48
#define F_L_K55	0XffffE1A2


//F2600_B200_10db-
#define F_L_K61	0x3e96
#define F_L_K62	0xffff8b3e
#define F_L_K63	0X3d47
#define F_L_K64	0x74c2
#define F_L_K65	0Xffffc423


//no any inc or dec when r_shift 14
#define F_L_K71	0x40ba
#define F_L_K72	0xffffc2cf
#define F_L_K73	0X39a9
#define F_L_K74	0x3d31
#define F_L_K75	0Xffffc59D
      
// input can't over -9db, and have a F5000_B3000_3DB-  low pass
#define F_L_K81	0x2f00
#define F_L_K82	0xffffbba6
#define F_L_K83	0X1afa
#define F_L_K84	0x627a
#define F_L_K85	0Xffffd7e4


//F7000_low pass when r_shift 16     
#define F_L_K91	0x7c7c
#define F_L_K92	0x54e5
#define F_L_K93	0X615a
#define F_L_K94	0xffffab1b
#define F_L_K95	0Xffffa229     


//F7000_low pass when r_shift 16
#define F_L_K101	0x2fc5
#define F_L_K102	0x5f8a
#define F_L_K103	0X2fc5
#define F_L_K104	0xa752
#define F_L_K105	0Xffffd99a   

*/
// input over -6db will ouput error, and no any inc and dec
#define F_R_K11	0x7fff    	     //0x7f59      //
#define F_R_K12	0x21		    //0xffff80a7  //  
#define F_R_K13	0X61             //0x7f59      //  
#define F_R_K14	0x34             //0x7f59      //  
#define F_R_K15	0X23		   //0xffff814c  //  
// input over -6db will ouput error, and no any inc and dec
#define F_R_K21	0x3fac
#define F_R_K22	0xffff80a7
#define F_R_K23	0X3fac
#define F_R_K24	0x7f59
#define F_R_K25	0XffffC0A6


// input can't over -9db, and have a F55_B100_6DB+
#define F_R_K31	0x3fbf
#define F_R_K32	0xffff8081
#define F_R_K33	0X3fbf
#define F_R_K34	0x7f7e
#define F_R_K35	0Xffffc081


// input can't over -9db, and have a F2500_B3000_3DB+
#define F_R_K41	0x3ff6
#define F_R_K42	0xffff8038
#define F_R_K43	0X3fd6
#define F_R_K44	0x7fc8
#define F_R_K45	0Xffffc034


//F6000_B6000 high pass WHEN R_SHIFT 14
//no any inc or dec when r_shift 13
#define F_R_K51	0x282C
#define F_R_K52	0xffffB193
#define F_R_K53	0X2657
#define F_R_K54	0x3E48
#define F_R_K55	0XffffE1A2

/*
//F2600_B200_10db-
#define F_R_K61	0x3e96
#define F_R_K62	0xffff8b3e
#define F_R_K63	0X3d47
#define F_R_K64	0x74c2
#define F_R_K65	0Xffffc423


//no any inc or dec when r_shift 14
#define F_R_K71	0x40ba
#define F_R_K72	0xffffc2cf
#define F_R_K73	0X39a9
#define F_R_K74	0x3d31
#define F_R_K75	0Xffffc59D
      
// input can't over -9db, and have a F5000_B3000_3DB-  low pass
#define F_R_K81	0x2f00
#define F_R_K82	0xffffbba6
#define F_R_K83	0X1afa
#define F_R_K84	0x627a
#define F_R_K85	0Xffffd7e4


//F7000_low pass when r_shift 16     
#define F_R_K91	0x7c7c
#define F_R_K92	0x54e5
#define F_R_K93	0X615a
#define F_R_K94	0xffffab1b
#define F_R_K95	0Xffffa229     


//F7000_low pass when r_shift 16
#define F_R_K101	0x2fc5
#define F_R_K102	0x5f8a
#define F_R_K103	0X2fc5
#define F_R_K104	0xa752
#define F_R_K105	0Xffffd99a   


//F1000_B100_10db
#define F_L_K11	0x3f13
#define F_L_K12	0xffff8322
#define F_L_K13	0x3ede
#define F_L_K14	0x7cde  
#define F_L_K15	0xffffc20f                                         



#define F_R_K11	0x3f13
#define F_R_K12	0xffff8322
#define F_R_K13	0x3ede
#define F_R_K14	0x7cde  
#define F_R_K15	0xffffc20f


//F5000_B2000_10db
#define F_L_K21	0x3b40
#define F_L_K22	0xffffA57a
#define F_L_K23	0x36db
#define F_L_K24	0x5a86
#define F_L_K25	0xffffCde5


#define F_R_K21	0x3b40
#define F_R_K22	0xffffA57a
#define F_R_K23	0x36db
#define F_R_K24	0x5a86
#define F_R_K25	0xffffCde5


//F500_B100_6db
#define F_L_K31	0x3fca
#define F_L_K32	0xffff811b
#define F_L_K33	0x3f60
#define F_L_K34	0x7ee5
#define F_L_K35	0xffffC0d5

#define F_R_K31	0x3fca
#define F_R_K32	0xffff811b
#define F_R_K33	0x3f60
#define F_R_K34	0x7ee5
#define F_R_K35	0xffffC0d5

//F10000_B100_6db
#define F_L_K41	0x3fd8
#define F_L_K42	0xffffDf08
#define F_L_K43	0x3f89
#define F_L_K44	0x20f8
#define F_L_K45	0xffffC09e

#define F_R_K41	0x3fd8
#define F_R_K42	0xffffDf08
#define F_R_K43	0x3f89
#define F_R_K44	0x20f8
#define F_R_K45	0xffffC09e 


#define F_L_K51	0x3fd8
#define F_L_K52	0xffffDf08
#define F_L_K53	0x3f89
#define F_L_K54	0x20f8
#define F_L_K55	0xffffC09e

#define F_R_K51	0x3fd8
#define F_R_K52	0xffffDf08
#define F_R_K53	0x3f89
#define F_R_K54	0x20f8
#define F_R_K55	0xffffC09e


#define F_L_K61	0X7fff//0x3f13
#define F_L_K62	0X0//0xffff8322
#define F_L_K63	0X0//0x3ede
#define F_L_K64	0X0//0x7cde
#define F_L_K65	0X0//0xffffc20f

#define F_R_K61	0X3E96//0x3f13
#define F_R_K62	0XFFFF8B3E//0xffff8322
#define F_R_K63	0X3D47//0x3ede
#define F_R_K64	0X74C2//0x7cde
#define F_R_K65	0XFFFFC423//0xffffc20f
*/

#endif




#endif


