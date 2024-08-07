; generated by ARM C/C++ Compiler, 5.03 [Build 76]
; commandline ArmCC [--debug -c --asm --info=totals -o.\output\main.o --depend=.\output\main.d --cpu=Cortex-M0 --apcs=interwork -O0 -I..\CMSIS\CM0\DeviceSupport\Nuvoton\NUC123Series -I..\CMSIS\CM0\CoreSupport -I..\INC\DrvInc -I..\INC\SysInc -I..\dw -IC:\Keil_v472\ARM\RV31\INC -IC:\Keil_v472\ARM\CMSIS\Include -IC:\Keil_v472\ARM\Inc\Nuvoton\NUC123Series -D__MICROLIB --omf_browse=.\output\main.crf ..\AppSrc\main.c]
        THUMB
        REQUIRE8
        PRESERVE8

        AREA ||.text||, CODE, READONLY, ALIGN=2

GPIO_SET_MODE PROC
        PUSH     {r4,r5,lr}
        LDR      r3,[r0,#0]
        LSLS     r5,r1,#1
        MOVS     r4,#3
        LSLS     r4,r4,r5
        BICS     r3,r3,r4
        LSLS     r5,r1,#1
        MOV      r4,r2
        LSLS     r4,r4,r5
        ORRS     r3,r3,r4
        STR      r3,[r0,#0]
        POP      {r4,r5,pc}
        ENDP

CLK_Init PROC
        PUSH     {r4,lr}
        NOP      
        MOVS     r0,#0x59
        LDR      r1,|L1.444|
        STR      r0,[r1,#0]
        MOVS     r0,#0x16
        STR      r0,[r1,#0]
        MOVS     r0,#0x88
        STR      r0,[r1,#0]
        NOP      
        NOP      
        LDR      r0,|L1.444|
        SUBS     r0,r0,#0xc0
        LDR      r0,[r0,#4]
        MOVS     r1,#3
        ORRS     r0,r0,r1
        LDR      r1,|L1.444|
        SUBS     r1,r1,#0xc0
        STR      r0,[r1,#4]
        NOP      
        NOP      
        LDR      r0,|L1.448|
        LDR      r0,[r0,#0]
        MOVS     r1,#1
        ORRS     r0,r0,r1
        LDR      r1,|L1.448|
        STR      r0,[r1,#0]
        NOP      
        NOP      
|L1.82|
        MOVS     r0,#1
        BL       CLK_IS_STABLE
        CMP      r0,#0
        BEQ      |L1.82|
        MOVS     r0,#0
        LDR      r1,|L1.452|
        MOV      r2,r0
        ORRS     r2,r2,r1
        LDR      r3,|L1.448|
        STR      r2,[r3,#0x20]
        NOP      
        NOP      
|L1.108|
        MOVS     r0,#4
        BL       CLK_IS_STABLE
        CMP      r0,#0
        BEQ      |L1.108|
        MOVS     r0,#2
        LDR      r1,|L1.448|
        LDR      r1,[r1,#0x10]
        LSRS     r1,r1,#3
        LSLS     r1,r1,#3
        ORRS     r1,r1,r0
        LDR      r2,|L1.448|
        STR      r1,[r2,#0x10]
        NOP      
        MOVS     r0,#1
        MOV      r1,r2
        LDR      r1,[r1,#0x2c]
        BICS     r1,r1,r0
        ORRS     r1,r1,r0
        STR      r1,[r2,#0x2c]
        NOP      
        BL       SystemCoreClockUpdate
        LDR      r0,|L1.456|
        BL       SysTickInit
        BL       adcKeyClkInit
        NOP      
        MOVS     r0,#0
        LDR      r1,|L1.444|
        STR      r0,[r1,#0]
        NOP      
        POP      {r4,pc}
        ENDP

Hal_Init PROC
        PUSH     {r4,lr}
        NOP      
        MRS      r0,PRIMASK
        MOV      r4,r0
        MOVS     r0,#1
        MSR      PRIMASK,r0
        NOP      
        BL       CLK_Init
        MOV      r0,r4
        MSR      PRIMASK,r0
        NOP      
        BL       sysTaskInit
        BL       ledInit
        POP      {r4,pc}
        ENDP

delay_count_time PROC
        PUSH     {r4,lr}
        MOVS     r2,#0
        MOVS     r3,#0
        NOP      
        B        |L1.238|
|L1.226|
        MOVS     r3,#0
        B        |L1.232|
|L1.230|
        ADDS     r3,r3,#1
|L1.232|
        CMP      r3,r1
        BCC      |L1.230|
        ADDS     r2,r2,#1
|L1.238|
        CMP      r2,r0
        BCC      |L1.226|
        POP      {r4,pc}
        ENDP

main PROC
        BL       Hal_Init
        MOVS     r0,#1
        LDR      r1,|L1.460|
        STRB     r0,[r1,#0]
        BL       ledTaskInit
        LDR      r0,|L1.464|
        LDR      r0,[r0,#0]
        MOVS     r1,#3
        LSLS     r1,r1,#10
        BICS     r0,r0,r1
        LDR      r1,|L1.464|
        STR      r0,[r1,#0]
        MOV      r0,r1
        LDR      r0,[r0,#0]
        MOVS     r1,#1
        LSLS     r1,r1,#10
        ORRS     r0,r0,r1
        LDR      r1,|L1.464|
        STR      r0,[r1,#0]
        NOP      
        NOP      
        MOVS     r0,#1
        LDR      r1,|L1.468|
        STR      r0,[r1,#0]
        NOP      
        NOP      
        BL       uartInit
        BL       ledInit
        MOVS     r0,#0xff
        LDR      r1,|L1.472|
        STR      r0,[r1,#0]  ; breath_flag
        MOVS     r0,#3
        BL       ledColorSet
        BL       i2cSysInit
        BL       adckeyTaskInit
        ADR      r0,|L1.476|
        BL       iputs
        ADR      r0,|L1.500|
        BL       iputs
        ADR      r0,|L1.508|
        BL       iputs
        BL       spiFlashInit
        MOVS     r2,#1
        MOVS     r1,#0xa
        LDR      r0,|L1.464|
        SUBS     r0,r0,#0xc0
        BL       GPIO_SET_MODE
        NOP      
        MOVS     r0,#0
        LDR      r1,|L1.468|
        SUBS     r1,r1,#0xac
        STR      r0,[r1,#0]
        NOP      
        MOVS     r2,#1
        MOVS     r1,#0xa
        LDR      r0,|L1.464|
        SUBS     r0,r0,#0x80
        BL       GPIO_SET_MODE
        NOP      
        MOVS     r0,#0
        LDR      r1,|L1.468|
        SUBS     r1,r1,#0x6c
        STR      r0,[r1,#0]
        NOP      
        B        |L1.430|
|L1.400|
        MOVS     r1,#5
        MOV      r0,r1
        BL       delay_count_time
        NOP      
        LDR      r0,|L1.468|
        SUBS     r0,r0,#0x6c
        LDR      r0,[r0,#0]
        MOVS     r1,#1
        EORS     r0,r0,r1
        LDR      r1,|L1.468|
        SUBS     r1,r1,#0x6c
        STR      r0,[r1,#0]
        NOP      
        NOP      
|L1.430|
        B        |L1.400|
        ENDP

CLK_IS_STABLE PROC
        MOV      r1,r0
        LDR      r0,|L1.448|
        LDR      r0,[r0,#0xc]
        ANDS     r0,r0,r1
        BX       lr
        ENDP

        DCW      0x0000
|L1.444|
        DCD      0x50000100
|L1.448|
        DCD      0x50000200
|L1.452|
        DCD      0x00004016
|L1.456|
        DCD      0x00001770
|L1.460|
        DCD      flg_softmute
|L1.464|
        DCD      0x500040c0
|L1.468|
        DCD      0x500042d4
|L1.472|
        DCD      breath_flag
|L1.476|
        DCB      "\r\n------------------ ",0
        DCB      0
        DCB      0
|L1.500|
        DCB      "v10.92",0
        DCB      0
|L1.508|
        DCB      " -----------------\r\n",0
        DCB      0
        DCB      0
        DCB      0

        AREA ||.arm_vfe_header||, DATA, READONLY, NOALLOC, ALIGN=2

        DCD      0x00000000

        AREA ||.data||, DATA, ALIGN=2

coco
        DCB      0x00,0x00,0x00
icmd
        DCB      0x00
ikey
        DCB      0x55
ikey_vol
        DCB      0x00
flg_irmute
        DCB      0x00,0x00
bvb
        DCD      0x00000064
DC_FLAG
        DCB      0x00
upgrade_buf
        DCB      0x00,0x00,0x00
        DCB      0x00

__ARM_use_no_argv EQU 0

        EXPORT __ARM_use_no_argv
        EXPORT CLK_Init [CODE]
        EXPORT Hal_Init [CODE]
        EXPORT delay_count_time [CODE]
        EXPORT main [CODE]
        EXPORT coco [DATA,SIZE=3]
        EXPORT icmd [DATA,SIZE=1]
        EXPORT ikey [DATA,SIZE=1]
        EXPORT ikey_vol [DATA,SIZE=1]
        EXPORT flg_irmute [DATA,SIZE=1]
        EXPORT bvb [DATA,SIZE=4]
        EXPORT DC_FLAG [DATA,SIZE=1]
        EXPORT upgrade_buf [DATA,SIZE=4]

        IMPORT ||Lib$$Request$$armlib|| [CODE,WEAK]
        IMPORT SystemCoreClockUpdate [CODE]
        IMPORT SysTickInit [CODE]
        IMPORT adcKeyClkInit [CODE]
        IMPORT sysTaskInit [CODE]
        IMPORT ledInit [CODE]
        IMPORT ledTaskInit [CODE]
        IMPORT uartInit [CODE]
        IMPORT ledColorSet [CODE]
        IMPORT i2cSysInit [CODE]
        IMPORT adckeyTaskInit [CODE]
        IMPORT iputs [CODE]
        IMPORT spiFlashInit [CODE]
        IMPORT flg_softmute [DATA]
        IMPORT breath_flag [DATA]

        KEEP GPIO_SET_MODE
        KEEP CLK_IS_STABLE

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
