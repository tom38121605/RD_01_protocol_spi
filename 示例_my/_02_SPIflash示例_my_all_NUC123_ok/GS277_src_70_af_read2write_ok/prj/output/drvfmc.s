; generated by ARM C/C++ Compiler, 5.02 [Build 28]
; commandline ArmCC [--min_array_alignment=4 --split_sections --debug -c --asm --info=totals -o.\output\drvfmc.o --depend=.\output\drvfmc.d --cpu=Cortex-M0 --apcs=interwork -I..\CMSIS\CM0\DeviceSupport\Nuvoton\NUC123Series -I..\CMSIS\CM0\CoreSupport -I..\INC\DrvInc -I..\INC\SysInc -IC:\Keil\ARM\RV31\INC -IC:\Keil\ARM\CMSIS\Include -D__MICROLIB --omf_browse=.\output\drvfmc.crf ..\DrvSrc\DrvFMC.c]
        THUMB
        REQUIRE8
        PRESERVE8

        AREA ||i.DrvFMC_Erase||, CODE, READONLY, ALIGN=2

DrvFMC_Erase PROC
        PUSH     {r4,lr}
        LDR      r2,|L1.44|
        MOVS     r1,#0x22
        STR      r1,[r2,#0xc]
        STR      r0,[r2,#4]
        BL       FMC_SET_ISPTRG_GO
        ISB      #0x0
|L1.18|
        BL       FMC_GET_ISPTRG_GO
        CMP      r0,#0
        BNE      |L1.18|
        BL       FMC_READ_FAIL_FLAG
        CMP      r0,#0
        BEQ      |L1.40|
        BL       FMC_CLEAR_FAIL_FLAG
        LDR      r0,|L1.48|
|L1.40|
        POP      {r4,pc}
        ENDP

        DCW      0x0000
|L1.44|
        DCD      0x5000c000
|L1.48|
        DCD      0xffff0010

        AREA ||i.DrvFMC_Read||, CODE, READONLY, ALIGN=2

DrvFMC_Read PROC
        PUSH     {r4-r6,lr}
        MOV      r5,r1
        LDR      r4,|L2.56|
        MOVS     r1,#0
        STR      r1,[r4,#0xc]
        STR      r0,[r4,#4]
        STR      r1,[r4,#8]
        BL       FMC_SET_ISPTRG_GO
        ISB      #0x0
|L2.22|
        BL       FMC_GET_ISPTRG_GO
        CMP      r0,#0
        BNE      |L2.22|
        BL       FMC_READ_FAIL_FLAG
        CMP      r0,#0
        BEQ      |L2.46|
        BL       FMC_CLEAR_FAIL_FLAG
        LDR      r0,|L2.60|
        POP      {r4-r6,pc}
|L2.46|
        LDR      r0,[r4,#8]
        STR      r0,[r5,#0]
        MOVS     r0,#0
        POP      {r4-r6,pc}
        ENDP

        DCW      0x0000
|L2.56|
        DCD      0x5000c000
|L2.60|
        DCD      0xffff0010

        AREA ||i.DrvFMC_ReadUID||, CODE, READONLY, ALIGN=2

DrvFMC_ReadUID PROC
        PUSH     {r4-r6,lr}
        MOV      r5,r1
        LDR      r4,|L3.60|
        MOVS     r1,#4
        STR      r1,[r4,#0xc]
        LSLS     r0,r0,#2
        STR      r0,[r4,#4]
        MOVS     r0,#0
        STR      r0,[r4,#8]
        BL       FMC_SET_ISPTRG_GO
        ISB      #0x0
|L3.26|
        BL       FMC_GET_ISPTRG_GO
        CMP      r0,#0
        BNE      |L3.26|
        BL       FMC_READ_FAIL_FLAG
        CMP      r0,#0
        BEQ      |L3.50|
        BL       FMC_CLEAR_FAIL_FLAG
        LDR      r0,|L3.64|
        POP      {r4-r6,pc}
|L3.50|
        LDR      r0,[r4,#8]
        STR      r0,[r5,#0]
        MOVS     r0,#0
        POP      {r4-r6,pc}
        ENDP

        DCW      0x0000
|L3.60|
        DCD      0x5000c000
|L3.64|
        DCD      0xffff0010

        AREA ||i.DrvFMC_Write||, CODE, READONLY, ALIGN=2

DrvFMC_Write PROC
        PUSH     {r4,lr}
        LDR      r2,|L4.44|
        MOVS     r3,#0x21
        STR      r3,[r2,#0xc]
        STR      r0,[r2,#4]
        STR      r1,[r2,#8]
        BL       FMC_SET_ISPTRG_GO
        ISB      #0x0
|L4.20|
        BL       FMC_GET_ISPTRG_GO
        CMP      r0,#0
        BNE      |L4.20|
        BL       FMC_READ_FAIL_FLAG
        CMP      r0,#0
        BEQ      |L4.42|
        BL       FMC_CLEAR_FAIL_FLAG
        LDR      r0,|L4.48|
|L4.42|
        POP      {r4,pc}
        ENDP

|L4.44|
        DCD      0x5000c000
|L4.48|
        DCD      0xffff0010

        AREA ||i.FMC_CLEAR_FAIL_FLAG||, CODE, READONLY, ALIGN=2

FMC_CLEAR_FAIL_FLAG PROC
        LDR      r0,|L5.12|
        LDR      r1,[r0,#0]
        MOVS     r2,#0x40
        ORRS     r1,r1,r2
        STR      r1,[r0,#0]
        BX       lr
        ENDP

|L5.12|
        DCD      0x5000c000

        AREA ||i.FMC_GET_ISPTRG_GO||, CODE, READONLY, ALIGN=2

FMC_GET_ISPTRG_GO PROC
        LDR      r0,|L6.8|
        LDR      r0,[r0,#0x10]
        UXTB     r0,r0
        BX       lr
        ENDP

|L6.8|
        DCD      0x5000c000

        AREA ||i.FMC_READ_FAIL_FLAG||, CODE, READONLY, ALIGN=2

FMC_READ_FAIL_FLAG PROC
        LDR      r0,|L7.12|
        LDR      r0,[r0,#0]
        MOVS     r1,#0x40
        ANDS     r0,r0,r1
        BX       lr
        ENDP

        DCW      0x0000
|L7.12|
        DCD      0x5000c000

        AREA ||i.FMC_SET_ISPTRG_GO||, CODE, READONLY, ALIGN=2

FMC_SET_ISPTRG_GO PROC
        LDR      r0,|L8.12|
        LDR      r1,[r0,#0x10]
        MOVS     r2,#1
        ORRS     r1,r1,r2
        STR      r1,[r0,#0x10]
        BX       lr
        ENDP

|L8.12|
        DCD      0x5000c000

        AREA ||.arm_vfe_header||, DATA, READONLY, NOALLOC, ALIGN=2

        DCD      0x00000000

        EXPORT DrvFMC_Erase [CODE]
        EXPORT DrvFMC_Read [CODE]
        EXPORT DrvFMC_ReadUID [CODE]
        EXPORT DrvFMC_Write [CODE]

        IMPORT ||Lib$$Request$$armlib|| [CODE,WEAK]

        KEEP FMC_CLEAR_FAIL_FLAG
        KEEP FMC_GET_ISPTRG_GO
        KEEP FMC_READ_FAIL_FLAG
        KEEP FMC_SET_ISPTRG_GO

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
