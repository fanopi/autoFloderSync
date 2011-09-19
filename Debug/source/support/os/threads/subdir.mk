################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
O_SRCS += \
../source/support/os/threads/test.o 

CC_SRCS += \
../source/support/os/threads/test.cc \
../source/support/os/threads/threads.cc 

OBJS += \
./source/support/os/threads/test.o \
./source/support/os/threads/threads.o 

CC_DEPS += \
./source/support/os/threads/test.d \
./source/support/os/threads/threads.d 


# Each subdirectory must supply rules for building sources it contributes
source/support/os/threads/%.o: ../source/support/os/threads/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


