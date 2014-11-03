RM := rm -rf
AR := ar rcs
CXX = g++
CXXFLAGS = -O2 -g -Wall -std=c++11 -fmessage-length=0

# Strict compiler options
CXXFLAGS += -Werror -Wformat-security -Wignored-qualifiers -Winit-self \
		-Wswitch-default -Wfloat-equal -Wshadow -Wpointer-arith \
		-Wtype-limits -Wempty-body -Wlogical-op \
		-Wmissing-field-initializers -Wctor-dtor-privacy \
		-Wnon-virtual-dtor -Wstrict-null-sentinel -Wold-style-cast \
		-Woverloaded-virtual -Wsign-promo -Wextra -pedantic

# Directories with source code
SRC_DIR = src
INCLUDE_DIR = include

BUILD_DIR = build
OBJ_DIR = $(BUILD_DIR)/obj
BIN_DIR = $(BUILD_DIR)/bin

# Add headers dirs to gcc search path
CXXFLAGS += -I $(INCLUDE_DIR)

# Helper macros
make_path = $(addsuffix $(1), $(basename $(subst $(2), $(3), $(4))))
# Takes path list with source files and returns pathes to related objects.
src_to_obj = $(call make_path,.o, $(SRC_DIR), $(OBJ_DIR), $(1))
# All source files in the project that must be built into movable object code.
CXXFILES := $(wildcard $(SRC_DIR)/*.cpp)
OBJFILES := $(call src_to_obj, $(CXXFILES))

# Suppress makefile rebuilding.
Makefile: ;

dir.touch:
	mkdir -p $(OBJ_DIR) $(BIN_DIR)

$(BIN_DIR)/GA_test_generator.exe: dir.touch $(OBJFILES)
	@echo 'Building target: $@'
	$(CXX) $(CXXFLAGS) $(OBJFILES) -o $@
	@echo 'Finished building target: $@'

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@echo 'Building file: $<'
	$(CXX) $(CXXFLAGS) -c -o $(call src_to_obj, $<) $<


# Default target
.DEFAULT_GOAL := all

# All Target
all: $(BIN_DIR)/GA_test_generator.exe

# Delete all temprorary and binary files
clean:
	rm -rf $(BUILD_DIR)
	rm -f *.txt

# Fictive target
.PHONY: all