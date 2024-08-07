; generated by ARM C/C++ Compiler, 5.02 [Build 28]
; commandline ArmCC [--min_array_alignment=4 --split_sections --debug -c --asm --info=totals -o.\output\core_cm0.o --depend=.\output\core_cm0.d --cpu=Cortex-M0 --apcs=interwork -I..\CMSIS\CM0\DeviceSupport\Nuvoton\NUC123Series -I..\CMSIS\CM0\CoreSupport -I..\INC\DrvInc -I..\INC\SysInc -IC:\Keil\ARM\RV31\INC -IC:\Keil\ARM\CMSIS\Include -D__MICROLIB --omf_browse=.\output\core_cm0.crf ..\cmsis\CM0\CoreSupport\core_cm0.c]
        THUMB
        PRESERVE8

        AREA ||.arm_vfe_header||, DATA, READONLY, NOALLOC, ALIGN=2

        DCD      0x00000000

;*** Start embedded assembler ***

#line 1 "..\\cmsis\\CM0\\CoreSupport\\core_cm0.c"
	AREA ||.emb_text||, CODE, READONLY
	THUMB
	EXPORT |__get_PSP|
#line 58
|__get_PSP| PROC
#line 59

 mrs r0, psp
 bx lr
	ENDP
	AREA ||.emb_text||, CODE, READONLY
	THUMB
	EXPORT |__get_SP|
#line 64
|__get_SP| PROC
#line 65

 mov r0,sp 
 bx lr
	ENDP
	AREA ||.emb_text||, CODE, READONLY
	THUMB
	EXPORT |__set_PSP|
#line 79
|__set_PSP| PROC
#line 80

 msr psp, r0
 bx lr
	ENDP
	AREA ||.emb_text||, CODE, READONLY
	THUMB
	EXPORT |__get_MSP|
#line 93
|__get_MSP| PROC
#line 94

 mrs r0, msp
 bx lr
	ENDP
	AREA ||.emb_text||, CODE, READONLY
	THUMB
	EXPORT |__set_MSP|
#line 107
|__set_MSP| PROC
#line 108

 msr msp, r0
 bx lr
	ENDP
	AREA ||.emb_text||, CODE, READONLY
	THUMB
	EXPORT |__REV16|
#line 121
|__REV16| PROC
#line 122

 rev16 r0, r0
 bx lr
	ENDP
	AREA ||.emb_text||, CODE, READONLY
	THUMB
	EXPORT |__REVSH|
#line 135
|__REVSH| PROC
#line 136

 revsh r0, r0
 bx lr
	ENDP

;*** End   embedded assembler ***

        IMPORT ||Lib$$Request$$armlib|| [CODE,WEAK]

        ATTR FILESCOPE
        ATTR SETVALUE Tag_ABI_PCS_wchar_t,2
        ATTR SETVALUE Tag_ABI_enum_size,1
        ATTR SETVALUE Tag_ABI_optimization_goals,3
        ATTR SETSTRING Tag_conformance,"2.06"
        ATTR SETVALUE AV,18,1

        ASSERT {ENDIAN} = "little"
        ASSERT {INTER} = {TRUE}
        ASSERT {ROPI} = {FALSE}
        ASSERT {RWPI} = {FALSE}
        ASSERT {IEEE_FULL} = {FALSE}
        ASSERT {IEEE_PART} = {FALSE}
        ASSERT {IEEE_JAVA} = {FALSE}
        END
