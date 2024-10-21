CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Werror -Wconversion -Wsign-conversion -Wcast-qual -Wformat=2 -Wundef -Werror=vla -Wcast-align -Wunused -Wold-style-cast -Woverloaded-virtual -Wsuggest-override -Wdouble-promotion
SRC_DIR = src
INCLUDE_DIR = include
TARGET_DIR = target
OBJ_DIR = $(TARGET_DIR)/obj
BIN_DIR = $(TARGET_DIR)
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
TARGET = $(BIN_DIR)/bin
CXXFLAGS += -I$(INCLUDE_DIR)

# Debug build settings
DEBUG ?= 0
ifeq ($(DEBUG), 1)
    CXXFLAGS += -g -O
    TARGET_DIR := $(TARGET_DIR)/debug
else
    CXXFLAGS += -O3
    TARGET_DIR := $(TARGET_DIR)/release
endif

# Update object and binary directories
OBJ_DIR = $(TARGET_DIR)/obj
BIN_DIR = $(TARGET_DIR)
OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)
TARGET = $(BIN_DIR)/bin

# Dependency files
DEPS = $(OBJECTS:.o=.d)

.PHONY: all clean

all: $(TARGET)
	@:

$(TARGET): $(OBJECTS)
	@mkdir -p $(BIN_DIR)
	@$(CXX) $(CXXFLAGS) $^ -o $@
	@echo "Build complete: $(TARGET)"

# Rule to generate a dep file by using the C preprocessor
# (see '-MM' option documentation)
$(OBJ_DIR)/%.d: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	@$(CXX) $(CXXFLAGS) -MM -MT '$(OBJ_DIR)/$*.o $@' $< > $@

# Include the dependency files
-include $(DEPS)

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(OBJ_DIR)
	@$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	@rm -rf $(TARGET_DIR)
	@echo "Cleaned build artifacts"

# Debugging target
.PHONY: debug
debug: DEBUG = 1
debug: all
