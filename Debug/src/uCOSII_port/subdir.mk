################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/uCOSII_port/os_cpu_c.c \
../src/uCOSII_port/os_dbg.c 

ASM_SRCS += \
../src/uCOSII_port/os_cpu_a.asm \
../src/uCOSII_port/os_cpu_i.asm 

OBJS += \
./src/uCOSII_port/os_cpu_a.o \
./src/uCOSII_port/os_cpu_c.o \
./src/uCOSII_port/os_cpu_i.o \
./src/uCOSII_port/os_dbg.o 

C_DEPS += \
./src/uCOSII_port/os_cpu_c.d \
./src/uCOSII_port/os_dbg.d 

ASM_DEPS += \
./src/uCOSII_port/os_cpu_a.d \
./src/uCOSII_port/os_cpu_i.d 


# Each subdirectory must supply rules for building sources it contributes
src/uCOSII_port/%.o: ../src/uCOSII_port/%.asm
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Assembler'
	avr-gcc -x assembler-with-cpp -I"/home/user/workspace/AVR_Sorter_Machine/src/uCOSII_port" -g2 -gstabs -mmcu=atmega2560 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

src/uCOSII_port/%.o: ../src/uCOSII_port/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"/home/user/workspace/AVR_Sorter_Machine/src/app" -I"/home/user/workspace/AVR_Sorter_Machine/src/app/BSP" -I"/home/user/workspace/AVR_Sorter_Machine/src/uCOSII" -I"/home/user/workspace/AVR_Sorter_Machine/src/uCOSII_port" -Wall -g2 -gdwarf-2 -O0 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega2560 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


