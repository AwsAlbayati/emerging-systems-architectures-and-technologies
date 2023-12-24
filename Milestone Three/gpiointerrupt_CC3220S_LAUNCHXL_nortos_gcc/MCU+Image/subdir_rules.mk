################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: GNU Compiler'
	"C:/ti/ccs1011/ccs/tools/compiler/gcc-arm-none-eabi-9-2019-q4-major/bin/arm-none-eabi-gcc-9.2.1.exe" -c -mcpu=cortex-m4 -march=armv7e-m -mthumb -mfloat-abi=soft -DDeviceFamily_CC3220 -DNORTOS_SUPPORT -I"C:/EDU/snhu/Emerging Sys Arch & Tech 23EW2/workspace_v10/MilestoneThree/gpiointerrupt_CC3220S_LAUNCHXL_nortos_gcc" -I"C:/EDU/snhu/Emerging Sys Arch & Tech 23EW2/workspace_v10/MilestoneThree/gpiointerrupt_CC3220S_LAUNCHXL_nortos_gcc/MCU+Image" -I"/source" -I"/kernel/nortos" -I"/kernel/nortos/posix" -I"C:/ti/ccs1011/ccs/tools/compiler/gcc-arm-none-eabi-9-2019-q4-major/arm-none-eabi/include/newlib-nano" -I"C:/ti/ccs1011/ccs/tools/compiler/gcc-arm-none-eabi-9-2019-q4-major/arm-none-eabi/include" -O3 -ffunction-sections -fdata-sections -g -gstrict-dwarf -Wall -MMD -MP -MF"$(basename $(<F)).d_raw" -MT"$(@)" -I"C:/EDU/snhu/Emerging Sys Arch & Tech 23EW2/workspace_v10/MilestoneThree/gpiointerrupt_CC3220S_LAUNCHXL_nortos_gcc/MCU+Image/syscfg" -std=c99 $(GEN_OPTS__FLAG) -o"$@" "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

build-1109335953: ../gpiointerrupt.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"C:/ti/sysconfig_1_12_0/sysconfig_cli.bat" --script "C:/EDU/snhu/Emerging Sys Arch & Tech 23EW2/workspace_v10/MilestoneThree/gpiointerrupt_CC3220S_LAUNCHXL_nortos_gcc/gpiointerrupt.syscfg" -o "syscfg" --compiler gcc
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/error.h: build-1109335953 ../gpiointerrupt.syscfg
syscfg/: build-1109335953

build-1438997727: ../image.syscfg
	@echo 'Building file: "$<"'
	@echo 'Invoking: SysConfig'
	"C:/ti/sysconfig_1_12_0/sysconfig_cli.bat" --script "C:/EDU/snhu/Emerging Sys Arch & Tech 23EW2/workspace_v10/MilestoneThree/gpiointerrupt_CC3220S_LAUNCHXL_nortos_gcc/image.syscfg" -o "syscfg" --compiler gcc
	@echo 'Finished building: "$<"'
	@echo ' '

syscfg/error.h: build-1438997727 ../image.syscfg
syscfg/: build-1438997727


