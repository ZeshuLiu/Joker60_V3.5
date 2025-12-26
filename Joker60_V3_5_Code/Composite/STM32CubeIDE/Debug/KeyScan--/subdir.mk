################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../KeyScan--/KeyScan.c \
../KeyScan--/Layout.c 

OBJS += \
./KeyScan--/KeyScan.o \
./KeyScan--/Layout.o 

C_DEPS += \
./KeyScan--/KeyScan.d \
./KeyScan--/Layout.d 


# Each subdirectory must supply rules for building sources it contributes
KeyScan--/%.o KeyScan--/%.su KeyScan--/%.cyclo: ../KeyScan--/%.c KeyScan--/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xC -c -I../../Core/Inc -I../../Composite -I../../Drivers/STM32F4xx_HAL_Driver/Inc -I../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../../Drivers/CMSIS/Include -I../../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Class/COMPOSITE/Inc -I../../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Class/HID_CUSTOM/Inc -I../../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App -I../../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Core/Inc -I../../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Target -I../../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Class/CDC_ACM/Inc -I../../HARDWARE/LCD -I../../KeyScan -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-KeyScan-2d--2d-

clean-KeyScan-2d--2d-:
	-$(RM) ./KeyScan--/KeyScan.cyclo ./KeyScan--/KeyScan.d ./KeyScan--/KeyScan.o ./KeyScan--/KeyScan.su ./KeyScan--/Layout.cyclo ./KeyScan--/Layout.d ./KeyScan--/Layout.o ./KeyScan--/Layout.su

.PHONY: clean-KeyScan-2d--2d-

