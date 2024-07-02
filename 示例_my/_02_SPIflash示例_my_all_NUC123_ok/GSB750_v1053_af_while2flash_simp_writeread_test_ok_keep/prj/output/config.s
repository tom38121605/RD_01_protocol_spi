; generated by ARM C/C++ Compiler, 5.02 [Build 28]
; commandline ArmCC [--split_sections --debug -c --asm --info=totals -o.\output\config.o --depend=.\output\config.d --cpu=Cortex-M0 --apcs=interwork -I..\CMSIS\CM0\DeviceSupport\Nuvoton\NUC123Series -I..\CMSIS\CM0\CoreSupport -I..\INC\DrvInc -I..\INC\SysInc -IC:\Keil\ARM\RV31\INC -IC:\Keil\ARM\CMSIS\Include -D__MICROLIB --omf_browse=.\output\config.crf ..\AppSrc\config.c]
        THUMB
        PRESERVE8

        AREA ||.arm_vfe_header||, DATA, READONLY, NOALLOC, ALIGN=2

        DCD      0x00000000

        AREA ||.data||, DATA, ALIGN=0

sys_error
        DCB      0x00

        AREA ||area_number.4||, DATA, ALIGN=0

        EXPORTAS ||area_number.4||, ||.data||
dev_state
        DCB      0x01

        AREA ||area_number.5||, DATA, ALIGN=0

        EXPORTAS ||area_number.5||, ||.data||
dev_input
        DCB      0x01

        AREA ||area_number.6||, DATA, ALIGN=0

        EXPORTAS ||area_number.6||, ||.data||
input_channel_color
        DCB      0x09,0x06,0x09,0x00
        %        8
        DCB      0x00,0x00,0x00

        AREA ||area_number.7||, DATA, ALIGN=2

        EXPORTAS ||area_number.7||, ||.data||
total_input_channel
        DCD      0x00000003

        EXPORT sys_error [DATA,SIZE=1]
        EXPORT dev_state [DATA,SIZE=1]
        EXPORT dev_input [DATA,SIZE=1]
        EXPORT input_channel_color [DATA,SIZE=15]
        EXPORT total_input_channel [DATA,SIZE=4]

        IMPORT ||Lib$$Request$$armlib|| [CODE,WEAK]

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
