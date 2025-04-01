################################################################################
# MRS Version: 2.1.0
# Automatically-generated file. Do not edit!
################################################################################
-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include src/subdir.mk
-include StdPeriphDriver/subdir.mk
-include Startup/subdir.mk
-include RVMSIS/subdir.mk
-include Code/subdir.mk
-include subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(S_DEPS)),)
-include $(S_DEPS)
endif
ifneq ($(strip $(S_UPPER_DEPS)),)
-include $(S_UPPER_DEPS)
endif
ifneq ($(strip $(ASM_DEPS)),)
-include $(ASM_DEPS)
endif
ifneq ($(strip $(ASM_UPPER_DEPS)),)
-include $(ASM_UPPER_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 
SECONDARY_FLASH += \
UART1.hex \

SECONDARY_LIST += \
UART1.lst \

SECONDARY_SIZE += \
UART1.siz \


# All Target
all: 
	$(MAKE) --no-print-directory main-build 

main-build: UART1.elf secondary-outputs

# Tool invocations
UART1.elf: $(OBJS) $(USER_OBJS)
	@	riscv-none-embed-gcc -march=rv32imac -mabi=ilp32 -mcmodel=medany -msmall-data-limit=8 -mno-save-restore -fmax-errors=20 -Os -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -fno-common -g -T "c:/Users/Administrator/Documents/MyCode/0324/my_ch582m/SRC/Ld/Link.ld" -nostartfiles -Xlinker --gc-sections -L"c:\Users\Administrator\Documents\MyCode\0324\my_ch582m\ch582m" -L"c:/Users/Administrator/Documents/MyCode/0324/my_ch582m/SRC/StdPeriphDriver" -Xlinker --print-memory-usage -Wl,-Map,"UART1.map" --specs=nano.specs --specs=nosys.specs -o "UART1.elf" $(OBJS) $(USER_OBJS) $(LIBS)
UART1.hex: UART1.elf
	@	riscv-none-embed-objcopy -O ihex "UART1.elf" "UART1.hex"
UART1.lst: UART1.elf
	@	riscv-none-embed-objdump --source --all-headers --demangle --line-numbers --wide "UART1.elf" > "UART1.lst"
UART1.siz: UART1.elf
	riscv-none-embed-size --format=berkeley "UART1.elf"

# Other Targets
clean:
	-$(RM) $(OBJS)$(SECONDARY_FLASH)$(SECONDARY_LIST)$(SECONDARY_SIZE)$(S_DEPS)$(S_UPPER_DEPS)$(ASM_DEPS)$(ASM_UPPER_DEPS)$(C_DEPS) UART1.elf

secondary-outputs: $(SECONDARY_FLASH) $(SECONDARY_LIST) $(SECONDARY_SIZE)

.PHONY: all clean dependents

-include ../makefile.targets