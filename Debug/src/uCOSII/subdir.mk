################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/uCOSII/os_core.c \
../src/uCOSII/os_dbg_r.c \
../src/uCOSII/os_flag.c \
../src/uCOSII/os_mbox.c \
../src/uCOSII/os_mem.c \
../src/uCOSII/os_mutex.c \
../src/uCOSII/os_q.c \
../src/uCOSII/os_sem.c \
../src/uCOSII/os_task.c \
../src/uCOSII/os_time.c \
../src/uCOSII/os_tmr.c \
../src/uCOSII/ucos_ii.c 

OBJS += \
./src/uCOSII/os_core.o \
./src/uCOSII/os_dbg_r.o \
./src/uCOSII/os_flag.o \
./src/uCOSII/os_mbox.o \
./src/uCOSII/os_mem.o \
./src/uCOSII/os_mutex.o \
./src/uCOSII/os_q.o \
./src/uCOSII/os_sem.o \
./src/uCOSII/os_task.o \
./src/uCOSII/os_time.o \
./src/uCOSII/os_tmr.o \
./src/uCOSII/ucos_ii.o 

C_DEPS += \
./src/uCOSII/os_core.d \
./src/uCOSII/os_dbg_r.d \
./src/uCOSII/os_flag.d \
./src/uCOSII/os_mbox.d \
./src/uCOSII/os_mem.d \
./src/uCOSII/os_mutex.d \
./src/uCOSII/os_q.d \
./src/uCOSII/os_sem.d \
./src/uCOSII/os_task.d \
./src/uCOSII/os_time.d \
./src/uCOSII/os_tmr.d \
./src/uCOSII/ucos_ii.d 


# Each subdirectory must supply rules for building sources it contributes
src/uCOSII/%.o: ../src/uCOSII/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: AVR Compiler'
	avr-gcc -I"/home/user/workspace/AVR_Sorter_Machine/src/app" -I"/home/user/workspace/AVR_Sorter_Machine/src/app/BSP" -I"/home/user/workspace/AVR_Sorter_Machine/src/uCOSII" -I"/home/user/workspace/AVR_Sorter_Machine/src/uCOSII_port" -Wall -g2 -gdwarf-2 -O0 -fpack-struct -fshort-enums -std=gnu99 -funsigned-char -funsigned-bitfields -mmcu=atmega2560 -DF_CPU=16000000UL -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -c -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


