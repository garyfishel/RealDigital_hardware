################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/freertos/subdir.mk
-include src/subdir.mk
ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

OPTIONAL_TOOL_DEPS := \
$(wildcard ../makefile.defs) \
$(wildcard ../makefile.init) \
$(wildcard ../makefile.targets) \


BUILD_ARTIFACT_NAME := RD_ProbeScope_FreeRTOS_Test
BUILD_ARTIFACT_EXTENSION := axf
BUILD_ARTIFACT_PREFIX :=
BUILD_ARTIFACT := $(BUILD_ARTIFACT_PREFIX)$(BUILD_ARTIFACT_NAME)$(if $(BUILD_ARTIFACT_EXTENSION),.$(BUILD_ARTIFACT_EXTENSION),)

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all:
	+@$(MAKE) --no-print-directory main-build && $(MAKE) --no-print-directory post-build

# Main-build Target
main-build: RD_ProbeScope_FreeRTOS_Test.axf

# Tool invocations
RD_ProbeScope_FreeRTOS_Test.axf: $(OBJS) $(USER_OBJS) makefile $(OPTIONAL_TOOL_DEPS)
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Users\chadb\Documents\MCUXpressoIDE_11.7.0_9198\workspace\lpc_chip_43xx\Debug" -Xlinker -Map="RD_ProbeScope_FreeRTOS_Test.map" -Xlinker --cref -Xlinker --gc-sections -Xlinker -print-memory-usage -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp -mthumb -T RD_ProbeScope_FreeRTOS_Test_Debug.ld -o "RD_ProbeScope_FreeRTOS_Test.axf" $(OBJS) $(USER_OBJS) $(LIBS) -llpc_chip_43xx
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) RD_ProbeScope_FreeRTOS_Test.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "RD_ProbeScope_FreeRTOS_Test.axf"; # arm-none-eabi-objcopy -v -O binary "RD_ProbeScope_FreeRTOS_Test.axf" "RD_ProbeScope_FreeRTOS_Test.bin" ; # checksum -p LPC4370 -d "RD_ProbeScope_FreeRTOS_Test.bin";
	-@echo ' '

.PHONY: all clean dependents main-build post-build

-include ../makefile.targets
