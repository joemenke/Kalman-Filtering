################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Joseph/Documents/LPCXpresso_5.2.6_2137/quadcopter/libraries/i2c.c \
C:/Users/Joseph/Documents/LPCXpresso_5.2.6_2137/quadcopter/libraries/serial_fifo.c \
C:/Users/Joseph/Documents/LPCXpresso_5.2.6_2137/quadcopter/libraries/timer.c 

OBJS += \
./libraries/i2c.o \
./libraries/serial_fifo.o \
./libraries/timer.o 

C_DEPS += \
./libraries/i2c.d \
./libraries/serial_fifo.d \
./libraries/timer.d 


# Each subdirectory must supply rules for building sources it contributes
libraries/i2c.o: C:/Users/Joseph/Documents/LPCXpresso_5.2.6_2137/quadcopter/libraries/i2c.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_CMSIS=CMSISv2p00_LPC17xx -I"C:\Users\Joseph\Documents\LPCXpresso_5.2.6_2137\quadcopter\RDB1768cmsis2_usbstack" -I"C:\Users\Joseph\Documents\LPCXpresso_5.2.6_2137\quadcopter\RDB1768cmsis2_usbstack\inc" -I"C:\Users\Joseph\Documents\LPCXpresso_5.2.6_2137\quadcopter\libraries" -I"C:\Users\Joseph\Documents\LPCXpresso_5.2.6_2137\quadcopter\CMSISv2_LPC17xx\inc" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

libraries/serial_fifo.o: C:/Users/Joseph/Documents/LPCXpresso_5.2.6_2137/quadcopter/libraries/serial_fifo.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_CMSIS=CMSISv2p00_LPC17xx -I"C:\Users\Joseph\Documents\LPCXpresso_5.2.6_2137\quadcopter\RDB1768cmsis2_usbstack" -I"C:\Users\Joseph\Documents\LPCXpresso_5.2.6_2137\quadcopter\RDB1768cmsis2_usbstack\inc" -I"C:\Users\Joseph\Documents\LPCXpresso_5.2.6_2137\quadcopter\libraries" -I"C:\Users\Joseph\Documents\LPCXpresso_5.2.6_2137\quadcopter\CMSISv2_LPC17xx\inc" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

libraries/timer.o: C:/Users/Joseph/Documents/LPCXpresso_5.2.6_2137/quadcopter/libraries/timer.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU C Compiler'
	arm-none-eabi-gcc -D__REDLIB__ -DDEBUG -D__CODE_RED -D__USE_CMSIS=CMSISv2p00_LPC17xx -I"C:\Users\Joseph\Documents\LPCXpresso_5.2.6_2137\quadcopter\RDB1768cmsis2_usbstack" -I"C:\Users\Joseph\Documents\LPCXpresso_5.2.6_2137\quadcopter\RDB1768cmsis2_usbstack\inc" -I"C:\Users\Joseph\Documents\LPCXpresso_5.2.6_2137\quadcopter\libraries" -I"C:\Users\Joseph\Documents\LPCXpresso_5.2.6_2137\quadcopter\CMSISv2_LPC17xx\inc" -O0 -g3 -Wall -c -fmessage-length=0 -fno-builtin -ffunction-sections -fdata-sections -mcpu=cortex-m3 -mthumb -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


