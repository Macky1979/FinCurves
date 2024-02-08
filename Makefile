# Thanks to Job Vranish (https://spin.atomicobject.com/2016/08/26/makefile-c-projects/)

# Specify C++ compiler.
CXX := /usr/bin/g++

# Name of the program.
TARGET_EXEC := FinCurves

# Debug with 'make' or compile mode with 'make compile'.
CPPFLAGS := -g
#CPPFLAGS := -O3

# Specify directories.
BUILD_DIR := ./build
SRC_DIRS := ./src
INCL_DIRS := ./include/my_libs

# Find all the C and C++ files we want to compile.
# Note the single quotes around the * expressions. The shell will incorrectly expand these otherwise, but we want to send the * directly to the find command.
SRCS := $(shell find $(SRC_DIRS) -name '*.cpp' -or -name '*.c' -or -name '*.s')
SRCS := $(SRCS) $(shell find $(INCL_DIRS) -name '*.cpp' -or -name '*.c' -or -name '*.s')

# Prepends BUILD_DIR and appends .o to every src file.
# As an example, ./your_dir/hello.cpp turns into ./build/./your_dir/hello.cpp.o.
OBJS := $(SRCS:%=$(BUILD_DIR)/%.o)

# String substitution (suffix version without %).
# As an example, ./build/hello.cpp.o turns into ./build/hello.cpp.d.
DEPS := $(OBJS:.o=.d)

# Every folder in ./src will need to be passed to GCC so that it can find header files.
INC_DIRS := $(shell find $(SRC_DIRS) -type d)
INC_DIRS := $(INC_DIR) $(shell find $(INCL_DIRS) -type d)

# Add a prefix to INC_DIRS. So moduleA would become -ImoduleA. GCC understands this -I flag.
INC_FLAGS := $(addprefix -I, $(INC_DIRS))

# Add folders with 3rd party code.
INC_FLAGS := $(INC_FLAGS) -I /usr/local/lib
INC_FLAGS := $(INC_FLAGS) -I /usr/local/include
INC_FLAGS := $(INC_FLAGS) -I include/sqlite-3.44.2
INC_FLAGS := $(INC_FLAGS) -I include/yaml-cpp
INC_FLAGS := $(INC_FLAGS) -l sqlite3
INC_FLAGS := $(INC_FLAGS) -l yaml-cpp

# The -MMD and -MP flags together generate Makefiles for us!
# These files will have .d instead of .o as the output.
CPPFLAGS := $(CPPFLAGS) -fdiagnostics-color=always
CPPFLAGS := $(CPPFLAGS) -std=c++20
CPPFLAGS := $(CPPFLAGS) -pthread
CPPFLAGS := $(CPPFLAGS) $(INC_FLAGS) -MMD -MP

# The final build step.
LDFLAGS := $(INC_FLAGS)
$(BUILD_DIR)/$(TARGET_EXEC): $(OBJS)
	$(CXX) $(OBJS) -o $(BUILD_DIR)/$(notdir $@) $(LDFLAGS)

# Build step for C source.
$(BUILD_DIR)/%.c.o: %.c
	mkdir -p $(dir $@)
	$(CC) $(CPPFLAGS) $(CFLAGS) -c $< -o $@

# Build step for C++ source.
$(BUILD_DIR)/%.cpp.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c $< -o $@

.PHONY: clean
clean:
	rm -r $(BUILD_DIR)

# Include the .d makefiles. The - at the front suppresses the errors of missing
# Makefiles. Initially, all the .d files will be missing, and we don't want those
# errors to show up.
-include $(DEPS)
