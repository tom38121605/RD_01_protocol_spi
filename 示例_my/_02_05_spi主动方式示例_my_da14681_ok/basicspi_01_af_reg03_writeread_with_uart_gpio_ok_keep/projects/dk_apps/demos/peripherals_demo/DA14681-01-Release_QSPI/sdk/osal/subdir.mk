################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
F:/test24/25_af_reg03_writeread_with_uart_gpio_ok_test/sdk/bsp/osal/msg_queues.c \
F:/test24/25_af_reg03_writeread_with_uart_gpio_ok_test/sdk/bsp/osal/resmgmt.c \
F:/test24/25_af_reg03_writeread_with_uart_gpio_ok_test/sdk/bsp/osal/usb_osal_wrapper.c 

OBJS += \
./sdk/osal/msg_queues.o \
./sdk/osal/resmgmt.o \
./sdk/osal/usb_osal_wrapper.o 

C_DEPS += \
./sdk/osal/msg_queues.d \
./sdk/osal/resmgmt.d \
./sdk/osal/usb_osal_wrapper.d 


# Each subdirectory must supply rules for building sources it contributes
sdk/osal/msg_queues.o: F:/test24/25_af_reg03_writeread_with_uart_gpio_ok_test/sdk/bsp/osal/msg_queues.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g -Ddg_configBLACK_ORCA_IC_REV=BLACK_ORCA_IC_REV_A -Ddg_configBLACK_ORCA_IC_STEP=BLACK_ORCA_IC_STEP_E -DCONFIG_AT45DB011D=1 -DCONFIG_24LC256=1 -DCONFIG_FM75=1 -DRELEASE_BUILD -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\memory\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\config" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\config\default" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\config" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\adapters\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\free_rtos\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\osal" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\peripherals\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\system\sys_man\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\app" -include"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\config\custom_config_qspi.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

sdk/osal/resmgmt.o: F:/test24/25_af_reg03_writeread_with_uart_gpio_ok_test/sdk/bsp/osal/resmgmt.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g -Ddg_configBLACK_ORCA_IC_REV=BLACK_ORCA_IC_REV_A -Ddg_configBLACK_ORCA_IC_STEP=BLACK_ORCA_IC_STEP_E -DCONFIG_AT45DB011D=1 -DCONFIG_24LC256=1 -DCONFIG_FM75=1 -DRELEASE_BUILD -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\memory\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\config" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\config\default" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\config" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\adapters\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\free_rtos\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\osal" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\peripherals\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\system\sys_man\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\app" -include"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\config\custom_config_qspi.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

sdk/osal/usb_osal_wrapper.o: F:/test24/25_af_reg03_writeread_with_uart_gpio_ok_test/sdk/bsp/osal/usb_osal_wrapper.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g -Ddg_configBLACK_ORCA_IC_REV=BLACK_ORCA_IC_REV_A -Ddg_configBLACK_ORCA_IC_STEP=BLACK_ORCA_IC_STEP_E -DCONFIG_AT45DB011D=1 -DCONFIG_24LC256=1 -DCONFIG_FM75=1 -DRELEASE_BUILD -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\memory\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\config" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\config\default" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\config" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\adapters\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\free_rtos\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\osal" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\peripherals\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\system\sys_man\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\app" -include"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\config\custom_config_qspi.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


