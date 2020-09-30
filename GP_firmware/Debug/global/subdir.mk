################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
S:/sourcetree/imu_board/common_source_files/global/global_variables.c \
S:/sourcetree/imu_board/common_source_files/global/initialize.c \
S:/sourcetree/imu_board/common_source_files/global/primary_loop.c 

OBJS += \
./global/global_variables.o \
./global/initialize.o \
./global/primary_loop.o 

C_DEPS += \
./global/global_variables.d \
./global/initialize.d \
./global/primary_loop.d 


# Each subdirectory must supply rules for building sources it contributes
global/global_variables.o: S:/sourcetree/imu_board/common_source_files/global/global_variables.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32G431xx -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../USB_Device/App -I../USB_Device/Target -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"S:/sourcetree/imu_board/GP_firmware/../common_source_files/fm25l16b" -I"S:/sourcetree/imu_board/GP_firmware/../common_source_files/calibration" -I"S:/sourcetree/imu_board/GP_firmware/../common_source_files/global" -I"S:/sourcetree/imu_board/GP_firmware/../common_source_files/icm20602" -I"S:/sourcetree/imu_board/GP_firmware/../common_source_files/kalman_filter" -I"S:/sourcetree/imu_board/GP_firmware/../common_source_files/math_functions" -I"S:/sourcetree/imu_board/GP_firmware/../common_source_files/mmc5983ma" -I"S:/sourcetree/imu_board/GP_firmware/../CMSIS_DSP/Include" -I"S:/sourcetree/imu_board/GP_firmware/../common_source_files/spi_wrapper" -I../Middlewares/Third_Party/ARM_CMSIS/CMSIS/Core/Include/ -I../Middlewares/Third_Party/ARM_CMSIS/CMSIS/Core_A/Include/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"global/global_variables.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
global/initialize.o: S:/sourcetree/imu_board/common_source_files/global/initialize.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32G431xx -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../USB_Device/App -I../USB_Device/Target -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"S:/sourcetree/imu_board/GP_firmware/../common_source_files/fm25l16b" -I"S:/sourcetree/imu_board/GP_firmware/../common_source_files/calibration" -I"S:/sourcetree/imu_board/GP_firmware/../common_source_files/global" -I"S:/sourcetree/imu_board/GP_firmware/../common_source_files/icm20602" -I"S:/sourcetree/imu_board/GP_firmware/../common_source_files/kalman_filter" -I"S:/sourcetree/imu_board/GP_firmware/../common_source_files/math_functions" -I"S:/sourcetree/imu_board/GP_firmware/../common_source_files/mmc5983ma" -I"S:/sourcetree/imu_board/GP_firmware/../CMSIS_DSP/Include" -I"S:/sourcetree/imu_board/GP_firmware/../common_source_files/spi_wrapper" -I../Middlewares/Third_Party/ARM_CMSIS/CMSIS/Core/Include/ -I../Middlewares/Third_Party/ARM_CMSIS/CMSIS/Core_A/Include/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"global/initialize.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
global/primary_loop.o: S:/sourcetree/imu_board/common_source_files/global/primary_loop.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32G431xx -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../USB_Device/App -I../USB_Device/Target -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"S:/sourcetree/imu_board/GP_firmware/../common_source_files/fm25l16b" -I"S:/sourcetree/imu_board/GP_firmware/../common_source_files/calibration" -I"S:/sourcetree/imu_board/GP_firmware/../common_source_files/global" -I"S:/sourcetree/imu_board/GP_firmware/../common_source_files/icm20602" -I"S:/sourcetree/imu_board/GP_firmware/../common_source_files/kalman_filter" -I"S:/sourcetree/imu_board/GP_firmware/../common_source_files/math_functions" -I"S:/sourcetree/imu_board/GP_firmware/../common_source_files/mmc5983ma" -I"S:/sourcetree/imu_board/GP_firmware/../CMSIS_DSP/Include" -I"S:/sourcetree/imu_board/GP_firmware/../common_source_files/spi_wrapper" -I../Middlewares/Third_Party/ARM_CMSIS/CMSIS/Core/Include/ -I../Middlewares/Third_Party/ARM_CMSIS/CMSIS/Core_A/Include/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"global/primary_loop.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

