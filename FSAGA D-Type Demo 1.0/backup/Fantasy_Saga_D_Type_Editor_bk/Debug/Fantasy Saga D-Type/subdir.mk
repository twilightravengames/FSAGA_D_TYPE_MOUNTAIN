################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
/Users/waltergress/Documents/workspace/Fantasy\ Saga\ D-Type/crossexectuablefunctions.cpp \
/Users/waltergress/Documents/workspace/Fantasy\ Saga\ D-Type/main.cpp 

OBJS += \
./Fantasy\ Saga\ D-Type/crossexectuablefunctions.o \
./Fantasy\ Saga\ D-Type/main.o 

CPP_DEPS += \
./Fantasy\ Saga\ D-Type/crossexectuablefunctions.d \
./Fantasy\ Saga\ D-Type/main.d 


# Each subdirectory must supply rules for building sources it contributes
Fantasy\ Saga\ D-Type/crossexectuablefunctions.o: /Users/waltergress/Documents/workspace/Fantasy\ Saga\ D-Type/crossexectuablefunctions.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/waltergress/Documents/workspace/Fantasy Saga D-Type" -O0 -g3 -Wall -c -fmessage-length=0 -frepo -MMD -MP -MF"Fantasy Saga D-Type/crossexectuablefunctions.d" -MT"Fantasy\ Saga\ D-Type/crossexectuablefunctions.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Fantasy\ Saga\ D-Type/main.o: /Users/waltergress/Documents/workspace/Fantasy\ Saga\ D-Type/main.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/Users/waltergress/Documents/workspace/Fantasy Saga D-Type" -O0 -g3 -Wall -c -fmessage-length=0 -frepo -MMD -MP -MF"Fantasy Saga D-Type/main.d" -MT"Fantasy\ Saga\ D-Type/main.d" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


