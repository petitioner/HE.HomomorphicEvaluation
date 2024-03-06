################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/AdditionManager.cpp \
../src/BigInteger.cpp \
../src/BootstrappingManager.cpp \
../src/DecryptionManager.cpp \
../src/EncryptionManager.cpp \
../src/FFT.cpp \
../src/MultiplicationManager.cpp \
../src/NumUtils.cpp \
../src/Ring.cpp \
../src/RingPair.cpp \
../src/RotationManager.cpp \
../src/SubstractionManager.cpp \
../src/Ciphertext.cpp \
../src/EvaluatorUtils.cpp \
../src/Key.cpp \
../src/Plaintext.cpp \
../src/Scheme.cpp \
../src/SecretKey.cpp \
../src/SerializationUtils.cpp \
../src/StringUtils.cpp \
../src/TestScheme.cpp \
../src/TimeUtils.cpp 

OBJS += \
./src/AdditionManager.o \
./src/BigInteger.o \
./src/BootstrappingManager.o \
./src/DecryptionManager.o \
./src/EncryptionManager.o \
./src/FFT.o \
./src/MultiplicationManager.o \
./src/NumUtils.o \
./src/Ring.o \
./src/RingPair.o \
./src/RotationManager.o \
./src/SubstractionManager.o \
./src/Ciphertext.o \
./src/EvaluatorUtils.o \
./src/Key.o \
./src/Plaintext.o \
./src/Scheme.o \
../src/SecretKey.o \
./src/SerializationUtils.o \
./src/StringUtils.o \
./src/TestScheme.o \
./src/TimeUtils.o 

CPP_DEPS += \
./src/AdditionManager.d \
./src/BigInteger.d \
./src/BootstrappingManager.d \
./src/DecryptionManager.d \
./src/EncryptionManager.d \
./src/FFT.d \
./src/MultiplicationManager.d \
./src/NumUtils.d \
./src/Ring.d \
./src/RingPair.d \
./src/RotationManager.d \
./src/SubstractionManager.d \
./src/Ciphertext.d \
./src/EvaluatorUtils.d \
./src/Key.d \
./src/Plaintext.d \
./src/Scheme.d \
./src/SecretKey.d \
./src/SerializationUtils.d \
./src/StringUtils.d \
./src/TestScheme.d \
./src/TimeUtils.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/usr/local/include -IIDASH2017/lib/include -IIDASH2017/lib/lib -O3 -c -std=c++11 -pthread -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


