################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
F:/test24/25_af_reg03_writeread_with_uart_gpio_ok_test/sdk/bsp/startup/config.c \
F:/test24/25_af_reg03_writeread_with_uart_gpio_ok_test/sdk/bsp/startup/system_ARMCM0.c 

S_UPPER_SRCS += \
F:/test24/25_af_reg03_writeread_with_uart_gpio_ok_test/sdk/bsp/startup/startup_ARMCM0.S \
F:/test24/25_af_reg03_writeread_with_uart_gpio_ok_test/sdk/bsp/startup/vector_table.S 

OBJS += \
./startup/config.o \
./startup/startup_ARMCM0.o \
./startup/system_ARMCM0.o \
./startup/vector_table.o 

S_UPPER_DEPS += \
./startup/startup_ARMCM0.d \
./startup/vector_table.d 

C_DEPS += \
./startup/config.d \
./startup/system_ARMCM0.d 


# Each subdirectory must supply rules for building sources it contributes
startup/config.o: F:/test24/25_af_reg03_writeread_with_uart_gpio_ok_test/sdk/bsp/startup/config.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g -Ddg_configBLACK_ORCA_IC_REV=BLACK_ORCA_IC_REV_A -Ddg_configBLACK_ORCA_IC_STEP=BLACK_ORCA_IC_STEP_E -DCONFIG_AT45DB011D=1 -DCONFIG_24LC256=1 -DCONFIG_FM75=1 -DRELEASE_BUILD -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\memory\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\config" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\config\default" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\config" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\adapters\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\free_rtos\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\osal" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\peripherals\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\system\sys_man\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\app" -include"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\config\custom_config_qspi.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

startup/startup_ARMCM0.o: F:/test24/25_af_reg03_writeread_with_uart_gpio_ok_test/sdk/bsp/startup/startup_ARMCM0.S
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g -x assembler-with-cpp -Ddg_configBLACK_ORCA_IC_REV=BLACK_ORCA_IC_REV_A -Ddg_configBLACK_ORCA_IC_STEP=BLACK_ORCA_IC_STEP_E -DRELEASE_BUILD -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\config" -include"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\config\custom_config_qspi.h" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

startup/system_ARMCM0.o: F:/test24/25_af_reg03_writeread_with_uart_gpio_ok_test/sdk/bsp/startup/system_ARMCM0.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g -Ddg_configBLACK_ORCA_IC_REV=BLACK_ORCA_IC_REV_A -Ddg_configBLACK_ORCA_IC_STEP=BLACK_ORCA_IC_STEP_E -DCONFIG_AT45DB011D=1 -DCONFIG_24LC256=1 -DCONFIG_FM75=1 -DRELEASE_BUILD -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\memory\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\config" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\config\default" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\config" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\adapters\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\free_rtos\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\osal" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\peripherals\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\system\sys_man\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\app" -include"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\config\custom_config_qspi.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

startup/vector_table.o: F:/test24/25_af_reg03_writeread_with_uart_gpio_ok_test/sdk/bsp/startup/vector_table.S
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM GNU Assembler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g -x assembler-with-cpp -Ddg_configBLACK_ORCA_IC_REV=BLACK_ORCA_IC_REV_A -Ddg_configBLACK_ORCA_IC_STEP=BLACK_ORCA_IC_STEP_E -DRELEASE_BUILD -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\config" -include"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\config\custom_config_qspi.h" -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


