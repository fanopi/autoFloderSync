################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../source/datamanager/filetree/filetreebuilder.cc 

OBJS += \
./source/datamanager/filetree/filetreebuilder.o 

CC_DEPS += \
./source/datamanager/filetree/filetreebuilder.d 


# Each subdirectory must supply rules for building sources it contributes
source/datamanager/filetree/%.o: ../source/datamanager/filetree/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


