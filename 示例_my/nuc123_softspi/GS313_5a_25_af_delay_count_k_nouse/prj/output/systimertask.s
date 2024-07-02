; generated by ARM C/C++ Compiler, 5.03 [Build 76]
; commandline ArmCC [--debug -c --asm --info=totals -o.\output\systimertask.o --depend=.\output\systimertask.d --cpu=Cortex-M0 --apcs=interwork -O0 -I..\CMSIS\CM0\DeviceSupport\Nuvoton\NUC123Series -I..\CMSIS\CM0\CoreSupport -I..\INC\DrvInc -I..\INC\SysInc -I..\dw -IC:\Keil_v472\ARM\RV31\INC -IC:\Keil_v472\ARM\CMSIS\Include -IC:\Keil_v472\ARM\Inc\Nuvoton\NUC123Series -D__MICROLIB --omf_browse=.\output\systimertask.crf ..\AppSrc\SysTimerTask.c]
        THUMB
        REQUIRE8
        PRESERVE8

        AREA ||.text||, CODE, READONLY, ALIGN=2

SysTickInit PROC
        PUSH     {r4,lr}
        MOV      r4,r0
        LSLS     r0,r4,#8
        LSRS     r0,r0,#8
        SUBS     r0,r0,#1
        LDR      r1,|L1.796|
        STR      r0,[r1,#0x14]
        MOVS     r1,#3
        SUBS     r0,r1,#4
        BL       NVIC_SetPriority
        MOVS     r0,#0
        LDR      r1,|L1.796|
        STR      r0,[r1,#0x18]
        MOVS     r0,#0x10
        LDR      r1,|L1.800|
        LDR      r1,[r1,#0x10]
        MOVS     r2,#0x38
        BICS     r1,r1,r2
        ORRS     r1,r1,r0
        LDR      r2,|L1.800|
        STR      r1,[r2,#0x10]
        NOP      
        MOVS     r0,#3
        LDR      r1,|L1.796|
        STR      r0,[r1,#0x10]
        POP      {r4,pc}
        ENDP

sysDelayUs PROC
        B        |L1.84|
|L1.56|
        SUBS     r0,r0,#1
        MOVS     r1,#0
        LDR      r2,|L1.804|
        STR      r1,[r2,#0]  ; delay_us
        B        |L1.76|
|L1.66|
        LDR      r1,|L1.804|
        LDR      r1,[r1,#0]  ; delay_us
        ADDS     r1,r1,#1
        LDR      r2,|L1.804|
        STR      r1,[r2,#0]  ; delay_us
|L1.76|
        LDR      r1,|L1.804|
        LDR      r1,[r1,#0]  ; delay_us
        CMP      r1,#5
        BCC      |L1.66|
|L1.84|
        CMP      r0,#0
        BNE      |L1.56|
        BX       lr
        ENDP

sysDelayMs PROC
        PUSH     {r4,lr}
        MOV      r4,r0
        LDR      r0,|L1.808|
        LDR      r0,[r0,#0]  ; systick_counter
        ADDS     r0,r0,r4
        LDR      r1,|L1.812|
        STR      r0,[r1,#0]  ; sys_delay_counter
        B        |L1.126|
|L1.106|
        BL       SYS_UnlockReg
        BL       WDT_TIMER_RESET
        NOP      
        MOVS     r0,#0
        LDR      r1,|L1.816|
        STR      r0,[r1,#0]
        NOP      
        NOP      
|L1.126|
        LDR      r0,|L1.812|
        LDR      r0,[r0,#0]  ; sys_delay_counter
        LDR      r1,|L1.808|
        LDR      r1,[r1,#0]  ; systick_counter
        CMP      r0,r1
        BCS      |L1.106|
        POP      {r4,pc}
        ENDP

sysDelayAMs PROC
        PUSH     {r4,lr}
        MOV      r4,r0
        MOVS     r0,#0
        LDR      r1,|L1.796|
        STR      r0,[r1,#0x18]
        NOP      
        NOP      
        NOP      
        NOP      
        NOP      
        NOP      
        NOP      
        NOP      
        LDR      r0,|L1.808|
        LDR      r0,[r0,#0]  ; systick_counter
        ADDS     r0,r0,r4
        LDR      r1,|L1.812|
        STR      r0,[r1,#0]  ; sys_delay_counter
        B        |L1.198|
|L1.178|
        BL       SYS_UnlockReg
        BL       WDT_TIMER_RESET
        NOP      
        MOVS     r0,#0
        LDR      r1,|L1.816|
        STR      r0,[r1,#0]
        NOP      
        NOP      
|L1.198|
        LDR      r0,|L1.812|
        LDR      r0,[r0,#0]  ; sys_delay_counter
        LDR      r1,|L1.808|
        LDR      r1,[r1,#0]  ; systick_counter
        CMP      r0,r1
        BHI      |L1.178|
        POP      {r4,pc}
        ENDP

SysTick_Handler PROC
        PUSH     {r4,lr}
        LDR      r0,|L1.808|
        LDR      r0,[r0,#0]  ; systick_counter
        ADDS     r0,r0,#1
        LDR      r1,|L1.808|
        STR      r0,[r1,#0]  ; systick_counter
        MOVS     r4,#0
        B        |L1.324|
|L1.228|
        MOVS     r0,#0xc
        MULS     r0,r4,r0
        LDR      r1,|L1.820|
        ADDS     r0,r0,r1
        LDRB     r0,[r0,#1]
        CMP      r0,#0
        BEQ      |L1.322|
        MOVS     r0,#0xc
        MULS     r0,r4,r0
        ADDS     r0,r0,r1
        LDR      r0,[r0,#4]
        CMP      r0,#0
        BEQ      |L1.274|
        MOVS     r0,#0xc
        MULS     r0,r4,r0
        ADDS     r0,r0,r1
        LDR      r0,[r0,#4]
        SUBS     r0,r0,#1
        MOVS     r1,#0xc
        MULS     r1,r4,r1
        LDR      r2,|L1.820|
        ADDS     r1,r1,r2
        STR      r0,[r1,#4]
|L1.274|
        MOVS     r0,#0xc
        MULS     r0,r4,r0
        LDR      r1,|L1.820|
        ADDS     r0,r0,r1
        LDR      r0,[r0,#4]
        CMP      r0,#0
        BNE      |L1.322|
        MOVS     r0,#0xc
        MULS     r0,r4,r0
        LDRB     r0,[r1,r0]
        CMP      r0,#0
        BEQ      |L1.322|
        MOVS     r0,#0xc
        MULS     r0,r4,r0
        ADDS     r0,r0,r1
        LDR      r0,[r0,#8]
        CMP      r0,#0
        BEQ      |L1.322|
        MOVS     r1,#0xc
        MULS     r1,r4,r1
        LDR      r2,|L1.820|
        ADDS     r1,r1,r2
        LDR      r0,[r1,#8]
        BLX      r0
|L1.322|
        ADDS     r4,r4,#1
|L1.324|
        CMP      r4,#0xa
        BCC      |L1.228|
        NOP      
        LDR      r0,|L1.824|
        LDR      r0,[r0,#0x20]
        MOVS     r1,#1
        LSLS     r1,r1,#11
        ORRS     r0,r0,r1
        LDR      r1,|L1.824|
        STR      r0,[r1,#0x20]
        NOP      
        BL       i2cSpirit
        POP      {r4,pc}
        ENDP

runTimedTasks PROC
        PUSH     {r4,lr}
        MOVS     r4,#0
        B        |L1.432|
|L1.358|
        MOVS     r0,#0xc
        MULS     r0,r4,r0
        LDR      r1,|L1.820|
        ADDS     r0,r0,r1
        LDRB     r0,[r0,#1]
        CMP      r0,#0
        BEQ      |L1.430|
        MOVS     r0,#0xc
        MULS     r0,r4,r0
        ADDS     r0,r0,r1
        LDR      r0,[r0,#4]
        CMP      r0,#0
        BNE      |L1.430|
        MOVS     r0,#0xc
        MULS     r0,r4,r0
        LDRB     r0,[r1,r0]
        CMP      r0,#0
        BNE      |L1.430|
        MOVS     r1,#0
        MOVS     r0,#0xc
        MULS     r0,r4,r0
        LDR      r2,|L1.820|
        ADDS     r0,r0,r2
        STRB     r1,[r0,#1]
        MOVS     r0,#0xc
        MULS     r0,r4,r0
        MOV      r1,r2
        ADDS     r0,r0,r1
        LDR      r0,[r0,#8]
        CMP      r0,#0
        BEQ      |L1.430|
        MOVS     r1,#0xc
        MULS     r1,r4,r1
        ADDS     r1,r1,r2
        LDR      r0,[r1,#8]
        BLX      r0
|L1.430|
        ADDS     r4,r4,#1
|L1.432|
        CMP      r4,#0xa
        BCC      |L1.358|
        POP      {r4,pc}
        ENDP

sysTaskInit PROC
        MOVS     r0,#0
        B        |L1.486|
|L1.442|
        MOVS     r2,#0
        MOVS     r1,#0xc
        MULS     r1,r0,r1
        LDR      r3,|L1.820|
        STRB     r2,[r3,r1]
        MOVS     r1,#0xc
        MULS     r1,r0,r1
        ADDS     r1,r1,r3
        STRB     r2,[r1,#1]
        MOVS     r1,#0xc
        MULS     r1,r0,r1
        ADDS     r1,r1,r3
        STRB     r2,[r1,#2]
        MOVS     r1,#0xc
        MULS     r1,r0,r1
        ADDS     r1,r1,r3
        STR      r2,[r1,#4]
        MOVS     r1,#0xc
        MULS     r1,r0,r1
        ADDS     r1,r1,r3
        STR      r2,[r1,#8]
        ADDS     r0,r0,#1
|L1.486|
        CMP      r0,#0xa
        BCC      |L1.442|
        BX       lr
        ENDP

getATask PROC
        MOVS     r1,#0
        B        |L1.530|
|L1.496|
        MOVS     r0,#0xc
        MULS     r0,r1,r0
        LDR      r2,|L1.820|
        ADDS     r0,r0,r2
        LDRB     r0,[r0,#2]
        CMP      r0,#0
        BNE      |L1.526|
        MOVS     r2,#1
        MOVS     r0,#0xc
        MULS     r0,r1,r0
        LDR      r3,|L1.820|
        ADDS     r0,r0,r3
        STRB     r2,[r0,#2]
        MOV      r0,r1
|L1.524|
        BX       lr
|L1.526|
        ADDS     r0,r1,#1
        UXTB     r1,r0
|L1.530|
        CMP      r1,#0xa
        BLT      |L1.496|
        MOVS     r0,#0xff
        B        |L1.524|
        ENDP

taskFlush PROC
        MOVS     r0,#0
        B        |L1.582|
|L1.542|
        NOP      
        MOVS     r2,#0
        MOVS     r1,#0xc
        MULS     r1,r0,r1
        LDR      r3,|L1.820|
        ADDS     r1,r1,r3
        STRB     r2,[r1,#1]
        NOP      
        NOP      
        MOVS     r1,#0xc
        MULS     r1,r0,r1
        ADDS     r1,r1,r3
        STR      r2,[r1,#8]
        MOVS     r1,#0xc
        MULS     r1,r0,r1
        ADDS     r1,r1,r3
        STRB     r2,[r1,#2]
        NOP      
        ADDS     r1,r0,#1
        UXTB     r0,r1
|L1.582|
        CMP      r0,#0xa
        BLT      |L1.542|
        BX       lr
        ENDP

startATask PROC
        PUSH     {r4-r6,lr}
        LDR      r4,|L1.796|
        LDR      r4,[r4,#0x10]
        MOVS     r5,#2
        BICS     r4,r4,r5
        LDR      r5,|L1.796|
        STR      r4,[r5,#0x10]
        NOP      
        NOP      
        MOVS     r4,#0xc
        MULS     r4,r0,r4
        LDR      r5,|L1.820|
        STRB     r2,[r5,r4]
        MOVS     r4,#0xc
        MULS     r4,r0,r4
        ADDS     r4,r4,r5
        STR      r1,[r4,#4]
        MOVS     r4,#0xc
        MULS     r4,r0,r4
        ADDS     r4,r4,r5
        STR      r3,[r4,#8]
        MOVS     r5,#1
        MOVS     r4,#0xc
        MULS     r4,r0,r4
        LDR      r6,|L1.820|
        ADDS     r4,r4,r6
        STRB     r5,[r4,#1]
        LDR      r4,|L1.796|
        LDR      r4,[r4,#0x10]
        MOVS     r5,#2
        ORRS     r4,r4,r5
        LDR      r5,|L1.796|
        STR      r4,[r5,#0x10]
        POP      {r4-r6,pc}
        ENDP

NVIC_SetPriority PROC
        PUSH     {r4,lr}
        CMP      r0,#0
        BGE      |L1.714|
        LDR      r2,|L1.828|
        LSLS     r3,r0,#28
        LSRS     r3,r3,#28
        SUBS     r3,r3,#8
        LSRS     r3,r3,#2
        LSLS     r3,r3,#2
        LDR      r2,[r2,r3]
        LSLS     r3,r0,#30
        LSRS     r4,r3,#27
        MOVS     r3,#0xff
        LSLS     r3,r3,r4
        BICS     r2,r2,r3
        LSLS     r3,r1,#30
        LSRS     r3,r3,#24
        LSLS     r4,r0,#30
        LSRS     r4,r4,#27
        LSLS     r3,r3,r4
        ORRS     r2,r2,r3
        LDR      r3,|L1.828|
        LSLS     r4,r0,#28
        LSRS     r4,r4,#28
        SUBS     r4,r4,#8
        LSRS     r4,r4,#2
        LSLS     r4,r4,#2
        STR      r2,[r3,r4]
        B        |L1.764|
|L1.714|
        LDR      r2,|L1.832|
        MOVS     r3,#3
        LSLS     r3,r3,#8
        ADDS     r2,r2,r3
        LSRS     r3,r0,#2
        LSLS     r3,r3,#2
        LDR      r2,[r2,r3]
        LSLS     r3,r0,#30
        LSRS     r4,r3,#27
        MOVS     r3,#0xff
        LSLS     r3,r3,r4
        BICS     r2,r2,r3
        LSLS     r3,r1,#30
        LSRS     r3,r3,#24
        LSLS     r4,r0,#30
        LSRS     r4,r4,#27
        LSLS     r3,r3,r4
        ORRS     r2,r2,r3
        LDR      r3,|L1.832|
        MOVS     r4,#3
        LSLS     r4,r4,#8
        ADDS     r3,r3,r4
        LSRS     r4,r0,#2
        LSLS     r4,r4,#2
        STR      r2,[r3,r4]
|L1.764|
        POP      {r4,pc}
        ENDP

SYS_UnlockReg PROC
        MOVS     r0,#0x59
        LDR      r1,|L1.816|
        STR      r0,[r1,#0]
        MOVS     r0,#0x16
        STR      r0,[r1,#0]
        MOVS     r0,#0x88
        STR      r0,[r1,#0]
        BX       lr
        ENDP

WDT_TIMER_RESET PROC
        LDR      r0,|L1.836|
        LDR      r0,[r0,#0]
        MOVS     r1,#1
        ORRS     r0,r0,r1
        LDR      r1,|L1.836|
        STR      r0,[r1,#0]
        BX       lr
        ENDP

|L1.796|
        DCD      0xe000e000
|L1.800|
        DCD      0x50000200
|L1.804|
        DCD      delay_us
|L1.808|
        DCD      systick_counter
|L1.812|
        DCD      sys_delay_counter
|L1.816|
        DCD      0x50000100
|L1.820|
        DCD      mytask
|L1.824|
        DCD      0x400e0000
|L1.828|
        DCD      0xe000ed1c
|L1.832|
        DCD      0xe000e100
|L1.836|
        DCD      0x40004000

        AREA ||.arm_vfe_header||, DATA, READONLY, NOALLOC, ALIGN=2

        DCD      0x00000000

        AREA ||.bss||, DATA, NOINIT, ALIGN=2

mytask
        %        120

        AREA ||.data||, DATA, ALIGN=2

systick_counter
        DCD      0x00000000
delay_us
        DCD      0x00000000
sys_delay_counter
        DCD      0x00000000
audio_L
        DCD      0x00000000
audio_R
        DCD      0x00000000
pwm_ccl
        DCD      0x0000001e
flg_touchin
        DCB      0x00

        EXPORT SysTickInit [CODE]
        EXPORT sysDelayUs [CODE]
        EXPORT sysDelayMs [CODE]
        EXPORT sysDelayAMs [CODE]
        EXPORT SysTick_Handler [CODE]
        EXPORT runTimedTasks [CODE]
        EXPORT sysTaskInit [CODE]
        EXPORT getATask [CODE]
        EXPORT taskFlush [CODE]
        EXPORT startATask [CODE]
        EXPORT mytask [DATA,SIZE=120]
        EXPORT systick_counter [DATA,SIZE=4]
        EXPORT delay_us [DATA,SIZE=4]
        EXPORT audio_L [DATA,SIZE=4]
        EXPORT audio_R [DATA,SIZE=4]
        EXPORT pwm_ccl [DATA,SIZE=4]
        EXPORT flg_touchin [DATA,SIZE=1]

        IMPORT ||Lib$$Request$$armlib|| [CODE,WEAK]
        IMPORT i2cSpirit [CODE]

        KEEP NVIC_SetPriority
        KEEP SYS_UnlockReg
        KEEP WDT_TIMER_RESET
        KEEP sys_delay_counter

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
