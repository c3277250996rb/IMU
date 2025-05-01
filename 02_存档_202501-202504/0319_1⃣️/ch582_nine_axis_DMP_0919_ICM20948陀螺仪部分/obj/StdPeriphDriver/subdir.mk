################################################################################
# MRS Version: 1.9.2
# 自动生成的文件。不要编辑！
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../driver/StdPeriphDriver/CH58x_clk.c \
../driver/StdPeriphDriver/CH58x_flash.c \
../driver/StdPeriphDriver/CH58x_gpio.c \
../driver/StdPeriphDriver/CH58x_i2c.c \
../driver/StdPeriphDriver/CH58x_pwr.c \
../driver/StdPeriphDriver/CH58x_spi1.c \
../driver/StdPeriphDriver/CH58x_sys.c \
../driver/StdPeriphDriver/CH58x_timer0.c \
../driver/StdPeriphDriver/CH58x_timer1.c \
../driver/StdPeriphDriver/CH58x_timer2.c \
../driver/StdPeriphDriver/CH58x_timer3.c \
../driver/StdPeriphDriver/CH58x_uart0.c \
../driver/StdPeriphDriver/CH58x_uart1.c \
../driver/StdPeriphDriver/CH58x_usb2dev.c \
../driver/StdPeriphDriver/CH58x_usb2hostBase.c \
../driver/StdPeriphDriver/CH58x_usb2hostClass.c 

OBJS += \
./StdPeriphDriver/CH58x_clk.o \
./StdPeriphDriver/CH58x_flash.o \
./StdPeriphDriver/CH58x_gpio.o \
./StdPeriphDriver/CH58x_i2c.o \
./StdPeriphDriver/CH58x_pwr.o \
./StdPeriphDriver/CH58x_spi1.o \
./StdPeriphDriver/CH58x_sys.o \
./StdPeriphDriver/CH58x_timer0.o \
./StdPeriphDriver/CH58x_timer1.o \
./StdPeriphDriver/CH58x_timer2.o \
./StdPeriphDriver/CH58x_timer3.o \
./StdPeriphDriver/CH58x_uart0.o \
./StdPeriphDriver/CH58x_uart1.o \
./StdPeriphDriver/CH58x_usb2dev.o \
./StdPeriphDriver/CH58x_usb2hostBase.o \
./StdPeriphDriver/CH58x_usb2hostClass.o 

C_DEPS += \
./StdPeriphDriver/CH58x_clk.d \
./StdPeriphDriver/CH58x_flash.d \
./StdPeriphDriver/CH58x_gpio.d \
./StdPeriphDriver/CH58x_i2c.d \
./StdPeriphDriver/CH58x_pwr.d \
./StdPeriphDriver/CH58x_spi1.d \
./StdPeriphDriver/CH58x_sys.d \
./StdPeriphDriver/CH58x_timer0.d \
./StdPeriphDriver/CH58x_timer1.d \
./StdPeriphDriver/CH58x_timer2.d \
./StdPeriphDriver/CH58x_timer3.d \
./StdPeriphDriver/CH58x_uart0.d \
./StdPeriphDriver/CH58x_uart1.d \
./StdPeriphDriver/CH58x_usb2dev.d \
./StdPeriphDriver/CH58x_usb2hostBase.d \
./StdPeriphDriver/CH58x_usb2hostClass.d 


