################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../02_getting_an_image_on_the_screen/02_getting_an_image_on_the_screen.c 

OBJS += \
./02_getting_an_image_on_the_screen/02_getting_an_image_on_the_screen.o 

C_DEPS += \
./02_getting_an_image_on_the_screen/02_getting_an_image_on_the_screen.d 


# Each subdirectory must supply rules for building sources it contributes
02_getting_an_image_on_the_screen/%.o: ../02_getting_an_image_on_the_screen/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


