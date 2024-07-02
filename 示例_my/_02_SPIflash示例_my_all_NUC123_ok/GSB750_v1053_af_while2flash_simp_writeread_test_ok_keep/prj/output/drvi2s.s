; generated by ARM C/C++ Compiler, 5.02 [Build 28]
; commandline ArmCC [--split_sections --debug -c --asm --info=totals -o.\output\drvi2s.o --depend=.\output\drvi2s.d --cpu=Cortex-M0 --apcs=interwork -I..\CMSIS\CM0\DeviceSupport\Nuvoton\NUC123Series -I..\CMSIS\CM0\CoreSupport -I..\INC\DrvInc -I..\INC\SysInc -IC:\Keil\ARM\RV31\INC -IC:\Keil\ARM\CMSIS\Include -D__MICROLIB --omf_browse=.\output\drvi2s.crf ..\DrvSrc\DrvI2S.c]
        THUMB
        REQUIRE8
        PRESERVE8

        AREA ||i.CLK_GetPLLClockFreq||, CODE, READONLY, ALIGN=2

CLK_GetPLLClockFreq PROC
        PUSH     {r3,lr}
        ADR      r0,|L1.72|
        LDR      r0,[r0,#0]
        LDR      r1,|L1.76|
        STR      r0,[sp,#0]
        LDR      r1,[r1,#0x20]
        MOVS     r0,#0
        MOVS     r2,#5
        LSLS     r2,r2,#16
        TST      r1,r2
        BNE      |L1.68|
        LSLS     r0,r1,#12
        BPL      |L1.30|
        LDR      r0,|L1.80|
        B        |L1.32|
|L1.30|
        LDR      r0,|L1.84|
|L1.32|
        LSLS     r2,r1,#14
        BMI      |L1.68|
        LSLS     r2,r1,#16
        LSRS     r2,r2,#30
        MOV      r3,sp
        LDRB     r2,[r3,r2]
        LSLS     r3,r1,#23
        LSLS     r1,r1,#18
        LSRS     r3,r3,#23
        LSRS     r1,r1,#27
        ADDS     r3,r3,#2
        ADDS     r1,r1,#2
        LSRS     r0,r0,#2
        MULS     r1,r2,r1
        MULS     r0,r3,r0
        BL       __aeabi_uidivmod
        LSLS     r0,r0,#2
|L1.68|
        POP      {r3,pc}
        ENDP

        DCW      0x0000
|L1.72|
        DCB      1,2,2,4
|L1.76|
        DCD      0x50000200
|L1.80|
        DCD      0x01518000
|L1.84|
        DCD      0x00b71b00

        AREA ||i.DrvI2S_DeInit||, CODE, READONLY, ALIGN=2

DrvI2S_DeInit PROC
        LDR      r0,|L2.20|
        MOVS     r2,#1
        LDR      r1,[r0,#8]
        LSLS     r2,r2,#29
        BICS     r1,r1,r2
        STR      r1,[r0,#8]
        LDR      r1,|L2.24|
        LSLS     r0,r0,#18
        STR      r0,[r1,#0]
        BX       lr
        ENDP

|L2.20|
        DCD      0x50000200
|L2.24|
        DCD      0xe000e180

        AREA ||i.DrvI2S_DisableInt||, CODE, READONLY, ALIGN=2

DrvI2S_DisableInt PROC
        LDR      r1,|L3.20|
        LDR      r2,[r1,#8]
        BICS     r2,r2,r0
        STR      r2,[r1,#8]
        MOVS     r0,#1
        LDR      r1,|L3.24|
        LSLS     r0,r0,#27
        STR      r0,[r1,#0]
        BX       lr
        ENDP

        DCW      0x0000
|L3.20|
        DCD      0x401a0000
|L3.24|
        DCD      0xe000e180

        AREA ||i.DrvI2S_EnableInt||, CODE, READONLY, ALIGN=2

DrvI2S_EnableInt PROC
        LDR      r1,|L4.20|
        LDR      r2,[r1,#8]
        ORRS     r2,r2,r0
        STR      r2,[r1,#8]
        MOVS     r0,#1
        LDR      r1,|L4.24|
        LSLS     r0,r0,#27
        STR      r0,[r1,#0]
        BX       lr
        ENDP

        DCW      0x0000
|L4.20|
        DCD      0x401a0000
|L4.24|
        DCD      0xe000e100

        AREA ||i.DrvI2S_GetSourceClockFreq||, CODE, READONLY, ALIGN=2

DrvI2S_GetSourceClockFreq PROC
        PUSH     {r4-r6,lr}
        LDR      r5,|L5.88|
        LDR      r0,[r5,#0x1c]
        LDR      r2,|L5.92|
        LSLS     r1,r0,#30
        LSRS     r1,r1,#30
        BEQ      |L5.46|
        CMP      r1,#1
        BEQ      |L5.50|
        LDR      r0,|L5.96|
        CMP      r1,#2
        BNE      |L5.48|
        LDR      r1,[r5,#0x10]
        LSLS     r4,r1,#29
        LSRS     r4,r4,#29
        BEQ      |L5.56|
        CMP      r4,#2
        BEQ      |L5.60|
        CMP      r4,#1
        BEQ      |L5.60|
        CMP      r4,#3
        BEQ      |L5.72|
        B        |L5.74|
|L5.46|
        MOV      r0,r2
|L5.48|
        POP      {r4-r6,pc}
|L5.50|
        BL       CLK_GetPLLClockFreq
        POP      {r4-r6,pc}
|L5.56|
        MOV      r0,r2
        B        |L5.74|
|L5.60|
        BL       CLK_GetPLLClockFreq
        CMP      r4,#1
        BNE      |L5.74|
        LSRS     r0,r0,#1
        B        |L5.74|
|L5.72|
        LDR      r0,|L5.100|
|L5.74|
        LDR      r1,[r5,#0x18]
        LSLS     r1,r1,#28
        LSRS     r1,r1,#28
        ADDS     r1,r1,#1
        BL       __aeabi_uidivmod
        POP      {r4-r6,pc}
        ENDP

|L5.88|
        DCD      0x50000200
|L5.92|
        DCD      0x00b71b00
|L5.96|
        DCD      0x01518000
|L5.100|
        DCD      0x00002710

        AREA ||i.DrvI2S_Init||, CODE, READONLY, ALIGN=2

DrvI2S_Init PROC
        PUSH     {r4-r6,lr}
        MOV      r4,r0
        LDR      r1,[r0,#0]
        LDR      r0,|L6.136|
        LDR      r2,[r0,#0x1c]
        LSRS     r2,r2,#2
        LSLS     r2,r2,#2
        ORRS     r2,r2,r1
        STR      r2,[r0,#0x1c]
        LDR      r2,[r0,#8]
        LSLS     r1,r0,#20
        ORRS     r2,r2,r1
        STR      r2,[r0,#8]
        MOVS     r0,#5
        LSLS     r0,r0,#28
        LDR      r2,[r0,#0xc]
        ORRS     r2,r2,r1
        STR      r2,[r0,#0xc]
        LDR      r2,[r0,#0xc]
        BICS     r2,r2,r1
        STR      r2,[r0,#0xc]
        LDR      r5,|L6.140|
        LDR      r0,[r5,#0]
        ASRS     r1,r1,#10
        ORRS     r0,r0,r1
        STR      r0,[r5,#0]
        LDR      r0,[r5,#0]
        ASRS     r1,r1,#1
        ORRS     r0,r0,r1
        STR      r0,[r5,#0]
        LDR      r1,[r4,#0x10]
        LDR      r0,[r4,#8]
        LDR      r2,[r4,#0xc]
        ORRS     r0,r0,r1
        LDR      r1,[r4,#0x14]
        ORRS     r1,r1,r2
        ORRS     r0,r0,r1
        LDR      r1,[r4,#0x18]
        ORRS     r0,r0,r1
        LDR      r1,[r4,#0x1c]
        ORRS     r0,r0,r1
        STR      r0,[r5,#0]
        BL       DrvI2S_GetSourceClockFreq
        LDR      r2,[r4,#8]
        LDR      r1,[r4,#4]
        ADDS     r2,r2,#1
        MULS     r1,r2,r1
        LSLS     r1,r1,#4
        BL       __aeabi_uidivmod
        LSRS     r0,r0,#1
        SUBS     r0,r0,#1
        LDR      r1,[r5,#4]
        LSLS     r0,r0,#24
        MOVS     r2,#0xff
        LSLS     r2,r2,#8
        BICS     r1,r1,r2
        LSRS     r0,r0,#16
        ORRS     r1,r1,r0
        STR      r1,[r5,#4]
        LDR      r0,[r5,#0]
        LDR      r1,|L6.144|
        ORRS     r0,r0,r1
        STR      r0,[r5,#0]
        MOVS     r0,#0
        POP      {r4-r6,pc}
        ENDP

        DCW      0x0000
|L6.136|
        DCD      0x50000200
|L6.140|
        DCD      0x401a0000
|L6.144|
        DCD      0x00008001

        AREA ||.arm_vfe_header||, DATA, READONLY, NOALLOC, ALIGN=2

        DCD      0x00000000

        EXPORT DrvI2S_DeInit [CODE]
        EXPORT DrvI2S_DisableInt [CODE]
        EXPORT DrvI2S_EnableInt [CODE]
        EXPORT DrvI2S_GetSourceClockFreq [CODE]
        EXPORT DrvI2S_Init [CODE]

        IMPORT ||Lib$$Request$$armlib|| [CODE,WEAK]
        IMPORT __aeabi_uidivmod [CODE]

        KEEP CLK_GetPLLClockFreq

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
