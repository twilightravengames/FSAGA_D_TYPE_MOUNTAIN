################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../TextWindow.cpp \
../action_interface_exposed.cpp \
../main.cpp 

OBJS += \
./TextWindow.o \
./action_interface_exposed.o \
./main.o 

CPP_DEPS += \
./TextWindow.d \
./action_interface_exposed.d \
./main.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include -I"/Users/waltergress/git/fsaga_d_editor/Fantasy_Saga_D_Type_Editor/shared" -O0 -g3 -Wall -c -fmessage-length=0 -frepo -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


