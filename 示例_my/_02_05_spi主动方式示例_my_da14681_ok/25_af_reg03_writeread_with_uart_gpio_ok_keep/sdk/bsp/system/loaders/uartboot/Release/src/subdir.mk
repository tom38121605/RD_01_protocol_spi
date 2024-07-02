################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/crc16.c \
../src/main.c 

OBJS += \
./src/crc16.o \
./src/main.o 

C_DEPS += \
./src/crc16.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Werror -Wall  -g -DRELEASE_BUILD -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\adapters\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\memory\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\config" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\peripherals\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\system\loaders\uartboot\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\osal" -include"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\system\loaders\uartboot\config\custom_config.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


