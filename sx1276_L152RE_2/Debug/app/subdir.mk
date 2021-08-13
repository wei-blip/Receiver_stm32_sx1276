################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app/board.c \
../app/continuous_wave.c \
../app/delay.c \
../app/lpm-board.c \
../app/rf_test.c \
../app/rtc-board.c \
../app/sx1276mb1las-board.c \
../app/systime.c \
../app/timer.c \
../app/utilities.c 

OBJS += \
./app/board.o \
./app/continuous_wave.o \
./app/delay.o \
./app/lpm-board.o \
./app/rf_test.o \
./app/rtc-board.o \
./app/sx1276mb1las-board.o \
./app/systime.o \
./app/timer.o \
./app/utilities.o 

C_DEPS += \
./app/board.d \
./app/continuous_wave.d \
./app/delay.d \
./app/lpm-board.d \
./app/rf_test.d \
./app/rtc-board.d \
./app/sx1276mb1las-board.d \
./app/systime.d \
./app/timer.d \
./app/utilities.d 


# Each subdirectory must supply rules for building sources it contributes
app/board.o: ../app/board.c app/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L152xE -c -I../Core/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/user/STM32CubeIDE/workspace_1.6.1_2/sx1276_L152RE_2/app" -I"C:/Users/user/STM32CubeIDE/workspace_1.6.1_2/sx1276_L152RE_2/app/LoRaMac-node/src/radio" -I"C:/Users/user/STM32CubeIDE/workspace_1.6.1_2/sx1276_L152RE_2/app/LoRaMac-node/src/radio/sx1276" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"app/board.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
app/continuous_wave.o: ../app/continuous_wave.c app/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L152xE -c -I../Core/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/user/STM32CubeIDE/workspace_1.6.1_2/sx1276_L152RE_2/app" -I"C:/Users/user/STM32CubeIDE/workspace_1.6.1_2/sx1276_L152RE_2/app/LoRaMac-node/src/radio" -I"C:/Users/user/STM32CubeIDE/workspace_1.6.1_2/sx1276_L152RE_2/app/LoRaMac-node/src/radio/sx1276" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"app/continuous_wave.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
app/delay.o: ../app/delay.c app/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L152xE -c -I../Core/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/user/STM32CubeIDE/workspace_1.6.1_2/sx1276_L152RE_2/app" -I"C:/Users/user/STM32CubeIDE/workspace_1.6.1_2/sx1276_L152RE_2/app/LoRaMac-node/src/radio" -I"C:/Users/user/STM32CubeIDE/workspace_1.6.1_2/sx1276_L152RE_2/app/LoRaMac-node/src/radio/sx1276" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"app/delay.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
app/lpm-board.o: ../app/lpm-board.c app/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L152xE -c -I../Core/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/user/STM32CubeIDE/workspace_1.6.1_2/sx1276_L152RE_2/app" -I"C:/Users/user/STM32CubeIDE/workspace_1.6.1_2/sx1276_L152RE_2/app/LoRaMac-node/src/radio" -I"C:/Users/user/STM32CubeIDE/workspace_1.6.1_2/sx1276_L152RE_2/app/LoRaMac-node/src/radio/sx1276" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"app/lpm-board.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
app/rf_test.o: ../app/rf_test.c app/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L152xE -c -I../Core/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/user/STM32CubeIDE/workspace_1.6.1_2/sx1276_L152RE_2/app" -I"C:/Users/user/STM32CubeIDE/workspace_1.6.1_2/sx1276_L152RE_2/app/LoRaMac-node/src/radio" -I"C:/Users/user/STM32CubeIDE/workspace_1.6.1_2/sx1276_L152RE_2/app/LoRaMac-node/src/radio/sx1276" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"app/rf_test.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
app/rtc-board.o: ../app/rtc-board.c app/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L152xE -c -I../Core/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/user/STM32CubeIDE/workspace_1.6.1_2/sx1276_L152RE_2/app" -I"C:/Users/user/STM32CubeIDE/workspace_1.6.1_2/sx1276_L152RE_2/app/LoRaMac-node/src/radio" -I"C:/Users/user/STM32CubeIDE/workspace_1.6.1_2/sx1276_L152RE_2/app/LoRaMac-node/src/radio/sx1276" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"app/rtc-board.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
app/sx1276mb1las-board.o: ../app/sx1276mb1las-board.c app/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L152xE -c -I../Core/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/user/STM32CubeIDE/workspace_1.6.1_2/sx1276_L152RE_2/app" -I"C:/Users/user/STM32CubeIDE/workspace_1.6.1_2/sx1276_L152RE_2/app/LoRaMac-node/src/radio" -I"C:/Users/user/STM32CubeIDE/workspace_1.6.1_2/sx1276_L152RE_2/app/LoRaMac-node/src/radio/sx1276" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"app/sx1276mb1las-board.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
app/systime.o: ../app/systime.c app/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L152xE -c -I../Core/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/user/STM32CubeIDE/workspace_1.6.1_2/sx1276_L152RE_2/app" -I"C:/Users/user/STM32CubeIDE/workspace_1.6.1_2/sx1276_L152RE_2/app/LoRaMac-node/src/radio" -I"C:/Users/user/STM32CubeIDE/workspace_1.6.1_2/sx1276_L152RE_2/app/LoRaMac-node/src/radio/sx1276" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"app/systime.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
app/timer.o: ../app/timer.c app/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L152xE -c -I../Core/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/user/STM32CubeIDE/workspace_1.6.1_2/sx1276_L152RE_2/app" -I"C:/Users/user/STM32CubeIDE/workspace_1.6.1_2/sx1276_L152RE_2/app/LoRaMac-node/src/radio" -I"C:/Users/user/STM32CubeIDE/workspace_1.6.1_2/sx1276_L152RE_2/app/LoRaMac-node/src/radio/sx1276" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"app/timer.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"
app/utilities.o: ../app/utilities.c app/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32L152xE -c -I../Core/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc -I../Drivers/STM32L1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32L1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/user/STM32CubeIDE/workspace_1.6.1_2/sx1276_L152RE_2/app" -I"C:/Users/user/STM32CubeIDE/workspace_1.6.1_2/sx1276_L152RE_2/app/LoRaMac-node/src/radio" -I"C:/Users/user/STM32CubeIDE/workspace_1.6.1_2/sx1276_L152RE_2/app/LoRaMac-node/src/radio/sx1276" -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3 -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"app/utilities.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

