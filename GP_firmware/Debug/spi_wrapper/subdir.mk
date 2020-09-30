################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
S:/sourcetree/imu_board/common_source_files/spi_wrapper/spi_wrapper.c 

OBJS += \
./spi_wrapper/spi_wrapper.o 

C_DEPS += \
./spi_wrapper/spi_wrapper.d 


# Each subdirectory must supply rules for building sources it contributes
spi_wrapper/spi_wrapper.o: S:/sourcetree/imu_board/common_source_files/spi_wrapper/spi_wrapper.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DSTM32G431xx -DUSE_HAL_DRIVER -DDEBUG -c -I../Core/Inc -I../USB_Device/App -I../USB_Device/Target -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -I"S:/sourcetree/imu_board/GP_firmware/../common_source_files/fm25l16b" -I"S:/sourcetree/imu_board/GP_firmware/../common_source_files/calibration" -I"S:/sourcetree/imu_board/GP_firmware/../common_source_files/global" -I"S:/sourcetree/imu_board/GP_firmware/../common_source_files/icm20602" -I"S:/sourcetree/imu_board/GP_firmware/../common_source_files/kalman_filter" -I"S:/sourcetree/imu_board/GP_firmware/../common_source_files/math_functions" -I"S:/sourcetree/imu_board/GP_firmware/../common_source_files/mmc5983ma" -I"S:/sourcetree/imu_board/GP_firmware/../CMSIS_DSP/Include" -I"S:/sourcetree/imu_board/GP_firmware/../common_source_files/spi_wrapper" -I../Middlewares/Third_Party/ARM_CMSIS/CMSIS/Core/Include/ -I../Middlewares/Third_Party/ARM_CMSIS/CMSIS/Core_A/Include/ -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"spi_wrapper/spi_wrapper.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

