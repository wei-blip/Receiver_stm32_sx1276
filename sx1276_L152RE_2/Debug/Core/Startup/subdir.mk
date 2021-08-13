################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (9-2020-q2-update)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Core/Startup/startup_stm32l152retx.s 

OBJS += \
./Core/Startup/startup_stm32l152retx.o 

S_DEPS += \
./Core/Startup/startup_stm32l152retx.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Startup/startup_stm32l152retx.o: ../Core/Startup/startup_stm32l152retx.s Core/Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m3 -g3 -c -I"C:/Users/user/STM32CubeIDE/workspace_1.6.1_2/sx1276_L152RE_2/app" -x assembler-with-cpp -MMD -MP -MF"Core/Startup/startup_stm32l152retx.d" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@" "$<"

