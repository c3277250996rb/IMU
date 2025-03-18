################################################################################
# MRS Version: 1.9.2
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../driver/RVMSIS/core_riscv.c 

OBJS += \
./RVMSIS/core_riscv.o 

C_DEPS += \
./RVMSIS/core_riscv.d 


# Each subdirectory must supply rules for building sources it contributes
RVMSIS/core_riscv.o: H:/ICM20948/ch582_nine_axis(5)_0813/ch582_nine_axis/driver/RVMSIS/core_riscv.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\driver\StdPeriphDriver\inc" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\driver\RVMSIS" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\src" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\data_filter" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\imu_algo" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\icm949" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\shell" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\ICM20948" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

