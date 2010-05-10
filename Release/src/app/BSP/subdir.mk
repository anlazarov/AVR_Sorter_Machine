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
	avr-gcc -I"/home/alex/Dropbox/Workspace/Semester 04/RIEI1/Micro_OSII_ATMEGA256x/src/app" -I"/home/alex/Dropbox/Workspace/Semester 04/RIEI1/Micro_OSII_ATMEGA256x/src/app/BSP" -I"/home/alex/Dropbox/Workspace/Semester 04/RIEI1/Micro_OSII_ATMEGA256x/src/uCOSII" -I"/home/alex/Dropbox/Workspace/Semester 04/RIEI1/Micro_OSII_ATMEGA256x/src/uCOSII_port" -Wall -Os -fpack-struct -fshort-enums -funsigned-char -funsigned-bitfields -mmcu=atmega2560 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


