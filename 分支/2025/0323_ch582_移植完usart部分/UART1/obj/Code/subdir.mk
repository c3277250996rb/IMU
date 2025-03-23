################################################################################
# MRS Version: 2.1.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Code/code.c \
../Code/my_interface.c \
../Code/my_module.c \
../Code/my_tool.c 

C_DEPS += \
./Code/code.d \
./Code/my_interface.d \
./Code/my_module.d \
./Code/my_tool.d 

OBJS += \
./Code/code.o \
./Code/my_interface.o \
./Code/my_module.o \
./Code/my_tool.o 



# Each subdirectory must supply rules for building sources it contributes
Code/%.o: ../Code/%.c
	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -I"c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/SRC/StdPeriphDriver/inc" -I"c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/SRC/RVMSIS" -I"c:/Users/Administrator/Documents/MyCode/0323/ch582_i2c/UART1/Code" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
