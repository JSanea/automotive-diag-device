################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Com/CAN/Src/can_cbuffer.c \
../Core/Src/Com/CAN/Src/can_drv.c 

OBJS += \
./Core/Src/Com/CAN/Src/can_cbuffer.o \
./Core/Src/Com/CAN/Src/can_drv.o 

C_DEPS += \
./Core/Src/Com/CAN/Src/can_cbuffer.d \
./Core/Src/Com/CAN/Src/can_drv.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Com/CAN/Src/%.o Core/Src/Com/CAN/Src/%.su Core/Src/Com/CAN/Src/%.cyclo: ../Core/Src/Com/CAN/Src/%.c Core/Src/Com/CAN/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I/home/alx/Desktop/Projects/Embedded/automotive_diag_device/firmware/Core/Src/Com/CAN/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Com-2f-CAN-2f-Src

clean-Core-2f-Src-2f-Com-2f-CAN-2f-Src:
	-$(RM) ./Core/Src/Com/CAN/Src/can_cbuffer.cyclo ./Core/Src/Com/CAN/Src/can_cbuffer.d ./Core/Src/Com/CAN/Src/can_cbuffer.o ./Core/Src/Com/CAN/Src/can_cbuffer.su ./Core/Src/Com/CAN/Src/can_drv.cyclo ./Core/Src/Com/CAN/Src/can_drv.d ./Core/Src/Com/CAN/Src/can_drv.o ./Core/Src/Com/CAN/Src/can_drv.su

.PHONY: clean-Core-2f-Src-2f-Com-2f-CAN-2f-Src

