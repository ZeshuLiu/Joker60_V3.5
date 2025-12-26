################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
E:/Develop/Joker60_V3.5/Joker60_V3_5_Code/Composite/Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Class/COMPOSITE/Src/usbd_composite.c 

OBJS += \
./Middlewares/Composite/USB/COMPOSITE/COMPOSITE/usbd_composite.o 

C_DEPS += \
./Middlewares/Composite/USB/COMPOSITE/COMPOSITE/usbd_composite.d 


# Each subdirectory must supply rules for building sources it contributes
Middlewares/Composite/USB/COMPOSITE/COMPOSITE/usbd_composite.o: E:/Develop/Joker60_V3.5/Joker60_V3_5_Code/Composite/Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Class/COMPOSITE/Src/usbd_composite.c Middlewares/Composite/USB/COMPOSITE/COMPOSITE/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F401xC -c -I../../Core/Inc -I../../Composite -I../../Drivers/STM32F4xx_HAL_Driver/Inc -I../../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../../Drivers/CMSIS/Include -I../../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Class/COMPOSITE/Inc -I../../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Class/HID_CUSTOM/Inc -I../../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/App -I../../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Core/Inc -I../../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Target -I../../Middlewares/Third_Party/AL94_USB_Composite/COMPOSITE/Class/CDC_ACM/Inc -I../../HARDWARE/LCD -I../../KeyScan -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Middlewares-2f-Composite-2f-USB-2f-COMPOSITE-2f-COMPOSITE

clean-Middlewares-2f-Composite-2f-USB-2f-COMPOSITE-2f-COMPOSITE:
	-$(RM) ./Middlewares/Composite/USB/COMPOSITE/COMPOSITE/usbd_composite.cyclo ./Middlewares/Composite/USB/COMPOSITE/COMPOSITE/usbd_composite.d ./Middlewares/Composite/USB/COMPOSITE/COMPOSITE/usbd_composite.o ./Middlewares/Composite/USB/COMPOSITE/COMPOSITE/usbd_composite.su

.PHONY: clean-Middlewares-2f-Composite-2f-USB-2f-COMPOSITE-2f-COMPOSITE

