; generated by ARM C/C++ Compiler, 5.02 [Build 28]
; commandline ArmCC [--split_sections --debug -c --asm --info=totals -o.\output\power.o --depend=.\output\power.d --cpu=Cortex-M0 --apcs=interwork -I..\CMSIS\CM0\DeviceSupport\Nuvoton\NUC123Series -I..\CMSIS\CM0\CoreSupport -I..\INC\DrvInc -I..\INC\SysInc -IC:\Keil\ARM\RV31\INC -IC:\Keil\ARM\CMSIS\Include -D__MICROLIB --omf_browse=.\output\power.crf ..\AppSrc\power.c]
        THUMB
        REQUIRE8
        PRESERVE8

        AREA ||i.GPAB_IRQHandler||, CODE, READONLY, ALIGN=2

GPAB_IRQHandler PROC
        LDR      r1,|L1.8|
        MOVS     r0,#1
        STR      r0,[r1,#8]  ; gpio_int
        BX       lr
        ENDP

|L1.8|
        DCD      ||.data||

        AREA ||i.GPCDF_IRQHandler||, CODE, READONLY, ALIGN=2

GPCDF_IRQHandler PROC
        MOVS     r3,#1
        LDR      r0,|L2.28|
        LSLS     r1,r3,#10
        STR      r1,[r0,#0x20]
        LDR      r2,[r0,#0x18]
        BICS     r2,r2,r1
        STR      r2,[r0,#0x18]
        LDR      r1,[r0,#0x1c]
        LDR      r2,|L2.32|
        BICS     r1,r1,r2
        STR      r1,[r0,#0x1c]
        LDR      r0,|L2.36|
        STR      r3,[r0,#8]  ; gpio_int
        BX       lr
        ENDP

|L2.28|
        DCD      0x50004080
|L2.32|
        DCD      0x04000400
|L2.36|
        DCD      ||.data||

        AREA ||i.GPIO_SET_MODE||, CODE, READONLY, ALIGN=1

GPIO_SET_MODE PROC
        PUSH     {r4,lr}
        LDR      r3,[r0,#0]
        LSLS     r1,r1,#1
        MOVS     r4,#3
        LSLS     r4,r4,r1
        BICS     r3,r3,r4
        LSLS     r2,r2,r1
        ORRS     r3,r3,r2
        STR      r3,[r0,#0]
        POP      {r4,pc}
        ENDP


        AREA ||i.HardFault_Handler||, CODE, READONLY, ALIGN=2

HardFault_Handler PROC
        LDR      r1,|L4.16|
        LDR      r0,|L4.12|
        STR      r0,[r1,#0xc]
        DSB      #0x0
|L4.10|
        B        |L4.10|
        ENDP

|L4.12|
        DCD      0x05fa0004
|L4.16|
        DCD      0xe000ed00

        AREA ||i.PWRWU_IRQHandler||, CODE, READONLY, ALIGN=2

PWRWU_IRQHandler PROC
        PUSH     {r4,lr}
        BL       SYS_UnlockReg
        MOVS     r0,#2
        BL       ledColorSet
        MOVS     r0,#0x7d
        LSLS     r0,r0,#4
        BL       sysDelayMs
        LDR      r1,|L5.28|
        MOVS     r0,#0
        STR      r0,[r1,#0]
        POP      {r4,pc}
        ENDP

|L5.28|
        DCD      0x50000100

        AREA ||i.SYS_UnlockReg||, CODE, READONLY, ALIGN=2

SYS_UnlockReg PROC
        LDR      r0,|L6.16|
        MOVS     r1,#0x59
        STR      r1,[r0,#0]
        MOVS     r1,#0x16
        STR      r1,[r0,#0]
        MOVS     r1,#0x88
        STR      r1,[r0,#0]
        BX       lr
        ENDP

|L6.16|
        DCD      0x50000100

        AREA ||i.WDTInit||, CODE, READONLY, ALIGN=2

WDTInit PROC
        PUSH     {r4,lr}
        BL       WDT_TIMER_RESET
        MOVS     r2,#1
        MOVS     r1,#3
        MOVS     r0,#5
        BL       DrvWDT_Init
        BL       SYS_UnlockReg
        BL       WDT_TIMER_RESET
        BL       WDT_ENABLE_COUNTING
        BL       WDT_CLEAR_RESET_FLAG
        LDR      r0,|L7.60|
        LDR      r1,[r0,#0]
        MOVS     r2,#0x10
        BICS     r1,r1,r2
        STR      r1,[r0,#0]
        LDR      r2,|L7.64|
        MOVS     r1,#0
        STR      r1,[r2,#0]
        LDR      r1,[r0,#0]
        MOVS     r2,#2
        ORRS     r1,r1,r2
        STR      r1,[r0,#0]
        POP      {r4,pc}
        ENDP

        DCW      0x0000
|L7.60|
        DCD      0x40004000
|L7.64|
        DCD      0x50000100

        AREA ||i.WDT_CLEAR_RESET_FLAG||, CODE, READONLY, ALIGN=2

WDT_CLEAR_RESET_FLAG PROC
        LDR      r0,|L8.12|
        LDR      r1,[r0,#0]
        MOVS     r2,#4
        ORRS     r1,r1,r2
        STR      r1,[r0,#0]
        BX       lr
        ENDP

|L8.12|
        DCD      0x40004000

        AREA ||i.WDT_ENABLE_COUNTING||, CODE, READONLY, ALIGN=2

WDT_ENABLE_COUNTING PROC
        LDR      r0,|L9.12|
        LDR      r1,[r0,#0]
        MOVS     r2,#0x80
        ORRS     r1,r1,r2
        STR      r1,[r0,#0]
        BX       lr
        ENDP

|L9.12|
        DCD      0x40004000

        AREA ||i.WDT_IRQHandler||, CODE, READONLY, ALIGN=2

WDT_IRQHandler PROC
        PUSH     {r4,lr}
        BL       WDTwake
        BL       SYS_UnlockReg
        LDR      r4,|L10.48|
        LDR      r0,[r4,#0]
        MOVS     r1,#8
        ORRS     r0,r0,r1
        STR      r0,[r4,#0]
        BL       WDT_TIMER_RESET
        LDR      r0,[r4,#0]
        MOVS     r1,#7
        LSLS     r1,r1,#8
        BICS     r0,r0,r1
        MOVS     r1,#5
        LSLS     r1,r1,#8
        ORRS     r0,r0,r1
        STR      r0,[r4,#0]
        LDR      r1,|L10.52|
        MOVS     r0,#0
        STR      r0,[r1,#0]
        POP      {r4,pc}
        ENDP

|L10.48|
        DCD      0x40004000
|L10.52|
        DCD      0x50000100

        AREA ||i.WDT_TIMER_RESET||, CODE, READONLY, ALIGN=2

WDT_TIMER_RESET PROC
        LDR      r0,|L11.12|
        LDR      r1,[r0,#0]
        MOVS     r2,#1
        ORRS     r1,r1,r2
        STR      r1,[r0,#0]
        BX       lr
        ENDP

|L11.12|
        DCD      0x40004000

        AREA ||i.WDTwake||, CODE, READONLY, ALIGN=2

WDTwake PROC
        PUSH     {r4,lr}
        MOVS     r2,#0
        MOVS     r1,#3
        MOVS     r0,#5
        BL       DrvWDT_Init
        BL       DrvWDT_EnableInt
        BL       SYS_UnlockReg
        BL       WDT_TIMER_RESET
        BL       WDT_ENABLE_COUNTING
        BL       WDT_CLEAR_RESET_FLAG
        LDR      r0,|L12.60|
        LDR      r1,[r0,#0]
        MOVS     r2,#0x10
        ORRS     r1,r1,r2
        STR      r1,[r0,#0]
        LDR      r1,[r0,#0]
        MOVS     r2,#2
        BICS     r1,r1,r2
        STR      r1,[r0,#0]
        LDR      r1,|L12.64|
        MOVS     r0,#0
        STR      r0,[r1,#0]
        POP      {r4,pc}
        ENDP

        DCW      0x0000
|L12.60|
        DCD      0x40004000
|L12.64|
        DCD      0x50000100

        AREA ||i.dealPowerKey||, CODE, READONLY, ALIGN=2

dealPowerKey PROC
        PUSH     {r3-r7,lr}
        LDR      r0,|L13.64|
        MOVS     r4,#0
        STR      r4,[r0,#0]  ; dc_time_stamp
        MOVS     r0,#5
        LSLS     r0,r0,#28
        LDR      r0,[r0,#4]
        MOV      r6,r4
        MOVS     r1,#0x20
        TST      r0,r1
        BEQ      |L13.24|
        MOVS     r4,#0x50
|L13.24|
        LDR      r5,|L13.68|
        LDR      r7,|L13.68|
        SUBS     r5,r5,#0x40
|L13.30|
        LDR      r0,[r5,#0]
        CMP      r0,#0
        BEQ      |L13.44|
        MOVS     r0,#5
        BL       sysDelayMs
        B        |L13.54|
|L13.44|
        STR      r6,[r7,#0]
        MOVS     r4,#0
|L13.48|
        LDR      r0,[r5,#0]
        CMP      r0,#0
        BEQ      |L13.48|
|L13.54|
        ADDS     r4,r4,#1
        CMP      r4,#0x64
        BCC      |L13.30|
        POP      {r3-r7,pc}
        ENDP

        DCW      0x0000
|L13.64|
        DCD      dc_time_stamp
|L13.68|
        DCD      0x500042a0

        AREA ||i.doPowerOff||, CODE, READONLY, ALIGN=2

doPowerOff PROC
        MOVS     r6,#0
        MOV      r5,r6
        MOV      r4,r6
        BL       DrvWDT_DisableInt
        LDR      r7,|L14.80|
|L14.12|
        MOVS     r0,#0x14
        BL       sysDelayMs
        CMP      r4,#0
        BNE      |L14.28|
        LDR      r0,[r7,#0]
        CMP      r0,#0
        BNE      |L14.38|
|L14.28|
        ADDS     r6,r6,#1
        CMP      r6,#3
        BEQ      |L14.42|
        CMP      r4,#1
        BEQ      |L14.44|
|L14.38|
        MOVS     r5,#0
        B        |L14.12|
|L14.42|
        MOVS     r4,#1
|L14.44|
        ADDS     r5,r5,#1
        CMP      r5,#0xa
        BNE      |L14.12|
        MOVS     r0,#0
        BL       ledColorSet
        LDR      r1,|L14.80|
        MOVS     r0,#0
        ADDS     r1,r1,#0x40
        STR      r0,[r1,#0]
        BL       SYS_UnlockReg
        LDR      r1,|L14.88|
        LDR      r0,|L14.84|
        STR      r0,[r1,#0xc]
        DSB      #0x0
|L14.78|
        B        |L14.78|
        ENDP

|L14.80|
        DCD      0x50004260
|L14.84|
        DCD      0x05fa0004
|L14.88|
        DCD      0xe000ed00

        AREA ||i.enterStandby||, CODE, READONLY, ALIGN=2

enterStandby PROC
        PUSH     {r3-r7,lr}
        LDR      r5,|L15.96|
        MOVS     r4,#1
        STRB     r4,[r5,#0]
        BL       setConfig
        LDR      r6,|L15.100|
        MOVS     r0,#0x64
        STRB     r4,[r6,#0]
        BL       sysDelayMs
        LDR      r1,|L15.104|
        MOVS     r0,#3
        STRB     r0,[r1,#0]
        MOVS     r0,#5
        BL       playMusic
        LDR      r0,|L15.108|
        MOVS     r7,#0
        STR      r7,[r0,#0]  ; BT_FIRST_START
        LDR      r0,|L15.112|
        BL       sysDelayMs
        MOVS     r4,#0xf
|L15.48|
        MOV      r0,r4
        BL       ampSetVolume
        MOVS     r0,#3
        BL       sysDelayMs
        SUBS     r4,r4,#1
        BNE      |L15.48|
        MOVS     r0,#0
        BL       ampSetVolume
        BL       ampMute
        STRB     r7,[r6,#0]
        LDRB     r0,[r5,#1]  ; enter_Standby_stop
        CMP      r0,#0
        BEQ      |L15.88|
        BL       outStandby
        POP      {r3-r7,pc}
|L15.88|
        BL       powerOff
        POP      {r3-r7,pc}
        ENDP

        DCW      0x0000
|L15.96|
        DCD      ||.data||
|L15.100|
        DCD      change_source_mute
|L15.104|
        DCD      dev_state
|L15.108|
        DCD      BT_FIRST_START
|L15.112|
        DCD      0x000005dc

        AREA ||i.openMcuCtrlPin||, CODE, READONLY, ALIGN=2

openMcuCtrlPin PROC
        PUSH     {r3-r7,lr}
        MOVS     r2,#1
        MOVS     r1,#8
        LDR      r0,|L16.256|
        BL       GPIO_SET_MODE
        LDR      r0,|L16.260|
        MOVS     r4,#0
        STR      r4,[r0,#0]
        LDR      r6,|L16.256|
        MOVS     r2,#1
        ADDS     r6,r6,#0x40
        MOVS     r1,#5
        MOV      r0,r6
        BL       GPIO_SET_MODE
        LDR      r0,|L16.260|
        SUBS     r0,r0,#8
        STR      r4,[r0,#0]
        MOVS     r2,#1
        MOVS     r1,#3
        MOV      r0,r6
        BL       GPIO_SET_MODE
        LDR      r0,|L16.260|
        SUBS     r0,r0,#0x5c
        STR      r4,[r0,#0]
        LDR      r0,|L16.256|
        MOVS     r2,#1
        MOVS     r1,#0xe
        SUBS     r0,r0,#0x40
        BL       GPIO_SET_MODE
        LDR      r5,|L16.256|
        MOVS     r2,#2
        ADDS     r5,r5,#0xc0
        MOVS     r1,#3
        MOV      r0,r5
        BL       GPIO_SET_MODE
        MOVS     r2,#2
        MOV      r1,r2
        MOV      r0,r5
        BL       GPIO_SET_MODE
        LDR      r0,|L16.260|
        SUBS     r0,r0,#0x70
        STR      r4,[r0,#0]
        LDR      r0,|L16.256|
        MOVS     r2,#1
        MOVS     r1,#9
        SUBS     r0,r0,#0x40
        BL       GPIO_SET_MODE
        LDR      r0,|L16.256|
        MOVS     r2,#0
        MOVS     r1,#8
        SUBS     r0,r0,#0x40
        BL       GPIO_SET_MODE
        MOVS     r2,#0
        MOVS     r1,#0xa
        LDR      r0,|L16.256|
        BL       GPIO_SET_MODE
        LDR      r5,|L16.260|
        SUBS     r5,r5,#0x98
        STR      r4,[r5,#0]
        LDR      r7,|L16.256|
        MOVS     r2,#1
        SUBS     r7,r7,#0x80
        MOVS     r1,#0xf
        MOV      r0,r7
        BL       GPIO_SET_MODE
        LDR      r0,|L16.260|
        SUBS     r0,r0,#0x20
        STR      r4,[r0,#0]
        MOVS     r2,#1
        MOVS     r1,#0xd
        LDR      r0,|L16.256|
        BL       GPIO_SET_MODE
        LDR      r0,|L16.260|
        SUBS     r0,r0,#0xc
        STR      r4,[r0,#0]
        MOVS     r2,#1
        MOVS     r1,#2
        MOV      r0,r6
        BL       GPIO_SET_MODE
        LDR      r0,|L16.260|
        SUBS     r0,r0,#0x30
        STR      r4,[r0,#0]
        MOVS     r2,#0
        MOVS     r1,#9
        LDR      r0,|L16.256|
        BL       GPIO_SET_MODE
        LDR      r0,|L16.260|
        SUBS     r0,r0,#0x6c
        STR      r4,[r0,#0]
        LDR      r0,|L16.256|
        MOVS     r2,#0
        MOVS     r1,#0xa
        SUBS     r0,r0,#0x40
        BL       GPIO_SET_MODE
        STR      r4,[r5,#0]
        MOVS     r2,#0
        MOVS     r1,#0xf
        MOV      r0,r7
        BL       GPIO_SET_MODE
        MOVS     r2,#1
        MOVS     r1,#0xb
        LDR      r0,|L16.256|
        BL       GPIO_SET_MODE
        MOVS     r0,#5
        LSLS     r0,r0,#28
        LDR      r1,[r0,#0x34]
        MOVS     r2,#1
        LSLS     r2,r2,#14
        BICS     r1,r1,r2
        STR      r1,[r0,#0x34]
        POP      {r3-r7,pc}
        ENDP

        DCW      0x0000
|L16.256|
        DCD      0x50004080
|L16.260|
        DCD      0x500042d4

        AREA ||i.outStandby||, CODE, READONLY, ALIGN=2

outStandby PROC
        PUSH     {r3-r7,lr}
        BL       resumeI2c
        LDR      r0,|L17.464|
        MOVS     r4,#0
        STRB     r4,[r0,#0]
        LDR      r7,|L17.468|
        STRB     r4,[r0,#1]
        LDRB     r0,[r7,#0]  ; dev_state
        MOVS     r5,#1
        CMP      r0,#0
        BEQ      |L17.26|
        STRB     r5,[r7,#0]
|L17.26|
        LDR      r0,|L17.472|
        STR      r4,[r0,#0]  ; BAT_STATE_DECT_DELAY_COUNT
        LDR      r0,|L17.476|
        STR      r4,[r0,#0]  ; BT_MATCH_FLAG
        LDR      r0,|L17.480|
        STR      r4,[r0,#0]  ; led_battery_flash
        LDR      r0,|L17.484|
        STR      r4,[r0,#0]  ; ccount
        LDR      r0,|L17.488|
        LDR      r0,[r0,#0]
        LDR      r2,|L17.492|
        CMP      r0,#0
        BEQ      |L17.66|
        STRB     r4,[r2,#0]
|L17.54|
        LDR      r1,|L17.496|
        LDR      r0,[r1,#0]  ; amp_volume
        CMP      r0,#0x18
        BLS      |L17.70|
        MOVS     r0,#0x18
        B        |L17.76|
|L17.66|
        STRB     r5,[r2,#0]
        B        |L17.54|
|L17.70|
        CMP      r0,#9
        BCS      |L17.78|
        MOVS     r0,#9
|L17.76|
        STR      r0,[r1,#0]  ; amp_volume
|L17.78|
        LDRB     r0,[r7,#0]  ; dev_state
        LDR      r6,|L17.500|
        CMP      r0,#0
        BEQ      |L17.340|
        LDR      r0,|L17.504|
        LDRB     r1,[r2,#0]  ; dev_input
        LDRB     r0,[r0,r1]
        STRB     r0,[r6,#0]
|L17.94|
        BL       ledColorSet
        LDR      r0,|L17.508|
        STRB     r5,[r0,#0]
        BL       spiFlashInit
        BL       powerOn
        LDR      r0,|L17.488|
        ADDS     r0,r0,#0x40
        LDR      r0,[r0,#0]
        CMP      r0,#0
        BEQ      |L17.134|
        MOVS     r0,#0
        BL       read_upgrade_eeprom
        CMP      r0,#0xa3
        BEQ      |L17.134|
        BL       bootLDROM
|L17.134|
        LDR      r0,|L17.488|
        ADDS     r0,r0,#0x60
        STR      r5,[r0,#0]
        LDR      r5,|L17.512|
        LDR      r0,[r5,#0]  ; f_test
        CMP      r0,#0
        BEQ      |L17.326|
        MOVS     r0,#0x18
        BL       ampSetVolume
        MOVS     r1,#0x16
        MOVS     r0,#6
        BL       ampSendCmd_38
        MOVS     r0,#8
        BL       ledColorSet
        MOVS     r0,#6
        BL       playMusic
        MOVS     r0,#0xff
        ADDS     r0,r0,#0x2d
        BL       sysDelayMs
        MOVS     r0,#6
        BL       playMusic
        MOVS     r0,#0xff
        ADDS     r0,r0,#0x2d
        BL       sysDelayMs
        LDR      r0,|L17.516|
        STRB     r4,[r0,#0]
        MOVS     r0,#6
        BL       ledColorSet
        MOVS     r1,#0x1a
        MOVS     r0,#6
        BL       ampSendCmd_38
        MOVS     r0,#0xc8
        BL       sysDelayMs
        MOVS     r0,#0xf
        STR      r0,[r5,#0]  ; f_test
|L17.224|
        MOVS     r0,#1
        BL       playMusic
        MOVS     r0,#0xc8
        BL       sysDelayMs
        LDR      r0,[r5,#0]  ; f_test
        SUBS     r0,r0,#1
        STR      r0,[r5,#0]  ; f_test
        BNE      |L17.224|
        LDR      r0,|L17.516|
        MOVS     r1,#0x1c
        STRB     r4,[r0,#0]
        MOVS     r0,#6
        BL       ampSendCmd_38
        MOVS     r0,#5
        BL       ledColorSet
        MOVS     r0,#0xc8
        BL       sysDelayMs
        MOVS     r0,#0xf
        STR      r0,[r5,#0]  ; f_test
|L17.272|
        MOVS     r0,#1
        BL       playMusic
        MOVS     r0,#0xc8
        BL       sysDelayMs
        LDR      r0,[r5,#0]  ; f_test
        SUBS     r0,r0,#1
        STR      r0,[r5,#0]  ; f_test
        BNE      |L17.272|
        LDR      r0,|L17.516|
        STRB     r4,[r0,#0]
        LDRB     r0,[r6,#0]  ; state_color
        BL       ledColorSet
        MOVS     r0,#0
        BL       ampSetVolume
        MOVS     r1,#0x10
        MOVS     r0,#6
        BL       ampSendCmd_38
        MOVS     r0,#0x7d
        LSLS     r0,r0,#3
        BL       sysDelayMs
        STR      r4,[r5,#0]  ; f_test
|L17.326|
        LDR      r0,|L17.520|
        STR      r4,[r0,#0]  ; audio_quiet_time
        LDR      r0,|L17.464|
        STR      r4,[r0,#4]  ; dc_flag
        LDRB     r0,[r7,#0]  ; dev_state
        CMP      r0,#0
        B        |L17.342|
|L17.340|
        B        |L17.444|
|L17.342|
        BEQ      |L17.354|
        LDR      r0,|L17.524|
        LDRB     r1,[r0,#0]  ; reset_en
        CMP      r1,#0
        BEQ      |L17.454|
        STRB     r4,[r0,#0]
|L17.354|
        BL       uartInit
        BL       sendStartInfo
        BL       ledTask
        LDRB     r0,[r6,#0]  ; state_color
        BL       ledColorSet
        BL       WDTInit
        MOVS     r0,#0xc8
        BL       sysDelayMs
        BL       adcReadKey
        BL       adcReadKey
        MOVS     r0,#0x4b
        LSLS     r0,r0,#4
        BL       sysDelayMs
        BL       check_coco
        CMP      r0,#0
        BEQ      |L17.416|
        BL       Dwhp83FixedCoCo
        MOVS     r0,#0x64
        BL       sysDelayMs
|L17.416|
        BL       Dwhp83CuMuDetect
        LDR      r0,|L17.528|
        LDR      r1,|L17.532|
        LDR      r0,[r0,#0]  ; Dwhp83_CuMu
        CMP      r0,r1
        BNE      |L17.438|
        BL       DWP83MuPowerOn
        BL       Dwhp83Slave
|L17.438|
        LDR      r0,|L17.508|
        STRB     r4,[r0,#0]
        POP      {r3-r7,pc}
|L17.444|
        MOVS     r0,#0x1d
        STR      r0,[r1,#0]  ; amp_volume
        STRB     r5,[r2,#0]
        MOVS     r0,#7
        B        |L17.94|
|L17.454|
        MOVS     r0,#6
        BL       playMusic
        B        |L17.354|
        ENDP

        DCW      0x0000
|L17.464|
        DCD      ||.data||
|L17.468|
        DCD      dev_state
|L17.472|
        DCD      BAT_STATE_DECT_DELAY_COUNT
|L17.476|
        DCD      BT_MATCH_FLAG
|L17.480|
        DCD      led_battery_flash
|L17.484|
        DCD      ccount
|L17.488|
        DCD      0x50004268
|L17.492|
        DCD      dev_input
|L17.496|
        DCD      amp_volume
|L17.500|
        DCD      state_color
|L17.504|
        DCD      input_channel_color
|L17.508|
        DCD      change_source_mute
|L17.512|
        DCD      f_test
|L17.516|
        DCD      flash_mode
|L17.520|
        DCD      audio_quiet_time
|L17.524|
        DCD      reset_en
|L17.528|
        DCD      Dwhp83_CuMu
|L17.532|
        DCD      0x000055aa

        AREA ||i.powerOff||, CODE, READONLY, ALIGN=2

powerOff PROC
        PUSH     {r4-r6,lr}
        LDR      r0,|L18.228|
        MOVS     r4,#0
        STR      r4,[r0,#0]
        LDR      r1,|L18.232|
        MOVS     r0,#0xff
        STR      r0,[r1,#0]  ; breath_flag
        LDR      r0,|L18.236|
        MOVS     r1,#0x80
        STR      r4,[r0,#0]  ; BT_FIRST_START
        LDR      r0,|L18.240|
        STRB     r4,[r0,#0]
        LDR      r0,|L18.244|
        LDR      r0,[r0,#0]  ; volume_back
        BICS     r0,r0,r1
        LDR      r1,|L18.248|
        STR      r0,[r1,#0]  ; amp_volume
        LDR      r0,|L18.252|
        LDR      r1,[r0,#8]
        LSRS     r1,r1,#1
        LSLS     r1,r1,#1
        STR      r1,[r0,#8]
        LDR      r1,|L18.256|
        MOVS     r0,#2
        STRB     r0,[r1,#0]
        BL       stopFlashLed
        MOVS     r0,#0xc8
        BL       sysDelayMs
        BL       ampPowerDown
        MOVS     r0,#0xc8
        BL       sysDelayMs
        LDR      r0,|L18.260|
        MOVS     r1,#1
        LDR      r2,[r0,#8]
        LSLS     r1,r1,#29
        BICS     r2,r2,r1
        STR      r2,[r0,#8]
        LDR      r0,|L18.264|
        LDR      r1,[r0,#0]
        MOVS     r3,#1
        LSLS     r3,r3,#15
        BICS     r1,r1,r3
        STR      r1,[r0,#0]
        MOVS     r0,#5
        LSLS     r0,r0,#28
        LDR      r1,[r0,#0x38]
        LSRS     r1,r1,#4
        LSLS     r1,r1,#4
        STR      r1,[r0,#0x38]
        LDR      r1,|L18.268|
        LDR      r2,[r1,#0x10]
        MOVS     r5,#0xff
        ADDS     r5,r5,#0xe1
        BICS     r2,r2,r5
        STR      r2,[r1,#0x10]
        LDR      r2,[r0,#0x30]
        BICS     r2,r2,r3
        STR      r2,[r0,#0x30]
        LDR      r0,[r1,#0x10]
        ASRS     r2,r3,#6
        BICS     r0,r0,r2
        STR      r0,[r1,#0x10]
        LDR      r5,|L18.272|
        MOVS     r2,#0
        MOVS     r1,#0xc
        MOV      r0,r5
        BL       GPIO_SET_MODE
        MOVS     r2,#0
        MOV      r1,r2
        MOV      r0,r5
        BL       GPIO_SET_MODE
        MOVS     r2,#0
        MOVS     r1,#1
        MOV      r0,r5
        BL       GPIO_SET_MODE
        MOVS     r2,#0
        MOVS     r1,#2
        MOV      r0,r5
        BL       GPIO_SET_MODE
        MOVS     r2,#0
        MOVS     r1,#3
        MOV      r0,r5
        BL       GPIO_SET_MODE
        MOVS     r0,#0x64
        BL       sysDelayMs
        MOVS     r0,#7
        BL       logEvent
        BL       logClose
        LDR      r0,|L18.276|
        LDRB     r0,[r0,#1]  ; enter_Standby_stop
        CMP      r0,#0
        BEQ      |L18.214|
        BL       outStandby
        POP      {r4-r6,pc}
|L18.214|
        BL       logClose
        LDR      r0,|L18.228|
        ADDS     r0,r0,#8
        STR      r4,[r0,#0]
        BL       doPowerOff
        ENDP

|L18.228|
        DCD      0x500042cc
|L18.232|
        DCD      breath_flag
|L18.236|
        DCD      BT_FIRST_START
|L18.240|
        DCD      flash_mode
|L18.244|
        DCD      volume_back
|L18.248|
        DCD      amp_volume
|L18.252|
        DCD      0x40130000
|L18.256|
        DCD      dev_state
|L18.260|
        DCD      0x50000200
|L18.264|
        DCD      0x401a0000
|L18.268|
        DCD      0x50000040
|L18.272|
        DCD      0x50004080
|L18.276|
        DCD      ||.data||

        AREA ||i.powerOn||, CODE, READONLY, ALIGN=2

powerOn PROC
        PUSH     {r3-r7,lr}
        LDR      r6,|L19.156|
        MOVS     r5,#0
        STR      r5,[r6,#0]
        BL       openMcuCtrlPin
        LDR      r0,|L19.156|
        MOVS     r4,#1
        SUBS     r0,r0,#0x2c
        STR      r4,[r0,#0]
        STR      r4,[r6,#0]
        LDR      r6,|L19.156|
        SUBS     r6,r6,#0x68
        STR      r5,[r6,#0]
        MOVS     r0,#0xc8
        BL       sysDelayMs
        LDR      r0,|L19.156|
        ADDS     r0,r0,#8
        STR      r4,[r0,#0]
        LDR      r7,|L19.156|
        SUBS     r7,r7,#0x90
        STR      r5,[r7,#0]
        MOVS     r0,#0xc8
        BL       sysDelayMs
        STR      r4,[r7,#0]
        LDR      r0,|L19.160|
        STR      r4,[r0,#0]  ; BT_FIRST_START
        LDR      r0,|L19.156|
        SUBS     r0,r0,#0x54
        STR      r4,[r0,#0]
        LDR      r0,|L19.164|
        BL       I2SClkInit
        MOVS     r0,#0xc8
        BL       sysDelayMs
        BL       i2cSysInit
        MOVS     r0,#0x64
        BL       sysDelayMs
        STR      r4,[r6,#0]
        MOVS     r0,#0x32
        BL       sysDelayMs
        MOVS     r0,#0xa
        BL       sysDelayMs
        BL       ampInitAllConf
        MOVS     r1,#0x3f
        MOVS     r0,#0
        BL       ampSendEqs_38
        MOVS     r0,#0
        BL       ampSetVolume
        BL       unMute
        MOVS     r0,#0xc8
        BL       sysDelayMs
        BL       i2sStart
        LDR      r0,|L19.156|
        SUBS     r0,r0,#4
        STR      r5,[r0,#0]
        MOVS     r0,#0xc8
        BL       sysDelayMs
        BL       getMuAddr
        BL       pt2314DataResume
        POP      {r3-r7,pc}
        ENDP

        DCW      0x0000
|L19.156|
        DCD      0x500042cc
|L19.160|
        DCD      BT_FIRST_START
|L19.164|
        DCD      0x0000bb80

        AREA ||i.resetHook||, CODE, READONLY, ALIGN=2

resetHook PROC
        PUSH     {r4,lr}
        LDR      r0,|L20.20|
        LDR      r0,[r0,#0]
        MOVS     r1,#4
        TST      r0,r1
        BEQ      |L20.16|
        BL       WDT_CLEAR_RESET_FLAG
|L20.16|
        POP      {r4,pc}
        ENDP

        DCW      0x0000
|L20.20|
        DCD      0x40004000

        AREA ||i.sleep||, CODE, READONLY, ALIGN=1

sleep PROC
        BX       lr
        ENDP


        AREA ||i.sysResume||, CODE, READONLY, ALIGN=1

sysResume PROC
        BX       lr
        ENDP


        AREA ||i.sysStore||, CODE, READONLY, ALIGN=1

sysStore PROC
        BX       lr
        ENDP


        AREA ||.arm_vfe_header||, DATA, READONLY, NOALLOC, ALIGN=2

        DCD      0x00000000

        AREA ||.data||, DATA, ALIGN=2

enter_standby_mode
        DCB      0x00
enter_Standby_stop
        DCB      0x00,0x00,0x00
dc_flag
        DCD      0x00000000
gpio_int
        DCD      0x00000000

        AREA ||area_number.27||, DATA, ALIGN=0

        EXPORTAS ||area_number.27||, ||.data||
wdt_handler
        DCB      0x00

        AREA ||area_number.28||, DATA, ALIGN=2

        EXPORTAS ||area_number.28||, ||.data||
wdt_timer
        DCD      0x00000000

        AREA ||area_number.29||, DATA, ALIGN=2

        EXPORTAS ||area_number.29||, ||.data||
pwrcon
        DCD      0x50000200

        AREA ||area_number.30||, DATA, ALIGN=0

        EXPORTAS ||area_number.30||, ||.data||
battery_state
        DCB      0x00

        EXPORT GPAB_IRQHandler [CODE]
        EXPORT GPCDF_IRQHandler [CODE]
        EXPORT HardFault_Handler [CODE]
        EXPORT PWRWU_IRQHandler [CODE]
        EXPORT WDTInit [CODE]
        EXPORT WDT_IRQHandler [CODE]
        EXPORT WDTwake [CODE]
        EXPORT dealPowerKey [CODE]
        EXPORT doPowerOff [CODE]
        EXPORT enterStandby [CODE]
        EXPORT outStandby [CODE]
        EXPORT powerOff [CODE]
        EXPORT powerOn [CODE]
        EXPORT resetHook [CODE]
        EXPORT sleep [CODE]
        EXPORT sysResume [CODE]
        EXPORT sysStore [CODE]
        EXPORT enter_standby_mode [DATA,SIZE=1]
        EXPORT enter_Standby_stop [DATA,SIZE=1]
        EXPORT dc_flag [DATA,SIZE=4]
        EXPORT gpio_int [DATA,SIZE=4]
        EXPORT wdt_handler [DATA,SIZE=1]
        EXPORT wdt_timer [DATA,SIZE=4]
        EXPORT pwrcon [DATA,SIZE=4]
        EXPORT battery_state [DATA,SIZE=1]

        IMPORT ||Lib$$Request$$armlib|| [CODE,WEAK]
        IMPORT sysDelayMs [CODE]
        IMPORT dc_time_stamp [DATA]
        IMPORT DrvWDT_DisableInt [CODE]
        IMPORT ledColorSet [CODE]
        IMPORT DrvWDT_Init [CODE]
        IMPORT I2SClkInit [CODE]
        IMPORT i2cSysInit [CODE]
        IMPORT ampInitAllConf [CODE]
        IMPORT ampSendEqs_38 [CODE]
        IMPORT ampSetVolume [CODE]
        IMPORT unMute [CODE]
        IMPORT i2sStart [CODE]
        IMPORT getMuAddr [CODE]
        IMPORT pt2314DataResume [CODE]
        IMPORT BT_FIRST_START [DATA]
        IMPORT resumeI2c [CODE]
        IMPORT spiFlashInit [CODE]
        IMPORT read_upgrade_eeprom [CODE]
        IMPORT bootLDROM [CODE]
        IMPORT ampSendCmd_38 [CODE]
        IMPORT playMusic [CODE]
        IMPORT uartInit [CODE]
        IMPORT sendStartInfo [CODE]
        IMPORT ledTask [CODE]
        IMPORT adcReadKey [CODE]
        IMPORT check_coco [CODE]
        IMPORT Dwhp83FixedCoCo [CODE]
        IMPORT Dwhp83CuMuDetect [CODE]
        IMPORT DWP83MuPowerOn [CODE]
        IMPORT Dwhp83Slave [CODE]
        IMPORT dev_state [DATA]
        IMPORT BAT_STATE_DECT_DELAY_COUNT [DATA]
        IMPORT BT_MATCH_FLAG [DATA]
        IMPORT led_battery_flash [DATA]
        IMPORT ccount [DATA]
        IMPORT dev_input [DATA]
        IMPORT amp_volume [DATA]
        IMPORT state_color [DATA]
        IMPORT input_channel_color [DATA]
        IMPORT change_source_mute [DATA]
        IMPORT f_test [DATA]
        IMPORT flash_mode [DATA]
        IMPORT audio_quiet_time [DATA]
        IMPORT reset_en [DATA]
        IMPORT Dwhp83_CuMu [DATA]
        IMPORT stopFlashLed [CODE]
        IMPORT ampPowerDown [CODE]
        IMPORT logEvent [CODE]
        IMPORT logClose [CODE]
        IMPORT breath_flag [DATA]
        IMPORT volume_back [DATA]
        IMPORT setConfig [CODE]
        IMPORT ampMute [CODE]
        IMPORT DrvWDT_EnableInt [CODE]

        KEEP GPIO_SET_MODE
        KEEP SYS_UnlockReg
        KEEP WDT_CLEAR_RESET_FLAG
        KEEP WDT_ENABLE_COUNTING
        KEEP WDT_TIMER_RESET
        KEEP openMcuCtrlPin

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
