################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include startup/subdir.mk
-include sdk/sys_man/subdir.mk
-include sdk/peripherals/src/subdir.mk
-include sdk/osal/subdir.mk
-include sdk/memory/src/subdir.mk
-include sdk/free_rtos/portable/MemMang/subdir.mk
-include sdk/free_rtos/portable/GCC/ARM_CM0/subdir.mk
-include sdk/free_rtos/subdir.mk
-include sdk/adapters/src/subdir.mk
-include demos/subdir.mk
-include app/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
peripherals_demo.bin \

SECONDARY_SIZE += \
peripherals_demo.siz \


# All Target
all: pre-build main-build

# Main-build Target
main-build: peripherals_demo.elf secondary-outputs

# Tool invocations
peripherals_demo.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross ARM C Linker'
	arm-none-eabi-gcc -mcpu=cortex-m0 -mthumb -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -Wall  -g -T "F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\projects\dk_apps\demos\peripherals_demo/DA14681-01-Release_QSPI/mem.ld" -T "F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\projects\dk_apps\demos\peripherals_demo/DA14681-01-Release_QSPI/sections.ld" -Xlinker --gc-sections -L"F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\misc" -Wl,-Map,"peripherals_demo.map" --specs=nano.specs --specs=nosys.specs -o "peripherals_demo.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

peripherals_demo.bin: peripherals_demo.elf
	@echo 'Invoking: Cross ARM GNU Create Flash Image'
	arm-none-eabi-objcopy -O binary "peripherals_demo.elf"  "peripherals_demo.bin"
	@echo 'Finished building: $@'
	@echo ' '

peripherals_demo.siz: peripherals_demo.elf
	@echo 'Invoking: Cross ARM GNU Print Size'
	arm-none-eabi-size --format=berkeley "peripherals_demo.elf"
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(SECONDARY_FLASH)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_UPPER_DEPS)$(C_DEPS) peripherals_demo.elf
	-@echo ' '

pre-build:
	-@echo 'Generate linker scripts.'
	-make generate_ldscripts LD_DEFS=-DRELEASE_BUILD IC_REV=A IC_STEP=E APP_CONFIG_H="F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\projects\dk_apps\demos\peripherals_demo\config\custom_config_qspi.h" CC="arm-none-eabi-gcc" BSP_CONFIG_DIR="F:\Awork_BLE5_DA14681\20170614_da_sdk\DA1468x_SDK_BTLE_v_1.0.8.1050.1\sdk\bsp\config"
	-@echo ' '

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_SIZE)

.PHONY: all clean dependents
.SECONDARY: main-build pre-build

-include ../makefile.targets
