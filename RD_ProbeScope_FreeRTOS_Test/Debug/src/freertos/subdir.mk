################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/freertos/FreeRTOSCommonHooks.c \
../src/freertos/croutine.c \
../src/freertos/heap_3.c \
../src/freertos/list.c \
../src/freertos/port.c \
../src/freertos/queue.c \
../src/freertos/tasks.c \
../src/freertos/timers.c 

C_DEPS += \
./src/freertos/FreeRTOSCommonHooks.d \
./src/freertos/croutine.d \
./src/freertos/heap_3.d \
./src/freertos/list.d \
./src/freertos/port.d \
./src/freertos/queue.d \
./src/freertos/tasks.d \
./src/freertos/timers.d 

OBJS += \
./src/freertos/FreeRTOSCommonHooks.o \
./src/freertos/croutine.o \
./src/freertos/heap_3.o \
./src/freertos/list.o \
./src/freertos/port.o \
./src/freertos/queue.o \
./src/freertos/tasks.o \
./src/freertos/timers.o 


# Each subdirectory must supply rules for building sources it contributes
src/freertos/%.o: ../src/freertos/%.c src/freertos/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M4 -D__USE_LPCOPEN -DNO_BOARD_LIB -D__LPC43XX__ -D__REDLIB__ -I"C:\Users\chadb\Documents\MCUXpressoIDE_11.7.0_9198\workspace\lpc_chip_43xx\inc" -I"C:\Users\chadb\Documents\MCUXpressoIDE_11.7.0_9198\workspace\freertos_blinky\inc" -I"C:\Users\chadb\Documents\MCUXpressoIDE_11.7.0_9198\workspace\freertos\inc" -I"C:\Users\chadb\Documents\MCUXpressoIDE_11.7.0_9198\workspace\usbd_rom_libusb\inc" -I"C:\Users\chadb\Documents\MCUXpressoIDE_11.7.0_9198\workspace\lpc_board_nxp_lpclink2_4370\inc" -I"C:\Users\chadb\Documents\MCUXpressoIDE_11.7.0_9198\workspace\freertos\inc_arm_cm4f" -I"C:\Users\chadb\Documents\MCUXpressoIDE_11.7.0_9198\workspace\freertos\inc_arm_m0" -I"C:\Users\chadb\Documents\MCUXpressoIDE_11.7.0_9198\workspace\freertos\inc_freertoslpc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fsingle-precision-constant -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src-2f-freertos

clean-src-2f-freertos:
	-$(RM) ./src/freertos/FreeRTOSCommonHooks.d ./src/freertos/FreeRTOSCommonHooks.o ./src/freertos/croutine.d ./src/freertos/croutine.o ./src/freertos/heap_3.d ./src/freertos/heap_3.o ./src/freertos/list.d ./src/freertos/list.o ./src/freertos/port.d ./src/freertos/port.o ./src/freertos/queue.d ./src/freertos/queue.o ./src/freertos/tasks.d ./src/freertos/tasks.o ./src/freertos/timers.d ./src/freertos/timers.o

.PHONY: clean-src-2f-freertos

