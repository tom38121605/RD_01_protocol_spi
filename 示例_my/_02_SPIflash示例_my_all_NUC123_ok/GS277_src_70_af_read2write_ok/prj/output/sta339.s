; generated by ARM C/C++ Compiler, 5.02 [Build 28]
; commandline ArmCC [--min_array_alignment=4 --split_sections --debug -c --asm --info=totals -o.\output\sta339.o --depend=.\output\sta339.d --cpu=Cortex-M0 --apcs=interwork -I..\CMSIS\CM0\DeviceSupport\Nuvoton\NUC123Series -I..\CMSIS\CM0\CoreSupport -I..\INC\DrvInc -I..\INC\SysInc -IC:\Keil\ARM\RV31\INC -IC:\Keil\ARM\CMSIS\Include -D__MICROLIB --omf_browse=.\output\sta339.crf ..\AppSrc\sta339.c]
        THUMB
        REQUIRE8
        PRESERVE8

        AREA ||i.SYS_UnlockReg||, CODE, READONLY, ALIGN=2

SYS_UnlockReg PROC
        LDR      r0,|L1.16|
        MOVS     r1,#0x59
        STR      r1,[r0,#0]
        MOVS     r1,#0x16
        STR      r1,[r0,#0]
        MOVS     r1,#0x88
        STR      r1,[r0,#0]
        BX       lr
        ENDP

|L1.16|
        DCD      0x50000100

        AREA ||i.WDT_TIMER_RESET||, CODE, READONLY, ALIGN=2

WDT_TIMER_RESET PROC
        LDR      r0,|L2.12|
        LDR      r1,[r0,#0]
        MOVS     r2,#1
        ORRS     r1,r1,r2
        STR      r1,[r0,#0]
        BX       lr
        ENDP

|L2.12|
        DCD      0x40004000

        AREA ||i.amp1EQbypass||, CODE, READONLY, ALIGN=1

amp1EQbypass PROC
        PUSH     {r4,lr}
        MOVS     r0,#0xe
        BL       ampGetCmd_38
        MOVS     r4,#2
        ORRS     r0,r0,r4
        MOV      r1,r0
        MOVS     r0,#0xe
        BL       ampSendCmd_38
        MOVS     r0,#0xf
        BL       ampGetCmd_38
        ORRS     r0,r0,r4
        MOV      r1,r0
        MOVS     r0,#0xf
        BL       ampSendCmd_38
        MOVS     r0,#0x10
        BL       ampGetCmd_38
        ORRS     r0,r0,r4
        MOV      r1,r0
        MOVS     r0,#0x10
        BL       ampSendCmd_38
        POP      {r4,pc}
        ENDP


        AREA ||i.amp1EQon||, CODE, READONLY, ALIGN=1

amp1EQon PROC
        PUSH     {r4,lr}
        MOVS     r0,#0xe
        BL       ampGetCmd_38
        MOVS     r4,#0xfd
        ANDS     r0,r0,r4
        MOV      r1,r0
        MOVS     r0,#0xe
        BL       ampSendCmd_38
        MOVS     r0,#0xf
        BL       ampGetCmd_38
        ANDS     r0,r0,r4
        MOV      r1,r0
        MOVS     r0,#0xf
        BL       ampSendCmd_38
        MOVS     r0,#0x10
        BL       ampGetCmd_38
        ANDS     r0,r0,r4
        MOV      r1,r0
        MOVS     r0,#0x10
        BL       ampSendCmd_38
        POP      {r4,pc}
        ENDP


        AREA ||i.amp2EQbypass||, CODE, READONLY, ALIGN=1

amp2EQbypass PROC
        PUSH     {r4,lr}
        MOVS     r0,#0xe
        BL       ampGetCmd_3A
        MOVS     r4,#2
        ORRS     r0,r0,r4
        MOV      r1,r0
        MOVS     r0,#0xe
        BL       ampSendCmd_3A
        MOVS     r0,#0xf
        BL       ampGetCmd_3A
        ORRS     r0,r0,r4
        MOV      r1,r0
        MOVS     r0,#0xf
        BL       ampSendCmd_3A
        MOVS     r0,#0x10
        BL       ampGetCmd_3A
        ORRS     r0,r0,r4
        MOV      r1,r0
        MOVS     r0,#0x10
        BL       ampSendCmd_3A
        POP      {r4,pc}
        ENDP


        AREA ||i.amp2EQon||, CODE, READONLY, ALIGN=1

amp2EQon PROC
        PUSH     {r4,lr}
        MOVS     r0,#0xe
        BL       ampGetCmd_3A
        MOVS     r4,#0xfd
        ANDS     r0,r0,r4
        MOV      r1,r0
        MOVS     r0,#0xe
        BL       ampSendCmd_3A
        MOVS     r0,#0xf
        BL       ampGetCmd_3A
        ANDS     r0,r0,r4
        MOV      r1,r0
        MOVS     r0,#0xf
        BL       ampSendCmd_3A
        MOVS     r0,#0x10
        BL       ampGetCmd_3A
        ANDS     r0,r0,r4
        MOV      r1,r0
        MOVS     r0,#0x10
        BL       ampSendCmd_3A
        POP      {r4,pc}
        ENDP


        AREA ||i.ampAddVolume||, CODE, READONLY, ALIGN=2

ampAddVolume PROC
        PUSH     {r4,lr}
        LDR      r0,|L7.76|
        LDRB     r1,[r0,#0]  ; amp_status
        SUBS     r4,r0,#1
        LDRB     r0,[r4,#0]
        CMP      r1,#0xa2
        BEQ      |L7.64|
        LDR      r1,[r4,#4]  ; amp_volume
        CMP      r1,#0x1f
        BCS      |L7.62|
        ADDS     r1,r1,#1
        STR      r1,[r4,#4]  ; amp_volume
        LDR      r2,|L7.80|
        MOVS     r1,#4
        STR      r1,[r2,#0]  ; led_flash
        BL       stopATask
        LDR      r0,[r4,#4]  ; amp_volume
        BL       ampSetVolume
        BL       DWP83CuCV
        LDR      r0,|L7.84|
        LDRB     r0,[r0,#0]  ; dwhp83_type
        CMP      r0,#0xa0
        BEQ      |L7.62|
        LDR      r0,|L7.88|
        MOVS     r2,#2
        LDR      r1,[r0,#0]  ; Dwhp83_flag
        ORRS     r1,r1,r2
        STR      r1,[r0,#0]  ; Dwhp83_flag
|L7.62|
        POP      {r4,pc}
|L7.64|
        LDR      r3,|L7.92|
        MOVS     r2,#0
        MOVS     r1,#2
        BL       startATask
        POP      {r4,pc}
        ENDP

|L7.76|
        DCD      ||.data||+0x1
|L7.80|
        DCD      led_flash
|L7.84|
        DCD      dwhp83_type
|L7.88|
        DCD      Dwhp83_flag
|L7.92|
        DCD      ampDecVolume

        AREA ||i.ampDecVolume||, CODE, READONLY, ALIGN=2

ampDecVolume PROC
        PUSH     {r4,lr}
        LDR      r0,|L8.76|
        LDRB     r1,[r0,#0]  ; amp_status
        SUBS     r4,r0,#1
        LDRB     r0,[r4,#0]
        CMP      r1,#0xa2
        BEQ      |L8.64|
        LDR      r1,[r4,#4]  ; amp_volume
        CMP      r1,#0
        BEQ      |L8.62|
        SUBS     r1,r1,#1
        STR      r1,[r4,#4]  ; amp_volume
        LDR      r2,|L8.80|
        MOVS     r1,#4
        STR      r1,[r2,#0]  ; led_flash
        BL       stopATask
        LDR      r0,[r4,#4]  ; amp_volume
        BL       ampSetVolume
        BL       DWP83CuCV
        LDR      r0,|L8.84|
        LDRB     r0,[r0,#0]  ; dwhp83_type
        CMP      r0,#0xa0
        BEQ      |L8.62|
        LDR      r0,|L8.88|
        MOVS     r2,#2
        LDR      r1,[r0,#0]  ; Dwhp83_flag
        ORRS     r1,r1,r2
        STR      r1,[r0,#0]  ; Dwhp83_flag
|L8.62|
        POP      {r4,pc}
|L8.64|
        LDR      r3,|L8.92|
        MOVS     r2,#0
        MOVS     r1,#2
        BL       startATask
        POP      {r4,pc}
        ENDP

|L8.76|
        DCD      ||.data||+0x1
|L8.80|
        DCD      led_flash
|L8.84|
        DCD      dwhp83_type
|L8.88|
        DCD      Dwhp83_flag
|L8.92|
        DCD      ampDecVolume

        AREA ||i.ampFlasfLed||, CODE, READONLY, ALIGN=2

ampFlasfLed PROC
        PUSH     {r4,lr}
        LDR      r1,|L9.24|
        MOVS     r0,#4
        STR      r0,[r1,#0]  ; led_flash
        LDR      r0,|L9.32|
        LDR      r3,|L9.28|
        MOVS     r2,#0
        MOVS     r1,#0xf0
        LDRB     r0,[r0,#0]  ; amp_id
        BL       startATask
        POP      {r4,pc}
        ENDP

|L9.24|
        DCD      led_flash
|L9.28|
        DCD      ampFlasfLed
|L9.32|
        DCD      ||.data||

        AREA ||i.ampGetCmd_38||, CODE, READONLY, ALIGN=2

ampGetCmd_38 PROC
        PUSH     {r2-r6,lr}
        MOV      r5,r0
        MOVS     r0,#1
        LDR      r4,|L10.52|
        MOV      r2,r0
        STR      r0,[sp,#0]
        MOVS     r3,#2
        MOVS     r1,#0x38
        STR      r4,[sp,#4]
        LDR      r0,|L10.56|
        BL       initI2cDataBlock
        STRB     r5,[r4,#1]
        MOVS     r0,#0
        STRB     r0,[r4,#2]
        MOVS     r0,#0xa2
        STRB     r0,[r4,#0]
        MOVS     r1,#0
        LDR      r0,|L10.56|
        BL       addI2cChain
|L10.42|
        LDRB     r0,[r4,#0]  ; amp_status
        CMP      r0,#0xa2
        BEQ      |L10.42|
        LDRB     r0,[r4,#2]  ; amp_status
        POP      {r2-r6,pc}
        ENDP

|L10.52|
        DCD      ||.data||+0x1
|L10.56|
        DCD      ||.bss||

        AREA ||i.ampGetCmd_3A||, CODE, READONLY, ALIGN=2

ampGetCmd_3A PROC
        PUSH     {r2-r6,lr}
        MOV      r5,r0
        MOVS     r0,#1
        LDR      r4,|L11.52|
        MOV      r2,r0
        STR      r0,[sp,#0]
        MOVS     r3,#2
        MOVS     r1,#0x3a
        STR      r4,[sp,#4]
        LDR      r0,|L11.56|
        BL       initI2cDataBlock
        STRB     r5,[r4,#1]
        MOVS     r0,#0
        STRB     r0,[r4,#2]
        MOVS     r0,#0xa2
        STRB     r0,[r4,#0]
        MOVS     r1,#0
        LDR      r0,|L11.56|
        BL       addI2cChain
|L11.42|
        LDRB     r0,[r4,#0]  ; amp_status
        CMP      r0,#0xa2
        BEQ      |L11.42|
        LDRB     r0,[r4,#2]  ; amp_status
        POP      {r2-r6,pc}
        ENDP

|L11.52|
        DCD      ||.data||+0x1
|L11.56|
        DCD      ||.bss||

        AREA ||i.ampGetStatus||, CODE, READONLY, ALIGN=2

ampGetStatus PROC
        PUSH     {r1-r7,lr}
        MOVS     r4,#0
        LDR      r7,|L12.120|
        LDR      r5,|L12.124|
        MOV      r6,r4
|L12.10|
        STRB     r4,[r5,#1]
        STRB     r6,[r5,#2]
        MOVS     r0,#0xa2
        STRB     r0,[r5,#0]
        MOVS     r0,#1
        MOV      r2,r0
        STR      r0,[sp,#0]
        LDR      r0,|L12.120|
        MOVS     r3,#2
        MOVS     r1,#0x38
        STR      r5,[sp,#4]
        SUBS     r0,r0,#0x10
        BL       initI2cDataBlock
        LDR      r0,|L12.120|
        MOVS     r1,#0
        SUBS     r0,r0,#0x10
        BL       addI2cChain
|L12.48|
        LDRB     r0,[r5,#0]  ; amp_status
        CMP      r0,#0xa2
        BEQ      |L12.48|
        LDRB     r0,[r5,#2]  ; amp_status
        STRB     r0,[r7,r4]
        BL       SYS_UnlockReg
        BL       WDT_TIMER_RESET
        LDR      r0,|L12.128|
        STR      r6,[r0,#0]
        ADDS     r4,r4,#1
        UXTB     r4,r4
        CMP      r4,#0x16
        BCC      |L12.10|
        MOVS     r0,#0x2d
        STRB     r0,[r5,#1]
        STRB     r6,[r5,#2]
        MOVS     r0,#0xa2
        STRB     r0,[r5,#0]
        LDR      r1,|L12.120|
        MOVS     r0,#0x38
        SUBS     r1,r1,#0x10
        STRB     r0,[r1,#0]
        LDR      r0,|L12.120|
        MOVS     r1,#0
        SUBS     r0,r0,#0x10
        BL       addI2cChain
|L12.106|
        LDRB     r0,[r5,#0]  ; amp_status
        CMP      r0,#0xa2
        BEQ      |L12.106|
        LDRB     r0,[r5,#2]  ; amp_status
        STRB     r0,[r7,#0x16]
        POP      {r1-r7,pc}
        ENDP

        DCW      0x0000
|L12.120|
        DCD      ||.bss||+0x10
|L12.124|
        DCD      ||.data||+0x1
|L12.128|
        DCD      0x50000100

        AREA ||i.ampInitAllConf||, CODE, READONLY, ALIGN=2

ampInitAllConf PROC
        PUSH     {r3-r7,lr}
        MOVS     r4,#0
        LDR      r7,|L13.136|
        LDR      r5,|L13.140|
        MOV      r6,r4
|L13.10|
        ADDS     r0,r5,r4
        LDRB     r1,[r0,#1]
        LDRB     r0,[r5,r4]
        BL       ampSendCmd_3A
        ADDS     r4,r4,#2
        BL       SYS_UnlockReg
        BL       WDT_TIMER_RESET
        STR      r6,[r7,#0]
        CMP      r4,#0x58
        BCC      |L13.10|
        MOVS     r1,#0xdf
        MOVS     r0,#5
        BL       ampSendCmd_3A
        LDR      r5,|L13.140|
        MOVS     r4,#0
        SUBS     r5,r5,#0x58
|L13.50|
        ADDS     r0,r5,r4
        LDRB     r1,[r0,#1]
        LDRB     r0,[r5,r4]
        BL       ampSendCmd_38
        ADDS     r4,r4,#2
        BL       SYS_UnlockReg
        BL       WDT_TIMER_RESET
        STR      r6,[r7,#0]
        CMP      r4,#0x58
        BCC      |L13.50|
        MOVS     r1,#0xdc
        MOVS     r0,#5
        BL       ampSendCmd_38
        MOVS     r4,#0
|L13.86|
        ADDS     r0,r5,r4
        LDRB     r1,[r0,#1]
        LDRB     r0,[r5,r4]
        BL       writeAmp38Cmd
        ADDS     r4,r4,#2
        BL       SYS_UnlockReg
        BL       WDT_TIMER_RESET
        STR      r6,[r7,#0]
        CMP      r4,#0x58
        BCC      |L13.86|
        MOVS     r1,#0xdc
        MOVS     r0,#5
        BL       writeAmp38Cmd
        BL       exitGpioI2c
        BL       getATask
        LDR      r1,|L13.140|
        SUBS     r1,r1,#0xa4
        STRB     r0,[r1,#0]
        POP      {r3-r7,pc}
        ENDP

|L13.136|
        DCD      0x50000100
|L13.140|
        DCD      ||.data||+0xa4

        AREA ||i.ampMute||, CODE, READONLY, ALIGN=1

ampMute PROC
        PUSH     {r4,lr}
        MOVS     r1,#0x1e
        MOVS     r0,#6
        BL       ampSendCmd_38
        POP      {r4,pc}
        ENDP


        AREA ||i.ampPowerDown||, CODE, READONLY, ALIGN=1

ampPowerDown PROC
        PUSH     {r4,lr}
        MOVS     r1,#0x7c
        MOVS     r0,#5
        BL       writeAmp38Cmd
        POP      {r4,pc}
        ENDP


        AREA ||i.ampResumeI2c||, CODE, READONLY, ALIGN=2

ampResumeI2c PROC
        LDR      r0,|L16.16|
        LDRB     r1,[r0,#4]  ; amp_i2c_block
        STRB     r1,[r0,#3]
        LDR      r1,|L16.20|
        MOVS     r0,#0xa0
        STRB     r0,[r1,#0]
        BX       lr
        ENDP

        DCW      0x0000
|L16.16|
        DCD      ||.bss||
|L16.20|
        DCD      ||.data||+0x1

        AREA ||i.ampSendCmd_38||, CODE, READONLY, ALIGN=2

ampSendCmd_38 PROC
        PUSH     {r2-r6,lr}
        MOV      r5,r0
        MOVS     r0,#1
        MOV      r6,r1
        LDR      r4,|L17.52|
        STR      r0,[sp,#0]
        MOVS     r3,#2
        MOVS     r2,#0
        MOVS     r1,#0x38
        STR      r4,[sp,#4]
        LDR      r0,|L17.56|
        BL       initI2cDataBlock
        STRB     r5,[r4,#1]
        STRB     r6,[r4,#2]
        MOVS     r0,#0xa2
        STRB     r0,[r4,#0]
        MOVS     r1,#0
        LDR      r0,|L17.56|
        BL       addI2cChain
|L17.42|
        LDRB     r0,[r4,#0]  ; amp_status
        CMP      r0,#0xa2
        BEQ      |L17.42|
        POP      {r2-r6,pc}
        ENDP

        DCW      0x0000
|L17.52|
        DCD      ||.data||+0x1
|L17.56|
        DCD      ||.bss||

        AREA ||i.ampSendCmd_3A||, CODE, READONLY, ALIGN=2

ampSendCmd_3A PROC
        PUSH     {r2-r6,lr}
        MOV      r5,r0
        MOVS     r0,#1
        MOV      r6,r1
        LDR      r4,|L18.52|
        STR      r0,[sp,#0]
        MOVS     r3,#2
        MOVS     r2,#0
        MOVS     r1,#0x3a
        STR      r4,[sp,#4]
        LDR      r0,|L18.56|
        BL       initI2cDataBlock
        STRB     r5,[r4,#1]
        STRB     r6,[r4,#2]
        MOVS     r0,#0xa2
        STRB     r0,[r4,#0]
        MOVS     r1,#0
        LDR      r0,|L18.56|
        BL       addI2cChain
|L18.42|
        LDRB     r0,[r4,#0]  ; amp_status
        CMP      r0,#0xa2
        BEQ      |L18.42|
        POP      {r2-r6,pc}
        ENDP

        DCW      0x0000
|L18.52|
        DCD      ||.data||+0x1
|L18.56|
        DCD      ||.bss||

        AREA ||i.ampSendEqs_38||, CODE, READONLY, ALIGN=2

ampSendEqs_38 PROC
        PUSH     {r4-r6,lr}
        MOV      r6,r1
        CMP      r0,#0x3f
        BHI      |L19.92|
        CMP      r6,#0x3f
        BHI      |L19.92|
        CMP      r0,r6
        BHI      |L19.92|
        MOV      r4,r0
        B        |L19.88|
|L19.20|
        MOVS     r1,#0
        MOVS     r0,#0x31
        BL       ampSendCmd_38
        UXTB     r1,r4
        MOVS     r0,#0x16
        BL       ampSendCmd_38
        LDR      r1,|L19.96|
        LSLS     r0,r4,#2
        ADDS     r1,r0,r1
        LDR      r0,|L19.96|
        LDR      r1,[r1,#0]
        SUBS     r0,r0,#0xfc
        MOV      r5,r0
        STR      r1,[r0,#8]  ; cfg_data
        ADDS     r5,r5,#8
        LDRB     r1,[r5,#2]  ; cfg_data
        MOVS     r0,#0x17
        BL       ampSendCmd_38
        LDRB     r1,[r5,#1]  ; cfg_data
        MOVS     r0,#0x18
        BL       ampSendCmd_38
        LDRB     r1,[r5,#0]  ; cfg_data
        MOVS     r0,#0x19
        BL       ampSendCmd_38
        MOVS     r1,#1
        MOVS     r0,#0x26
        BL       ampSendCmd_38
        ADDS     r4,r4,#1
|L19.88|
        CMP      r4,r6
        BLS      |L19.20|
|L19.92|
        POP      {r4-r6,pc}
        ENDP

        DCW      0x0000
|L19.96|
        DCD      ||.data||+0xfc

        AREA ||i.ampSendEqs_38b||, CODE, READONLY, ALIGN=2

ampSendEqs_38b PROC
        PUSH     {r4-r6,lr}
        MOV      r6,r1
        CMP      r0,#0x3f
        BHI      |L20.90|
        CMP      r6,#0x3f
        BHI      |L20.90|
        CMP      r0,r6
        BHI      |L20.90|
        MOV      r4,r0
        B        |L20.86|
|L20.20|
        MOVS     r1,#0
        MOVS     r0,#0x31
        BL       writeAmp38Cmd
        UXTB     r1,r4
        MOVS     r0,#0x16
        BL       writeAmp38Cmd
        LDR      r1,|L20.92|
        LSLS     r0,r4,#2
        ADDS     r1,r0,r1
        LDR      r0,|L20.96|
        LDR      r1,[r1,#0]
        MOV      r5,r0
        STR      r1,[r0,#8]  ; cfg_data
        ADDS     r5,r5,#8
        LDRB     r1,[r5,#2]  ; cfg_data
        MOVS     r0,#0x17
        BL       writeAmp38Cmd
        LDRB     r1,[r5,#1]  ; cfg_data
        MOVS     r0,#0x18
        BL       writeAmp38Cmd
        LDRB     r1,[r5,#0]  ; cfg_data
        MOVS     r0,#0x19
        BL       writeAmp38Cmd
        MOVS     r1,#1
        MOVS     r0,#0x26
        BL       writeAmp38Cmd
        ADDS     r4,r4,#1
|L20.86|
        CMP      r4,r6
        BLS      |L20.20|
|L20.90|
        POP      {r4-r6,pc}
        ENDP

|L20.92|
        DCD      ||.data||+0x1fc
|L20.96|
        DCD      ||.data||

        AREA ||i.ampSendEqs_3A||, CODE, READONLY, ALIGN=2

ampSendEqs_3A PROC
        PUSH     {r4-r6,lr}
        MOV      r6,r1
        CMP      r0,#0x3f
        BHI      |L21.90|
        CMP      r6,#0x3f
        BHI      |L21.90|
        CMP      r0,r6
        BHI      |L21.90|
        MOV      r4,r0
        B        |L21.86|
|L21.20|
        MOVS     r1,#0
        MOVS     r0,#0x31
        BL       ampSendCmd_3A
        UXTB     r1,r4
        MOVS     r0,#0x16
        BL       ampSendCmd_3A
        LDR      r1,|L21.92|
        LSLS     r0,r4,#2
        ADDS     r1,r0,r1
        LDR      r0,|L21.96|
        LDR      r1,[r1,#0]
        MOV      r5,r0
        STR      r1,[r0,#8]  ; cfg_data
        ADDS     r5,r5,#8
        LDRB     r1,[r5,#2]  ; cfg_data
        MOVS     r0,#0x17
        BL       ampSendCmd_3A
        LDRB     r1,[r5,#1]  ; cfg_data
        MOVS     r0,#0x18
        BL       ampSendCmd_3A
        LDRB     r1,[r5,#0]  ; cfg_data
        MOVS     r0,#0x19
        BL       ampSendCmd_3A
        MOVS     r1,#1
        MOVS     r0,#0x26
        BL       ampSendCmd_3A
        ADDS     r4,r4,#1
|L21.86|
        CMP      r4,r6
        BLS      |L21.20|
|L21.90|
        POP      {r4-r6,pc}
        ENDP

|L21.92|
        DCD      ||.data||+0x2fc
|L21.96|
        DCD      ||.data||

        AREA ||i.ampSetEqDefault||, CODE, READONLY, ALIGN=2

ampSetEqDefault PROC
        PUSH     {r3-r7,lr}
        MOV      r5,r0
        MOVS     r4,#0
        MOV      r7,r2
        MOV      r1,r4
        MOVS     r0,#0x31
        BL       ampSendCmd_3A
        UXTB     r1,r5
        MOVS     r0,#0x16
        BL       ampSendCmd_3A
        LDR      r6,|L22.92|
|L22.26|
        LSLS     r0,r4,#2
        ADDS     r1,r0,r7
        LDR      r0,|L22.92|
        LDR      r1,[r1,#0]
        SUBS     r0,r0,#8
        STR      r1,[r0,#8]  ; cfg_data
        LSLS     r0,r4,#1
        ADDS     r5,r4,r0
        MOV      r0,r5
        ADDS     r0,r0,#0x17
        LDRB     r1,[r6,#2]  ; cfg_data
        UXTB     r0,r0
        BL       ampSendCmd_3A
        MOV      r0,r5
        ADDS     r0,r0,#0x18
        LDRB     r1,[r6,#1]  ; cfg_data
        UXTB     r0,r0
        BL       ampSendCmd_3A
        ADDS     r5,r5,#0x19
        LDRB     r1,[r6,#0]  ; cfg_data
        UXTB     r0,r5
        BL       ampSendCmd_3A
        ADDS     r4,r4,#1
        CMP      r4,#5
        BCC      |L22.26|
        MOVS     r1,#2
        MOVS     r0,#0x26
        BL       ampSendCmd_3A
        POP      {r3-r7,pc}
        ENDP

|L22.92|
        DCD      ||.data||+0x8

        AREA ||i.ampSetVolume||, CODE, READONLY, ALIGN=2

ampSetVolume PROC
        PUSH     {r4-r6,lr}
        CMP      r0,#0x1f
        BEQ      |L23.24|
        CMP      r0,#0x1e
        BEQ      |L23.28|
        CMP      r0,#0x1d
        BEQ      |L23.34|
        CMP      r0,#0x1c
        BEQ      |L23.40|
        CMP      r0,#0x1b
        BEQ      |L23.46|
        B        |L23.58|
|L23.24|
        LDR      r2,|L23.128|
        B        |L23.50|
|L23.28|
        LDR      r2,|L23.128|
        ADDS     r2,r2,#0x14
        B        |L23.50|
|L23.34|
        LDR      r2,|L23.128|
        ADDS     r2,r2,#0x28
        B        |L23.50|
|L23.40|
        LDR      r2,|L23.128|
        ADDS     r2,r2,#0x3c
        B        |L23.50|
|L23.46|
        LDR      r2,|L23.128|
        ADDS     r2,r2,#0x50
|L23.50|
        MOVS     r1,#4
        MOVS     r0,#0
        BL       ampSetEqDefault
|L23.58|
        LDR      r5,|L23.132|
        MOV      r4,r5
        SUBS     r4,r4,#0xc
        LDR      r0,[r4,#4]  ; amp_volume
        LDRB     r1,[r5,r0]
        MOVS     r0,#8
        BL       writeAmp38Cmd
        LDR      r0,[r4,#4]  ; amp_volume
        LDRB     r1,[r5,r0]
        MOVS     r0,#9
        BL       writeAmp38Cmd
        LDR      r0,[r4,#4]  ; amp_volume
        LDRB     r1,[r5,r0]
        MOVS     r0,#8
        BL       ampSendCmd_38
        LDR      r0,[r4,#4]  ; amp_volume
        LDRB     r1,[r5,r0]
        MOVS     r0,#9
        BL       ampSendCmd_38
        MOV      r0,r4
        LDR      r1,[r4,#4]  ; amp_volume
        ADDS     r0,r0,#0x2c
        LDRB     r1,[r0,r1]
        MOVS     r0,#0xa
        BL       ampSendCmd_3A
        LDR      r0,[r4,#4]  ; amp_volume
        BL       pcm5121SetVolume
        POP      {r4-r6,pc}
        ENDP

        DCW      0x0000
|L23.128|
        DCD      ||.data||+0x3fc
|L23.132|
        DCD      ||.data||+0xc

        AREA ||i.ampunMute||, CODE, READONLY, ALIGN=1

ampunMute PROC
        PUSH     {r4,lr}
        MOVS     r1,#0x10
        MOVS     r0,#6
        BL       ampSendCmd_38
        POP      {r4,pc}
        ENDP


        AREA ||i.callOffMode||, CODE, READONLY, ALIGN=1

callOffMode PROC
        PUSH     {r4,lr}
        BL       DWHP83UnMute
        MOVS     r0,#5
        BL       sysDelayMs
        MOVS     r0,#0
        BL       seW58SetBypass
        MOVS     r1,#0x10
        MOVS     r0,#6
        BL       writeAmp38Cmd
        MOVS     r1,#0x10
        MOVS     r0,#6
        BL       ampSendCmd_3A
        POP      {r4,pc}
        ENDP


        AREA ||i.callOnMode||, CODE, READONLY, ALIGN=1

callOnMode PROC
        PUSH     {r4,lr}
        BL       DWHP83Mute
        MOVS     r0,#5
        BL       sysDelayMs
        MOVS     r0,#1
        BL       seW58SetBypass
        MOVS     r1,#0x1e
        MOVS     r0,#6
        BL       writeAmp38Cmd
        MOVS     r1,#0x1e
        MOVS     r0,#6
        BL       ampSendCmd_3A
        POP      {r4,pc}
        ENDP


        AREA ||i.stopATask||, CODE, READONLY, ALIGN=2

stopATask PROC
        MOVS     r2,#0xc
        MULS     r0,r2,r0
        LDR      r2,|L27.16|
        MOVS     r1,#0
        ADDS     r0,r0,r2
        STRB     r1,[r0,#1]
        STR      r1,[r0,#8]
        BX       lr
        ENDP

|L27.16|
        DCD      mytask

        AREA ||i.stopAmpFlash||, CODE, READONLY, ALIGN=2

stopAmpFlash PROC
        PUSH     {r4,lr}
        LDR      r0,|L28.12|
        LDRB     r0,[r0,#0]  ; amp_id
        BL       stopATask
        POP      {r4,pc}
        ENDP

|L28.12|
        DCD      ||.data||

        AREA ||.arm_vfe_header||, DATA, READONLY, NOALLOC, ALIGN=2

        DCD      0x00000000

        AREA ||.bss||, DATA, NOINIT, ALIGN=2

amp_i2c_block
        %        16
amp_info
        %        23

        AREA ||.data||, DATA, ALIGN=2

amp_id
        DCB      0x00
amp_status
        DCB      0x00,0x00,0x00
amp_volume
        DCD      0x00000018
cfg_data
        DCD      0x00000000
amp_treble_tbl
        DCB      0xff,0xd4,0xd0,0xcc
        DCB      0xc8,0xc4,0xc0,0xbc
        DCB      0xb8,0xb4,0xb0,0xac
        DCB      0xa8,0xa4,0xa0,0x9c
        DCB      0x98,0x94,0x90,0x8c
        DCB      0x88,0x84,0x80,0x7c
        DCB      0x78,0x74,0x70,0x6c
        DCB      0x68,0x64,0x60,0x5c
amp_bass_tbl
        DCB      0xff,0xbb,0xb9,0xb6
        DCB      0xb3,0xb0,0xac,0xa9
        DCB      0xa6,0xa2,0x9e,0x9b
        DCB      0x97,0x94,0x90,0x8c
        DCB      0x89,0x85,0x81,0x7e
        DCB      0x7a,0x76,0x73,0x6f
        DCB      0x6b,0x67,0x63,0x5f
        DCB      0x5b,0x57,0x53,0x4f
amp_conf_38
        DCB      0x00,0x63,0x01,0x80
        DCB      0x02,0x97,0x03,0x50
        DCB      0x04,0xc2,0x05,0xdc
        DCB      0x06,0x10,0x07,0x08
        DCB      0x08,0x54,0x09,0x54
        DCB      0x0a,0x60,0x0b,0x80
        DCB      0x0c,0x00,0x0d,0x00
        DCB      0x0e,0x00,0x0f,0x40
        DCB      0x10,0x80,0x11,0x77
        DCB      0x12,0x6a,0x13,0x69
        DCB      0x14,0x6a,0x15,0x69
        DCB      0x27,0x1a,0x28,0xc0
        DCB      0x29,0xf3,0x2a,0x33
        DCB      0x2b,0x00,0x2c,0x0c
        DCB      0x2d,0x7f,0x31,0x00
        DCB      0x32,0xa8,0x33,0x98
        DCB      0x34,0x30,0x35,0x30
        DCB      0x36,0x07,0x37,0x00
        DCB      0x38,0x00,0x39,0x01
        DCB      0x3a,0xee,0x3b,0xff
        DCB      0x3c,0x7e,0x3d,0xc0
        DCB      0x3e,0x26,0x3f,0x00
amp_conf_3A
        DCB      0x00,0x63,0x01,0x80
        DCB      0x02,0x97,0x03,0x50
        DCB      0x04,0xc2,0x05,0xdf
        DCB      0x06,0x16,0x07,0x04
        DCB      0x08,0x69,0x09,0x56
        DCB      0x0a,0x3a,0x0b,0x80
        DCB      0x0c,0x00,0x0d,0x00
        DCB      0x0e,0x00,0x0f,0x40
        DCB      0x10,0x90,0x11,0x77
        DCB      0x12,0x2d,0x13,0x69
        DCB      0x14,0x6a,0x15,0x69
        DCB      0x27,0x1a,0x28,0xc0
        DCB      0x29,0xf3,0x2a,0x33
        DCB      0x2b,0x00,0x2c,0x0c
        DCB      0x2d,0x7f,0x31,0x00
        DCB      0x32,0xc4,0x33,0xb0
        DCB      0x34,0x30,0x35,0x30
        DCB      0x36,0x07,0x37,0x00
        DCB      0x38,0x00,0x39,0x01
        DCB      0x3a,0xee,0x3b,0xff
        DCB      0x3c,0x7e,0x3d,0xc0
        DCB      0x3e,0x26,0x3f,0x00
amp_eq_tbl_38
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00400000
        DCD      0x008067d1
        DCD      0x007f580e
        DCD      0x007f982f
        DCD      0x0080c4aa
        DCD      0x003ff1a4
        DCD      0x00807d55
        DCD      0x007f3540
        DCD      0x007f82ab
        DCD      0x0080e1fd
        DCD      0x003ff461
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00400000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00400000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00400000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00400000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00400000
        DCD      0x00808b06
        DCD      0x007f74fa
        DCD      0x007f74a0
        DCD      0x00811559
        DCD      0x003fba7d
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00400000
        DCD      0x007fffff
        DCD      0x007fffff
        DCD      0x007fffff
        DCD      0x007fffff
        DCD      0x007fffff
        DCD      0x005a9df7
        DCD      0x007fffff
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x007fffff
        DCD      0x00400000
        DCD      0x00400000
        DCD      0x00000000
        DCD      0x00000000
amp_eq_tbl_38_backward
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00400000
        DCD      0x008067d1
        DCD      0x007f580e
        DCD      0x007f982f
        DCD      0x0080c4aa
        DCD      0x003ff1a4
        DCD      0x00807d55
        DCD      0x007f3540
        DCD      0x007f82ab
        DCD      0x0080e1fd
        DCD      0x003ff461
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00400000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00400000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00400000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00400000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00400000
        DCD      0x00808b06
        DCD      0x007f74fa
        DCD      0x007f74a0
        DCD      0x00811559
        DCD      0x003fba7d
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00400000
        DCD      0x007fffff
        DCD      0x007fffff
        DCD      0x007fffff
        DCD      0x007fffff
        DCD      0x007fffff
        DCD      0x005a9df7
        DCD      0x007fffff
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x007fffff
        DCD      0x00400000
        DCD      0x00400000
        DCD      0x00000000
        DCD      0x00000000
amp_eq_tbl_3A
        DCD      0x00805a7f
        DCD      0x007fa581
        DCD      0x007fa560
        DCD      0x0080b4be
        DCD      0x003fd2c0
        DCD      0x008088cc
        DCD      0x007ec2d7
        DCD      0x007f776c
        DCD      0x00811005
        DCD      0x004016ca
        DCD      0x00800bee
        DCD      0x007fdcae
        DCD      0x007ff412
        DCD      0x00801796
        DCD      0x004005de
        DCD      0x00803623
        DCD      0x007fa9c2
        DCD      0x007fc9dd
        DCD      0x008069c1
        DCD      0x003ff63e
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00400000
        DCD      0x008062de
        DCD      0x007ecd9c
        DCD      0x007f9d22
        DCD      0x0080c4d0
        DCD      0x004036c9
        DCD      0x00000273
        DCD      0x00000273
        DCD      0x007e6c92
        DCD      0x00831d0d
        DCD      0x00000139
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00400000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00400000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x00400000
        DCD      0x007fffff
        DCD      0x007fffff
        DCD      0x007fffff
        DCD      0x007fffff
        DCD      0x007fffff
        DCD      0x005a9df7
        DCD      0x007fffff
        DCD      0x00000000
        DCD      0x00000000
        DCD      0x007fffff
        DCD      0x00400000
        DCD      0x00400000
        DCD      0x00000000
        DCD      0x00000000
eq_1st
        DCD      0x00805a7f
        DCD      0x007fa581
        DCD      0x007fa560
        DCD      0x0080b4be
        DCD      0x003fd2c0
eq_2nd
        DCD      0x00805a7f
        DCD      0x007fa581
        DCD      0x007fa560
        DCD      0x0080b4be
        DCD      0x003fd2c0
eq_3rd
        DCD      0x00805a7f
        DCD      0x007fa581
        DCD      0x007fa560
        DCD      0x0080b4be
        DCD      0x003fd2c0
eq_4th
        DCD      0x00805a7f
        DCD      0x007fa581
        DCD      0x007fa560
        DCD      0x0080b4be
        DCD      0x003fd2c0
eq_5th
        DCD      0x00805a7f
        DCD      0x007fa581
        DCD      0x007fa560
        DCD      0x0080b4be
        DCD      0x003fd2c0

        AREA ||area_number.33||, DATA, ALIGN=2

        EXPORTAS ||area_number.33||, ||.data||
amp_conf_38_back
        DCB      0x00,0x63,0x01,0x80
        DCB      0x02,0x97,0x03,0x50
        DCB      0x04,0xc2,0x05,0xdc
        DCB      0x06,0x10,0x07,0x08
        DCB      0x08,0x54,0x09,0x54
        DCB      0x0a,0x60,0x0b,0x80
        DCB      0x0c,0x00,0x0d,0x00
        DCB      0x0e,0x00,0x0f,0x40
        DCB      0x10,0x80,0x11,0x77
        DCB      0x12,0x6a,0x13,0x69
        DCB      0x14,0x6a,0x15,0x69
        DCB      0x27,0x1a,0x28,0xc0
        DCB      0x29,0xf3,0x2a,0x33
        DCB      0x2b,0x00,0x2c,0x0c
        DCB      0x2d,0x7f,0x31,0x00
        DCB      0x32,0xa8,0x33,0x98
        DCB      0x34,0x30,0x35,0x30
        DCB      0x36,0x07,0x37,0x00
        DCB      0x38,0x00,0x39,0x01
        DCB      0x3a,0xee,0x3b,0xff
        DCB      0x3c,0x7e,0x3d,0xc0
        DCB      0x3e,0x26,0x3f,0x00

        EXPORT amp1EQbypass [CODE]
        EXPORT amp1EQon [CODE]
        EXPORT amp2EQbypass [CODE]
        EXPORT amp2EQon [CODE]
        EXPORT ampAddVolume [CODE]
        EXPORT ampDecVolume [CODE]
        EXPORT ampFlasfLed [CODE]
        EXPORT ampGetCmd_38 [CODE]
        EXPORT ampGetCmd_3A [CODE]
        EXPORT ampGetStatus [CODE]
        EXPORT ampInitAllConf [CODE]
        EXPORT ampMute [CODE]
        EXPORT ampPowerDown [CODE]
        EXPORT ampResumeI2c [CODE]
        EXPORT ampSendCmd_38 [CODE]
        EXPORT ampSendCmd_3A [CODE]
        EXPORT ampSendEqs_38 [CODE]
        EXPORT ampSendEqs_38b [CODE]
        EXPORT ampSendEqs_3A [CODE]
        EXPORT ampSetEqDefault [CODE]
        EXPORT ampSetVolume [CODE]
        EXPORT ampunMute [CODE]
        EXPORT callOffMode [CODE]
        EXPORT callOnMode [CODE]
        EXPORT stopAmpFlash [CODE]
        EXPORT amp_info [DATA,SIZE=23]
        EXPORT amp_id [DATA,SIZE=1]
        EXPORT amp_status [DATA,SIZE=3]
        EXPORT amp_volume [DATA,SIZE=4]
        EXPORT amp_treble_tbl [DATA,SIZE=32]
        EXPORT amp_bass_tbl [DATA,SIZE=32]
        EXPORT amp_conf_38 [DATA,SIZE=88]
        EXPORT amp_conf_3A [DATA,SIZE=88]
        EXPORT amp_conf_38_back [DATA,SIZE=88]

        IMPORT ||Lib$$Request$$armlib|| [CODE,WEAK]
        IMPORT initI2cDataBlock [CODE]
        IMPORT addI2cChain [CODE]
        IMPORT writeAmp38Cmd [CODE]
        IMPORT exitGpioI2c [CODE]
        IMPORT getATask [CODE]
        IMPORT startATask [CODE]
        IMPORT led_flash [DATA]
        IMPORT pcm5121SetVolume [CODE]
        IMPORT DWP83CuCV [CODE]
        IMPORT dwhp83_type [DATA]
        IMPORT Dwhp83_flag [DATA]
        IMPORT DWHP83Mute [CODE]
        IMPORT sysDelayMs [CODE]
        IMPORT seW58SetBypass [CODE]
        IMPORT DWHP83UnMute [CODE]
        IMPORT mytask [DATA]

        KEEP SYS_UnlockReg
        KEEP WDT_TIMER_RESET
        KEEP stopATask
        KEEP amp_i2c_block
        KEEP cfg_data
        KEEP amp_eq_tbl_38
        KEEP amp_eq_tbl_38_backward
        KEEP amp_eq_tbl_3A
        KEEP eq_1st
        KEEP eq_2nd
        KEEP eq_3rd
        KEEP eq_4th
        KEEP eq_5th

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
