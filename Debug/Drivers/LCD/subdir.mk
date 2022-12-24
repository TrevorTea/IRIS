################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/LCD/LCD_low.c 

OBJS += \
./Drivers/LCD/LCD_low.o 

C_DEPS += \
./Drivers/LCD/LCD_low.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/LCD/%.o Drivers/LCD/%.su: ../Drivers/LCD/%.c Drivers/LCD/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L412xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-LCD

clean-Drivers-2f-LCD:
	-$(RM) ./Drivers/LCD/LCD_low.d ./Drivers/LCD/LCD_low.o ./Drivers/LCD/LCD_low.su

.PHONY: clean-Drivers-2f-LCD

