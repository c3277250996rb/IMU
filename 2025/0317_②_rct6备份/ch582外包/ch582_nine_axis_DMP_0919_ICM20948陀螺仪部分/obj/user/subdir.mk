################################################################################
# MRS Version: 1.9.1
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../user/attitude.c \
../user/imu_app.c \
../user/my_math.c 

OBJS += \
./user/attitude.o \
./user/imu_app.o \
./user/my_math.o 

C_DEPS += \
./user/attitude.d \
./user/imu_app.d \
./user/my_math.d 


# Each subdirectory must supply rules for building sources it contributes
user/%.o: ../user/%.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -I"J:\GIT\ch582_nine_axis\ch582_nine_axis\driver\StdPeriphDriver\inc" -I"J:\GIT\ch582_nine_axis\ch582_nine_axis\driver\RVMSIS" -I"J:\GIT\ch582_nine_axis\ch582_nine_axis\src" -I"J:\GIT\ch582_nine_axis\ch582_nine_axis\user" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

