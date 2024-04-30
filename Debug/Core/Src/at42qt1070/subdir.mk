################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Core/Src/at42qt1070/at42qt1070.c \
../Core/Src/at42qt1070/circ_buffer.c 

OBJS += \
./Core/Src/at42qt1070/at42qt1070.o \
./Core/Src/at42qt1070/circ_buffer.o 

C_DEPS += \
./Core/Src/at42qt1070/at42qt1070.d \
./Core/Src/at42qt1070/circ_buffer.d 


# Each subdirectory must supply rules for building sources it contributes
Core/Src/at42qt1070/%.o Core/Src/at42qt1070/%.su Core/Src/at42qt1070/%.cyclo: ../Core/Src/at42qt1070/%.c Core/Src/at42qt1070/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DCORE_CM4 -DUSE_HAL_DRIVER -DSTM32WLE5xx -DTX_INCLUDE_USER_DEFINE_FILE -c -I../Core/Inc -I../Drivers/STM32WLxx_HAL_Driver/Inc -I../Drivers/STM32WLxx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32WLxx/Include -I../Drivers/CMSIS/Include -I../AZURE_RTOS/App -I../Middlewares/ST/threadx/common/inc/ -I../Middlewares/ST/threadx/ports/cortex_m4/gnu/inc/ -I../Middlewares/ST/threadx/utility/low_power/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-Core-2f-Src-2f-at42qt1070

clean-Core-2f-Src-2f-at42qt1070:
	-$(RM) ./Core/Src/at42qt1070/at42qt1070.cyclo ./Core/Src/at42qt1070/at42qt1070.d ./Core/Src/at42qt1070/at42qt1070.o ./Core/Src/at42qt1070/at42qt1070.su ./Core/Src/at42qt1070/circ_buffer.cyclo ./Core/Src/at42qt1070/circ_buffer.d ./Core/Src/at42qt1070/circ_buffer.o ./Core/Src/at42qt1070/circ_buffer.su

.PHONY: clean-Core-2f-Src-2f-at42qt1070

