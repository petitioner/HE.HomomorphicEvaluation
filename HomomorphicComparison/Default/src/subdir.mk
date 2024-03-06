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
	g++ -I"/home/sunly/Downloads/HomomorphicComparison-master/HomomorphicComparison/HEAAN/HEAAN/src" -I"/home/sunly/Downloads/HomomorphicComparison-master/HomomorphicComparison/lib/include" -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


