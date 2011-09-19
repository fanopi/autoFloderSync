################################################################################
# Automatically-generated file. Do not edit!
################################################################################

-include ../makefile.init

RM := rm -rf

# All of the sources participating in the build are defined here
-include sources.mk
-include subdir.mk
-include source/view/control/subdir.mk
-include source/support/support/subdir.mk
-include source/support/support/trace/subdir.mk
-include source/support/support/memoryleak/subdir.mk
-include source/support/os/threads/subdir.mk
-include source/support/os/threads/linux/subdir.mk
-include source/support/os/file/windows/subdir.mk
-include source/support/os/file/linux/subdir.mk
-include source/support/os/file/subdir.mk
-include source/support/library/subdir.mk
-include source/support/datastruct/subdir.mk
-include source/operator/types/subdir.mk
-include source/operator/folder/subdir.mk
-include source/operator/subdir.mk
-include source/datamanager/smallobject/subdir.mk
-include source/datamanager/serialize/subdir.mk
-include source/datamanager/filetree/subdir.mk
-include objects.mk

ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C++_DEPS)),)
-include $(C++_DEPS)
endif
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
ifneq ($(strip $(CC_DEPS)),)
-include $(CC_DEPS)
endif
ifneq ($(strip $(CPP_DEPS)),)
-include $(CPP_DEPS)
endif
ifneq ($(strip $(CXX_DEPS)),)
-include $(CXX_DEPS)
endif
ifneq ($(strip $(C_UPPER_DEPS)),)
-include $(C_UPPER_DEPS)
endif
endif

-include ../makefile.defs

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: autoFloderSync

# Tool invocations
autoFloderSync: $(OBJS) $(USER_OBJS)
	@echo 'Building target: $@'
	@echo 'Invoking: Cross G++ Linker'
	g++  -o "autoFloderSync" $(OBJS) $(USER_OBJS) $(LIBS)
	@echo 'Finished building target: $@'
	@echo ' '

# Other Targets
clean:
	-$(RM) $(C++_DEPS)$(OBJS)$(C_DEPS)$(CC_DEPS)$(CPP_DEPS)$(EXECUTABLES)$(CXX_DEPS)$(C_UPPER_DEPS) autoFloderSync
	-@echo ' '

.PHONY: all clean dependents
.SECONDARY:

-include ../makefile.targets
