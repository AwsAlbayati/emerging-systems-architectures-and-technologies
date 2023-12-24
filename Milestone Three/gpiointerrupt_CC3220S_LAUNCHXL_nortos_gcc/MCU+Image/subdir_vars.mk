################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
SYSCFG_SRCS += \
../gpiointerrupt.syscfg \
../image.syscfg 

LDS_SRCS += \
../cc32xxs_nortos.lds 

C_SRCS += \
../gpiointerrupt.c \
../main_nortos.c 

GEN_MISC_DIRS += \
./syscfg/ \
./syscfg/ 

C_DEPS += \
./gpiointerrupt.d \
./main_nortos.d 

OBJS += \
./gpiointerrupt.o \
./main_nortos.o 

GEN_MISC_FILES += \
./syscfg/error.h \
./syscfg/error.h 

GEN_MISC_DIRS__QUOTED += \
"syscfg\" \
"syscfg\" 

OBJS__QUOTED += \
"gpiointerrupt.o" \
"main_nortos.o" 

GEN_MISC_FILES__QUOTED += \
"syscfg\error.h" \
"syscfg\error.h" 

C_DEPS__QUOTED += \
"gpiointerrupt.d" \
"main_nortos.d" 

C_SRCS__QUOTED += \
"../gpiointerrupt.c" \
"../main_nortos.c" 

SYSCFG_SRCS__QUOTED += \
"../gpiointerrupt.syscfg" \
"../image.syscfg" 


