################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../demos/at45db011d.c \
../demos/bh1750.c \
../demos/demo_ad_gpadc.c \
../demos/demo_breath.c \
../demos/demo_gpadc.c \
../demos/demo_i2c.c \
../demos/demo_i2c_async.c \
../demos/demo_i2c_spi.c \
../demos/demo_irgen.c \
../demos/demo_power_mode.c \
../demos/demo_qspi.c \
../demos/demo_quad.c \
../demos/demo_sensors.c \
../demos/demo_spi_os.c \
../demos/demo_timer0.c \
../demos/demo_timer1.c \
../demos/demo_timer2.c \
../demos/demo_uart_os.c \
../demos/demo_uart_printf.c \
../demos/demo_wkup.c \
../demos/eeprom_24xx256.c \
../demos/periph_setup.c \
../demos/qspi_w25q16dv.c \
../demos/system_clock.c 

OBJS += \
./demos/at45db011d.o \
./demos/bh1750.o \
./demos/demo_ad_gpadc.o \
./demos/demo_breath.o \
./demos/demo_gpadc.o \
./demos/demo_i2c.o \
./demos/demo_i2c_async.o \
./demos/demo_i2c_spi.o \
./demos/demo_irgen.o \
./demos/demo_power_mode.o \
./demos/demo_qspi.o \
./demos/demo_quad.o \
./demos/demo_sensors.o \
./demos/demo_spi_os.o \
./demos/demo_timer0.o \
./demos/demo_timer1.o \
./demos/demo_timer2.o \
./demos/demo_uart_os.o \
./demos/demo_uart_printf.o \
./demos/demo_wkup.o \
./demos/eeprom_24xx256.o \
./demos/periph_setup.o \
./demos/qspi_w25q16dv.o \
./demos/system_clock.o 

C_DEPS += \
./demos/at45db011d.d \
./demos/bh1750.d \
./demos/demo_ad_gpadc.d \
./demos/demo_breath.d \
./demos/demo_gpadc.d \
./demos/demo_i2c.d \
./demos/demo_i2c_async.d \
./demos/demo_i2c_spi.d \
./demos/demo_irgen.d \
./demos/demo_power_mode.d \
./demos/demo_qspi.d \
./demos/demo_quad.d \
./demos/demo_sensors.d \
./demos/demo_spi_os.d \
./demos/demo_timer0.d \
./demos/demo_timer1.d \
./demos/demo_timer2.d \
./demos/demo_uart_os.d \
./demos/demo_uart_printf.d \
./demos/demo_wkup.d \
./demos/eeprom_24xx256.d \
./demos/periph_setup.d \
./demos/qspi_w25q16dv.d \
./demos/system_clock.d 


# Each subdirectory must supply rules for building sources it contributes
demos/%.o: ../demos/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g -Ddg_configBLACK_ORCA_IC_REV=BLACK_ORCA_IC_REV_A -Ddg_configBLACK_ORCA_IC_STEP=BLACK_ORCA_IC_STEP_E -DCONFIG_AT45DB011D=1 -DCONFIG_24LC256=1 -DCONFIG_FM75=1 -DRELEASE_BUILD -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\memory\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\config" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\config\default" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\config" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\adapters\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\free_rtos\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\osal" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\peripherals\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\sdk\bsp\system\sys_man\include" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo" -I"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\app" -include"F:\test24\25_af_reg03_writeread_with_uart_gpio_ok_test\projects\dk_apps\demos\peripherals_demo\config\custom_config_qspi.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


