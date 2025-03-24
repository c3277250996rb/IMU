################################################################################
# MRS Version: 2.1.0
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
c:/Users/Administrator/Documents/MyCode/0324/my_ch582m/SRC/RVMSIS/core_riscv.c 

C_DEPS += \
./RVMSIS/core_riscv.d 

OBJS += \
./RVMSIS/core_riscv.o 



# Each subdirectory must supply rules for building sources it contributes
RVMSIS/core_riscv.o: c:/Users/Administrator/Documents/MyCode/0324/my_ch582m/SRC/RVMSIS/core_riscv.c
	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -DDEBUG=1 -I"c:/Users/Administrator/Documents/MyCode/0324/my_ch582m/SRC/StdPeriphDriver/inc" -I"c:/Users/Administrator/Documents/MyCode/0324/my_ch582m/SRC/RVMSIS" -I"c:/Users/Administrator/Documents/MyCode/0324/my_ch582m/ch582m/Code" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"

