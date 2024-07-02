; generated by ARM C/C++ Compiler, 5.03 [Build 76]
; commandline ArmCC [--debug -c --asm --info=totals -o.\output\system_nuc123series.o --depend=.\output\system_nuc123series.d --cpu=Cortex-M0 --apcs=interwork -O0 -I..\CMSIS\CM0\DeviceSupport\Nuvoton\NUC123Series -I..\CMSIS\CM0\CoreSupport -I..\INC\DrvInc -I..\INC\SysInc -IC:\Keil_v472\ARM\RV31\INC -IC:\Keil_v472\ARM\CMSIS\Include -D__MICROLIB --omf_browse=.\output\system_nuc123series.crf ..\cmsis\CM0\DeviceSupport\Nuvoton\NUC123Series\system_NUC123Series.c]
        THUMB
        REQUIRE8
        PRESERVE8

        AREA ||.text||, CODE, READONLY, ALIGN=2

SystemCoreClockUpdate PROC
        PUSH     {r4-r6,lr}
        BL       CLK_GetPLLClockFreq
        LDR      r1,|L1.184|
        STR      r0,[r1,#0]  ; PllClock
        LDR      r0,|L1.188|
        LDR      r0,[r0,#0x10]
        LSLS     r5,r0,#29
        LSRS     r5,r5,#29
        CMP      r5,#2
        BEQ      |L1.30|
        LSLS     r0,r5,#2
        LDR      r1,|L1.192|
        LDR      r4,[r1,r0]
        B        |L1.34|
|L1.30|
        LDR      r0,|L1.184|
        LDR      r4,[r0,#0]  ; PllClock
|L1.34|
        LDR      r0,|L1.188|
        LDR      r0,[r0,#0x18]
        LSLS     r0,r0,#28
        LSRS     r0,r0,#28
        ADDS     r6,r0,#1
        MOV      r1,r6
        MOV      r0,r4
        BL       __aeabi_uidivmod
        LDR      r1,|L1.196|
        STR      r0,[r1,#0]  ; SystemCoreClock
        LDR      r1,|L1.200|
        LDR      r0,|L1.196|
        LDR      r0,[r0,#0]  ; SystemCoreClock
        ASRS     r2,r1,#1
        ADDS     r0,r0,r2
        BL       __aeabi_uidivmod
        LDR      r1,|L1.204|
        STR      r0,[r1,#0]  ; CyclesPerUs
        POP      {r4-r6,pc}
        ENDP

SystemInit PROC
        BX       lr
        ENDP

CLK_GetPLLClockFreq PROC
        PUSH     {r1-r7,lr}
        ADR      r0,|L1.208|
        LDR      r0,[r0,#0]
        STR      r0,[sp,#0]
        MOVS     r0,#0
        STR      r0,[sp,#8]
        LDR      r0,|L1.188|
        LDR      r4,[r0,#0x20]
        ASRS     r0,r0,#12
        TST      r4,r0
        BNE      |L1.180|
        MOVS     r0,#1
        LSLS     r0,r0,#19
        TST      r4,r0
        BEQ      |L1.112|
        LDR      r0,|L1.212|
        B        |L1.114|
|L1.112|
        LDR      r0,|L1.216|
|L1.114|
        MOV      r5,r0
        MOVS     r0,#1
        LSLS     r0,r0,#17
        TST      r4,r0
        BEQ      |L1.128|
        STR      r5,[sp,#8]
        B        |L1.180|
|L1.128|
        MOVS     r0,#3
        LSLS     r0,r0,#14
        ANDS     r0,r0,r4
        LSRS     r0,r0,#14
        MOV      r1,sp
        LDRB     r7,[r1,r0]
        LSLS     r0,r4,#23
        LSRS     r0,r0,#23
        ADDS     r0,r0,#2
        STR      r0,[sp,#4]
        MOVS     r0,#0x1f
        LSLS     r0,r0,#9
        ANDS     r0,r0,r4
        LSRS     r0,r0,#9
        ADDS     r6,r0,#2
        MOV      r2,r6
        MULS     r2,r7,r2
        MOV      r1,r2
        LSRS     r3,r5,#2
        LDR      r2,[sp,#4]
        MULS     r3,r2,r3
        MOV      r0,r3
        BL       __aeabi_uidivmod
        LSLS     r0,r0,#2
        STR      r0,[sp,#8]
|L1.180|
        LDR      r0,[sp,#8]
        POP      {r1-r7,pc}
        ENDP

|L1.184|
        DCD      PllClock
|L1.188|
        DCD      0x50000200
|L1.192|
        DCD      gau32ClkSrcTbl
|L1.196|
        DCD      SystemCoreClock
|L1.200|
        DCD      0x000f4240
|L1.204|
        DCD      CyclesPerUs
|L1.208|
        DCB      1,2,2,4
|L1.212|
        DCD      0x01518000
|L1.216|
        DCD      0x00b71b00

        AREA ||.arm_vfe_header||, DATA, READONLY, NOALLOC, ALIGN=2

        DCD      0x00000000

        AREA ||.data||, DATA, ALIGN=2

SystemCoreClock
        DCD      0x0464dff8
CyclesPerUs
        DCD      0x00000049
PllClock
        DCD      0x0464dff8
gau32ClkSrcTbl
        DCD      0x00b71b00
        DCD      0x0464dff8
        DCD      0x00002710
        DCD      0x01518000

        EXPORT SystemCoreClockUpdate [CODE]
        EXPORT SystemInit [CODE]
        EXPORT SystemCoreClock [DATA,SIZE=4]
        EXPORT CyclesPerUs [DATA,SIZE=4]
        EXPORT PllClock [DATA,SIZE=4]
        EXPORT gau32ClkSrcTbl [DATA,SIZE=16]

        IMPORT ||Lib$$Request$$armlib|| [CODE,WEAK]
        IMPORT __aeabi_uidivmod [CODE]

        KEEP CLK_GetPLLClockFreq

        ATTR FILESCOPE
        ATTR SETVALUE Tag_ABI_PCS_wchar_t,2
        ATTR SETVALUE Tag_ABI_enum_size,1
        ATTR SETVALUE Tag_ABI_optimization_goals,6
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
