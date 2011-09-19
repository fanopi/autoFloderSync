################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../source/support/os/threads/linux/threads.cc 

OBJS += \
./source/support/os/threads/linux/threads.o 

CC_DEPS += \
./source/support/os/threads/linux/threads.d 


# Each subdirectory must supply rules for building sources it contributes
source/support/os/threads/linux/%.o: ../source/support/os/threads/linux/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


