; generated by ARM C/C++ Compiler, 5.02 [Build 28]
; commandline ArmCC [--min_array_alignment=4 --split_sections --debug -c --asm --info=totals -o.\output\drvi2c.o --depend=.\output\drvi2c.d --cpu=Cortex-M0 --apcs=interwork -I..\CMSIS\CM0\DeviceSupport\Nuvoton\NUC123Series -I..\CMSIS\CM0\CoreSupport -I..\INC\DrvInc -I..\INC\SysInc -IC:\Keil\ARM\RV31\INC -IC:\Keil\ARM\CMSIS\Include -D__MICROLIB --omf_browse=.\output\drvi2c.crf ..\DrvSrc\DrvI2C.c]
        THUMB
        REQUIRE8
        PRESERVE8

        AREA ||i.CLK_GetAPBClockFreq||, CODE, READONLY, ALIGN=2

CLK_GetAPBClockFreq PROC
        PUSH     {r4,lr}
        BL       CLK_GetHCLKFreq
        LDR      r1,|L1.20|
        LDR      r1,[r1,#0x2c]
        ADDS     r1,r1,#1
        BL       __aeabi_uidivmod
        POP      {r4,pc}
        ENDP

        DCW      0x0000
|L1.20|
        DCD      0x50000200

        AREA ||i.CLK_GetHCLKFreq||, CODE, READONLY, ALIGN=2

CLK_GetHCLKFreq PROC
        PUSH     {r3-r7,lr}
        LDR      r5,|L2.124|
        LDR      r0,[r5,#0x10]
        LDR      r2,|L2.128|
        LSLS     r4,r0,#29
        LSRS     r4,r4,#29
        BEQ      |L2.44|
        LDR      r3,|L2.132|
        CMP      r4,#2
        BEQ      |L2.48|
        CMP      r4,#1
        BEQ      |L2.48|
        CMP      r4,#3
        BEQ      |L2.118|
        MOV      r0,r3
|L2.30|
        LDR      r1,[r5,#0x18]
        LSLS     r1,r1,#28
        LSRS     r1,r1,#28
        ADDS     r1,r1,#1
        BL       __aeabi_uidivmod
        POP      {r3-r7,pc}
|L2.44|
        MOV      r0,r2
        B        |L2.30|
|L2.48|
        ADR      r0,|L2.136|
        LDR      r0,[r0,#0]
        STR      r0,[sp,#0]
        LDR      r1,[r5,#0x20]
        MOVS     r0,#0
        MOVS     r6,#5
        LSLS     r6,r6,#16
        TST      r1,r6
        BNE      |L2.110|
        LSLS     r0,r1,#12
        BPL      |L2.72|
        MOV      r2,r3
|L2.72|
        MOV      r0,r2
        LSLS     r2,r1,#14
        BMI      |L2.110|
        LSLS     r2,r1,#16
        LSRS     r2,r2,#30
        MOV      r3,sp
        LDRB     r3,[r3,r2]
        LSLS     r2,r1,#23
        LSLS     r1,r1,#18
        LSRS     r2,r2,#23
        LSRS     r1,r1,#27
        ADDS     r2,r2,#2
        ADDS     r1,r1,#2
        LSRS     r0,r0,#2
        MULS     r1,r3,r1
        MULS     r0,r2,r0
        BL       __aeabi_uidivmod
        LSLS     r0,r0,#2
|L2.110|
        CMP      r4,#1
        BNE      |L2.30|
        LSRS     r0,r0,#1
        B        |L2.30|
|L2.118|
        LDR      r0,|L2.140|
        B        |L2.30|
        ENDP

        DCW      0x0000
|L2.124|
        DCD      0x50000200
|L2.128|
        DCD      0x00b71b00
|L2.132|
        DCD      0x01518000
|L2.136|
        DCB      1,2,2,4
|L2.140|
        DCD      0x00002710

        AREA ||i.DrvI2C_Ctrl||, CODE, READONLY, ALIGN=1

DrvI2C_Ctrl PROC
        PUSH     {r4,r5,lr}
        MOVS     r4,#0
        LDR      r5,[sp,#0xc]
        CMP      r1,#0
        BEQ      |L3.12|
        MOVS     r4,#0x20
|L3.12|
        CMP      r2,#0
        BEQ      |L3.20|
        MOVS     r1,#0x10
        ORRS     r4,r4,r1
|L3.20|
        CMP      r3,#0
        BEQ      |L3.28|
        MOVS     r1,#8
        ORRS     r4,r4,r1
|L3.28|
        CMP      r5,#0
        BEQ      |L3.36|
        MOVS     r1,#4
        ORRS     r4,r4,r1
|L3.36|
        LDR      r1,[r0,#0]
        MOVS     r2,#0x3c
        BICS     r1,r1,r2
        ORRS     r1,r1,r4
        STR      r1,[r0,#0]
        POP      {r4,r5,pc}
        ENDP


        AREA ||i.DrvI2C_DeInit||, CODE, READONLY, ALIGN=2

DrvI2C_DeInit PROC
        PUSH     {r4,lr}
        LDR      r1,[r0,#0]
        MOVS     r2,#0x80
        BICS     r1,r1,r2
        STR      r1,[r0,#0]
        LDR      r1,[r0,#0]
        MOVS     r2,#0x40
        BICS     r1,r1,r2
        STR      r1,[r0,#0]
        LDR      r1,|L4.44|
        CMP      r0,r1
        BNE      |L4.28|
        MOVS     r0,#0x12
        B        |L4.36|
|L4.28|
        LDR      r1,|L4.48|
        CMP      r0,r1
        BNE      |L4.40|
        MOVS     r0,#0x13
|L4.36|
        BL       NVIC_DisableIRQ
|L4.40|
        POP      {r4,pc}
        ENDP

        DCW      0x0000
|L4.44|
        DCD      0x40020000
|L4.48|
        DCD      0x40120000

        AREA ||i.DrvI2C_DisableInt||, CODE, READONLY, ALIGN=2

DrvI2C_DisableInt PROC
        PUSH     {r4,lr}
        LDR      r1,[r0,#0]
        MOVS     r2,#0x80
        BICS     r1,r1,r2
        STR      r1,[r0,#0]
        LDR      r1,|L5.36|
        CMP      r0,r1
        BNE      |L5.20|
        MOVS     r0,#0x12
        B        |L5.28|
|L5.20|
        LDR      r1,|L5.40|
        CMP      r0,r1
        BNE      |L5.32|
        MOVS     r0,#0x13
|L5.28|
        BL       NVIC_DisableIRQ
|L5.32|
        POP      {r4,pc}
        ENDP

        DCW      0x0000
|L5.36|
        DCD      0x40020000
|L5.40|
        DCD      0x40120000

        AREA ||i.DrvI2C_EnableInt||, CODE, READONLY, ALIGN=2

DrvI2C_EnableInt PROC
        PUSH     {r4,lr}
        LDR      r1,[r0,#0]
        MOVS     r2,#0x80
        ORRS     r1,r1,r2
        STR      r1,[r0,#0]
        LDR      r1,|L6.36|
        CMP      r0,r1
        BNE      |L6.20|
        MOVS     r0,#0x12
        B        |L6.28|
|L6.20|
        LDR      r1,|L6.40|
        CMP      r0,r1
        BNE      |L6.32|
        MOVS     r0,#0x13
|L6.28|
        BL       NVIC_EnableIRQ
|L6.32|
        POP      {r4,pc}
        ENDP

        DCW      0x0000
|L6.36|
        DCD      0x40020000
|L6.40|
        DCD      0x40120000

        AREA ||i.DrvI2C_GetClockFreq||, CODE, READONLY, ALIGN=1

DrvI2C_GetClockFreq PROC
        PUSH     {r4,lr}
        LDR      r4,[r0,#0x10]
        BL       CLK_GetHCLKFreq
        LSLS     r1,r4,#2
        ADDS     r1,r1,#4
        BL       __aeabi_uidivmod
        POP      {r4,pc}
        ENDP


        AREA ||i.DrvI2C_Init||, CODE, READONLY, ALIGN=2

DrvI2C_Init PROC
        PUSH     {r0-r2,r4-r7,lr}
        MOV      r5,r1
        MOV      r4,r0
        BL       CLK_GetAPBClockFreq
        MOVS     r1,#0xa
        MULS     r0,r1,r0
        LSLS     r1,r5,#2
        BL       __aeabi_uidivmod
        ADDS     r0,r0,#5
        MOVS     r1,#0xa
        BL       __aeabi_uidivmod
        LDR      r6,|L8.120|
        SUBS     r5,r0,#1
        LDR      r7,|L8.124|
        CMP      r4,r6
        BNE      |L8.50|
        ASRS     r0,r4,#21
        BL       SYS_RESET_IP2
        LDR      r0,[r7,#8]
        ASRS     r1,r4,#21
        B        |L8.64|
|L8.50|
        MOVS     r0,#0xff
        ADDS     r0,#1
        BL       SYS_RESET_IP2
        LDR      r0,[r7,#8]
        MOVS     r1,#0xff
        ADDS     r1,#1
|L8.64|
        ORRS     r0,r0,r1
        STR      r0,[r7,#8]
        LDR      r0,[r4,#0x10]
        ORRS     r0,r0,r5
        STR      r0,[r4,#0x10]
        LDR      r0,[sp,#8]
        CMP      r0,#0
        BEQ      |L8.108|
        LDR      r0,[r4,#0]
        MOVS     r1,#0x80
        ORRS     r0,r0,r1
        STR      r0,[r4,#0]
        LDR      r0,|L8.128|
        CMP      r4,r0
        BNE      |L8.98|
        MOVS     r0,#0x12
        B        |L8.104|
|L8.98|
        CMP      r4,r6
        BNE      |L8.108|
        MOVS     r0,#0x13
|L8.104|
        BL       NVIC_EnableIRQ
|L8.108|
        LDR      r0,[r4,#0]
        MOVS     r1,#0x40
        ORRS     r0,r0,r1
        STR      r0,[r4,#0]
        POP      {r1-r7,pc}
        ENDP

        DCW      0x0000
|L8.120|
        DCD      0x40120000
|L8.124|
        DCD      0x50000200
|L8.128|
        DCD      0x40020000

        AREA ||i.DrvI2C_SetAddress||, CODE, READONLY, ALIGN=1

DrvI2C_SetAddress PROC
        CMP      r1,#0
        BEQ      |L9.22|
        CMP      r1,#1
        BEQ      |L9.30|
        CMP      r1,#2
        BEQ      |L9.38|
        CMP      r1,#3
        BEQ      |L9.46|
        MOVS     r0,#0
        MVNS     r0,r0
        BX       lr
|L9.22|
        LSLS     r1,r2,#1
        ORRS     r1,r1,r3
        STR      r1,[r0,#4]
        B        |L9.52|
|L9.30|
        LSLS     r1,r2,#1
        ORRS     r1,r1,r3
        STR      r1,[r0,#0x18]
        B        |L9.52|
|L9.38|
        LSLS     r1,r2,#1
        ORRS     r1,r1,r3
        STR      r1,[r0,#0x1c]
        B        |L9.52|
|L9.46|
        LSLS     r1,r2,#1
        ORRS     r1,r1,r3
        STR      r1,[r0,#0x20]
|L9.52|
        MOVS     r0,#1
        BX       lr
        ENDP


        AREA ||i.DrvI2C_SetAddressMask||, CODE, READONLY, ALIGN=1

DrvI2C_SetAddressMask PROC
        CMP      r1,#0
        BEQ      |L10.22|
        CMP      r1,#1
        BEQ      |L10.28|
        CMP      r1,#2
        BEQ      |L10.34|
        CMP      r1,#3
        BEQ      |L10.40|
        MOVS     r0,#0
        MVNS     r0,r0
        BX       lr
|L10.22|
        LSLS     r1,r2,#1
        STR      r1,[r0,#0x24]
        B        |L10.44|
|L10.28|
        LSLS     r1,r2,#1
        STR      r1,[r0,#0x28]
        B        |L10.44|
|L10.34|
        LSLS     r1,r2,#1
        STR      r1,[r0,#0x2c]
        B        |L10.44|
|L10.40|
        LSLS     r1,r2,#1
        STR      r1,[r0,#0x30]
|L10.44|
        MOVS     r0,#0
        BX       lr
        ENDP


        AREA ||i.NVIC_DisableIRQ||, CODE, READONLY, ALIGN=2

NVIC_DisableIRQ PROC
        LSLS     r1,r0,#27
        LSRS     r1,r1,#27
        MOVS     r0,#1
        LSLS     r0,r0,r1
        LDR      r1,|L11.16|
        STR      r0,[r1,#0]
        BX       lr
        ENDP

        DCW      0x0000
|L11.16|
        DCD      0xe000e180

        AREA ||i.NVIC_EnableIRQ||, CODE, READONLY, ALIGN=2

NVIC_EnableIRQ PROC
        LSLS     r1,r0,#27
        LSRS     r1,r1,#27
        MOVS     r0,#1
        LSLS     r0,r0,r1
        LDR      r1,|L12.16|
        STR      r0,[r1,#0]
        BX       lr
        ENDP

        DCW      0x0000
|L12.16|
        DCD      0xe000e100

        AREA ||i.SYS_RESET_IP2||, CODE, READONLY, ALIGN=1

SYS_RESET_IP2 PROC
        MOVS     r1,#5
        LSLS     r1,r1,#28
        LDR      r2,[r1,#0xc]
        ORRS     r2,r2,r0
        STR      r2,[r1,#0xc]
        LDR      r2,[r1,#0xc]
        BICS     r2,r2,r0
        STR      r2,[r1,#0xc]
        BX       lr
        ENDP


        AREA ||.arm_vfe_header||, DATA, READONLY, NOALLOC, ALIGN=2

        DCD      0x00000000

        EXPORT CLK_GetAPBClockFreq [CODE]
        EXPORT DrvI2C_Ctrl [CODE]
        EXPORT DrvI2C_DeInit [CODE]
        EXPORT DrvI2C_DisableInt [CODE]
        EXPORT DrvI2C_EnableInt [CODE]
        EXPORT DrvI2C_GetClockFreq [CODE]
        EXPORT DrvI2C_Init [CODE]
        EXPORT DrvI2C_SetAddress [CODE]
        EXPORT DrvI2C_SetAddressMask [CODE]

        IMPORT ||Lib$$Request$$armlib|| [CODE,WEAK]
        IMPORT __aeabi_uidivmod [CODE]

        KEEP CLK_GetHCLKFreq
        KEEP NVIC_DisableIRQ
        KEEP NVIC_EnableIRQ
        KEEP SYS_RESET_IP2

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
