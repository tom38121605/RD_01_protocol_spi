; generated by ARM C/C++ Compiler, 5.02 [Build 28]
; commandline ArmCC [--split_sections --debug -c --asm --info=totals -o.\output\adckey.o --depend=.\output\adckey.d --cpu=Cortex-M0 --apcs=interwork -I..\CMSIS\CM0\DeviceSupport\Nuvoton\NUC123Series -I..\CMSIS\CM0\CoreSupport -I..\INC\DrvInc -I..\INC\SysInc -IC:\Keil\ARM\RV31\INC -IC:\Keil\ARM\CMSIS\Include -D__MICROLIB --omf_browse=.\output\adckey.crf ..\AppSrc\adcKey.c]
        THUMB
        REQUIRE8
        PRESERVE8

        AREA ||i.ADC_GET_RSLT||, CODE, READONLY, ALIGN=2

ADC_GET_RSLT PROC
        LDR      r1,|L1.16|
        LSLS     r0,r0,#2
        ADDS     r0,r0,r1
        LDR      r0,[r0,#0]
        LSLS     r0,r0,#22
        LSRS     r0,r0,#22
        BX       lr
        ENDP

        DCW      0x0000
|L1.16|
        DCD      0x400e0000

        AREA ||i.ADC_IRQHandler||, CODE, READONLY, ALIGN=2

ADC_IRQHandler PROC
        PUSH     {r4,lr}
        LDR      r1,|L2.40|
        MOVS     r0,#1
        STR      r0,[r1,#0x30]
        MOVS     r0,#0
        BL       ADC_GET_RSLT
        MOV      r4,r0
        MOVS     r0,#4
        BL       ADC_GET_RSLT
        LDR      r1,|L2.44|
        STR      r0,[r1,#0x38]  ; bt_volage_adc
        MOVS     r0,#0
        STRH     r0,[r1,#0xe]
        MOV      r0,r4
        BL       adcKeyRemap
        POP      {r4,pc}
        ENDP

        DCW      0x0000
|L2.40|
        DCD      0x400e0000
|L2.44|
        DCD      ||.data||

        AREA ||i.DEVtest||, CODE, READONLY, ALIGN=1

DEVtest PROC
        BX       lr
        ENDP


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


        AREA ||i.adcKeyClkInit||, CODE, READONLY, ALIGN=2

adcKeyClkInit PROC
        LDR      r0,|L5.52|
        LDR      r1,[r0,#0x14]
        MOVS     r2,#0xc
        BICS     r1,r1,r2
        STR      r1,[r0,#0x14]
        LDR      r1,[r0,#0x18]
        MOVS     r2,#0xff
        LSLS     r2,r2,#16
        BICS     r1,r1,r2
        LSLS     r2,r0,#8
        ORRS     r1,r1,r2
        STR      r1,[r0,#0x18]
        LDR      r2,[r0,#8]
        LSLS     r1,r0,#19
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
        BX       lr
        ENDP

        DCW      0x0000
|L5.52|
        DCD      0x50000200

        AREA ||i.adcKeyHook||, CODE, READONLY, ALIGN=2

adcKeyHook PROC
        PUSH     {r4-r6,lr}
        BL       audioDetect
        MOVS     r5,#1
        MOVS     r4,#0
        LDR      r6,|L6.204|
        CMP      r0,#0
        BNE      |L6.60|
        LDR      r0,|L6.208|
        LDRB     r0,[r0,#0]  ; dev_state
        CMP      r0,#0
        BEQ      |L6.60|
        LDR      r0,[r6,#0x1c]  ; audio_quiet_time
        MOVS     r2,#0x7d
        ADDS     r0,r0,#1
        STR      r0,[r6,#0x1c]  ; audio_quiet_time
        LDR      r1,[r6,#0x14]  ; auto_standy_delay_minunte
        LSLS     r2,r2,#4
        MULS     r2,r1,r2
        CMP      r0,r2
        BLS      |L6.76|
        CMP      r1,#0xa
        BEQ      |L6.72|
        MOVS     r0,#0x81
|L6.48|
        BL       logEvent
        BL       enterStandby
|L6.56|
        STR      r4,[r6,#0x1c]  ; audio_quiet_time
        B        |L6.82|
|L6.60|
        LDR      r0,[r6,#0x20]  ; audio_open_time
        SUBS     r0,r0,#1
        STR      r0,[r6,#0x20]  ; audio_open_time
        BNE      |L6.82|
        STR      r5,[r6,#0x20]  ; audio_open_time
        B        |L6.56|
|L6.72|
        MOVS     r0,#0x80
        B        |L6.48|
|L6.76|
        CMP      r0,#0x63
        BLS      |L6.82|
        STR      r5,[r6,#0x20]  ; audio_open_time
|L6.82|
        LDR      r0,|L6.212|
        LDR      r1,|L6.216|
        LDRB     r0,[r0,#0]  ; WB_paired_flag
        LDRB     r1,[r1,#0]  ; dev_input
        LDR      r2,|L6.224|
        ORRS     r0,r0,r1
        LDR      r1,|L6.220|
        LDR      r0,[r2,#0]  ; led_saving_couter
        BEQ      |L6.120|
        ADDS     r0,r0,#1
        MOVS     r3,#0x7d
        LSLS     r3,r3,#4
        STR      r0,[r2,#0]  ; led_saving_couter
|L6.108|
        CMP      r0,r3
        BLS      |L6.128|
        STRB     r5,[r1,#0]
        SUBS     r0,r0,#1
        STR      r0,[r2,#0]  ; led_saving_couter
        B        |L6.130|
|L6.120|
        ADDS     r0,r0,#1
        LDR      r3,|L6.228|
        STR      r0,[r2,#0]  ; led_saving_couter
        B        |L6.108|
|L6.128|
        STRB     r4,[r1,#0]
|L6.130|
        LDR      r0,|L6.232|
        STR      r4,[r0,#0]  ; audio_L
        LDR      r0,|L6.236|
        STR      r4,[r0,#0]  ; audio_R
        LDR      r0,|L6.240|
        STR      r4,[r0,#0]  ; audio_detect_R
        LDR      r0,|L6.244|
        STR      r4,[r0,#0]  ; audio_detect_L
        LDR      r1,[r6,#0x38]  ; bt_volage_adc
        LDR      r0,[r6,#0x3c]  ; bt_volage
        ADDS     r1,r0,r1
        STR      r1,[r6,#0x3c]  ; bt_volage
        LDR      r0,[r6,#0x18]  ; bt_count
        ADDS     r0,r0,#1
        STR      r0,[r6,#0x18]  ; bt_count
        CMP      r0,#0x80
        BCC      |L6.196|
        LSRS     r0,r1,#7
        MOVS     r2,#9
        LSLS     r2,r2,#6
        LDR      r1,|L6.248|
        STR      r4,[r6,#0x18]  ; bt_count
        STR      r0,[r6,#0x3c]  ; bt_volage
        CMP      r0,r2
        BCS      |L6.184|
        MOVS     r0,#2
        B        |L6.194|
|L6.184|
        LDR      r0,|L6.252|
        LDR      r0,[r0,#0]
        CMP      r0,#0
        BEQ      |L6.198|
        MOVS     r0,#3
|L6.194|
        STRB     r0,[r1,#0]
|L6.196|
        POP      {r4-r6,pc}
|L6.198|
        STRB     r5,[r1,#0]
        POP      {r4-r6,pc}
        ENDP

        DCW      0x0000
|L6.204|
        DCD      ||.data||
|L6.208|
        DCD      dev_state
|L6.212|
        DCD      WB_paired_flag
|L6.216|
        DCD      dev_input
|L6.220|
        DCD      led_saving_power_flag
|L6.224|
        DCD      led_saving_couter
|L6.228|
        DCD      0x00002710
|L6.232|
        DCD      audio_L
|L6.236|
        DCD      audio_R
|L6.240|
        DCD      audio_detect_R
|L6.244|
        DCD      audio_detect_L
|L6.248|
        DCD      battery_state
|L6.252|
        DCD      0x500042a4

        AREA ||i.adcKeyInit||, CODE, READONLY, ALIGN=2

adcKeyInit PROC
        PUSH     {r3-r7,lr}
        MOVS     r1,#5
        LSLS     r1,r1,#28
        LDR      r0,[r1,#0x3c]
        MOVS     r5,#1
        ORRS     r0,r0,r5
        STR      r0,[r1,#0x3c]
        LDR      r0,|L7.104|
        LDR      r2,[r0,#0x14]
        LSLS     r6,r0,#10
        ORRS     r2,r2,r6
        STR      r2,[r0,#0x14]
        LDR      r2,[r1,#0x3c]
        MOVS     r3,#0x10
        ORRS     r2,r2,r3
        STR      r2,[r1,#0x3c]
        LDR      r1,[r0,#0x14]
        LSLS     r7,r0,#14
        ORRS     r1,r1,r7
        STR      r1,[r0,#0x14]
        LDR      r4,|L7.108|
        MOVS     r2,#0
        MOV      r1,r2
        MOV      r0,r4
        BL       GPIO_SET_MODE
        MOVS     r2,#0
        MOVS     r1,#4
        MOV      r0,r4
        BL       GPIO_SET_MODE
        LDR      r0,[r4,#4]
        ORRS     r0,r0,r6
        STR      r0,[r4,#4]
        LDR      r0,[r4,#4]
        ORRS     r0,r0,r7
        STR      r0,[r4,#4]
        LDR      r0,|L7.112|
        MOVS     r1,#8
        LDR      r2,[r0,#0x20]
        MOVS     r3,#0xc
        BICS     r2,r2,r3
        ORRS     r2,r2,r1
        STR      r2,[r0,#0x20]
        LDR      r2,[r0,#0x24]
        MOVS     r1,#0x11
        ORRS     r2,r2,r1
        STR      r2,[r0,#0x24]
        LDR      r1,[r0,#0x20]
        ORRS     r1,r1,r5
        STR      r1,[r0,#0x20]
        POP      {r3-r7,pc}
        ENDP

|L7.104|
        DCD      0x50000040
|L7.108|
        DCD      0x500040c0
|L7.112|
        DCD      0x400e0000

        AREA ||i.adcKeyRemap||, CODE, READONLY, ALIGN=2

adcKeyRemap PROC
        PUSH     {r4-r6,lr}
        LDR      r1,|L8.264|
        LDRH     r2,[r1,#8]  ; key_bak
        CMP      r0,r2
        BLS      |L8.14|
        SUBS     r2,r0,r2
        B        |L8.16|
|L8.14|
        SUBS     r2,r2,r0
|L8.16|
        MOVS     r3,#0
        CMP      r2,#0x73
        BLE      |L8.30|
        STRH     r3,[r1,#0xa]
        STRH     r3,[r1,#0xc]
        STRH     r0,[r1,#8]
|L8.28|
        POP      {r4-r6,pc}
|L8.30|
        LDRH     r2,[r1,#0xc]  ; key_adc_value
        ADDS     r2,r2,r0
        UXTH     r4,r2
        STRH     r4,[r1,#0xc]
        STRH     r0,[r1,#8]
        LDRH     r0,[r1,#0x10]  ; device_type_adc2
        LDRH     r2,[r1,#0xe]  ; device_type_adc1
        ADDS     r0,r0,r2
        UXTH     r2,r0
        STRH     r2,[r1,#0x10]
        LDRH     r0,[r1,#0xa]  ; key_irq_index
        ADDS     r0,r0,#1
        UXTH     r0,r0
        STRH     r0,[r1,#0xa]
        CMP      r0,#8
        BNE      |L8.28|
        LSRS     r0,r4,#3
        LSRS     r2,r2,#3
        STRH     r3,[r1,#0xa]
        STRH     r0,[r1,#0xc]
        MOV      r6,r2
        STRH     r2,[r1,#0x12]
        SUBS     r6,r6,#0xb9
        MOVS     r4,#0xa3
        LDR      r5,|L8.268|
        STRH     r3,[r1,#0x10]
        CMP      r6,#0x1f
        BCS      |L8.92|
        STRB     r4,[r5,#0]
        MOVS     r2,#0xa1
        B        |L8.118|
|L8.92|
        MOV      r6,r2
        SUBS     r6,r6,#0xff
        SUBS     r6,r6,#0x8a
        CMP      r6,#0x27
        BCS      |L8.106|
        STRB     r4,[r5,#0]
        B        |L8.136|
|L8.106|
        LDR      r6,|L8.272|
        ADDS     r6,r2,r6
        CMP      r6,#0x3f
        BCS      |L8.122|
        STRB     r4,[r5,#0]
        MOVS     r2,#0xa2
|L8.118|
        STR      r2,[r1,#0x28]  ; device_subt
        B        |L8.138|
|L8.122|
        LDR      r6,|L8.272|
        SUBS     r6,r6,#0xcc
        ADDS     r2,r2,r6
        CMP      r2,#0x2b
        BCS      |L8.138|
        MOVS     r2,#0xa0
        STRB     r2,[r5,#0]
|L8.136|
        STR      r4,[r1,#0x28]  ; device_subt
|L8.138|
        MOVS     r4,#0x7d
        LSLS     r4,r4,#3
        MOVS     r2,#0xb
        CMP      r0,r4
        BHI      |L8.250|
        LDR      r4,|L8.276|
        CMP      r0,r4
        BHI      |L8.250|
        LDR      r4,|L8.276|
        SUBS     r4,r4,#0x28
        CMP      r0,r4
        BLS      |L8.166|
        MOVS     r0,#9
        B        |L8.242|
|L8.166|
        MOVS     r4,#0x5f
        LSLS     r4,r4,#3
        CMP      r0,r4
        BHI      |L8.250|
        MOVS     r4,#0x2d
        LSLS     r4,r4,#4
        CMP      r0,r4
        BLS      |L8.186|
        MOVS     r0,#0xa
        B        |L8.242|
|L8.186|
        MOVS     r4,#0xa5
        LSLS     r4,r4,#2
        CMP      r0,r4
        BHI      |L8.250|
        MOVS     r4,#0x9b
        LSLS     r4,r4,#2
        CMP      r0,r4
        BLS      |L8.206|
        MOVS     r0,#0xc
        B        |L8.242|
|L8.206|
        LDR      r4,|L8.280|
        CMP      r0,r4
        BHI      |L8.250|
        LDR      r4,|L8.280|
        SUBS     r4,r4,#0x28
        CMP      r0,r4
        BLS      |L8.224|
        MOVS     r0,#8
        B        |L8.242|
|L8.224|
        MOVS     r4,#0xff
        ADDS     r4,r4,#0x7d
        CMP      r0,r4
        BHI      |L8.250|
        MOVS     r4,#0xff
        ADDS     r4,r4,#0x55
        CMP      r0,r4
        BLS      |L8.246|
        MOVS     r0,#7
|L8.242|
        STRB     r0,[r1,#3]
        B        |L8.252|
|L8.246|
        CMP      r0,#0
        BEQ      |L8.260|
|L8.250|
        STRB     r2,[r1,#3]
|L8.252|
        STRH     r3,[r1,#0xc]
        LDRB     r0,[r1,#3]  ; key_buf
        STRB     r0,[r1,#4]
        POP      {r4-r6,pc}
|L8.260|
        STRB     r3,[r1,#3]
        B        |L8.252|
        ENDP

|L8.264|
        DCD      ||.data||
|L8.268|
        DCD      dwhp83_type
|L8.272|
        DCD      0xfffffdb7
|L8.276|
        DCD      0x00000361
|L8.280|
        DCD      0x0000022b

        AREA ||i.adcReadKey||, CODE, READONLY, ALIGN=2

adcReadKey PROC
        LDR      r3,|L9.60|
        MOVS     r1,#0xff
        LDRB     r0,[r3,#3]  ; key_buf
        CMP      r0,#0xff
        BEQ      |L9.20|
        LDRB     r2,[r3,#6]  ; key_word_r
        CMP      r2,r0
        BNE      |L9.28|
        STRB     r1,[r3,#0]
        B        |L9.50|
|L9.20|
        STRB     r1,[r3,#0]
        STRB     r1,[r3,#7]
        STRB     r1,[r3,#6]
        B        |L9.56|
|L9.28|
        LDRB     r2,[r3,#7]  ; key_word_bak
        CMP      r2,#0xff
        BEQ      |L9.44|
        CMP      r2,r0
        BNE      |L9.44|
        STRB     r0,[r3,#0]
        STRB     r0,[r3,#6]
        B        |L9.50|
|L9.44|
        STRB     r0,[r3,#7]
        STRB     r1,[r3,#0]
        STRB     r1,[r3,#6]
|L9.50|
        CMP      r0,#0
        BNE      |L9.56|
        STRB     r0,[r3,#0]
|L9.56|
        LDRB     r0,[r3,#0]  ; key_word
        BX       lr
        ENDP

|L9.60|
        DCD      ||.data||

        AREA ||i.adcReadKeyContinue||, CODE, READONLY, ALIGN=2

adcReadKeyContinue PROC
        LDR      r2,|L10.60|
        MOVS     r0,#0xff
        STRB     r0,[r2,#1]
        LDRB     r1,[r2,#0]  ; key_word
        MOV      r0,r2
        LDRB     r0,[r0,#3]
        CMP      r1,#0xff
        BEQ      |L10.24|
        MOVS     r1,#5
        MVNS     r1,r1
        STR      r1,[r2,#0x2c]  ; kwcdc
        B        |L10.50|
|L10.24|
        LDR      r1,[r2,#0x2c]  ; kwcdc
        ADDS     r1,r1,#1
        STR      r1,[r2,#0x2c]  ; kwcdc
        CMP      r1,#1
        BGE      |L10.38|
        STRB     r0,[r2,#5]
        B        |L10.50|
|L10.38|
        MOVS     r1,#0
        STR      r1,[r2,#0x2c]  ; kwcdc
        LDRB     r1,[r2,#5]  ; key_c_r
        CMP      r1,r0
        BNE      |L10.50|
        STRB     r0,[r2,#1]
|L10.50|
        CMP      r0,#7
        BNE      |L10.56|
        STRB     r0,[r2,#1]
|L10.56|
        LDRB     r0,[r2,#1]  ; key_word_continue
        BX       lr
        ENDP

|L10.60|
        DCD      ||.data||

        AREA ||i.adckeyScan||, CODE, READONLY, ALIGN=2

adckeyScan PROC
        PUSH     {r4-r6,lr}
        BL       adcReadKey
        LDR      r4,|L11.124|
        STRB     r0,[r4,#0]
        BL       adcReadKeyContinue
        LDR      r5,|L11.128|
        STRB     r0,[r4,#1]
        LDRB     r0,[r5,#0]  ; dev_state
        CMP      r0,#0
        BEQ      |L11.50|
        CMP      r0,#1
        BEQ      |L11.34|
        CMP      r0,#2
        BNE      |L11.50|
        B        |L11.40|
|L11.34|
        BL       devStartDealAdcKey
        B        |L11.44|
|L11.40|
        BL       devStandbyDealAdcKey
|L11.44|
        LDRB     r0,[r5,#0]  ; dev_state
        CMP      r0,#2
        BEQ      |L11.54|
|L11.50|
        BL       adcKeyHook
|L11.54|
        LDR      r0,[r4,#0x24]  ; adcKeySubHook
        CMP      r0,#0
        BEQ      |L11.66|
        BLX      r0
        MOVS     r0,#0
        STR      r0,[r4,#0x24]  ; adcKeySubHook
|L11.66|
        LDR      r1,[r4,#0x34]  ; device_subt_b
        LDR      r0,[r4,#0x28]  ; device_subt
        CMP      r1,r0
        BEQ      |L11.98|
        LDR      r1,|L11.132|
        LDR      r1,[r1,#0]  ; systick_counter
        LDR      r2,|L11.136|
        CMP      r1,r2
        BLS      |L11.98|
        STR      r0,[r4,#0x34]  ; device_subt_b
        CMP      r0,#0xa2
        BEQ      |L11.112|
        CMP      r0,#0xa1
        BEQ      |L11.118|
        BL       ampSetS
|L11.98|
        LDR      r3,|L11.140|
        MOVS     r2,#0
        MOVS     r1,#0x1e
        LDRB     r0,[r4,#2]  ; adc_key_task
        BL       startATask
        POP      {r4-r6,pc}
|L11.112|
        BL       ampSetR
        B        |L11.98|
|L11.118|
        BL       ampSetL
        B        |L11.98|
        ENDP

|L11.124|
        DCD      ||.data||
|L11.128|
        DCD      dev_state
|L11.132|
        DCD      systick_counter
|L11.136|
        DCD      0x00001388
|L11.140|
        DCD      adckeyScan

        AREA ||i.adckeyTaskInit||, CODE, READONLY, ALIGN=2

adckeyTaskInit PROC
        PUSH     {r4,lr}
        BL       adcKeyInit
        LDR      r0,|L12.48|
        LDR      r1,[r0,#0]
        MOVS     r2,#0xff
        LSLS     r2,r2,#8
        BICS     r1,r1,r2
        MOVS     r2,#1
        LSLS     r2,r2,#15
        ORRS     r1,r1,r2
        STR      r1,[r0,#0]
        BL       DrvADC_EnableInt
        BL       getATask
        LDR      r1,|L12.52|
        LDR      r3,|L12.56|
        STRB     r0,[r1,#2]
        MOVS     r2,#0
        MOVS     r1,#0x1e
        BL       startATask
        POP      {r4,pc}
        ENDP

|L12.48|
        DCD      0xe000e41c
|L12.52|
        DCD      ||.data||
|L12.56|
        DCD      adckeyScan

        AREA ||i.audioDetect||, CODE, READONLY, ALIGN=2

audioDetect PROC
        LDR      r0,|L13.64|
        LDR      r0,[r0,#0]  ; amp_volume
        CMP      r0,#0x1a
        BCS      |L13.12|
        MOVS     r0,#0x40
        B        |L13.22|
|L13.12|
        CMP      r0,#0x1d
        BCS      |L13.20|
        MOVS     r0,#0x60
        B        |L13.22|
|L13.20|
        MOVS     r0,#0x80
|L13.22|
        LDR      r1,|L13.68|
        LDR      r2,|L13.72|
        LDRB     r1,[r1,#0]  ; dev_input
        LDR      r2,[r2,#0]  ; BT_MATCH_FLAG
        ORRS     r1,r1,r2
        BNE      |L13.36|
        MOVS     r0,#0x80
|L13.36|
        LDR      r1,|L13.76|
        LDR      r1,[r1,#0]  ; audio_L
        LSRS     r1,r1,#10
        CMP      r1,r0
        BHI      |L13.56|
        LDR      r1,|L13.80|
        LDR      r1,[r1,#0]  ; audio_R
        LSRS     r1,r1,#10
        CMP      r1,r0
        BLS      |L13.60|
|L13.56|
        MOVS     r0,#1
        BX       lr
|L13.60|
        MOVS     r0,#0
        BX       lr
        ENDP

|L13.64|
        DCD      amp_volume
|L13.68|
        DCD      dev_input
|L13.72|
        DCD      BT_MATCH_FLAG
|L13.76|
        DCD      audio_L
|L13.80|
        DCD      audio_R

        AREA ||i.devStandbyDealAdcKey||, CODE, READONLY, ALIGN=2

devStandbyDealAdcKey PROC
        PUSH     {r4,lr}
        LDR      r0,|L14.12|
        LDRB     r0,[r0,#0]  ; key_word
        BL       delStandbytkey
        POP      {r4,pc}
        ENDP

|L14.12|
        DCD      ||.data||

        AREA ||i.devStartDealAdcKey||, CODE, READONLY, ALIGN=2

devStartDealAdcKey PROC
        PUSH     {r4,lr}
        LDR      r4,|L15.40|
        LDRB     r0,[r4,#0]  ; key_word
        BL       delStartkey
        LDRB     r0,[r4,#1]  ; key_word_continue
        CMP      r0,#0xa
        BEQ      |L15.20|
        CMP      r0,#0xc
        BNE      |L15.38|
|L15.20|
        LDR      r1,[r4,#0x30]  ; k_counter0
        ADDS     r1,r1,#1
        STR      r1,[r4,#0x30]  ; k_counter0
        CMP      r1,#8
        BCC      |L15.38|
        BL       delStartkey
        MOVS     r0,#0
        STR      r0,[r4,#0x30]  ; k_counter0
|L15.38|
        POP      {r4,pc}
        ENDP

|L15.40|
        DCD      ||.data||

        AREA ||i.devTest||, CODE, READONLY, ALIGN=1

devTest PROC
        BX       lr
        ENDP


        AREA ||.arm_vfe_header||, DATA, READONLY, NOALLOC, ALIGN=2

        DCD      0x00000000

        AREA ||.data||, DATA, ALIGN=2

key_word
        DCB      0xff
key_word_continue
        DCB      0xff
adc_key_task
        DCB      0xff
key_buf
        DCB      0xff
key_buf_adc
        DCB      0xff
key_c_r
        DCB      0xff
key_word_r
        DCB      0x00
key_word_bak
        DCB      0xff
key_bak
        DCW      0x0000
key_irq_index
        DCW      0x0000
key_adc_value
        DCW      0x0000
device_type_adc1
        DCW      0x0000
device_type_adc2
        DCW      0x0000
device_type_adc
        DCW      0x0000
auto_standy_delay_minunte
        DCD      0x0000000f
bt_count
        DCD      0x00000000
audio_quiet_time
        DCD      0x00000000
audio_open_time
        DCD      0x00000001
adcKeySubHook
        DCD      0x00000000
device_subt
        DCD      0x00000000
kwcdc
        DCD      0x00000000
k_counter0
        DCD      0x00000000
device_subt_b
        DCD      0x00000000
bt_volage_adc
        DCD      0x00000000
bt_volage
        DCD      0x00000000

        AREA ||area_number.20||, DATA, ALIGN=1

        EXPORTAS ||area_number.20||, ||.data||
u16ADCBuf
        DCW      0x0000

        AREA ||area_number.21||, DATA, ALIGN=2

        EXPORTAS ||area_number.21||, ||.data||
battery_volage
        DCD      0x00000000

        AREA ||area_number.22||, DATA, ALIGN=2

        EXPORTAS ||area_number.22||, ||.data||
dc_time_stamp
        DCD      0x00000001

        AREA ||area_number.23||, DATA, ALIGN=2

        EXPORTAS ||area_number.23||, ||.data||
power_delay
        DCD      0x00000000

        EXPORT ADC_IRQHandler [CODE]
        EXPORT DEVtest [CODE]
        EXPORT adcKeyClkInit [CODE]
        EXPORT adcKeyHook [CODE]
        EXPORT adcKeyInit [CODE]
        EXPORT adcKeyRemap [CODE]
        EXPORT adcReadKey [CODE]
        EXPORT adcReadKeyContinue [CODE]
        EXPORT adckeyScan [CODE]
        EXPORT adckeyTaskInit [CODE]
        EXPORT audioDetect [CODE]
        EXPORT devStandbyDealAdcKey [CODE]
        EXPORT devStartDealAdcKey [CODE]
        EXPORT devTest [CODE]
        EXPORT key_word [DATA,SIZE=1]
        EXPORT key_word_continue [DATA,SIZE=1]
        EXPORT adc_key_task [DATA,SIZE=1]
        EXPORT key_buf [DATA,SIZE=1]
        EXPORT key_buf_adc [DATA,SIZE=1]
        EXPORT device_type_adc1 [DATA,SIZE=2]
        EXPORT device_type_adc2 [DATA,SIZE=2]
        EXPORT device_type_adc [DATA,SIZE=2]
        EXPORT auto_standy_delay_minunte [DATA,SIZE=4]
        EXPORT audio_quiet_time [DATA,SIZE=4]
        EXPORT adcKeySubHook [DATA,SIZE=4]
        EXPORT device_subt [DATA,SIZE=4]
        EXPORT u16ADCBuf [DATA,SIZE=2]
        EXPORT battery_volage [DATA,SIZE=4]
        EXPORT dc_time_stamp [DATA,SIZE=4]
        EXPORT power_delay [DATA,SIZE=4]

        IMPORT ||Lib$$Request$$armlib|| [CODE,WEAK]
        IMPORT delStandbytkey [CODE]
        IMPORT delStartkey [CODE]
        IMPORT amp_volume [DATA]
        IMPORT dev_input [DATA]
        IMPORT BT_MATCH_FLAG [DATA]
        IMPORT audio_L [DATA]
        IMPORT audio_R [DATA]
        IMPORT logEvent [CODE]
        IMPORT enterStandby [CODE]
        IMPORT dev_state [DATA]
        IMPORT WB_paired_flag [DATA]
        IMPORT led_saving_power_flag [DATA]
        IMPORT led_saving_couter [DATA]
        IMPORT audio_detect_R [DATA]
        IMPORT audio_detect_L [DATA]
        IMPORT battery_state [DATA]
        IMPORT ampSetR [CODE]
        IMPORT ampSetL [CODE]
        IMPORT ampSetS [CODE]
        IMPORT startATask [CODE]
        IMPORT systick_counter [DATA]
        IMPORT DrvADC_EnableInt [CODE]
        IMPORT getATask [CODE]
        IMPORT dwhp83_type [DATA]

        KEEP ADC_GET_RSLT
        KEEP GPIO_SET_MODE
        KEEP key_c_r
        KEEP key_word_r
        KEEP key_word_bak
        KEEP key_bak
        KEEP key_irq_index
        KEEP key_adc_value
        KEEP bt_count
        KEEP audio_open_time
        KEEP kwcdc
        KEEP k_counter0
        KEEP device_subt_b
        KEEP bt_volage_adc
        KEEP bt_volage

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
