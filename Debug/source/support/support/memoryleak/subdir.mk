################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../source/support/support/memoryleak/memoryleaktrace.cc 

OBJS += \
./source/support/support/memoryleak/memoryleaktrace.o 

CC_DEPS += \
./source/support/support/memoryleak/memoryleaktrace.d 


# Each subdirectory must supply rules for building sources it contributes
source/support/support/memoryleak/%.o: ../source/support/support/memoryleak/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


