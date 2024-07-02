################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../main.c 

OBJS += \
./main.o 

C_DEPS += \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g -Ddg_configBLACK_ORCA_IC_REV=BLACK_ORCA_IC_REV_A -Ddg_configBLACK_ORCA_IC_STEP=BLACK_ORCA_IC_STEP_E -DCONFIG_AT45DB011D=1 -DCONFIG_24LC256=1 -DCONFIG_FM75=1 -DRELEASE_BUILD -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\memory\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\projects\dk_apps\demos\peripherals_demo\config" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\projects\dk_apps\demos\peripherals_demo\config\default" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\config" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\adapters\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\free_rtos\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\osal" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\peripherals\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\system\sys_man\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\projects\dk_apps\demos\peripherals_demo" -include"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\projects\dk_apps\demos\peripherals_demo\config\custom_config_qspi.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


