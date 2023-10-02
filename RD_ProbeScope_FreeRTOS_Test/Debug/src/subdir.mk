################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/RD_ProbeScope_FreeRTOS_Test.c \
../src/cr_start_m0.c \
../src/cr_startup_lpc43xx.c \
../src/crp.c \
../src/sysinit.c 

C_DEPS += \
./src/RD_ProbeScope_FreeRTOS_Test.d \
./src/cr_start_m0.d \
./src/cr_startup_lpc43xx.d \
./src/crp.d \
./src/sysinit.d 

OBJS += \
./src/RD_ProbeScope_FreeRTOS_Test.o \
./src/cr_start_m0.o \
./src/cr_startup_lpc43xx.o \
./src/crp.o \
./src/sysinit.o 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c src/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -DDEBUG -D__CODE_RED -DCORE_M4 -D__USE_LPCOPEN -DNO_BOARD_LIB -D__LPC43XX__ -D__REDLIB__ -I"C:\Users\chadb\Documents\MCUXpressoIDE_11.7.0_9198\workspace\lpc_chip_43xx\inc" -I"C:\Users\chadb\Documents\MCUXpressoIDE_11.7.0_9198\workspace\freertos_blinky\inc" -I"C:\Users\chadb\Documents\MCUXpressoIDE_11.7.0_9198\workspace\freertos\inc" -I"C:\Users\chadb\Documents\MCUXpressoIDE_11.7.0_9198\workspace\usbd_rom_libusb\inc" -I"C:\Users\chadb\Documents\MCUXpressoIDE_11.7.0_9198\workspace\lpc_board_nxp_lpclink2_4370\inc" -I"C:\Users\chadb\Documents\MCUXpressoIDE_11.7.0_9198\workspace\freertos\inc_arm_cm4f" -I"C:\Users\chadb\Documents\MCUXpressoIDE_11.7.0_9198\workspace\freertos\inc_arm_m0" -I"C:\Users\chadb\Documents\MCUXpressoIDE_11.7.0_9198\workspace\freertos\inc_freertoslpc" -O0 -fno-common -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -fsingle-precision-constant -fmerge-constants -fmacro-prefix-map="$(<D)/"= -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -fstack-usage -specs=redlib.specs -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


clean: clean-src

clean-src:
	-$(RM) ./src/RD_ProbeScope_FreeRTOS_Test.d ./src/RD_ProbeScope_FreeRTOS_Test.o ./src/cr_start_m0.d ./src/cr_start_m0.o ./src/cr_startup_lpc43xx.d ./src/cr_startup_lpc43xx.o ./src/crp.d ./src/crp.o ./src/sysinit.d ./src/sysinit.o

.PHONY: clean-src

