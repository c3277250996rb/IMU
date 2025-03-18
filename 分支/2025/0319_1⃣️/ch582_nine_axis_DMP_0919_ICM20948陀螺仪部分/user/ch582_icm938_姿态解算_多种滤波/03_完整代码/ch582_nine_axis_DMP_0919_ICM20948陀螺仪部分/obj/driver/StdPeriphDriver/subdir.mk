################################################################################
# MRS Version: 1.9.1
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../driver/StdPeriphDriver/CH58x_adc.c \
../driver/StdPeriphDriver/CH58x_pwm.c \
../driver/StdPeriphDriver/CH58x_spi0.c \
../driver/StdPeriphDriver/CH58x_timer0.c \
../driver/StdPeriphDriver/CH58x_timer1.c \
../driver/StdPeriphDriver/CH58x_timer2.c \
../driver/StdPeriphDriver/CH58x_timer3.c \
../driver/StdPeriphDriver/CH58x_uart2.c \
../driver/StdPeriphDriver/CH58x_uart3.c \
../driver/StdPeriphDriver/CH58x_usbdev.c \
../driver/StdPeriphDriver/CH58x_usbhostBase.c \
../driver/StdPeriphDriver/CH58x_usbhostClass.c 

OBJS += \
./driver/StdPeriphDriver/CH58x_adc.o \
./driver/StdPeriphDriver/CH58x_clk.o \
./driver/StdPeriphDriver/CH58x_flash.o \
./driver/StdPeriphDriver/CH58x_gpio.o \
./driver/StdPeriphDriver/CH58x_i2c.o \
./driver/StdPeriphDriver/CH58x_pwm.o \
./driver/StdPeriphDriver/CH58x_pwr.o \
./driver/StdPeriphDriver/CH58x_spi0.o \
./driver/StdPeriphDriver/CH58x_spi1.o \
./driver/StdPeriphDriver/CH58x_sys.o \
./driver/StdPeriphDriver/CH58x_timer0.o \
./driver/StdPeriphDriver/CH58x_timer1.o \
./driver/StdPeriphDriver/CH58x_timer2.o \
./driver/StdPeriphDriver/CH58x_timer3.o \
./driver/StdPeriphDriver/CH58x_uart0.o \
./driver/StdPeriphDriver/CH58x_uart1.o \
./driver/StdPeriphDriver/CH58x_uart2.o \
./driver/StdPeriphDriver/CH58x_uart3.o \
./driver/StdPeriphDriver/CH58x_usb2dev.o \
./driver/StdPeriphDriver/CH58x_usb2hostBase.o \
./driver/StdPeriphDriver/CH58x_usb2hostClass.o \
./driver/StdPeriphDriver/CH58x_usbdev.o \
./driver/StdPeriphDriver/CH58x_usbhostBase.o \
./driver/StdPeriphDriver/CH58x_usbhostClass.o 

C_DEPS += \
./driver/StdPeriphDriver/CH58x_adc.d \
./driver/StdPeriphDriver/CH58x_clk.d \
./driver/StdPeriphDriver/CH58x_flash.d \
./driver/StdPeriphDriver/CH58x_gpio.d \
./driver/StdPeriphDriver/CH58x_i2c.d \
./driver/StdPeriphDriver/CH58x_pwm.d \
./driver/StdPeriphDriver/CH58x_pwr.d \
./driver/StdPeriphDriver/CH58x_spi0.d \
./driver/StdPeriphDriver/CH58x_spi1.d \
./driver/StdPeriphDriver/CH58x_sys.d \
./driver/StdPeriphDriver/CH58x_timer0.d \
./driver/StdPeriphDriver/CH58x_timer1.d \
./driver/StdPeriphDriver/CH58x_timer2.d \
./driver/StdPeriphDriver/CH58x_timer3.d \
./driver/StdPeriphDriver/CH58x_uart0.d \
./driver/StdPeriphDriver/CH58x_uart1.d \
./driver/StdPeriphDriver/CH58x_uart2.d \
./driver/StdPeriphDriver/CH58x_uart3.d \
./driver/StdPeriphDriver/CH58x_usb2dev.d \
./driver/StdPeriphDriver/CH58x_usb2hostBase.d \
./driver/StdPeriphDriver/CH58x_usb2hostClass.d \
./driver/StdPeriphDriver/CH58x_usbdev.d \
./driver/StdPeriphDriver/CH58x_usbhostBase.d \
./driver/StdPeriphDriver/CH58x_usbhostClass.d 


# Each subdirectory must supply rules for building sources it contributes
driver/StdPeriphDriver/%.o: ../driver/StdPeriphDriver/%.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -I"J:\GIT\ch582_nine_axis\ch582_nine_axis\driver\StdPeriphDriver\inc" -I"J:\GIT\ch582_nine_axis\ch582_nine_axis\driver\RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

