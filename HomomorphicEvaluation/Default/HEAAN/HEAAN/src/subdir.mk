################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../HEAAN/HEAAN/src/AdditionManager.cpp \
../HEAAN/HEAAN/src/BigInteger.cpp \
../HEAAN/HEAAN/src/BootstrappingManager.cpp \
../HEAAN/HEAAN/src/DecryptionManager.cpp \
../HEAAN/HEAAN/src/EncryptionManager.cpp \
../HEAAN/HEAAN/src/FFT.cpp \
../HEAAN/HEAAN/src/MultiplicationManager.cpp \
../HEAAN/HEAAN/src/NumUtils.cpp \
../HEAAN/HEAAN/src/Ring.cpp \
../HEAAN/HEAAN/src/RingPair.cpp \
../HEAAN/HEAAN/src/RotationManager.cpp \
../HEAAN/HEAAN/src/SubstractionManager.cpp \
../HEAAN/HEAAN/src/Ciphertext.cpp \
../HEAAN/HEAAN/src/EvaluatorUtils.cpp \
../HEAAN/HEAAN/src/Key.cpp \
../HEAAN/HEAAN/src/Plaintext.cpp \
../HEAAN/HEAAN/src/Scheme.cpp \
../HEAAN/HEAAN/src/SecretKey.cpp \
../HEAAN/HEAAN/src/SerializationUtils.cpp \
../HEAAN/HEAAN/src/StringUtils.cpp \
../HEAAN/HEAAN/src/TestScheme.cpp \
../HEAAN/HEAAN/src/TimeUtils.cpp 

OBJS += \
./HEAAN/HEAAN/src/AdditionManager.o \
./HEAAN/HEAAN/src/BigInteger.o \
./HEAAN/HEAAN/src/BootstrappingManager.o \
./HEAAN/HEAAN/src/DecryptionManager.o \
./HEAAN/HEAAN/src/EncryptionManager.o \
./HEAAN/HEAAN/src/FFT.o \
./HEAAN/HEAAN/src/MultiplicationManager.o \
./HEAAN/HEAAN/src/NumUtils.o \
./HEAAN/HEAAN/src/Ring.o \
./HEAAN/HEAAN/src/RingPair.o \
./HEAAN/HEAAN/src/RotationManager.o \
./HEAAN/HEAAN/src/SubstractionManager.o \
./HEAAN/HEAAN/src/Ciphertext.o \
./HEAAN/HEAAN/src/EvaluatorUtils.o \
./HEAAN/HEAAN/src/Key.o \
./HEAAN/HEAAN/src/Plaintext.o \
./HEAAN/HEAAN/src/Scheme.o \
./HEAAN/HEAAN/src/SecretKey.o \
./HEAAN/HEAAN/src/SerializationUtils.o \
./HEAAN/HEAAN/src/StringUtils.o \
./HEAAN/HEAAN/src/TestScheme.o \
./HEAAN/HEAAN/src/TimeUtils.o 

CPP_DEPS += \
./HEAAN/HEAAN/src/AdditionManager.d \
./HEAAN/HEAAN/src/BigInteger.d \
./HEAAN/HEAAN/src/BootstrappingManager.d \
./HEAAN/HEAAN/src/DecryptionManager.d \
./HEAAN/HEAAN/src/EncryptionManager.d \
./HEAAN/HEAAN/src/FFT.d \
./HEAAN/HEAAN/src/MultiplicationManager.d \
./HEAAN/HEAAN/src/NumUtils.d \
./HEAAN/HEAAN/src/Ring.d \
./HEAAN/HEAAN/src/RingPair.d \
./HEAAN/HEAAN/src/RotationManager.d \
./HEAAN/HEAAN/src/SubstractionManager.d \
./HEAAN/HEAAN/src/Ciphertext.d \
./HEAAN/HEAAN/src/EvaluatorUtils.d \
./HEAAN/HEAAN/src/Key.d \
./HEAAN/HEAAN/src/Plaintext.d \
./HEAAN/HEAAN/src/Scheme.d \
./HEAAN/HEAAN/src/SecretKey.d \
./HEAAN/HEAAN/src/SerializationUtils.d \
./HEAAN/HEAAN/src/StringUtils.d \
./HEAAN/HEAAN/src/TestScheme.d \
./HEAAN/HEAAN/src/TimeUtils.d 


# Each subdirectory must supply rules for building sources it contributes
HEAAN/HEAAN/src/%.o: ../HEAAN/HEAAN/src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I"/home/sunly/Downloads/HomomorphicComparison-master/HomomorphicComparison/HEAAN/HEAAN/src" -I"/home/sunly/Downloads/HomomorphicComparison-master/HomomorphicComparison/lib/include" -O2 -g -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


