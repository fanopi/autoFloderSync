################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../source/operator/filetypderegistor.cc 

OBJS += \
./source/operator/filetypderegistor.o 

CC_DEPS += \
./source/operator/filetypderegistor.d 


# Each subdirectory must supply rules for building sources it contributes
source/operator/%.o: ../source/operator/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


