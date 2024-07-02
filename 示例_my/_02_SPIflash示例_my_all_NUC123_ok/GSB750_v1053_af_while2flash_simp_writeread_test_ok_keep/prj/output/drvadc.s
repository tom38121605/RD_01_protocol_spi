; generated by ARM C/C++ Compiler, 5.02 [Build 28]
; commandline ArmCC [--split_sections --debug -c --asm --info=totals -o.\output\drvadc.o --depend=.\output\drvadc.d --cpu=Cortex-M0 --apcs=interwork -I..\CMSIS\CM0\DeviceSupport\Nuvoton\NUC123Series -I..\CMSIS\CM0\CoreSupport -I..\INC\DrvInc -I..\INC\SysInc -IC:\Keil\ARM\RV31\INC -IC:\Keil\ARM\CMSIS\Include -D__MICROLIB --omf_browse=.\output\drvadc.crf ..\DrvSrc\DrvADC.c]
        THUMB
        REQUIRE8
        PRESERVE8

        AREA ||i.ADC_CMP_ENABLE||, CODE, READONLY, ALIGN=2

ADC_CMP_ENABLE PROC
        LSLS     r1,r0,#2
        LDR      r0,|L1.16|
        ADDS     r0,r1,r0
        LDR      r1,[r0,#0x28]
        MOVS     r2,#1
        ORRS     r1,r1,r2
        STR      r1,[r0,#0x28]
        BX       lr
        ENDP

|L1.16|
        DCD      0x400e0000

        AREA ||i.ADC_CMP_INT_DISABLE||, CODE, READONLY, ALIGN=2

ADC_CMP_INT_DISABLE PROC
        LSLS     r1,r0,#2
        LDR      r0,|L2.16|
        ADDS     r0,r1,r0
        LDR      r1,[r0,#0x28]
        MOVS     r2,#2
        BICS     r1,r1,r2
        STR      r1,[r0,#0x28]
        BX       lr
        ENDP

|L2.16|
        DCD      0x400e0000

        AREA ||i.ADC_CMP_INT_ENABLE||, CODE, READONLY, ALIGN=2

ADC_CMP_INT_ENABLE PROC
        LSLS     r1,r0,#2
        LDR      r0,|L3.16|
        ADDS     r0,r1,r0
        LDR      r1,[r0,#0x28]
        MOVS     r2,#2
        ORRS     r1,r1,r2
        STR      r1,[r0,#0x28]
        BX       lr
        ENDP

|L3.16|
        DCD      0x400e0000

        AREA ||i.ADC_SET_CMP||, CODE, READONLY, ALIGN=2

ADC_SET_CMP PROC
        PUSH     {r4-r6,lr}
        LSLS     r5,r0,#2
        LDR      r0,|L4.32|
        LDR      r4,[sp,#0x10]
        ADDS     r0,r5,r0
        LDR      r5,[r0,#0x28]
        LDR      r6,|L4.36|
        LSLS     r1,r1,#16
        ANDS     r5,r5,r6
        ORRS     r5,r5,r1
        LSLS     r1,r2,#8
        ORRS     r5,r5,r1
        ORRS     r5,r5,r3
        ORRS     r5,r5,r4
        STR      r5,[r0,#0x28]
        POP      {r4-r6,pc}
        ENDP

|L4.32|
        DCD      0x400e0000
|L4.36|
        DCD      0xf0000003

        AREA ||i.ADC_SET_CMP_DATA||, CODE, READONLY, ALIGN=2

ADC_SET_CMP_DATA PROC
        LSLS     r2,r0,#2
        LDR      r0,|L5.20|
        ADDS     r0,r2,r0
        LDR      r2,[r0,#0x28]
        LDR      r3,|L5.24|
        LSLS     r1,r1,#16
        ANDS     r2,r2,r3
        ORRS     r2,r2,r1
        STR      r2,[r0,#0x28]
        BX       lr
        ENDP

|L5.20|
        DCD      0x400e0000
|L5.24|
        DCD      0xf000ffff

        AREA ||i.DrvADC_DeInit||, CODE, READONLY, ALIGN=2

DrvADC_DeInit PROC
        PUSH     {r4,lr}
        LDR      r0,|L6.52|
        MOVS     r1,#0
        STR      r1,[r0,#0x24]
        STR      r1,[r0,#0x20]
        STR      r1,[r0,#0x28]
        STR      r1,[r0,#0x2c]
        LDR      r0,|L6.56|
        MOVS     r2,#1
        LDR      r1,[r0,#8]
        LSLS     r2,r2,#28
        BICS     r1,r1,r2
        STR      r1,[r0,#8]
        MOVS     r1,#0
        MOV      r0,r1
        BL       ADC_SET_CMP_DATA
        MOVS     r1,#0
        MOVS     r0,#1
        BL       ADC_SET_CMP_DATA
        MOVS     r0,#0x1d
        BL       NVIC_DisableIRQ
        POP      {r4,pc}
        ENDP

        DCW      0x0000
|L6.52|
        DCD      0x400e0000
|L6.56|
        DCD      0x50000200

        AREA ||i.DrvADC_DisableComp0Int||, CODE, READONLY, ALIGN=2

DrvADC_DisableComp0Int PROC
        PUSH     {r4,lr}
        MOVS     r0,#0
        BL       ADC_CMP_INT_DISABLE
        LDR      r0,|L7.32|
        LDR      r1,[r0,#0x20]
        LSLS     r1,r1,#30
        BMI      |L7.28|
        LDR      r0,[r0,#0x2c]
        LSLS     r0,r0,#30
        BMI      |L7.28|
        MOVS     r0,#0x1d
        BL       NVIC_DisableIRQ
|L7.28|
        POP      {r4,pc}
        ENDP

        DCW      0x0000
|L7.32|
        DCD      0x400e0000

        AREA ||i.DrvADC_DisableComp1Int||, CODE, READONLY, ALIGN=2

DrvADC_DisableComp1Int PROC
        PUSH     {r4,lr}
        MOVS     r0,#1
        BL       ADC_CMP_INT_DISABLE
        LDR      r0,|L8.32|
        LDR      r1,[r0,#0x20]
        LSLS     r1,r1,#30
        BMI      |L8.28|
        LDR      r0,[r0,#0x28]
        LSLS     r0,r0,#30
        BMI      |L8.28|
        MOVS     r0,#0x1d
        BL       NVIC_DisableIRQ
|L8.28|
        POP      {r4,pc}
        ENDP

        DCW      0x0000
|L8.32|
        DCD      0x400e0000

        AREA ||i.DrvADC_DisableInt||, CODE, READONLY, ALIGN=2

DrvADC_DisableInt PROC
        PUSH     {r4,lr}
        LDR      r0,|L9.32|
        LDR      r1,[r0,#0x20]
        MOVS     r2,#2
        BICS     r1,r1,r2
        STR      r1,[r0,#0x20]
        LDR      r1,[r0,#0x28]
        LSLS     r1,r1,#30
        BMI      |L9.30|
        LDR      r0,[r0,#0x2c]
        LSLS     r0,r0,#30
        BMI      |L9.30|
        MOVS     r0,#0x1d
        BL       NVIC_DisableIRQ
|L9.30|
        POP      {r4,pc}
        ENDP

|L9.32|
        DCD      0x400e0000

        AREA ||i.DrvADC_EnableComp0||, CODE, READONLY, ALIGN=1

DrvADC_EnableComp0 PROC
        PUSH     {r3-r5,lr}
        MOV      r4,r3
        MOV      r5,r2
        MOV      r3,r0
        STR      r1,[sp,#0]
        MOV      r2,r4
        MOV      r1,r5
        MOVS     r0,#0
        BL       ADC_SET_CMP
        MOVS     r0,#0
        BL       ADC_CMP_ENABLE
        POP      {r3-r5,pc}
        ENDP


        AREA ||i.DrvADC_EnableComp0Int||, CODE, READONLY, ALIGN=2

DrvADC_EnableComp0Int PROC
        PUSH     {r4,lr}
        LDR      r1,|L11.24|
        MOVS     r0,#2
        STR      r0,[r1,#0x30]
        MOVS     r0,#0
        BL       ADC_CMP_INT_ENABLE
        MOVS     r0,#1
        LDR      r1,|L11.28|
        LSLS     r0,r0,#29
        STR      r0,[r1,#0]
        POP      {r4,pc}
        ENDP

|L11.24|
        DCD      0x400e0000
|L11.28|
        DCD      0xe000e100

        AREA ||i.DrvADC_EnableComp1||, CODE, READONLY, ALIGN=1

DrvADC_EnableComp1 PROC
        PUSH     {r3-r5,lr}
        MOV      r4,r3
        MOV      r5,r2
        MOV      r3,r0
        STR      r1,[sp,#0]
        MOV      r2,r4
        MOV      r1,r5
        MOVS     r0,#1
        BL       ADC_SET_CMP
        MOVS     r0,#1
        BL       ADC_CMP_ENABLE
        POP      {r3-r5,pc}
        ENDP


        AREA ||i.DrvADC_EnableComp1Int||, CODE, READONLY, ALIGN=2

DrvADC_EnableComp1Int PROC
        PUSH     {r4,lr}
        LDR      r1,|L13.24|
        MOVS     r0,#4
        STR      r0,[r1,#0x30]
        MOVS     r0,#1
        BL       ADC_CMP_INT_ENABLE
        MOVS     r0,#1
        LDR      r1,|L13.28|
        LSLS     r0,r0,#29
        STR      r0,[r1,#0]
        POP      {r4,pc}
        ENDP

|L13.24|
        DCD      0x400e0000
|L13.28|
        DCD      0xe000e100

        AREA ||i.DrvADC_EnableExtTrigger||, CODE, READONLY, ALIGN=2

DrvADC_EnableExtTrigger PROC
        PUSH     {r4,lr}
        LDR      r2,|L14.36|
        LDR      r3,[r2,#0x20]
        MOVS     r4,#0x30
        BICS     r3,r3,r4
        ORRS     r3,r3,r0
        STR      r3,[r2,#0x20]
        LDR      r0,[r2,#0x20]
        MOVS     r3,#0xc0
        BICS     r0,r0,r3
        ORRS     r0,r0,r1
        STR      r0,[r2,#0x20]
        LDR      r0,[r2,#0x20]
        ASRS     r1,r2,#22
        ORRS     r0,r0,r1
        STR      r0,[r2,#0x20]
        POP      {r4,pc}
        ENDP

        DCW      0x0000
|L14.36|
        DCD      0x400e0000

        AREA ||i.DrvADC_EnableInt||, CODE, READONLY, ALIGN=2

DrvADC_EnableInt PROC
        LDR      r0,|L15.24|
        MOVS     r1,#7
        STR      r1,[r0,#0x30]
        LDR      r1,[r0,#0x20]
        MOVS     r2,#2
        ORRS     r1,r1,r2
        STR      r1,[r0,#0x20]
        LDR      r1,|L15.28|
        LSLS     r0,r2,#28
        STR      r0,[r1,#0]
        BX       lr
        ENDP

        DCW      0x0000
|L15.24|
        DCD      0x400e0000
|L15.28|
        DCD      0xe000e100

        AREA ||i.DrvADC_Init||, CODE, READONLY, ALIGN=2

DrvADC_Init PROC
        PUSH     {r4-r7,lr}
        MOVS     r5,#1
        LSLS     r5,r5,#28
        MOVS     r4,#5
        LSLS     r4,r4,#28
        LDR      r6,[r4,#0xc]
        ORRS     r6,r6,r5
        STR      r6,[r4,#0xc]
        LDR      r6,[r4,#0xc]
        BICS     r6,r6,r5
        STR      r6,[r4,#0xc]
        LDR      r4,|L16.84|
        LDR      r7,[r4,#0x14]
        MOVS     r6,#0xc
        BICS     r7,r7,r6
        ORRS     r7,r7,r2
        STR      r7,[r4,#0x14]
        LDR      r2,[r4,#0x18]
        MOVS     r7,#0xff
        LSLS     r7,r7,#16
        BICS     r2,r2,r7
        LSLS     r3,r3,#16
        LSLS     r7,r4,#7
        SUBS     r3,r3,r7
        ORRS     r2,r2,r3
        STR      r2,[r4,#0x18]
        LDR      r2,[r4,#8]
        ORRS     r2,r2,r5
        STR      r2,[r4,#8]
        LDR      r2,|L16.88|
        LDR      r3,[r2,#0x20]
        MOVS     r4,#1
        ORRS     r3,r3,r4
        STR      r3,[r2,#0x20]
        LDR      r3,[r2,#0x20]
        BICS     r3,r3,r6
        ORRS     r3,r3,r0
        STR      r3,[r2,#0x20]
        LDR      r0,[r2,#0x24]
        ORRS     r0,r0,r1
        STR      r0,[r2,#0x24]
        POP      {r4-r7,pc}
        ENDP

|L16.84|
        DCD      0x50000200
|L16.88|
        DCD      0x400e0000

        AREA ||i.NVIC_DisableIRQ||, CODE, READONLY, ALIGN=2

NVIC_DisableIRQ PROC
        LSLS     r1,r0,#27
        LSRS     r1,r1,#27
        MOVS     r0,#1
        LSLS     r0,r0,r1
        LDR      r1,|L17.16|
        STR      r0,[r1,#0]
        BX       lr
        ENDP

        DCW      0x0000
|L17.16|
        DCD      0xe000e180

        AREA ||.arm_vfe_header||, DATA, READONLY, NOALLOC, ALIGN=2

        DCD      0x00000000

        EXPORT DrvADC_DeInit [CODE]
        EXPORT DrvADC_DisableComp0Int [CODE]
        EXPORT DrvADC_DisableComp1Int [CODE]
        EXPORT DrvADC_DisableInt [CODE]
        EXPORT DrvADC_EnableComp0 [CODE]
        EXPORT DrvADC_EnableComp0Int [CODE]
        EXPORT DrvADC_EnableComp1 [CODE]
        EXPORT DrvADC_EnableComp1Int [CODE]
        EXPORT DrvADC_EnableExtTrigger [CODE]
        EXPORT DrvADC_EnableInt [CODE]
        EXPORT DrvADC_Init [CODE]

        IMPORT ||Lib$$Request$$armlib|| [CODE,WEAK]

        KEEP ADC_CMP_ENABLE
        KEEP ADC_CMP_INT_DISABLE
        KEEP ADC_CMP_INT_ENABLE
        KEEP ADC_SET_CMP
        KEEP ADC_SET_CMP_DATA
        KEEP NVIC_DisableIRQ

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
