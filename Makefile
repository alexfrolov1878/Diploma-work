RM := rm -rf
AR := ar rcs
CXX = g++
CXXFLAGS = -O2 -g -Wall -std=c++0x -fmessage-length=0

# Strict compiler options
CXXFLAGS += -Werror -Wformat-security -Wignored-qualifiers -Winit-self \
		-Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith \
		-Wtype-limits -Wempty-body -Wlogical-op \
		-Wmissing-field-initializers -Wctor-dtor-privacy \
		-Wnon-virtual-dtor -Wstrict-null-sentinel -Wold-style-cast \
		-Woverloaded-virtual -Wsign-promo -Wextra -pedantic

CPP_SRCS := \
src/GeneticAlgorithm.cpp \
src/MemoryVector.cpp \
src/Population.cpp \
src/Process.cpp \
src/Processor.cpp \
src/Solution.cpp \
src/main.cpp

OBJS := \
./GeneticAlgorithm.o \
./MemoryVector.o \
./Population.o \
./Process.o \
./Processor.o \
./Solution.o \
./main.o

%.o: src/%.cpp
	@echo 'Building file: $<'
	$(CXX) $(CXXFLAGS) -c -o "$@" "$<"

lib: $(OBJS)
	$(AR) libGA.a $(OBJS)

GA.exe: lib
	@echo 'Building target: $@'
	$(CXX) $(CXXFLAGS) -static -L. -lGA -o "GA.exe"
	@echo 'Finished building target: $@'

	# All Target
all: GA.exe clean

# Other Targets
clean:
	$(RM) $(OBJS)
cleanall:
	$(RM) $(OBJS) GA.exe libGA.a

.PHONY: all clean
.SECONDARY: