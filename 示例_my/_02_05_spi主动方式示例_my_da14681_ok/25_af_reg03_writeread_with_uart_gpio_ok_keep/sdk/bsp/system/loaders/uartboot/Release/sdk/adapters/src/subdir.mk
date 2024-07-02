################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
F:/Awork_BLE5_DA14681/20170614_da_sdk/DA1468x_SDK_BTLE_v_1.0.8.1050.1/sdk/bsp/adapters/src/ad_battery.c \
F:/Awork_BLE5_DA14681/20170614_da_sdk/DA1468x_SDK_BTLE_v_1.0.8.1050.1/sdk/bsp/adapters/src/ad_crypto.c \
F:/Awork_BLE5_DA14681/20170614_da_sdk/DA1468x_SDK_BTLE_v_1.0.8.1050.1/sdk/bsp/adapters/src/ad_flash.c \
F:/Awork_BLE5_DA14681/20170614_da_sdk/DA1468x_SDK_BTLE_v_1.0.8.1050.1/sdk/bsp/adapters/src/ad_keyboard_scanner.c \
F:/Awork_BLE5_DA14681/20170614_da_sdk/DA1468x_SDK_BTLE_v_1.0.8.1050.1/sdk/bsp/adapters/src/ad_nvms.c \
F:/Awork_BLE5_DA14681/20170614_da_sdk/DA1468x_SDK_BTLE_v_1.0.8.1050.1/sdk/bsp/adapters/src/ad_nvms_direct.c \
F:/Awork_BLE5_DA14681/20170614_da_sdk/DA1468x_SDK_BTLE_v_1.0.8.1050.1/sdk/bsp/adapters/src/ad_nvms_ves.c 

OBJS += \
./sdk/adapters/src/ad_battery.o \
./sdk/adapters/src/ad_crypto.o \
./sdk/adapters/src/ad_flash.o \
./sdk/adapters/src/ad_keyboard_scanner.o \
./sdk/adapters/src/ad_nvms.o \
./sdk/adapters/src/ad_nvms_direct.o \
./sdk/adapters/src/ad_nvms_ves.o 

C_DEPS += \
./sdk/adapters/src/ad_battery.d \
./sdk/adapters/src/ad_crypto.d \
./sdk/adapters/src/ad_flash.d \
./sdk/adapters/src/ad_keyboard_scanner.d \
./sdk/adapters/src/ad_nvms.d \
./sdk/adapters/src/ad_nvms_direct.d \
./sdk/adapters/src/ad_nvms_ves.d 


# Each subdirectory must supply rules for building sources it contributes
sdk/adapters/src/ad_battery.o: F:/Awork_BLE5_DA14681/20170614_da_sdk/DA1468x_SDK_BTLE_v_1.0.8.1050.1/sdk/bsp/adapters/src/ad_battery.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Werror -Wall  -g -DRELEASE_BUILD -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\adapters\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\memory\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\config" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\peripherals\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\system\loaders\uartboot\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\osal" -include"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\system\loaders\uartboot\config\custom_config.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

sdk/adapters/src/ad_crypto.o: F:/Awork_BLE5_DA14681/20170614_da_sdk/DA1468x_SDK_BTLE_v_1.0.8.1050.1/sdk/bsp/adapters/src/ad_crypto.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Werror -Wall  -g -DRELEASE_BUILD -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\adapters\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\memory\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\config" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\peripherals\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\system\loaders\uartboot\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\osal" -include"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\system\loaders\uartboot\config\custom_config.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

sdk/adapters/src/ad_flash.o: F:/Awork_BLE5_DA14681/20170614_da_sdk/DA1468x_SDK_BTLE_v_1.0.8.1050.1/sdk/bsp/adapters/src/ad_flash.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Werror -Wall  -g -DRELEASE_BUILD -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\adapters\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\memory\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\config" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\peripherals\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\system\loaders\uartboot\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\osal" -include"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\system\loaders\uartboot\config\custom_config.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

sdk/adapters/src/ad_keyboard_scanner.o: F:/Awork_BLE5_DA14681/20170614_da_sdk/DA1468x_SDK_BTLE_v_1.0.8.1050.1/sdk/bsp/adapters/src/ad_keyboard_scanner.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Werror -Wall  -g -DRELEASE_BUILD -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\adapters\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\memory\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\config" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\peripherals\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\system\loaders\uartboot\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\osal" -include"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\system\loaders\uartboot\config\custom_config.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

sdk/adapters/src/ad_nvms.o: F:/Awork_BLE5_DA14681/20170614_da_sdk/DA1468x_SDK_BTLE_v_1.0.8.1050.1/sdk/bsp/adapters/src/ad_nvms.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Werror -Wall  -g -DRELEASE_BUILD -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\adapters\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\memory\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\config" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\peripherals\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\system\loaders\uartboot\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\osal" -include"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\system\loaders\uartboot\config\custom_config.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

sdk/adapters/src/ad_nvms_direct.o: F:/Awork_BLE5_DA14681/20170614_da_sdk/DA1468x_SDK_BTLE_v_1.0.8.1050.1/sdk/bsp/adapters/src/ad_nvms_direct.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Werror -Wall  -g -DRELEASE_BUILD -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\adapters\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\memory\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\config" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\peripherals\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\system\loaders\uartboot\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\osal" -include"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\system\loaders\uartboot\config\custom_config.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

sdk/adapters/src/ad_nvms_ves.o: F:/Awork_BLE5_DA14681/20170614_da_sdk/DA1468x_SDK_BTLE_v_1.0.8.1050.1/sdk/bsp/adapters/src/ad_nvms_ves.c
	@echo 'Building file: $<'
	@echo 'Invoking: Cross ARM C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Werror -Wall  -g -DRELEASE_BUILD -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\adapters\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\memory\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\config" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\peripherals\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\system\loaders\uartboot\include" -I"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\osal" -include"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\system\loaders\uartboot\config\custom_config.h" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