# Each subdirectory must supply rules for building sources it contributes
StdPeriphDriver/CH58x_clk.o: H:/ICM20948/ch582_nine_axis(5)_0813/ch582_nine_axis/driver/StdPeriphDriver/CH58x_clk.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\driver\StdPeriphDriver\inc" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\driver\RVMSIS" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\src" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\data_filter" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\imu_algo" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\icm949" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\shell" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\ICM20948" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
StdPeriphDriver/CH58x_flash.o: H:/ICM20948/ch582_nine_axis(5)_0813/ch582_nine_axis/driver/StdPeriphDriver/CH58x_flash.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\driver\StdPeriphDriver\inc" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\driver\RVMSIS" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\src" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\data_filter" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\imu_algo" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\icm949" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\shell" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\ICM20948" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
StdPeriphDriver/CH58x_gpio.o: H:/ICM20948/ch582_nine_axis(5)_0813/ch582_nine_axis/driver/StdPeriphDriver/CH58x_gpio.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\driver\StdPeriphDriver\inc" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\driver\RVMSIS" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\src" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\data_filter" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\imu_algo" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\icm949" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\shell" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\ICM20948" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
StdPeriphDriver/CH58x_i2c.o: H:/ICM20948/ch582_nine_axis(5)_0813/ch582_nine_axis/driver/StdPeriphDriver/CH58x_i2c.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\driver\StdPeriphDriver\inc" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\driver\RVMSIS" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\src" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\data_filter" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\imu_algo" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\icm949" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\shell" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\ICM20948" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
StdPeriphDriver/CH58x_pwr.o: H:/ICM20948/ch582_nine_axis(5)_0813/ch582_nine_axis/driver/StdPeriphDriver/CH58x_pwr.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\driver\StdPeriphDriver\inc" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\driver\RVMSIS" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\src" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\data_filter" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\imu_algo" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\icm949" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\shell" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\ICM20948" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
StdPeriphDriver/CH58x_spi1.o: H:/ICM20948/ch582_nine_axis(5)_0813/ch582_nine_axis/driver/StdPeriphDriver/CH58x_spi1.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\driver\StdPeriphDriver\inc" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\driver\RVMSIS" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\src" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\data_filter" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\imu_algo" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\icm949" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\shell" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\ICM20948" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
StdPeriphDriver/CH58x_sys.o: H:/ICM20948/ch582_nine_axis(5)_0813/ch582_nine_axis/driver/StdPeriphDriver/CH58x_sys.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\driver\StdPeriphDriver\inc" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\driver\RVMSIS" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\src" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\data_filter" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\imu_algo" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\icm949" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\shell" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\ICM20948" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
StdPeriphDriver/CH58x_timer0.o: H:/ICM20948/ch582_nine_axis(5)_0813/ch582_nine_axis/driver/StdPeriphDriver/CH58x_timer0.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\driver\StdPeriphDriver\inc" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\driver\RVMSIS" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\src" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\data_filter" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\imu_algo" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\icm949" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\shell" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\ICM20948" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
StdPeriphDriver/CH58x_timer1.o: H:/ICM20948/ch582_nine_axis(5)_0813/ch582_nine_axis/driver/StdPeriphDriver/CH58x_timer1.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\driver\StdPeriphDriver\inc" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\driver\RVMSIS" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\src" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\data_filter" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\imu_algo" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\icm949" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\shell" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\ICM20948" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
StdPeriphDriver/CH58x_timer2.o: H:/ICM20948/ch582_nine_axis(5)_0813/ch582_nine_axis/driver/StdPeriphDriver/CH58x_timer2.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\driver\StdPeriphDriver\inc" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\driver\RVMSIS" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\src" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\data_filter" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\imu_algo" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\icm949" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\shell" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\ICM20948" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
StdPeriphDriver/CH58x_timer3.o: H:/ICM20948/ch582_nine_axis(5)_0813/ch582_nine_axis/driver/StdPeriphDriver/CH58x_timer3.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\driver\StdPeriphDriver\inc" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\driver\RVMSIS" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\src" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\data_filter" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\imu_algo" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\icm949" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\shell" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\ICM20948" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
StdPeriphDriver/CH58x_uart0.o: H:/ICM20948/ch582_nine_axis(5)_0813/ch582_nine_axis/driver/StdPeriphDriver/CH58x_uart0.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\driver\StdPeriphDriver\inc" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\driver\RVMSIS" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\src" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\data_filter" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\imu_algo" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\icm949" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\shell" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\ICM20948" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
StdPeriphDriver/CH58x_uart1.o: H:/ICM20948/ch582_nine_axis(5)_0813/ch582_nine_axis/driver/StdPeriphDriver/CH58x_uart1.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\driver\StdPeriphDriver\inc" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\driver\RVMSIS" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\src" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\data_filter" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\imu_algo" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\icm949" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\shell" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\ICM20948" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
StdPeriphDriver/CH58x_usb2dev.o: H:/ICM20948/ch582_nine_axis(5)_0813/ch582_nine_axis/driver/StdPeriphDriver/CH58x_usb2dev.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\driver\StdPeriphDriver\inc" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\driver\RVMSIS" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\src" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\data_filter" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\imu_algo" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\icm949" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\shell" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\ICM20948" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
StdPeriphDriver/CH58x_usb2hostBase.o: H:/ICM20948/ch582_nine_axis(5)_0813/ch582_nine_axis/driver/StdPeriphDriver/CH58x_usb2hostBase.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\driver\StdPeriphDriver\inc" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\driver\RVMSIS" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\src" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\data_filter" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\imu_algo" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\icm949" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\shell" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\ICM20948" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@
StdPeriphDriver/CH58x_usb2hostClass.o: H:/ICM20948/ch582_nine_axis(5)_0813/ch582_nine_axis/driver/StdPeriphDriver/CH58x_usb2hostClass.c
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -DDEBUG=1 -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\driver\StdPeriphDriver\inc" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\driver\RVMSIS" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\src" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\data_filter" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\imu_algo" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\icm949" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\user\shell" -I"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\ICM20948" -std=gnu99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@	@

