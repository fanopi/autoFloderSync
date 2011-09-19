################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../source/support/library/stdio.cc \
../source/support/library/stdlib.cc \
../source/support/library/string.cc 

OBJS += \
./source/support/library/stdio.o \
./source/support/library/stdlib.o \
./source/support/library/string.o 

CC_DEPS += \
./source/support/library/stdio.d \
./source/support/library/stdlib.d \
./source/support/library/string.d 


# Each subdirectory must supply rules for building sources it contributes
source/support/library/%.o: ../source/support/library/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


