################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/MyHomomorphicComparison.cpp \
../src/MyMethods.cpp \
../src/MyTools.cpp 

OBJS += \
./src/MyHomomorphicComparison.o \
./src/MyMethods.o \
./src/MyTools.o 

CPP_DEPS += \
./src/MyHomomorphicComparison.d \
./src/MyMethods.d \
./src/MyTools.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include -I/Users/kimandrik/git/HEAAN/HEAAN/src -O3 -pthread -c -std=c++11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


