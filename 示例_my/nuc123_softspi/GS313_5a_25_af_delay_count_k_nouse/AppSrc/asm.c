
// dx:cx = ax*bx
/*-----------------------------------------------------------------------
 * @fucname:	 SMULL
 * @brief: 	 24bit * 24bit -> 48bit with high 16bit in *dx, and low 32bit in *cx 
 * @param: 
 * @return: 
 * @note:		// 24*24 = 48 bit signed and just low 48bits is right
 *-----------------------------------------------------------------------*/
#define ax_high_16bit r0
#define bx_high_16bit r1
#define ax_high_16b r2
#define bx_high_16b r3
#define ax_low_16bit r6
#define bx_low_16bit r7
#define abh	r3
#define bah	r2
#define ahbh	r1
#define blah	r0
#define bhal	r6
#define albl	r7


__ASM __inline volatile void _smlal_head(S32 ax, S32 bx, s32 cx, s32 dx)
{
	mov r8,r4
	mov r11,r5
	mov r12,r6
	//push {r7,lr}
//------------------------------------------------
	uxth ax_low_16bit,r0 	// put ax_low_16bit in r6
	asrs ax_high_16bit,#16	// put ax_high_16bit in r0
	uxth bx_low_16bit,r1 	// put bx_low_16bit in r7
	asrs bx_high_16bit,#16	// put bx_high_16bit in r1
	mov r4,bx_high_16bit
	muls ahbh,ax_high_16bit, bx_high_16bit
	muls blah,bx_low_16bit, ax_high_16bit
	mov r5, ax_low_16bit
	muls bhal,r4,ax_low_16bit
	muls albl,r5,bx_low_16bit
	adds bhal,blah 
	mov blah,bhal
	lsrs bhal,bhal,#16
	lsls r5,blah,#16
	adds albl,r5
	adcs ahbh,bhal
	mov r9, albl
	mov r10, ahbh	
//=======================================
	uxth ax_low_16bit,r2 	// put ax_low_16bit in r6
	asrs ax_high_16b,#16	// put ax_high_16bit in r0
	uxth bx_low_16bit,r3	// put bx_low_16bit in r7
	asrs bx_high_16b,#16	// put bx_high_16bit in r1
	mov r4,bx_high_16b
	muls abh,ax_high_16b, bx_high_16b
	muls bah,bx_low_16bit, ax_high_16b
	mov r5, ax_low_16bit
	muls bhal,r4,ax_low_16bit
	muls albl,r5,bx_low_16bit
	adds bhal,bah 
	mov bah,bhal
	lsrs bhal,bhal,#16
	lsls r5,bah,#16
	adds albl,r5
	adcs abh,bhal
/*---------------------------  ADD ---------------------- */
	mov ax_low_16bit, r9
	mov ax_high_16b, r10
	adds albl,ax_low_16bit
	adcs abh,ax_high_16b
/*--------------------------- ADD -----------------------*/	
	mov r9, albl
	mov r10, abh
	
	mov r4,r8
	mov r5,r11
	mov r6,r12
	bx lr //pop {r7,pc}
}

