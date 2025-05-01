################################################################################
# MRS Version: 2.1.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/SRC/StdPeriphDriver/CH59x_clk.c \
c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/SRC/StdPeriphDriver/CH59x_flash.c \
c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/SRC/StdPeriphDriver/CH59x_gpio.c \
c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/SRC/StdPeriphDriver/CH59x_i2c.c \
c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/SRC/StdPeriphDriver/CH59x_lcd.c \
c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/SRC/StdPeriphDriver/CH59x_pwr.c \
c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/SRC/StdPeriphDriver/CH59x_sys.c \
c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/SRC/StdPeriphDriver/CH59x_uart0.c \
c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/SRC/StdPeriphDriver/CH59x_uart1.c 

C_DEPS += \
./StdPeriphDriver/CH59x_clk.d \
./StdPeriphDriver/CH59x_flash.d \
./StdPeriphDriver/CH59x_gpio.d \
./StdPeriphDriver/CH59x_i2c.d \
./StdPeriphDriver/CH59x_lcd.d \
./StdPeriphDriver/CH59x_pwr.d \
./StdPeriphDriver/CH59x_sys.d \
./StdPeriphDriver/CH59x_uart0.d \
./StdPeriphDriver/CH59x_uart1.d 

OBJS += \
./StdPeriphDriver/CH59x_clk.o \
./StdPeriphDriver/CH59x_flash.o \
./StdPeriphDriver/CH59x_gpio.o \
./StdPeriphDriver/CH59x_i2c.o \
./StdPeriphDriver/CH59x_lcd.o \
./StdPeriphDriver/CH59x_pwr.o \
./StdPeriphDriver/CH59x_sys.o \
./StdPeriphDriver/CH59x_uart0.o \
./StdPeriphDriver/CH59x_uart1.o 



# Each subdirectory must supply rules for building sources it contributes
StdPeriphDriver/CH59x_clk.o: c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/SRC/StdPeriphDriver/CH59x_clk.c
	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -I"c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/SRC/StdPeriphDriver/inc" -I"c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/SRC/RVMSIS" -I"c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/UART1/Code" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
StdPeriphDriver/CH59x_flash.o: c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/SRC/StdPeriphDriver/CH59x_flash.c
	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -I"c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/SRC/StdPeriphDriver/inc" -I"c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/SRC/RVMSIS" -I"c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/UART1/Code" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
StdPeriphDriver/CH59x_gpio.o: c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/SRC/StdPeriphDriver/CH59x_gpio.c
	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -I"c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/SRC/StdPeriphDriver/inc" -I"c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/SRC/RVMSIS" -I"c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/UART1/Code" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
StdPeriphDriver/CH59x_i2c.o: c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/SRC/StdPeriphDriver/CH59x_i2c.c
	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -I"c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/SRC/StdPeriphDriver/inc" -I"c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/SRC/RVMSIS" -I"c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/UART1/Code" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
StdPeriphDriver/CH59x_lcd.o: c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/SRC/StdPeriphDriver/CH59x_lcd.c
	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -I"c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/SRC/StdPeriphDriver/inc" -I"c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/SRC/RVMSIS" -I"c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/UART1/Code" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
StdPeriphDriver/CH59x_pwr.o: c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/SRC/StdPeriphDriver/CH59x_pwr.c
	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -I"c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/SRC/StdPeriphDriver/inc" -I"c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/SRC/RVMSIS" -I"c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/UART1/Code" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
StdPeriphDriver/CH59x_sys.o: c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/SRC/StdPeriphDriver/CH59x_sys.c
	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -I"c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/SRC/StdPeriphDriver/inc" -I"c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/SRC/RVMSIS" -I"c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/UART1/Code" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
StdPeriphDriver/CH59x_uart0.o: c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/SRC/StdPeriphDriver/CH59x_uart0.c
	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -I"c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/SRC/StdPeriphDriver/inc" -I"c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/SRC/RVMSIS" -I"c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/UART1/Code" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
StdPeriphDriver/CH59x_uart1.o: c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/SRC/StdPeriphDriver/CH59x_uart1.c
	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -I"c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/SRC/StdPeriphDriver/inc" -I"c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/SRC/RVMSIS" -I"c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/UART1/Code" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

