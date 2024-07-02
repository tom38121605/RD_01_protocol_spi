; generated by ARM C/C++ Compiler, 5.03 [Build 76]
; commandline ArmCC [--debug -c --asm --info=totals -o.\output\drvspi.o --depend=.\output\drvspi.d --cpu=Cortex-M0 --apcs=interwork -O0 -I..\CMSIS\CM0\DeviceSupport\Nuvoton\NUC123Series -I..\CMSIS\CM0\CoreSupport -I..\INC\DrvInc -I..\INC\SysInc -I..\dw -IC:\Keil_v472\ARM\RV31\INC -IC:\Keil_v472\ARM\CMSIS\Include -IC:\Keil_v472\ARM\Inc\Nuvoton\NUC123Series -D__MICROLIB --omf_browse=.\output\drvspi.crf ..\DrvSrc\DrvSPI.c]
        THUMB
        REQUIRE8
        PRESERVE8

        AREA ||.text||, CODE, READONLY, ALIGN=2

CLK_GetPLLClockFreq PROC
        PUSH     {r1-r7,lr}
        ADR      r0,|L1.1008|
        LDR      r0,[r0,#0]
        STR      r0,[sp,#0]
        MOVS     r0,#0
        STR      r0,[sp,#8]
        LDR      r0,|L1.1012|
        LDR      r4,[r0,#0x20]
        ASRS     r0,r0,#12
        TST      r4,r0
        BNE      |L1.102|
        MOVS     r0,#1
        LSLS     r0,r0,#19
        TST      r4,r0
        BEQ      |L1.34|
        LDR      r0,|L1.1016|
        B        |L1.36|
|L1.34|
        LDR      r0,|L1.1020|
|L1.36|
        MOV      r5,r0
        MOVS     r0,#1
        LSLS     r0,r0,#17
        TST      r4,r0
        BEQ      |L1.50|
        STR      r5,[sp,#8]
        B        |L1.102|
|L1.50|
        MOVS     r0,#3
        LSLS     r0,r0,#14
        ANDS     r0,r0,r4
        LSRS     r0,r0,#14
        MOV      r1,sp
        LDRB     r7,[r1,r0]
        LSLS     r0,r4,#23
        LSRS     r0,r0,#23
        ADDS     r0,r0,#2
        STR      r0,[sp,#4]
        MOVS     r0,#0x1f
        LSLS     r0,r0,#9
        ANDS     r0,r0,r4
        LSRS     r0,r0,#9
        ADDS     r6,r0,#2
        MOV      r2,r6
        MULS     r2,r7,r2
        MOV      r1,r2
        LSRS     r3,r5,#2
        LDR      r2,[sp,#4]
        MULS     r3,r2,r3
        MOV      r0,r3
        BL       __aeabi_uidivmod
        LSLS     r0,r0,#2
        STR      r0,[sp,#8]
|L1.102|
        LDR      r0,[sp,#8]
        POP      {r1-r7,pc}
        ENDP

DrvSPI_DeInit PROC
        PUSH     {r4,lr}
        MOV      r4,r0
        LDR      r0,|L1.1024|
        CMP      r4,r0
        BNE      |L1.130|
        ASRS     r0,r0,#18
        BL       APBCLK_DISABLE
        MOVS     r0,#0xe
        BL       NVIC_DisableIRQ
        B        |L1.172|
|L1.130|
        LDR      r0,|L1.1028|
        CMP      r4,r0
        BNE      |L1.152|
        MOVS     r0,#1
        LSLS     r0,r0,#13
        BL       APBCLK_DISABLE
        MOVS     r0,#0xf
        BL       NVIC_DisableIRQ
        B        |L1.172|
|L1.152|
        LDR      r0,|L1.1032|
        CMP      r4,r0
        BNE      |L1.172|
        MOVS     r0,#1
        LSLS     r0,r0,#14
        BL       APBCLK_DISABLE
        MOVS     r0,#0x10
        BL       NVIC_DisableIRQ
|L1.172|
        POP      {r4,pc}
        ENDP

DrvSPI_Init PROC
        PUSH     {r3-r7,lr}
        MOV      r4,r0
        MOV      r5,r1
        MOV      r6,r2
        MOV      r7,r3
        LDR      r0,|L1.1024|
        CMP      r4,r0
        BNE      |L1.198|
        ASRS     r0,r0,#18
        BL       APBCLK_ENABLE
        B        |L1.228|
|L1.198|
        LDR      r0,|L1.1028|
        CMP      r4,r0
        BNE      |L1.214|
        MOVS     r0,#1
        LSLS     r0,r0,#13
        BL       APBCLK_ENABLE
        B        |L1.228|
|L1.214|
        LDR      r0,|L1.1032|
        CMP      r4,r0
        BNE      |L1.228|
        MOVS     r0,#1
        LSLS     r0,r0,#14
        BL       APBCLK_ENABLE
|L1.228|
        LDR      r0,|L1.1024|
        CMP      r4,r0
        BNE      |L1.242|
        ASRS     r0,r0,#18
        BL       SYS_RESET_IP2
        B        |L1.278|
|L1.242|
        LDR      r0,|L1.1028|
        CMP      r4,r0
        BNE      |L1.258|
        MOVS     r0,#1
        LSLS     r0,r0,#13
        BL       SYS_RESET_IP2
        B        |L1.278|
|L1.258|
        LDR      r0,|L1.1032|
        CMP      r4,r0
        BNE      |L1.274|
        MOVS     r0,#1
        LSLS     r0,r0,#14
        BL       SYS_RESET_IP2
        B        |L1.278|
|L1.274|
        MOVS     r0,#0
|L1.276|
        POP      {r3-r7,pc}
|L1.278|
        CMP      r7,#0x20
        BNE      |L1.284|
        MOVS     r7,#0
|L1.284|
        MOV      r0,r7
        LDR      r1,[r4,#0]
        MOVS     r2,#0xf8
        BICS     r1,r1,r2
        CMP      r0,#0x20
        BEQ      |L1.300|
        MOV      r2,r0
        B        |L1.302|
|L1.300|
        MOVS     r2,#0
|L1.302|
        LSLS     r2,r2,#3
        ORRS     r1,r1,r2
        STR      r1,[r4,#0]
        NOP      
        NOP      
        LDR      r0,[r4,#0]
        MOVS     r1,#1
        LSLS     r1,r1,#18
        BICS     r0,r0,r1
        ORRS     r0,r0,r5
        STR      r0,[r4,#0]
        NOP      
        NOP      
        LDR      r0,[r4,#0]
        MOVS     r1,#6
        BICS     r0,r0,r1
        ORRS     r0,r0,r6
        STR      r0,[r4,#0]
        NOP      
        MOVS     r0,#1
        B        |L1.276|
        ENDP

DrvSPI_GetSourceClockFreq PROC
        PUSH     {r1-r7,lr}
        MOV      r6,r0
        LDR      r0,|L1.1020|
        STR      r0,[sp,#4]
        LDR      r0,|L1.1024|
        CMP      r6,r0
        BNE      |L1.368|
        LDR      r0,|L1.1012|
        LDR      r4,[r0,#0x1c]
        MOVS     r1,#0x10
        ANDS     r4,r4,r1
        B        |L1.392|
|L1.368|
        LDR      r0,|L1.1028|
        CMP      r6,r0
        BNE      |L1.384|
        LDR      r0,|L1.1012|
        LDR      r4,[r0,#0x1c]
        MOVS     r1,#0x20
        ANDS     r4,r4,r1
        B        |L1.392|
|L1.384|
        LDR      r0,|L1.1012|
        LDR      r4,[r0,#0x1c]
        MOVS     r1,#0x40
        ANDS     r4,r4,r1
|L1.392|
        CMP      r4,#0
        BEQ      |L1.400|
        BEQ      |L1.400|
        BNE      |L1.406|
|L1.400|
        BL       CLK_GetPLLClockFreq
|L1.404|
        POP      {r1-r7,pc}
|L1.406|
        CMP      r4,#0x10
        BEQ      |L1.418|
        CMP      r4,#0x20
        BEQ      |L1.418|
        CMP      r4,#0x40
        BNE      |L1.492|
|L1.418|
        NOP      
        MOVS     r7,#0
        LDR      r0,|L1.1012|
        LDR      r0,[r0,#0x10]
        LSLS     r5,r0,#29
        LSRS     r5,r5,#29
        CMP      r5,#0
        BNE      |L1.438|
        LDR      r7,|L1.1020|
        B        |L1.470|
|L1.438|
        CMP      r5,#2
        BEQ      |L1.446|
        CMP      r5,#1
        BNE      |L1.460|
|L1.446|
        BL       CLK_GetPLLClockFreq
        MOV      r7,r0
        CMP      r5,#1
        BNE      |L1.470|
        LSRS     r7,r7,#1
        B        |L1.470|
|L1.460|
        CMP      r5,#3
        BNE      |L1.468|
        LDR      r7,|L1.1036|
        B        |L1.470|
|L1.468|
        LDR      r7,|L1.1016|
|L1.470|
        LDR      r0,|L1.1012|
        LDR      r0,[r0,#0x18]
        LSLS     r0,r0,#28
        LSRS     r0,r0,#28
        ADDS     r0,r0,#1
        STR      r0,[sp,#0]
        MOV      r0,r7
        LDR      r1,[sp,#0]
        BL       __aeabi_uidivmod
        B        |L1.404|
|L1.492|
        LDR      r0,[sp,#4]
        B        |L1.404|
        ENDP

DrvSPI_SetClockFreq PROC
        PUSH     {r1-r7,lr}
        MOV      r5,r0
        MOV      r7,r1
        MOV      r6,r2
        MOVS     r0,#0
        STR      r0,[sp,#0]
        MOV      r0,r5
        BL       DrvSPI_GetSourceClockFreq
        STR      r0,[sp,#4]
        CMP      r6,#0
        BEQ      |L1.576|
        MOV      r1,r6
        LDR      r0,[sp,#4]
        BL       __aeabi_uidivmod
        ADDS     r0,r0,#1
        LSRS     r0,r0,#1
        SUBS     r4,r0,#1
        CMP      r4,#0xff
        BLS      |L1.540|
        MOVS     r4,#0xff
|L1.540|
        ADDS     r0,r4,#1
        LSLS     r1,r0,#1
        LDR      r0,[sp,#4]
        BL       __aeabi_uidivmod
        STR      r0,[sp,#0]
        LDR      r0,[sp,#0]
        CMP      r0,r6
        BLS      |L1.560|
        ADDS     r4,r4,#1
|L1.560|
        LDR      r0,[r5,#4]
        MOVS     r1,#0xff
        LSLS     r1,r1,#16
        BICS     r0,r0,r1
        LSLS     r1,r4,#16
        ORRS     r0,r0,r1
        STR      r0,[r5,#4]
        B        |L1.586|
|L1.576|
        LDR      r0,[r5,#4]
        MOVS     r1,#0xff
        LSLS     r1,r1,#16
        ORRS     r0,r0,r1
        STR      r0,[r5,#4]
|L1.586|
        CMP      r7,#0
        BEQ      |L1.656|
        MOV      r1,r7
        LDR      r0,[sp,#4]
        BL       __aeabi_uidivmod
        LSRS     r4,r0,#1
        CMP      r4,#0x7f
        BLS      |L1.606|
        MOVS     r4,#0x7f
|L1.606|
        CMP      r4,#0
        BNE      |L1.616|
        LDR      r0,[sp,#4]
        STR      r0,[sp,#0]
        B        |L1.626|
|L1.616|
        LSLS     r1,r4,#1
        LDR      r0,[sp,#4]
        BL       __aeabi_uidivmod
        STR      r0,[sp,#0]
|L1.626|
        LDR      r0,[sp,#0]
        CMP      r0,r7
        BLS      |L1.644|
        ADDS     r4,r4,#1
        LSLS     r1,r4,#1
        LDR      r0,[sp,#4]
        BL       __aeabi_uidivmod
        STR      r0,[sp,#0]
|L1.644|
        LDR      r0,[r5,#4]
        LSRS     r0,r0,#8
        LSLS     r0,r0,#8
        ORRS     r0,r0,r4
        STR      r0,[r5,#4]
        B        |L1.664|
|L1.656|
        LDR      r0,[r5,#4]
        MOVS     r1,#0xff
        ORRS     r0,r0,r1
        STR      r0,[r5,#4]
|L1.664|
        LDR      r0,[sp,#0]
        POP      {r1-r7,pc}
        ENDP

DrvSPI_GetClock1Freq PROC
        PUSH     {r4-r6,lr}
        MOV      r4,r0
        MOV      r0,r4
        BL       DrvSPI_GetSourceClockFreq
        MOV      r6,r0
        LDR      r0,[r4,#4]
        UXTB     r5,r0
        CMP      r5,#0
        BNE      |L1.692|
        MOV      r0,r6
|L1.690|
        POP      {r4-r6,pc}
|L1.692|
        LSRS     r0,r6,#1
        MOV      r1,r5
        BL       __aeabi_uidivmod
        B        |L1.690|
        ENDP

DrvSPI_GetClock2Freq PROC
        PUSH     {r4-r6,lr}
        MOV      r4,r0
        MOV      r0,r4
        BL       DrvSPI_GetSourceClockFreq
        MOV      r6,r0
        LDR      r0,[r4,#4]
        MOVS     r1,#0xff
        LSLS     r1,r1,#16
        ANDS     r0,r0,r1
        LSRS     r5,r0,#16
        LSRS     r0,r6,#1
        ADDS     r1,r5,#1
        BL       __aeabi_uidivmod
        POP      {r4-r6,pc}
        ENDP

DrvSPI_EnableInt PROC
        PUSH     {r4,lr}
        MOV      r4,r0
        NOP      
        LDR      r0,[r4,#0]
        MOVS     r1,#1
        LSLS     r1,r1,#17
        ORRS     r0,r0,r1
        STR      r0,[r4,#0]
        NOP      
        LDR      r0,|L1.1024|
        CMP      r4,r0
        BNE      |L1.766|
        MOVS     r0,#0xe
        BL       NVIC_EnableIRQ
        B        |L1.792|
|L1.766|
        LDR      r0,|L1.1028|
        CMP      r4,r0
        BNE      |L1.780|
        MOVS     r0,#0xf
        BL       NVIC_EnableIRQ
        B        |L1.792|
|L1.780|
        LDR      r0,|L1.1032|
        CMP      r4,r0
        BNE      |L1.792|
        MOVS     r0,#0x10
        BL       NVIC_EnableIRQ
|L1.792|
        POP      {r4,pc}
        ENDP

DrvSPI_DisableInt PROC
        PUSH     {r4,lr}
        MOV      r4,r0
        NOP      
        LDR      r0,[r4,#0]
        MOVS     r1,#1
        LSLS     r1,r1,#17
        BICS     r0,r0,r1
        STR      r0,[r4,#0]
        NOP      
        LDR      r0,|L1.1024|
        CMP      r4,r0
        BNE      |L1.826|
        MOVS     r0,#0xe
        BL       NVIC_DisableIRQ
        B        |L1.852|
|L1.826|
        LDR      r0,|L1.1028|
        CMP      r4,r0
        BNE      |L1.840|
        MOVS     r0,#0xf
        BL       NVIC_DisableIRQ
        B        |L1.852|
|L1.840|
        LDR      r0,|L1.1032|
        CMP      r4,r0
        BNE      |L1.852|
        MOVS     r0,#0x10
        BL       NVIC_DisableIRQ
|L1.852|
        POP      {r4,pc}
        ENDP

SPI_Enable3WireStartInt PROC
        PUSH     {r4,lr}
        MOV      r4,r0
        NOP      
        LDR      r0,[r4,#0x3c]
        MOVS     r1,#1
        LSLS     r1,r1,#10
        ORRS     r0,r0,r1
        STR      r0,[r4,#0x3c]
        NOP      
        LDR      r0,|L1.1024|
        CMP      r4,r0
        BNE      |L1.886|
        MOVS     r0,#0xe
        BL       NVIC_EnableIRQ
        B        |L1.912|
|L1.886|
        LDR      r0,|L1.1028|
        CMP      r4,r0
        BNE      |L1.900|
        MOVS     r0,#0xf
        BL       NVIC_EnableIRQ
        B        |L1.912|
|L1.900|
        LDR      r0,|L1.1032|
        CMP      r4,r0
        BNE      |L1.912|
        MOVS     r0,#0x10
        BL       NVIC_EnableIRQ
|L1.912|
        POP      {r4,pc}
        ENDP

SPI_Disable3WireStartInt PROC
        PUSH     {r4,lr}
        MOV      r4,r0
        NOP      
        LDR      r0,[r4,#0x3c]
        MOVS     r1,#1
        LSLS     r1,r1,#10
        BICS     r0,r0,r1
        STR      r0,[r4,#0x3c]
        NOP      
        LDR      r0,|L1.1024|
        CMP      r4,r0
        BNE      |L1.946|
        MOVS     r0,#0xe
        BL       NVIC_DisableIRQ
        B        |L1.972|
|L1.946|
        LDR      r0,|L1.1028|
        CMP      r4,r0
        BNE      |L1.960|
        MOVS     r0,#0xf
        BL       NVIC_DisableIRQ
        B        |L1.972|
|L1.960|
        LDR      r0,|L1.1032|
        CMP      r4,r0
        BNE      |L1.972|
        MOVS     r0,#0x10
        BL       NVIC_DisableIRQ
|L1.972|
        POP      {r4,pc}
        ENDP

SPI_SetFIFOMode PROC
        NOP      
        LDR      r2,[r0,#0]
        MOVS     r3,#0xf
        LSLS     r3,r3,#12
        BICS     r2,r2,r3
        LSLS     r3,r1,#12
        ORRS     r2,r2,r3
        STR      r2,[r0,#0]
        NOP      
        NOP      
        LDR      r2,[r0,#0]
        MOVS     r3,#1
        LSLS     r3,r3,#21
        ORRS     r2,r2,r3
        STR      r2,[r0,#0]
        NOP      
        BX       lr
|L1.1008|
        DCB      1,2,2,4
|L1.1012|
        DCD      0x50000200
|L1.1016|
        DCD      0x01518000
|L1.1020|
        DCD      0x00b71b00
|L1.1024|
        DCD      0x40030000
|L1.1028|
        DCD      0x40034000
|L1.1032|
        DCD      0x40130000
|L1.1036|
        DCD      0x00002710
        ENDP

DrvSPI_FIFOWrite8 PROC
        PUSH     {r3-r7,lr}
        MOV      r5,r0
        MOV      r6,r1
        MOV      r7,r2
        MOVS     r4,#0
        B        |L1.1070|
|L1.1052|
        NOP      
|L1.1054|
        MOV      r0,r5
        BL       SPI_GET_TX_FULL
        CMP      r0,#0
        BNE      |L1.1054|
        LDRB     r0,[r6,r4]
        STR      r0,[r5,#0x20]
        ADDS     r4,r4,#1
|L1.1070|
        CMP      r4,r7
        BCC      |L1.1052|
        MOVS     r0,#1
        POP      {r3-r7,pc}
        ENDP

SPI_FIFOWrite16 PROC
        PUSH     {r3-r7,lr}
        MOV      r5,r0
        MOV      r6,r1
        MOV      r7,r2
        MOVS     r4,#0
        B        |L1.1110|
|L1.1090|
        NOP      
|L1.1092|
        MOV      r0,r5
        BL       SPI_GET_TX_FULL
        CMP      r0,#0
        BNE      |L1.1092|
        LSLS     r0,r4,#1
        LDRH     r0,[r6,r0]
        STR      r0,[r5,#0x20]
        ADDS     r4,r4,#1
|L1.1110|
        CMP      r4,r7
        BCC      |L1.1090|
        MOVS     r0,#1
        POP      {r3-r7,pc}
        ENDP

SPI_FIFOWrite32 PROC
        PUSH     {r3-r7,lr}
        MOV      r5,r0
        MOV      r6,r1
        MOV      r7,r2
        MOVS     r4,#0
        B        |L1.1150|
|L1.1130|
        NOP      
|L1.1132|
        MOV      r0,r5
        BL       SPI_GET_TX_FULL
        CMP      r0,#0
        BNE      |L1.1132|
        LSLS     r0,r4,#2
        LDR      r0,[r6,r0]
        STR      r0,[r5,#0x20]
        ADDS     r4,r4,#1
|L1.1150|
        CMP      r4,r7
        BCC      |L1.1130|
        MOVS     r0,#1
        POP      {r3-r7,pc}
        ENDP

SPI_FIFORead8 PROC
        PUSH     {r3-r7,lr}
        MOV      r5,r0
        MOV      r6,r1
        MOV      r7,r2
        MOVS     r4,#0
        B        |L1.1188|
|L1.1170|
        NOP      
|L1.1172|
        MOV      r0,r5
        BL       SPI_GET_RX_EMPTY
        CMP      r0,#0
        BNE      |L1.1172|
        LDR      r0,[r5,#0x10]
        STRB     r0,[r6,r4]
        ADDS     r4,r4,#1
|L1.1188|
        CMP      r4,r7
        BCC      |L1.1170|
        MOVS     r0,#1
        POP      {r3-r7,pc}
        ENDP

SPI_FIFORead16 PROC
        PUSH     {r3-r7,lr}
        MOV      r5,r0
        MOV      r6,r1
        MOV      r7,r2
        MOVS     r4,#0
        B        |L1.1228|
|L1.1208|
        NOP      
|L1.1210|
        MOV      r0,r5
        BL       SPI_GET_RX_EMPTY
        CMP      r0,#0
        BNE      |L1.1210|
        LDR      r0,[r5,#0x10]
        LSLS     r1,r4,#1
        STRH     r0,[r6,r1]
        ADDS     r4,r4,#1
|L1.1228|
        CMP      r4,r7
        BCC      |L1.1208|
        MOVS     r0,#1
        POP      {r3-r7,pc}
        ENDP

SPI_FIFORead32 PROC
        PUSH     {r3-r7,lr}
        MOV      r5,r0
        MOV      r6,r1
        MOV      r7,r2
        MOVS     r4,#0
        B        |L1.1268|
|L1.1248|
        NOP      
|L1.1250|
        MOV      r0,r5
        BL       SPI_GET_RX_EMPTY
        CMP      r0,#0
        BNE      |L1.1250|
        LDR      r0,[r5,#0x10]
        LSLS     r1,r4,#2
        STR      r0,[r6,r1]
        ADDS     r4,r4,#1
|L1.1268|
        CMP      r4,r7
        BCC      |L1.1248|
        MOVS     r0,#1
        POP      {r3-r7,pc}
        ENDP

SPI_FIFOReadWrite8 PROC
        PUSH     {r0-r7,lr}
        SUB      sp,sp,#4
        MOV      r4,r0
        MOV      r5,r1
        MOV      r6,r2
        MOVS     r0,#0
        STR      r0,[sp,#0]
        MOVS     r7,#0
        B        |L1.1370|
|L1.1294|
        B        |L1.1320|
|L1.1296|
        B        |L1.1310|
|L1.1298|
        LDR      r0,[r4,#0x10]
        UXTB     r2,r0
        LDR      r0,[sp,#0]
        ADDS     r1,r0,#1
        STR      r1,[sp,#0]
        STRB     r2,[r6,r0]
|L1.1310|
        MOV      r0,r4
        BL       SPI_GET_RX_EMPTY
        CMP      r0,#0
        BEQ      |L1.1298|
|L1.1320|
        MOV      r0,r4
        BL       SPI_GET_TX_FULL
        CMP      r0,#0
        BNE      |L1.1296|
        CMP      r5,#0
        BNE      |L1.1340|
        SUBS     r0,r0,#1
        STR      r0,[r4,#0x20]
        B        |L1.1344|
|L1.1340|
        LDRB     r0,[r5,r7]
        STR      r0,[r4,#0x20]
|L1.1344|
        B        |L1.1358|
|L1.1346|
        LDR      r0,[r4,#0x10]
        UXTB     r2,r0
        LDR      r0,[sp,#0]
        ADDS     r1,r0,#1
        STR      r1,[sp,#0]
        STRB     r2,[r6,r0]
|L1.1358|
        MOV      r0,r4
        BL       SPI_GET_RX_EMPTY
        CMP      r0,#0
        BEQ      |L1.1346|
        ADDS     r7,r7,#1
|L1.1370|
        LDR      r0,[sp,#0x10]
        CMP      r7,r0
        BCC      |L1.1294|
        ADD      sp,sp,#0x14
        POP      {r4-r7,pc}
        ENDP

SPI_FIFOReadWrite16 PROC
        PUSH     {r0-r7,lr}
        SUB      sp,sp,#4
        MOV      r4,r0
        MOV      r5,r1
        MOV      r6,r2
        MOVS     r0,#0
        STR      r0,[sp,#0]
        MOVS     r7,#0
        B        |L1.1476|
|L1.1398|
        B        |L1.1424|
|L1.1400|
        B        |L1.1414|
|L1.1402|
        LDR      r2,[r4,#0x10]
        LDR      r0,[sp,#0]
        ADDS     r1,r0,#1
        LSLS     r0,r0,#2
        STR      r1,[sp,#0]
        STR      r2,[r6,r0]
|L1.1414|
        MOV      r0,r4
        BL       SPI_GET_RX_EMPTY
        CMP      r0,#0
        BEQ      |L1.1402|
|L1.1424|
        MOV      r0,r4
        BL       SPI_GET_TX_FULL
        CMP      r0,#0
        BNE      |L1.1400|
        CMP      r5,#0
        BNE      |L1.1444|
        SUBS     r0,r0,#1
        STR      r0,[r4,#0x20]
        B        |L1.1450|
|L1.1444|
        LSLS     r0,r7,#1
        LDRH     r0,[r5,r0]
        STR      r0,[r4,#0x20]
|L1.1450|
        B        |L1.1464|
|L1.1452|
        LDR      r2,[r4,#0x10]
        LDR      r0,[sp,#0]
        ADDS     r1,r0,#1
        LSLS     r0,r0,#2
        STR      r1,[sp,#0]
        STR      r2,[r6,r0]
|L1.1464|
        MOV      r0,r4
        BL       SPI_GET_RX_EMPTY
        CMP      r0,#0
        BEQ      |L1.1452|
        ADDS     r7,r7,#1
|L1.1476|
        LDR      r0,[sp,#0x10]
        CMP      r7,r0
        BCC      |L1.1398|
        ADD      sp,sp,#0x14
        POP      {r4-r7,pc}
        ENDP

SPI_FIFOReadWrite32 PROC
        PUSH     {r0-r7,lr}
        SUB      sp,sp,#4
        MOV      r4,r0
        MOV      r5,r1
        MOV      r6,r2
        MOVS     r0,#0
        STR      r0,[sp,#0]
        MOVS     r7,#0
        B        |L1.1582|
|L1.1504|
        B        |L1.1530|
|L1.1506|
        B        |L1.1520|
|L1.1508|
        LDR      r2,[r4,#0x10]
        LDR      r0,[sp,#0]
        ADDS     r1,r0,#1
        LSLS     r0,r0,#2
        STR      r1,[sp,#0]
        STR      r2,[r6,r0]
|L1.1520|
        MOV      r0,r4
        BL       SPI_GET_RX_EMPTY
        CMP      r0,#0
        BEQ      |L1.1508|
|L1.1530|
        MOV      r0,r4
        BL       SPI_GET_TX_FULL
        CMP      r0,#0
        BNE      |L1.1506|
        CMP      r5,#0
        BNE      |L1.1550|
        SUBS     r0,r0,#1
        STR      r0,[r4,#0x20]
        B        |L1.1556|
|L1.1550|
        LSLS     r0,r7,#2
        LDR      r0,[r5,r0]
        STR      r0,[r4,#0x20]
|L1.1556|
        B        |L1.1570|
|L1.1558|
        LDR      r2,[r4,#0x10]
        LDR      r0,[sp,#0]
        ADDS     r1,r0,#1
        LSLS     r0,r0,#2
        STR      r1,[sp,#0]
        STR      r2,[r6,r0]
|L1.1570|
        MOV      r0,r4
        BL       SPI_GET_RX_EMPTY
        CMP      r0,#0
        BEQ      |L1.1558|
        ADDS     r7,r7,#1
|L1.1582|
        LDR      r0,[sp,#0x10]
        CMP      r7,r0
        BCC      |L1.1504|
        ADD      sp,sp,#0x14
        POP      {r4-r7,pc}
        ENDP

NVIC_EnableIRQ PROC
        LSLS     r2,r0,#27
        LSRS     r2,r2,#27
        MOVS     r1,#1
        LSLS     r1,r1,r2
        LDR      r2,|L1.1696|
        STR      r1,[r2,#0]
        BX       lr
        ENDP

NVIC_DisableIRQ PROC
        LSLS     r2,r0,#27
        LSRS     r2,r2,#27
        MOVS     r1,#1
        LSLS     r1,r1,r2
        LDR      r2,|L1.1696|
        ADDS     r2,r2,#0x80
        STR      r1,[r2,#0]
        BX       lr
        ENDP

APBCLK_ENABLE PROC
        LDR      r1,|L1.1700|
        LDR      r1,[r1,#8]
        ORRS     r1,r1,r0
        LDR      r2,|L1.1700|
        STR      r1,[r2,#8]
        BX       lr
        ENDP

APBCLK_DISABLE PROC
        LDR      r1,|L1.1700|
        LDR      r1,[r1,#8]
        BICS     r1,r1,r0
        LDR      r2,|L1.1700|
        STR      r1,[r2,#8]
        BX       lr
        ENDP

SPI_GET_TX_FULL PROC
        MOV      r1,r0
        LDR      r0,[r1,#0]
        MOVS     r2,#1
        LSLS     r2,r2,#27
        ANDS     r0,r0,r2
        BX       lr
        ENDP

SPI_GET_RX_EMPTY PROC
        MOV      r1,r0
        LDR      r0,[r1,#0]
        MOVS     r2,#1
        LSLS     r2,r2,#24
        ANDS     r0,r0,r2
        BX       lr
        ENDP

SYS_RESET_IP2 PROC
        MOVS     r1,#5
        LSLS     r1,r1,#28
        LDR      r1,[r1,#0xc]
        ORRS     r1,r1,r0
        MOVS     r2,#5
        LSLS     r2,r2,#28
        STR      r1,[r2,#0xc]
        MOV      r1,r2
        LDR      r1,[r1,#0xc]
        BICS     r1,r1,r0
        STR      r1,[r2,#0xc]
        BX       lr
        ENDP

        DCW      0x0000
|L1.1696|
        DCD      0xe000e100
|L1.1700|
        DCD      0x50000200

        AREA ||.arm_vfe_header||, DATA, READONLY, NOALLOC, ALIGN=2

        DCD      0x00000000

        EXPORT DrvSPI_DeInit [CODE]
        EXPORT DrvSPI_Init [CODE]
        EXPORT DrvSPI_GetSourceClockFreq [CODE]
        EXPORT DrvSPI_SetClockFreq [CODE]
        EXPORT DrvSPI_GetClock1Freq [CODE]
        EXPORT DrvSPI_GetClock2Freq [CODE]
        EXPORT DrvSPI_EnableInt [CODE]
        EXPORT DrvSPI_DisableInt [CODE]
        EXPORT SPI_Enable3WireStartInt [CODE]
        EXPORT SPI_Disable3WireStartInt [CODE]
        EXPORT SPI_SetFIFOMode [CODE]
        EXPORT DrvSPI_FIFOWrite8 [CODE]
        EXPORT SPI_FIFOWrite16 [CODE]
        EXPORT SPI_FIFOWrite32 [CODE]
        EXPORT SPI_FIFORead8 [CODE]
        EXPORT SPI_FIFORead16 [CODE]
        EXPORT SPI_FIFORead32 [CODE]
        EXPORT SPI_FIFOReadWrite8 [CODE]
        EXPORT SPI_FIFOReadWrite16 [CODE]
        EXPORT SPI_FIFOReadWrite32 [CODE]

        IMPORT ||Lib$$Request$$armlib|| [CODE,WEAK]
        IMPORT __aeabi_uidivmod [CODE]

        KEEP CLK_GetPLLClockFreq
        KEEP NVIC_EnableIRQ
        KEEP NVIC_DisableIRQ
        KEEP APBCLK_ENABLE
        KEEP APBCLK_DISABLE
        KEEP SPI_GET_TX_FULL
        KEEP SPI_GET_RX_EMPTY
        KEEP SYS_RESET_IP2

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
