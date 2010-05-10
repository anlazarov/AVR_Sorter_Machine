################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/app/lego_interface/lego_interface.c 

OBJS += \
./src/app/lego_interface/lego_interface.o 

C_DEPS += \
./src/app/lego_interface/lego_interface.d 


# Each subdirectory must supply rules for building sources it contributes
src/app/lego_interface/%.o: ../src/app/lego_interface/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"/home/alex/Dropbox/Workspace/Semester 04/RIEI1/Micro_OSII_ATMEGA256x/src/app" -I"/home/alex/Dropbox/Workspace/Semester 04/RIEI1/Micro_OSII_ATMEGA256x/src/app/BSP" -I"/home/alex/Dropbox/Workspace/Semester 04/RIEI1/Micro_OSII_ATMEGA256x/src/uCOSII" -I"/home/alex/Dropbox/Workspace/Semester 04/RIEI1/Micro_OSII_ATMEGA256x/src/uCOSII_port" -Wall -g2 -gdwarf-2 -O0 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega2560 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


