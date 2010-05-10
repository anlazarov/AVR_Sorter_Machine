################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/app/BSP/bsp.c 

OBJS += \
./src/app/BSP/bsp.o 

C_DEPS += \
./src/app/BSP/bsp.d 


# Each subdirectory must supply rules for building sources it contributes
src/app/BSP/%.o: ../src/app/BSP/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"/home/user/workspace/AVR_Sorter_Machine/src/app" -I"/home/user/workspace/AVR_Sorter_Machine/src/app/BSP" -I"/home/user/workspace/AVR_Sorter_Machine/src/uCOSII" -I"/home/user/workspace/AVR_Sorter_Machine/src/uCOSII_port" -Wall -g2 -gdwarf-2 -O0 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega2560 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