__ASM __inline volatile void _smlal_middle(S32 ax, S32 bx, s32 cx, s32 dx)
{
	//mov r8,r4
	uxth ax_low_16bit,r0 	// put ax_low_16bit in r6
	asrs ax_high_16bit,#16	// put ax_high_16bit in r0
	uxth bx_low_16bit,r1 	// put bx_low_16bit in r7
	asrs bx_high_16bit,#16	// put bx_high_16bit in r1
	mov r4,bx_high_16bit
	muls ahbh,ax_high_16bit, bx_high_16bit
	muls blah,bx_low_16bit, ax_high_16bit
	mov r5, ax_low_16bit
	muls bhal,r4,ax_low_16bit
	muls albl,r5,bx_low_16bit
	adds bhal,blah 
	mov blah,bhal
	lsrs bhal,bhal,#16
	lsls r5,blah,#16
	adds albl,r5
	adcs ahbh,bhal
/*---------------------------  ADD ---------------------- */
	mov ax_low_16bit, r9
	mov ax_high_16bit, r10
	adds albl,ax_low_16bit
	adcs ahbh,ax_high_16bit
/*--------------------------- ADD -----------------------*/	
	mov r9, albl
	mov r10, ahbh

//====================================
	uxth ax_low_16bit,r2 	// put ax_low_16bit in r6
	asrs ax_high_16b,#16	// put ax_high_16bit in r0
	uxth bx_low_16bit,r3	// put bx_low_16bit in r7
	asrs bx_high_16b,#16	// put bx_high_16bit in r1
	mov r4,bx_high_16b
	muls abh,ax_high_16b, bx_high_16b
	muls bah,bx_low_16bit, ax_high_16b
	mov r5, ax_low_16bit
	muls bhal,r4,ax_low_16bit
	muls albl,r5,bx_low_16bit
	adds bhal,bah 
	mov bah,bhal
	lsrs bhal,bhal,#16
	lsls r5,bah,#16
	adds albl,r5
	adcs abh,bhal
/*---------------------------  ADD ---------------------- */
	mov ax_low_16bit, r9
	mov ax_high_16b, r10
	adds albl,ax_low_16bit
	adcs abh,ax_high_16b
/*--------------------------- ADD -----------------------*/	
	mov r9, albl
	mov r10, abh

	mov r4,r8
	mov r5,r11
	mov r6,r12
	bx lr
	
}

__ASM __inline volatile void _smlal_end(S32 ax, S32 bx, s32 *cx)
{
	//mov r3,r2
	uxth ax_low_16bit,r0 	// put ax_low_16bit in r6
	asrs ax_high_16bit,#16	// put ax_high_16bit in r0
	uxth bx_low_16bit,r1 	// put bx_low_16bit in r7
	asrs bx_high_16bit,#16	// put bx_high_16bit in r1
	mov r4,bx_high_16bit
	muls ahbh,ax_high_16bit, bx_high_16bit
	muls blah,bx_low_16bit, ax_high_16bit
	mov r5, ax_low_16bit
	muls bhal,r4,ax_low_16bit
	muls albl,r5,bx_low_16bit
	adds bhal,blah 
	mov blah,bhal
	lsrs bhal,bhal,#16
	lsls r5,blah,#16
	adds albl,r5
	adcs ahbh,bhal
/*---------------------------  ADD ---------------------- */
	mov ax_low_16bit, r9
	mov ax_high_16bit, r10
	adds albl,ax_low_16bit
	adcs ahbh,ax_high_16bit
/*--------------------------- ADD -----------------------*/	
	lsrs albl, #14	
	lsls ahbh, #18
	orrs albl,ahbh
	//mov r2,r3
	str albl,[r2]

	mov r4,r8
	mov r5,r11
	mov r6,r12
	//sub sp,#8
	//ldr r7,[sp];
	//add sp,#8
	bx lr
}


__ASM __inline volatile void SMLAL(S32 ax, S32 bx, S32 *cx, S32 *dx)
{
	push {r4-r7,lr}
	uxth ax_low_16bit,r0 	// put ax_low_16bit in r6
	asrs ax_high_16bit,#16	// put ax_high_16bit in r0
	uxth bx_low_16bit,r1 	// put bx_low_16bit in r7
	asrs bx_high_16bit,#16	// put bx_high_16bit in r1
	mov r4,bx_high_16bit
	muls ahbh,ax_high_16bit, bx_high_16bit
	muls blah,bx_low_16bit, ax_high_16bit
	mov r5, ax_low_16bit
	muls bhal,r4,ax_low_16bit
	muls albl,r5,bx_low_16bit
	adds bhal,blah 
	mov blah,bhal
	lsrs bhal,bhal,#16
	lsls r5,blah,#16
	adds albl,r5
	adcs ahbh,bhal
/*---------------------------  ADD ---------------------- */
	ldr ax_low_16bit,[r2]
	ldr ax_high_16bit,[r3]
	adds albl,ax_low_16bit
	adcs ahbh,ax_high_16bit
/*--------------------------- ADD -----------------------*/	
	str albl,[r2]
	str ahbh,[r3]
	pop {r4-r7,pc}
}

