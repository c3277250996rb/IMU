################################################################################
# MRS Version: 1.9.1
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
OBJS += \
./driver/RVMSIS/core_riscv.o 

C_DEPS += \
./driver/RVMSIS/core_riscv.d 


# Each subdirectory must supply rules for building sources it contributes
driver/RVMSIS/%.o: ../driver/RVMSIS/%.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -I"J:\GIT\ch582_nine_axis\ch582_nine_axis\driver\StdPeriphDriver\inc" -I"J:\GIT\ch582_nine_axis\ch582_nine_axis\driver\RVMSIS" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

