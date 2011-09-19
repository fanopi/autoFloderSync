################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../source/support/support/runnable.cpp 

CC_SRCS += \
../source/support/support/runnable-test.cc \
../source/support/support/scopepointer.cc \
../source/support/support/stringhash.cc \
../source/support/support/utility.cc 

OBJS += \
./source/support/support/runnable-test.o \
./source/support/support/runnable.o \
./source/support/support/scopepointer.o \
./source/support/support/stringhash.o \
./source/support/support/utility.o 

CC_DEPS += \
./source/support/support/runnable-test.d \
./source/support/support/scopepointer.d \
./source/support/support/stringhash.d \
./source/support/support/utility.d 

CPP_DEPS += \
./source/support/support/runnable.d 


# Each subdirectory must supply rules for building sources it contributes
source/support/support/%.o: ../source/support/support/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

source/support/support/%.o: ../source/support/support/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


