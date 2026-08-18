################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (14.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app/Src/app.c \
../app/Src/display.c \
../app/Src/logger.c \
../app/Src/task_actuator.c \
../app/Src/task_actuator_interface.c \
../app/Src/task_normal.c \
../app/Src/task_normal_interface.c \
../app/Src/task_sensor.c \
../app/Src/task_set_up.c \
../app/Src/task_set_up_interface.c \
../app/Src/task_temperature.c 

OBJS += \
./app/Src/app.o \
./app/Src/display.o \
./app/Src/logger.o \
./app/Src/task_actuator.o \
./app/Src/task_actuator_interface.o \
./app/Src/task_normal.o \
./app/Src/task_normal_interface.o \
./app/Src/task_sensor.o \
./app/Src/task_set_up.o \
./app/Src/task_set_up_interface.o \
./app/Src/task_temperature.o 

C_DEPS += \
./app/Src/app.d \
./app/Src/display.d \
./app/Src/logger.d \
./app/Src/task_actuator.d \
./app/Src/task_actuator_interface.d \
./app/Src/task_normal.d \
./app/Src/task_normal_interface.d \
./app/Src/task_sensor.d \
./app/Src/task_set_up.d \
./app/Src/task_set_up_interface.d \
./app/Src/task_temperature.d 


# Each subdirectory must supply rules for building sources it contributes
app/Src/%.o app/Src/%.su app/Src/%.cyclo: ../app/Src/%.c app/Src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I../app/inc -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-app-2f-Src

clean-app-2f-Src:
	-$(RM) ./app/Src/app.cyclo ./app/Src/app.d ./app/Src/app.o ./app/Src/app.su ./app/Src/display.cyclo ./app/Src/display.d ./app/Src/display.o ./app/Src/display.su ./app/Src/logger.cyclo ./app/Src/logger.d ./app/Src/logger.o ./app/Src/logger.su ./app/Src/task_actuator.cyclo ./app/Src/task_actuator.d ./app/Src/task_actuator.o ./app/Src/task_actuator.su ./app/Src/task_actuator_interface.cyclo ./app/Src/task_actuator_interface.d ./app/Src/task_actuator_interface.o ./app/Src/task_actuator_interface.su ./app/Src/task_normal.cyclo ./app/Src/task_normal.d ./app/Src/task_normal.o ./app/Src/task_normal.su ./app/Src/task_normal_interface.cyclo ./app/Src/task_normal_interface.d ./app/Src/task_normal_interface.o ./app/Src/task_normal_interface.su ./app/Src/task_sensor.cyclo ./app/Src/task_sensor.d ./app/Src/task_sensor.o ./app/Src/task_sensor.su ./app/Src/task_set_up.cyclo ./app/Src/task_set_up.d ./app/Src/task_set_up.o ./app/Src/task_set_up.su ./app/Src/task_set_up_interface.cyclo ./app/Src/task_set_up_interface.d ./app/Src/task_set_up_interface.o ./app/Src/task_set_up_interface.su ./app/Src/task_temperature.cyclo ./app/Src/task_temperature.d ./app/Src/task_temperature.o ./app/Src/task_temperature.su

.PHONY: clean-app-2f-Src

