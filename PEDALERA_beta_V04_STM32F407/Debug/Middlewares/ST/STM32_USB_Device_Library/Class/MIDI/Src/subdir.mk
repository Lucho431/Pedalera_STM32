################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Middlewares/ST/STM32_USB_Device_Library/Class/MIDI/Src/usbd_midi.c 

OBJS += \
./Middlewares/ST/STM32_USB_Device_Library/Class/MIDI/Src/usbd_midi.o 

C_DEPS += \
./Middlewares/ST/STM32_USB_Device_Library/Class/MIDI/Src/usbd_midi.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/ST/STM32_USB_Device_Library/Class/MIDI/Src/usbd_midi.o: ../Middlewares/ST/STM32_USB_Device_Library/Class/MIDI/Src/usbd_midi.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DDEBUG -DSTM32F407xx -c -I../Core/Inc -I../USB_DEVICE/App -I../USB_DEVICE/Target -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Middlewares/ST/STM32_USB_Device_Library/Core/Inc -I../Middlewares/ST/STM32_USB_Device_Library/Class/AUDIO/Inc -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I"/home/lucho/STM32CubeIDE/ws_pedalera/pedalera_STM32/PEDALERA_beta_V04_STM32F407/Middlewares/ST/STM32_USB_Device_Library/Class/MIDI/Inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"Middlewares/ST/STM32_USB_Device_Library/Class/MIDI/Src/usbd_midi.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

