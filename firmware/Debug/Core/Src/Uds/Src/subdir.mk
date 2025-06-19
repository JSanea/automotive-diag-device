################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/Uds/Src/uds_services.c 

OBJS += \
./Core/Src/Uds/Src/uds_services.o 

C_DEPS += \
./Core/Src/Uds/Src/uds_services.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/Uds/Src/%.o Core/Src/Uds/Src/%.su Core/Src/Uds/Src/%.cyclo: ../Core/Src/Uds/Src/%.c Core/Src/Uds/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F446xx -c -I../Core/Inc -I/home/alx/Desktop/Projects/Embedded/automotive_diag_device/firmware/Core/Src/Com/CAN/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-Uds-2f-Src

clean-Core-2f-Src-2f-Uds-2f-Src:
	-$(RM) ./Core/Src/Uds/Src/uds_services.cyclo ./Core/Src/Uds/Src/uds_services.d ./Core/Src/Uds/Src/uds_services.o ./Core/Src/Uds/Src/uds_services.su

.PHONY: clean-Core-2f-Src-2f-Uds-2f-Src

