################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include src/uCOSII_port/subdir.mk
-include src/uCOSII/subdir.mk
-include src/app/lego_interface/subdir.mk
-include src/app/subdir.mk
-include src/app/BSP/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
LSS += \
Micro_OSII_ATMEGA256x.lss \

FLASH_IMAGE += \
Micro_OSII_ATMEGA256x.hex \

SIZEDUMMY += \
sizedummy \


# All Target
all: Micro_OSII_ATMEGA256x.elf secondary-outputs

# Tool invocations
Micro_OSII_ATMEGA256x.elf: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: AVR C Linker'
	avr-gcc -Wl,-Map,Micro_OSII_ATMEGA256x.map -Xlinker -allow-multiple-definition -mmcu=atmega2560 -o"Micro_OSII_ATMEGA256x.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

Micro_OSII_ATMEGA256x.lss: Micro_OSII_ATMEGA256x.elf
	@echo 'Invoking: AVR Create Extended Listing'
	-avr-objdump -h -S Micro_OSII_ATMEGA256x.elf  >"Micro_OSII_ATMEGA256x.lss"
	@echo 'Finished building: $@'
	@echo ' '

Micro_OSII_ATMEGA256x.hex: Micro_OSII_ATMEGA256x.elf
	@echo 'Create Flash image (ihex format)'
	-avr-objcopy -R .eeprom -O ihex Micro_OSII_ATMEGA256x.elf  "Micro_OSII_ATMEGA256x.hex"
	@echo 'Finished building: $@'
	@echo ' '

sizedummy: Micro_OSII_ATMEGA256x.elf
	@echo 'Invoking: Print Size'
	-avr-size --format=berkeley -t Micro_OSII_ATMEGA256x.elf
	@echo 'Finished building: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(ASM_DEPS)$(FLASH_IMAGE)$(ELFS)$(LSS)$(S_DEPS)$(SIZEDUMMY)$(S_UPPER_DEPS) Micro_OSII_ATMEGA256x.elf
	-@echo ' '

secondary-outputs: $(LSS) $(FLASH_IMAGE) $(SIZEDUMMY)

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
