################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
F:/test24/25_af_reg03_writeread_with_uart_gpio_ok_test/sdk/bsp/free_rtos/croutine.c \
F:/test24/25_af_reg03_writeread_with_uart_gpio_ok_test/sdk/bsp/free_rtos/event_groups.c \
F:/test24/25_af_reg03_writeread_with_uart_gpio_ok_test/sdk/bsp/free_rtos/list.c \
F:/test24/25_af_reg03_writeread_with_uart_gpio_ok_test/sdk/bsp/free_rtos/queue.c \
F:/test24/25_af_reg03_writeread_with_uart_gpio_ok_test/sdk/bsp/free_rtos/tasks.c \
F:/test24/25_af_reg03_writeread_with_uart_gpio_ok_test/sdk/bsp/free_rtos/timers.c 

OBJS += \
./sdk/free_rtos/croutine.o \
./sdk/free_rtos/event_groups.o \
./sdk/free_rtos/list.o \
./sdk/free_rtos/queue.o \
./sdk/free_rtos/tasks.o \
./sdk/free_rtos/timers.o 

C_DEPS += \
./sdk/free_rtos/croutine.d \
./sdk/free_rtos/event_groups.d \
./sdk/free_rtos/list.d \
./sdk/free_rtos/queue.d \
./sdk/free_rtos/tasks.d \
./sdk/free_rtos/timers.d 


# Each subdirectory must supply rules for building sources it contributes
sdk/free_rtos/croutine.o: F:/test24/25_af_reg03_writeread_with_uart_gpio_ok_test/sdk/bsp/free_rtos/croutine.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g -Ddg_configBLACK_ORCA_IC_REV=BLACK_ORCA_IC_REV_A -Ddg_configBLACK_ORCA_IC_STEP=BLACK_ORCA_IC_STEP_E -DCONFIG_AT45DB011D=1 -DCONFIG_24LC256=1 -DCONFIG_FM75=1 -DRELEASE_BUILD -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\memory\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\config" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\config\default" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\config" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\adapters\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\free_rtos\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\osal" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\peripherals\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\system\sys_man\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\app" -include"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\config\custom_config_qspi.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

sdk/free_rtos/event_groups.o: F:/test24/25_af_reg03_writeread_with_uart_gpio_ok_test/sdk/bsp/free_rtos/event_groups.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g -Ddg_configBLACK_ORCA_IC_REV=BLACK_ORCA_IC_REV_A -Ddg_configBLACK_ORCA_IC_STEP=BLACK_ORCA_IC_STEP_E -DCONFIG_AT45DB011D=1 -DCONFIG_24LC256=1 -DCONFIG_FM75=1 -DRELEASE_BUILD -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\memory\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\config" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\config\default" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\config" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\adapters\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\free_rtos\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\osal" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\peripherals\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\system\sys_man\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\app" -include"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\config\custom_config_qspi.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

sdk/free_rtos/list.o: F:/test24/25_af_reg03_writeread_with_uart_gpio_ok_test/sdk/bsp/free_rtos/list.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g -Ddg_configBLACK_ORCA_IC_REV=BLACK_ORCA_IC_REV_A -Ddg_configBLACK_ORCA_IC_STEP=BLACK_ORCA_IC_STEP_E -DCONFIG_AT45DB011D=1 -DCONFIG_24LC256=1 -DCONFIG_FM75=1 -DRELEASE_BUILD -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\memory\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\config" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\config\default" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\config" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\adapters\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\free_rtos\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\osal" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\peripherals\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\system\sys_man\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\app" -include"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\config\custom_config_qspi.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

sdk/free_rtos/queue.o: F:/test24/25_af_reg03_writeread_with_uart_gpio_ok_test/sdk/bsp/free_rtos/queue.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g -Ddg_configBLACK_ORCA_IC_REV=BLACK_ORCA_IC_REV_A -Ddg_configBLACK_ORCA_IC_STEP=BLACK_ORCA_IC_STEP_E -DCONFIG_AT45DB011D=1 -DCONFIG_24LC256=1 -DCONFIG_FM75=1 -DRELEASE_BUILD -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\memory\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\config" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\config\default" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\config" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\adapters\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\free_rtos\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\osal" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\peripherals\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\system\sys_man\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\app" -include"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\config\custom_config_qspi.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

sdk/free_rtos/tasks.o: F:/test24/25_af_reg03_writeread_with_uart_gpio_ok_test/sdk/bsp/free_rtos/tasks.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g -Ddg_configBLACK_ORCA_IC_REV=BLACK_ORCA_IC_REV_A -Ddg_configBLACK_ORCA_IC_STEP=BLACK_ORCA_IC_STEP_E -DCONFIG_AT45DB011D=1 -DCONFIG_24LC256=1 -DCONFIG_FM75=1 -DRELEASE_BUILD -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\memory\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\config" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\config\default" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\config" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\adapters\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\free_rtos\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\osal" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\peripherals\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\system\sys_man\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\app" -include"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\config\custom_config_qspi.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

sdk/free_rtos/timers.o: F:/test24/25_af_reg03_writeread_with_uart_gpio_ok_test/sdk/bsp/free_rtos/timers.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g -Ddg_configBLACK_ORCA_IC_REV=BLACK_ORCA_IC_REV_A -Ddg_configBLACK_ORCA_IC_STEP=BLACK_ORCA_IC_STEP_E -DCONFIG_AT45DB011D=1 -DCONFIG_24LC256=1 -DCONFIG_FM75=1 -DRELEASE_BUILD -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\memory\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\config" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\config\default" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\config" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\adapters\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\free_rtos\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\osal" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\peripherals\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\system\sys_man\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\app" -include"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\config\custom_config_qspi.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


