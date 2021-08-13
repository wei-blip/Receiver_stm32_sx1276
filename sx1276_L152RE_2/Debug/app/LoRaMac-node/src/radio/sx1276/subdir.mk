################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app/LoRaMac-node/src/radio/sx1276/sx1276.c 

OBJS += \
./app/LoRaMac-node/src/radio/sx1276/sx1276.o 

C_DEPS += \
./app/LoRaMac-node/src/radio/sx1276/sx1276.d 


# Each subdirectory must supply rules for building sources it contributes
app/LoRaMac-node/src/radio/sx1276/sx1276.o: ../app/LoRaMac-node/src/radio/sx1276/sx1276.c app/LoRaMac-node/src/radio/sx1276/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L152xE -c -I../Core/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/user/STM32CubeIDE/workspace_1.6.1_2/sx1276_L152RE_2/app" -I"C:/Users/user/STM32CubeIDE/workspace_1.6.1_2/sx1276_L152RE_2/app/LoRaMac-node/src/radio" -I"C:/Users/user/STM32CubeIDE/workspace_1.6.1_2/sx1276_L152RE_2/app/LoRaMac-node/src/radio/sx1276" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"app/LoRaMac-node/src/radio/sx1276/sx1276.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

