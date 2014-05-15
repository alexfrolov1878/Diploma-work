RM := rm -rf
CPP_SRCS := 
OBJS := 

CPP_SRCS += \
../src/GeneticAlgorithm.cpp \
../src/MemoryVector.cpp \
../src/Population.cpp \
../src/Process.cpp \
../src/Processor.cpp \
../src/Solution.cpp \
../src/main.cpp 

OBJS += \
./GeneticAlgorithm.o \
./MemoryVector.o \
./Population.o \
./Process.o \
./Processor.o \
./Solution.o \
./main.o 

%.o: ../src/%.cpp
	@echo 'Building file: $<'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -o "$@" "$<"

# All Target
all: GA.exe

lib: $(OBJS) 
	ar rcs libGA.a $(OBJS) 

# Tool invocations
GA.exe: $(OBJS)
	@echo 'Building target: $@'
	g++  -o "GA.exe" $(OBJS)
	@echo 'Finished building target: $@'
# Other Targets
clean:
	-$(RM) $(OBJS)
cleanall:
	-$(RM) $(OBJS) GA.exe libGA.a

.PHONY: all clean dependents
.SECONDARY:

