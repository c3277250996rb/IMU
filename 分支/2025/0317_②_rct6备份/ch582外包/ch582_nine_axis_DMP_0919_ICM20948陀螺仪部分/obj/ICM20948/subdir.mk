################################################################################
# MRS Version: 1.9.2
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../ICM20948/DataConverter.c \
../ICM20948/Icm20948Augmented.c \
../ICM20948/Icm20948AuxCompassAkm.c \
../ICM20948/Icm20948AuxTransport.c \
../ICM20948/Icm20948DataBaseControl.c \
../ICM20948/Icm20948DataBaseDriver.c \
../ICM20948/Icm20948DataConverter.c \
../ICM20948/Icm20948Dmp3Driver.c \
../ICM20948/Icm20948LoadFirmware.c \
../ICM20948/Icm20948MPUFifoControl.c \
../ICM20948/Icm20948SelfTest.c \
../ICM20948/Icm20948Setup.c \
../ICM20948/Icm20948Transport.c \
../ICM20948/Message.c 

OBJS += \
./ICM20948/DataConverter.o \
./ICM20948/Icm20948Augmented.o \
./ICM20948/Icm20948AuxCompassAkm.o \
./ICM20948/Icm20948AuxTransport.o \
./ICM20948/Icm20948DataBaseControl.o \
./ICM20948/Icm20948DataBaseDriver.o \
./ICM20948/Icm20948DataConverter.o \
./ICM20948/Icm20948Dmp3Driver.o \
./ICM20948/Icm20948LoadFirmware.o \
./ICM20948/Icm20948MPUFifoControl.o \
./ICM20948/Icm20948SelfTest.o \
./ICM20948/Icm20948Setup.o \
./ICM20948/Icm20948Transport.o \
./ICM20948/Message.o 

C_DEPS += \
./ICM20948/DataConverter.d \
./ICM20948/Icm20948Augmented.d \
./ICM20948/Icm20948AuxCompassAkm.d \
./ICM20948/Icm20948AuxTransport.d \
./ICM20948/Icm20948DataBaseControl.d \
./ICM20948/Icm20948DataBaseDriver.d \
./ICM20948/Icm20948DataConverter.d \
./ICM20948/Icm20948Dmp3Driver.d \
./ICM20948/Icm20948LoadFirmware.d \
./ICM20948/Icm20948MPUFifoControl.d \
./ICM20948/Icm20948SelfTest.d \
./ICM20948/Icm20948Setup.d \
./ICM20948/Icm20948Transport.d \
./ICM20948/Message.d 


# Each subdirectory must supply rules for building sources it contributes
ICM20948/%.o: ../ICM20948/%.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\driver\StdPeriphDriver\inc" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\driver\RVMSIS" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\src" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\data_filter" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\imu_algo" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\icm949" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\shell" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\ICM20948" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

