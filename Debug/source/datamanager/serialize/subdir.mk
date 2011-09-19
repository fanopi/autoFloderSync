################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CC_SRCS += \
../source/datamanager/serialize/xml.cc \
../source/datamanager/serialize/xmlparser.cc 

OBJS += \
./source/datamanager/serialize/xml.o \
./source/datamanager/serialize/xmlparser.o 

CC_DEPS += \
./source/datamanager/serialize/xml.d \
./source/datamanager/serialize/xmlparser.d 


# Each subdirectory must supply rules for building sources it contributes
source/datamanager/serialize/%.o: ../source/datamanager/serialize/%.cc
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


