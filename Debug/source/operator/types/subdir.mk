################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../source/operator/types/epuboperator.cpp \
../source/operator/types/pdfoperator.cpp \
../source/operator/types/txtoperator.cpp 

OBJS += \
./source/operator/types/epuboperator.o \
./source/operator/types/pdfoperator.o \
./source/operator/types/txtoperator.o 

CPP_DEPS += \
./source/operator/types/epuboperator.d \
./source/operator/types/pdfoperator.d \
./source/operator/types/txtoperator.d 


# Each subdirectory must supply rules for building sources it contributes
source/operator/types/%.o: ../source/operator/types/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


