/****************************************************************************
 * @file:		filter.h
 * @version:	V1.00	
 * @breif:		filter header file
 * @date: 	2013.4.12	
 * 
 * @note 		
 *  Copyright (C) 2013-2015 Global Silicon . All rights reserved.
 * 
 ****************************************************************************/
#ifndef __FILTER_H__
#define __FILTER_H__

#define	IIR16_ModuleX2(x1, x2, x3, s, y2, y3, y22, y23, k1, k2, k3, k4, k5, Rshift) 	{ \
			x1 = s;\
			s = x1*((k1<<16)>>16); \
			s += x2*(k1>>16); \
			s += x3*((k2<<16)>>16); \
			s += y2*(k2>>16); \
			s += y3*((k3<<16)>>16); \
			s >>= Rshift; \
			x3 = x2; \
			x2 = x1; \
			x1 = s;\
			s = x1*(k3>>16);\
			s += y2*((k4<<16)>>16);\
			s += y3*(k4>>16);\
			y3 = y2; \
			y2 = x1; \
			s += y22*((k5<<16)>>16);\
			s += y23*(k5>>16);\
			s >>= Rshift;\
			y23 = y22;\
			y22 = s;\
		}


#define	IIR16s_Module(x1, x2, x3, s, temp1, temp2, y2, k1, k2, k3, Rshift) 	{ \
			y2 = s;\
			temp1 = s;\
			s = s*((k1<<16)>>16); \
			temp2 = (x1<<16);\
			s += (temp2>>16)*(k1>>16); \
			s += (x1>>16)*((k2<<16)>>16); \
			x1 = temp2 + (temp1&0xffff);\
			temp2 = (x2<<16);\
			s += ((x2<<16)>>16)*(k2>>16); \
			s += (x2>>16)*((k3<<16)>>16); \
			s >>= Rshift; \
			temp1 = s;\
			x2 = temp2 + (temp1&0xffff);\
			s = y2;\
		}

// input should be 24bit
			//_smlal_head(x1,k1,x2,k2);
			//_smlal_middle(x3,k3,y2,k4);
			//_smlal_end(y3,k5,&sumL);
#define  IIR48_Module(x1, x2, x3, sum, y2, y3, k1, k2, k3, k4, k5, Rshift){\
			x1 = sum;\
			_smlal_head(x1,k1,x2,k2);\
			_smlal_middle(x3,k3,y2,k4);\
			_smlal_end(y3,k5,&sum);\
			x3 = x2; \
			x2 = x1; \
			y3 = y2; \
			y2 = sum>>8; \
		}

// input should be 24bit.
#define	IIR24_24_Module(x1, x2, x3, sumL, sumH, y2, y3, k1, k2, k3, k4, k5, Rshift) 	{ \
			x1 = sumL;\
			sumL= sumH = 0;\
			SMLAL(x1,k1,&sumL,&sumH);\
			SMLAL(x2,k2,&sumL,&sumH);\
			SMLAL(x3,k3,&sumL,&sumH);\
			SMLAL(y2,k4,&sumL,&sumH);\
			SMLAL(y3,k5,&sumL,&sumH);\
			sumH <<= (32-Rshift);\
			sumL = (((u32)sumL) >> Rshift); \
			sumL |= sumH;\
			x3 = x2; \
			x2 = x1; \
			y3 = y2; \
			y2 = sumL>>8; \
		}


#define	IIR32_32_Module(x1, x2, x3, sumL, sumH, y2, y3, k1, k2, k3, k4, k5, Rshift) 	{ \
			x1 = sumL;\
			sumL= sumH = 0;\
			SMLAL(x1,k1,&sumL,&sumH);\
			SMLAL(x2,k2,&sumL,&sumH);\
			SMLAL(x3,k3,&sumL,&sumH);\
			SMLAL(y2,k4,&sumL,&sumH);\
			SMLAL(y3,k5,&sumL,&sumH);\
			sumH <<= (32-Rshift);\
			sumL = (((u32)sumL) >> Rshift); \
			sumL |= sumH;\
			x3 = x2; \
			x2 = x1; \
			y3 = y2; \
			y2 = sumL; \
		}

#define	HPF_FILTER1(x_in ,x_hpL, x_hpH, y_hpL, y_hpH, acc_L, acc_H, K_HP)	{\
			acc_L -= x_hpL;\
			x_hpL = x_in;\
			acc_L += x_in;\
			acc_L -= K_HP*y_hpL;\
			y_hpL = acc_L>>15;\
			acc_H -= x_hpH;\
			x_hpH = (y_hpL<<15);\
			acc_H += x_hpH;\
			acc_H -= K_HP*y_hpH;\
			y_hpH = acc_H>>15;\
			x_in = y_hpH;\
		}//x_in = y_hpH;\

#define	HPF_FILTER2(x_in ,x_hp, y_hp, K_HP)	{\
			x_in -= x_hp; \
			x_hp = x_in; \
			y_hp *= K_HP; \
			y_hp >>= 16; \
			y_hp += x_in; \
			x_in = y_hp; \
		}



#define L_IIR32(n,sum,Rshift) IIR48_Module(x##n##_L1, x##n##_L2, x##n##_L3, sum,  y##n##_L2, y##n##_L3,\
							F_L_K##n##1, F_L_K##n##2, F_L_K##n##3, F_L_K##n##4, F_L_K##n##5, Rshift)

#define R_IIR32(n,sum,Rshift) IIR48_Module(x##n##_R1, x##n##_R2, x##n##_R3, sum,   y##n##_R2, y##n##_R3,\
							F_L_K##n##1, F_L_K##n##2, F_L_K##n##3, F_L_K##n##4, F_L_K##n##5, Rshift)

#define L_IIR(n,m,sum,Rshift) IIR16_ModuleX2(temp1, x##n##_L2, x##n##_L3, sum,  y##n##_L2, y##n##_L3, y##m##_L2, y##m##_L3,\
							F_L_K##n##1, F_L_K##n##2, F_L_K##n##3, F_L_K##n##4, F_L_K##n##5, Rshift)

#define R_IIR(n,m,sum,Rshift) IIR16_ModuleX2(temp1, x##n##_R2, x##n##_R3, sum, y##n##_R2, y##n##_R3, y##m##_R2, y##m##_R3,\
							F_L_K##n##1, F_L_K##n##2, F_L_K##n##3, F_L_K##n##4, F_L_K##n##5, Rshift)

#define L_IIR_S(n,sum,Rshift) IIR16s_Module(x##n##_L1, x##n##_L2, x##n##_L3, sum, temp1, temp2,\
							y##n##_L2, F_L_K##n##1, F_L_K##n##2, F_L_K##n##3, Rshift)

#define R_IIR_S(n,sum,Rshift) IIR16s_Module(x##n##_R1, x##n##_R2, x##n##_R3, sum, temp1, temp2,\
							y##n##_R2, F_L_K##n##1, F_L_K##n##2, F_L_K##n##3, Rshift)

#define HPF(n,sum)		HPF_FILTER1(sum, x##n##_hpL, x##n##_hpH, y##n##_hpL,  y##n##_hpH,\
							acc##n##_L, acc##n##_H, K##n##_HP)						


extern void triggerFilter(void);
extern void filter(void);
extern void filterInit(void);
extern s32 limiter( s32 data);

#endif

