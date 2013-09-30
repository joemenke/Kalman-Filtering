################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include libraries/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: Quadcopter.axf

dependents:
	-cd C:/Users/Joseph/Documents/LPCXpresso_5.2.6_2137/quadcopter/RDB1768cmsis2_usbstack/Debug && $(MAKE) all

# Tool invocations
Quadcopter.axf: $(OBJS) $(USER_OBJS) C:/Users/Joseph/Documents/LPCXpresso_5.2.6_2137/quadcopter/RDB1768cmsis2_usbstack/Debug/libRDB1768cmsis2_usbstack.a
	@echo 'Building target: $@'
	@echo 'Invoking: MCU Linker'
	arm-none-eabi-gcc -nostdlib -L"C:\Users\Joseph\Documents\LPCXpresso_5.2.6_2137\quadcopter\RDB1768cmsis2_usbstack\Debug" -L"C:\Users\Joseph\Documents\LPCXpresso_5.2.6_2137\quadcopter\CMSISv2_LPC17xx\Debug" -Xlinker -Map="Quadcopter.map" -Xlinker --gc-sections -mcpu=cortex-m3 -mthumb -T "Quadcopter_Debug.ld" -o "Quadcopter.axf" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '
	$(MAKE) --no-print-directory post-build

# Other Targets
clean:
	-$(RM) $(OBJS)$(C_DEPS)$(EXECUTABLES) Quadcopter.axf
	-@echo ' '

post-build:
	-@echo 'Performing post-build steps'
	-arm-none-eabi-size "Quadcopter.axf"; # arm-none-eabi-objcopy -O binary "Quadcopter.axf" "Quadcopter.bin" ; checksum -p LPC1769 -d "Quadcopter.bin";
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY: post-build
C:/Users/Joseph/Documents/LPCXpresso_5.2.6_2137/quadcopter/RDB1768cmsis2_usbstack/Debug/libRDB1768cmsis2_usbstack.a:

-include ../makefile.targets
