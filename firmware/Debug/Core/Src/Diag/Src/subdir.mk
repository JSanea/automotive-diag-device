################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Diag/Src/diag.c 

OBJS += \
./Core/Src/Diag/Src/diag.o 

C_DEPS += \
./Core/Src/Diag/Src/diag.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Diag/Src/%.o Core/Src/Diag/Src/%.su Core/Src/Diag/Src/%.cyclo: ../Core/Src/Diag/Src/%.c Core/Src/Diag/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I/home/alx/Desktop/Projects/Embedded/automotive_diag_device/firmware/Core/Src/Com/CAN/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I/home/alx/Desktop/Projects/Embedded/automotive_diag_device/firmware/Core/Src/util/Inc -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Diag-2f-Src

clean-Core-2f-Src-2f-Diag-2f-Src:
	-$(RM) ./Core/Src/Diag/Src/diag.cyclo ./Core/Src/Diag/Src/diag.d ./Core/Src/Diag/Src/diag.o ./Core/Src/Diag/Src/diag.su

.PHONY: clean-Core-2f-Src-2f-Diag-2f-Src

