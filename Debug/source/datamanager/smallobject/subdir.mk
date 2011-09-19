################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../source/datamanager/smallobject/messageobject.cc \
../source/datamanager/smallobject/sharedstring.cc 

OBJS += \
./source/datamanager/smallobject/messageobject.o \
./source/datamanager/smallobject/sharedstring.o 

CC_DEPS += \
./source/datamanager/smallobject/messageobject.d \
./source/datamanager/smallobject/sharedstring.d 


# Each subdirectory must supply rules for building sources it contributes
source/datamanager/smallobject/%.o: ../source/datamanager/smallobject/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


