; generated by ARM C/C++ Compiler, 5.02 [Build 28]
; commandline ArmCC [--split_sections --debug -c --asm --info=totals -o.\output\pt2314.o --depend=.\output\pt2314.d --cpu=Cortex-M0 --apcs=interwork -I..\CMSIS\CM0\DeviceSupport\Nuvoton\NUC123Series -I..\CMSIS\CM0\CoreSupport -I..\INC\DrvInc -I..\INC\SysInc -IC:\Keil\ARM\RV31\INC -IC:\Keil\ARM\CMSIS\Include -D__MICROLIB --omf_browse=.\output\pt2314.crf ..\AppSrc\pt2314.c]
        THUMB
        REQUIRE8
        PRESERVE8

        AREA ||i.BT_POWER_ON||, CODE, READONLY, ALIGN=2

BT_POWER_ON PROC
        LDR      r1,|L1.12|
        MOVS     r0,#1
        STR      r0,[r1,#0]  ; BT_FIRST_START
        LDR      r1,|L1.16|
        STR      r0,[r1,#0]
        BX       lr
        ENDP

|L1.12|
        DCD      BT_FIRST_START
|L1.16|
        DCD      0x50004278

        AREA ||i.btCheck||, CODE, READONLY, ALIGN=2

btCheck PROC
        PUSH     {r4,lr}
        LDR      r0,|L2.60|
        LDR      r1,|L2.64|
        LDR      r2,[r0,#0]  ; BT_MATCH_FLAG
        LDRB     r0,[r1,#0]
        CMP      r2,#1
        BEQ      |L2.44|
        LDR      r2,|L2.68|
        LDR      r2,[r2,#0]  ; systick_counter
        LDR      r3,[r1,#8]  ; stamp
        SUBS     r2,r2,r3
        LDR      r3,|L2.72|
        CMP      r2,r3
        BLS      |L2.50|
        MOVS     r2,#1
        STR      r2,[r1,#4]  ; bt_check_flag
        LDR      r3,|L2.76|
        MOVS     r2,#0
        LDR      r1,|L2.80|
|L2.38|
        BL       startATask
        POP      {r4,pc}
|L2.44|
        BL       stopATask
        POP      {r4,pc}
|L2.50|
        MOVS     r2,#0
        LDR      r3,|L2.84|
        MOVS     r1,#0xa
        B        |L2.38|
        ENDP

        DCW      0x0000
|L2.60|
        DCD      BT_MATCH_FLAG
|L2.64|
        DCD      ||.data||
|L2.68|
        DCD      systick_counter
|L2.72|
        DCD      0x00001388
|L2.76|
        DCD      btCheckDone
|L2.80|
        DCD      0x000005dc
|L2.84|
        DCD      btCheck

        AREA ||i.btCheckDone||, CODE, READONLY, ALIGN=2

btCheckDone PROC
        PUSH     {r4,lr}
        LDR      r0,|L3.16|
        MOVS     r1,#0
        STR      r1,[r0,#4]  ; bt_check_flag
        LDRB     r0,[r0,#0]  ; pt2314_id
        BL       stopATask
        POP      {r4,pc}
        ENDP

|L3.16|
        DCD      ||.data||

        AREA ||i.changeInputChannel||, CODE, READONLY, ALIGN=2

changeInputChannel PROC
        PUSH     {r3-r7,lr}
        BL       ampMute
        LDR      r6,|L4.260|
        LDR      r2,|L4.264|
        LDRB     r0,[r6,#0]  ; dev_input
        LDR      r2,[r2,#0]  ; total_input_channel
        MOVS     r4,#0
        ADDS     r1,r0,#1
        SUBS     r2,r2,#1
        CMP      r1,r2
        BLS      |L4.28|
        MOVS     r0,#0
        B        |L4.30|
|L4.28|
        ADDS     r0,r0,#1
|L4.30|
        UXTB     r0,r0
        LDR      r1,|L4.268|
        STRB     r0,[r6,#0]
        LDR      r5,|L4.272|
        LDRB     r0,[r1,r0]
        STRB     r0,[r5,#0]
        BL       ledColorSet
        LDRB     r0,[r6,#0]  ; dev_input
        CMP      r0,#2
        BEQ      |L4.106|
        LDR      r7,|L4.276|
        LDRB     r0,[r7,#0]  ; pt2314_id
        BL       stopATask
        LDR      r0,|L4.280|
        MOVS     r1,#1
        STR      r1,[r0,#0]
        LDRB     r0,[r6,#0]  ; dev_input
        LDR      r6,|L4.280|
        SUBS     r6,r6,#8
        CMP      r0,#0
        BEQ      |L4.190|
        LDR      r7,|L4.284|
        MOVS     r0,#0x96
        STR      r4,[r7,#0]  ; BT_FIRST_START
        BL       sysDelayMs
        BL       BT_POWER_ON
        MOVS     r0,#0xc8
        BL       sysDelayMs
        STR      r4,[r7,#0]  ; BT_FIRST_START
        STR      r4,[r6,#0]
        LDR      r0,|L4.288|
        STR      r4,[r0,#0]  ; BT_MATCH_FLAG
        B        |L4.184|
|L4.106|
        LDR      r7,|L4.284|
        MOVS     r0,#0x96
        STR      r4,[r7,#0]  ; BT_FIRST_START
        BL       sysDelayMs
        BL       BT_POWER_ON
        MOVS     r0,#0xc8
        BL       sysDelayMs
        LDR      r0,|L4.280|
        STR      r4,[r7,#0]  ; BT_FIRST_START
        STR      r4,[r0,#0]
        LDR      r0,|L4.268|
        LDRB     r1,[r6,#0]  ; dev_input
        LDRB     r0,[r0,r1]
        STRB     r0,[r5,#0]
        BL       ledColorSet
        LDR      r7,|L4.296|
        LDR      r6,|L4.292|
        STR      r6,[r7,#0]  ; Dwhp83_CuMu
        BL       Dwhp83CuMuDetect
        LDR      r0,[r7,#0]  ; Dwhp83_CuMu
        CMP      r0,r6
        BNE      |L4.184|
        BL       Dwhp83Slave
        MOVS     r0,#0xc8
        BL       sysDelayMs
        LDR      r0,|L4.276|
        LDR      r3,|L4.300|
        MOVS     r2,#0
        LDR      r1,|L4.304|
        LDRB     r0,[r0,#0]  ; pt2314_id
        BL       startATask
|L4.184|
        LDR      r0,|L4.308|
        STRB     r4,[r0,#0]
        B        |L4.236|
|L4.190|
        BL       BT_POWER_ON
        MOVS     r0,#0xff
        ADDS     r0,r0,#0x2d
        BL       sysDelayMs
        MOVS     r0,#0x6a
        BL       logEvent
        MOVS     r0,#0x3a
        BL       logEvent
        MOVS     r0,#1
        STR      r0,[r6,#0]
        LDR      r1,|L4.312|
        LDR      r3,|L4.316|
        STR      r4,[r1,#0]  ; ccount
        STR      r0,[r7,#4]  ; bt_check_flag
        MOVS     r2,#0
        LDR      r1,|L4.320|
        LDRB     r0,[r7,#0]  ; pt2314_id
        BL       startATask
|L4.236|
        MOVS     r0,#5
        BL       sysDelayMs
        BL       unMute
        LDRB     r0,[r5,#0]  ; state_color
        BL       ledColorSet
        BL       changeInputVolumeUp
        POP      {r3-r7,pc}
        ENDP

        DCW      0x0000
|L4.260|
        DCD      dev_input
|L4.264|
        DCD      total_input_channel
|L4.268|
        DCD      input_channel_color
|L4.272|
        DCD      state_color
|L4.276|
        DCD      ||.data||
|L4.280|
        DCD      0x500042b4
|L4.284|
        DCD      BT_FIRST_START
|L4.288|
        DCD      BT_MATCH_FLAG
|L4.292|
        DCD      0x000055aa
|L4.296|
        DCD      Dwhp83_CuMu
|L4.300|
        DCD      dwph83Check
|L4.304|
        DCD      0x000009c4
|L4.308|
        DCD      WB_paired_flag
|L4.312|
        DCD      ccount
|L4.316|
        DCD      btCheckDone
|L4.320|
        DCD      0x000005dc

        AREA ||i.pt2314DataResume||, CODE, READONLY, ALIGN=2

pt2314DataResume PROC
        PUSH     {r3-r7,lr}
        LDR      r7,|L5.184|
        MOVS     r4,#0
        LDR      r0,[r7,#0xc]  ; nfc_start
        LDR      r6,|L5.188|
        MOV      r5,r4
        CMP      r0,#1
        BNE      |L5.34|
        LDR      r0,|L5.192|
        STRB     r5,[r6,#0]
        LDR      r0,[r0,#0]  ; systick_counter
        STR      r5,[r7,#0xc]  ; nfc_start
        STR      r5,[r7,#4]  ; bt_check_flag
        MOVS     r4,#1
        STR      r0,[r7,#8]  ; stamp
        BL       btCheck
|L5.34|
        LDR      r1,|L5.196|
        LDRB     r0,[r6,#0]  ; dev_input
        LDRB     r2,[r1,r0]
        LDR      r1,|L5.200|
        CMP      r0,#2
        STRB     r2,[r1,#0]
        BEQ      |L5.106|
        LDRB     r0,[r7,#0]  ; pt2314_id
        BL       stopATask
        LDR      r1,|L5.204|
        MOVS     r0,#1
        STR      r0,[r1,#0]
        LDR      r1,|L5.204|
        LDRB     r2,[r6,#0]  ; dev_input
        SUBS     r1,r1,#8
        CMP      r2,#0
        BEQ      |L5.144|
        LDR      r0,|L5.208|
        STR      r5,[r0,#0]  ; BT_FIRST_START
        STR      r5,[r1,#0]
        MOVS     r0,#0x3c
        BL       logEvent
        MOVS     r0,#0x6c
        BL       logEvent
|L5.88|
        LDR      r0,|L5.212|
        LDRB     r0,[r0,#0]  ; dev_state
        CMP      r0,#0
        BEQ      |L5.180|
        LDR      r0,|L5.200|
        LDRB     r0,[r0,#0]  ; state_color
|L5.100|
        BL       ledColorSet
        POP      {r3-r7,pc}
|L5.106|
        LDR      r0,|L5.208|
        STR      r5,[r0,#0]  ; BT_FIRST_START
        MOVS     r0,#0xff
        ADDS     r0,r0,#0x2d
        BL       sysDelayMs
        LDR      r0,|L5.204|
        STR      r5,[r0,#0]
        LDR      r0,|L5.196|
        LDRB     r1,[r6,#0]  ; dev_input
        LDRB     r0,[r0,r1]
        LDR      r1,|L5.200|
        STRB     r0,[r1,#0]
        BL       ledColorSet
        MOVS     r2,#0
        LDR      r3,|L5.216|
        MOVS     r1,#0xa
        B        |L5.172|
|L5.144|
        MOV      r5,r0
        STR      r0,[r1,#0]
        MOVS     r0,#0x6a
        BL       logEvent
        MOVS     r0,#0x3a
        BL       logEvent
        CMP      r4,#0
        BNE      |L5.88|
        LDR      r3,|L5.220|
        LDR      r1,|L5.224|
        MOVS     r2,#0
        STR      r5,[r7,#4]  ; bt_check_flag
|L5.172|
        LDRB     r0,[r7,#0]  ; pt2314_id
        BL       startATask
        B        |L5.88|
|L5.180|
        MOVS     r0,#7
        B        |L5.100|
        ENDP

|L5.184|
        DCD      ||.data||
|L5.188|
        DCD      dev_input
|L5.192|
        DCD      systick_counter
|L5.196|
        DCD      input_channel_color
|L5.200|
        DCD      state_color
|L5.204|
        DCD      0x500042b4
|L5.208|
        DCD      BT_FIRST_START
|L5.212|
        DCD      dev_state
|L5.216|
        DCD      dwph83Check
|L5.220|
        DCD      btCheckDone
|L5.224|
        DCD      0x000005dc

        AREA ||i.pt2314Init||, CODE, READONLY, ALIGN=2

pt2314Init PROC
        PUSH     {r4,lr}
        BL       getATask
        LDR      r4,|L6.20|
        STRB     r0,[r4,#0]
        BL       getATask
        STRB     r0,[r4,#1]
        POP      {r4,pc}
        ENDP

        DCW      0x0000
|L6.20|
        DCD      ||.data||

        AREA ||i.pt2314InputChannelCheck||, CODE, READONLY, ALIGN=2

pt2314InputChannelCheck PROC
        PUSH     {r4-r6,lr}
        LDR      r0,|L7.116|
        LDR      r5,|L7.120|
        LDR      r0,[r0,#8]  ; pt2314_data_block
        LDRB     r0,[r0,#0]
        CMP      r0,#0xa0
        BEQ      |L7.24|
        CMP      r0,#0xa2
        BEQ      |L7.100|
        BL       changeInputChannel
        POP      {r4-r6,pc}
|L7.24|
        LDR      r4,|L7.124|
        LDR      r1,|L7.128|
        LDRB     r0,[r4,#0]  ; dev_input
        LDR      r1,[r1,#0]  ; total_input_channel
        ADDS     r2,r0,#1
        SUBS     r1,r1,#1
        CMP      r2,r1
        BLS      |L7.44|
        MOVS     r0,#0
        B        |L7.46|
|L7.44|
        ADDS     r0,r0,#1
|L7.46|
        UXTB     r0,r0
        LDR      r1,|L7.132|
        STRB     r0,[r4,#0]
        LDRB     r0,[r1,r0]
        LDR      r1,|L7.136|
        STRB     r0,[r1,#0]
        BL       ledColorSet
        LDR      r0,|L7.120|
        LDRB     r1,[r4,#0]  ; dev_input
        ADDS     r0,r0,#0x17
        LDRB     r0,[r0,r1]
        MOVS     r6,#0
        STRB     r0,[r5,#2]
        BL       unMute
        LDRB     r0,[r4,#0]  ; dev_input
        CMP      r0,#0
        BEQ      |L7.94|
        LDR      r0,|L7.140|
        STR      r6,[r0,#0]  ; BT_FIRST_START
|L7.88|
        BL       changeInputVolumeUp
        POP      {r4-r6,pc}
|L7.94|
        BL       BT_POWER_ON
        B        |L7.88|
|L7.100|
        LDR      r3,|L7.144|
        MOVS     r2,#0
        MOVS     r1,#2
        LDRB     r0,[r5,#1]  ; pt2314_id_CIC
        BL       startATask
        POP      {r4-r6,pc}
        ENDP

        DCW      0x0000
|L7.116|
        DCD      ||.bss||
|L7.120|
        DCD      ||.data||
|L7.124|
        DCD      dev_input
|L7.128|
        DCD      total_input_channel
|L7.132|
        DCD      input_channel_color
|L7.136|
        DCD      state_color
|L7.140|
        DCD      BT_FIRST_START
|L7.144|
        DCD      pt2314InputChannelCheck

        AREA ||i.pt2314ResumeI2c||, CODE, READONLY, ALIGN=2

pt2314ResumeI2c PROC
        LDR      r0,|L8.16|
        LDRB     r1,[r0,#4]  ; pt2314_data_block
        STRB     r1,[r0,#3]
        LDR      r1,|L8.20|
        MOVS     r0,#0xa0
        STRB     r0,[r1,#0]
        BX       lr
        ENDP

        DCW      0x0000
|L8.16|
        DCD      ||.bss||
|L8.20|
        DCD      ||.data||+0x10

        AREA ||i.stopATask||, CODE, READONLY, ALIGN=2

stopATask PROC
        MOVS     r2,#0xc
        MULS     r0,r2,r0
        LDR      r2,|L9.16|
        MOVS     r1,#0
        ADDS     r0,r0,r2
        STRB     r1,[r0,#1]
        BX       lr
        ENDP

        DCW      0x0000
|L9.16|
        DCD      mytask

        AREA ||.arm_vfe_header||, DATA, READONLY, NOALLOC, ALIGN=2

        DCD      0x00000000

        AREA ||.bss||, DATA, NOINIT, ALIGN=2

pt2314_data_block
        %        16

        AREA ||.data||, DATA, ALIGN=2

pt2314_id
        DCB      0xff
pt2314_id_CIC
        DCB      0xff
pt2314_input
        DCB      0x00,0x00
bt_check_flag
        DCD      0x00000000
stamp
        DCD      0x00000000
nfc_start
        DCD      0x00000000
pt2314_status
        DCB      0xa0,0x00,0xc0,0xe0
        DCB      0x5c,0x77,0x67
pt2314_input_table
        DCB      0x00
        DCB      0x01,0x02,0x00,0x00
        %        8
        DCB      0x00,0x00

        AREA ||area_number.14||, DATA, ALIGN=0

        EXPORTAS ||area_number.14||, ||.data||
pt2314_switch_remap
        DCB      0x5c,0x5d,0x5e,0x5f

        AREA ||area_number.15||, DATA, ALIGN=0

        EXPORTAS ||area_number.15||, ||.data||
pt2314_bass_tbl
        DCB      0x62,0x63,0x64,0x65
        DCB      0x66,0x67,0x6e,0x6d
        DCB      0x6c,0x6b,0x6a,0x00

        AREA ||area_number.16||, DATA, ALIGN=0

        EXPORTAS ||area_number.16||, ||.data||
pt2314_trieb_tbl
        DCB      0x72,0x73,0x74,0x75
        DCB      0x76,0x77,0x7e,0x7d
        DCB      0x7c,0x7b,0x7a,0x00

        AREA ||area_number.17||, DATA, ALIGN=0

        EXPORTAS ||area_number.17||, ||.data||
pt2314_mode
        DCB      0x00

        EXPORT btCheck [CODE]
        EXPORT btCheckDone [CODE]
        EXPORT changeInputChannel [CODE]
        EXPORT pt2314DataResume [CODE]
        EXPORT pt2314Init [CODE]
        EXPORT pt2314InputChannelCheck [CODE]
        EXPORT pt2314ResumeI2c [CODE]
        EXPORT pt2314_id [DATA,SIZE=1]
        EXPORT pt2314_id_CIC [DATA,SIZE=1]
        EXPORT pt2314_input [DATA,SIZE=1]
        EXPORT bt_check_flag [DATA,SIZE=4]
        EXPORT nfc_start [DATA,SIZE=4]
        EXPORT pt2314_status [DATA,SIZE=7]
        EXPORT pt2314_input_table [DATA,SIZE=15]
        EXPORT pt2314_switch_remap [DATA,SIZE=4]
        EXPORT pt2314_bass_tbl [DATA,SIZE=12]
        EXPORT pt2314_trieb_tbl [DATA,SIZE=12]
        EXPORT pt2314_mode [DATA,SIZE=1]

        IMPORT ||Lib$$Request$$armlib|| [CODE,WEAK]
        IMPORT startATask [CODE]
        IMPORT BT_MATCH_FLAG [DATA]
        IMPORT systick_counter [DATA]
        IMPORT ampMute [CODE]
        IMPORT ledColorSet [CODE]
        IMPORT sysDelayMs [CODE]
        IMPORT Dwhp83CuMuDetect [CODE]
        IMPORT Dwhp83Slave [CODE]
        IMPORT logEvent [CODE]
        IMPORT unMute [CODE]
        IMPORT changeInputVolumeUp [CODE]
        IMPORT dev_input [DATA]
        IMPORT total_input_channel [DATA]
        IMPORT input_channel_color [DATA]
        IMPORT state_color [DATA]
        IMPORT BT_FIRST_START [DATA]
        IMPORT Dwhp83_CuMu [DATA]
        IMPORT dwph83Check [CODE]
        IMPORT WB_paired_flag [DATA]
        IMPORT ccount [DATA]
        IMPORT dev_state [DATA]
        IMPORT getATask [CODE]
        IMPORT mytask [DATA]

        KEEP BT_POWER_ON
        KEEP stopATask
        KEEP pt2314_data_block
        KEEP stamp

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
