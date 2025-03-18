################################################################################
# MRS Version: 1.9.2
# 自动生成的文件。不要编辑！
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include user/shell/subdir.mk
-include user/imu_algo/subdir.mk
-include user/icm949/subdir.mk
-include user/data_filter/subdir.mk
-include src/subdir.mk
-include StdPeriphDriver/subdir.mk
-include Startup/subdir.mk
-include RVMSIS/subdir.mk
-include ICM20948/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
nine_axis_prj.hex \

SECONDARY_LIST += \
nine_axis_prj.lst \

SECONDARY_SIZE += \
nine_axis_prj.siz \


# 所有目标
all: nine_axis_prj.elf secondary-outputs

# 工具调用
nine_axis_prj.elf: $(OBJS) $(USER_OBJS)
	@	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common  -g -T "H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\driver\Ld\Link.ld" -nostartfiles -Xlinker --gc-sections -L"../" -L"H:\ICM20948\ch582_nine_axis(5)_0813\ch582_nine_axis\driver\StdPeriphDriver" -Xlinker --print-memory-usage -Wl,-Map,"nine_axis_prj.map" --specs=nano.specs -u _printf_float --specs=nosys.specs -o "nine_axis_prj.elf" $(OBJS) $(USER_OBJS) $(LIBS)
	@	@
nine_axis_prj.hex: nine_axis_prj.elf
	@	riscv-none-embed-objcopy -O ihex "nine_axis_prj.elf"  "nine_axis_prj.hex"
	@	@
nine_axis_prj.lst: nine_axis_prj.elf
	@	riscv-none-embed-objdump --source --all-headers --demangle --line-numbers --wide "nine_axis_prj.elf" > "nine_axis_prj.lst"
	@	@
nine_axis_prj.siz: nine_axis_prj.elf
	@	riscv-none-embed-size --format=berkeley "nine_axis_prj.elf"
	@	@
# 其他目标
clean:
	-$(RM) $(ASM_UPPER_DEPS)$(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(ASM_DEPS)$(S_DEPS)$(S_UPPER_DEPS)$(C_DEPS) nine_axis_prj.elf
	-@
secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets
