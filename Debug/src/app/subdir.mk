################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/app/app.c 

ASM_SRCS += \
../src/app/app_isr.asm 

OBJS += \
./src/app/app.o \
./src/app/app_isr.o 

C_DEPS += \
./src/app/app.d 

ASM_DEPS += \
./src/app/app_isr.d 


# Each subdirectory must supply rules for building sources it contributes
src/app/%.o: ../src/app/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"/home/user/workspace/AVR_Sorter_Machine/src/app" -I"/home/user/workspace/AVR_Sorter_Machine/src/app/BSP" -I"/home/user/workspace/AVR_Sorter_Machine/src/uCOSII" -I"/home/user/workspace/AVR_Sorter_Machine/src/uCOSII_port" -Wall -g2 -gdwarf-2 -O0 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega2560 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/app/%.o: ../src/app/%.asm
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Assembler'
	avr-gcc -x assembler-with-cpp -I"/home/user/workspace/AVR_Sorter_Machine/src/uCOSII_port" -g2 -gstabs -mmcu=atmega2560 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


