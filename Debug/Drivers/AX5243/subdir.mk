################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/AX5243/ax5243.c \
../Drivers/AX5243/ax5243_api.c \
../Drivers/AX5243/ax5243_report.c 

OBJS += \
./Drivers/AX5243/ax5243.o \
./Drivers/AX5243/ax5243_api.o \
./Drivers/AX5243/ax5243_report.o 

C_DEPS += \
./Drivers/AX5243/ax5243.d \
./Drivers/AX5243/ax5243_api.d \
./Drivers/AX5243/ax5243_report.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/AX5243/%.o Drivers/AX5243/%.su: ../Drivers/AX5243/%.c Drivers/AX5243/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L412xx -c -I../Core/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc -I../Drivers/STM32L4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-AX5243

clean-Drivers-2f-AX5243:
	-$(RM) ./Drivers/AX5243/ax5243.d ./Drivers/AX5243/ax5243.o ./Drivers/AX5243/ax5243.su ./Drivers/AX5243/ax5243_api.d ./Drivers/AX5243/ax5243_api.o ./Drivers/AX5243/ax5243_api.su ./Drivers/AX5243/ax5243_report.d ./Drivers/AX5243/ax5243_report.o ./Drivers/AX5243/ax5243_report.su

.PHONY: clean-Drivers-2f-AX5243

