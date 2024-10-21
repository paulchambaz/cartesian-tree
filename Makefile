CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -Wpedantic -Werror -Wconversion -Wsign-conversion -Wcast-qual -Wformat=2 -Wundef -Werror=vla -Wcast-align -Wunused -Wold-style-cast -Woverloaded-virtual -Wsuggest-override -Wdouble-promotion
SRC_DIR = src
INCLUDE_DIR = include
TARGET_DIR = target
SOURCES = $(wildcard $(SRC_DIR)/*.cpp)

# Common flags
CXXFLAGS += -I$(INCLUDE_DIR)

# Release settings
RELEASE_FLAGS = -O2 -D_FORTIFY_SOURCE=2
RELEASE_DIR = $(TARGET_DIR)/release
RELEASE_OBJ_DIR = $(RELEASE_DIR)/obj
RELEASE_OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(RELEASE_OBJ_DIR)/%.o)
RELEASE_TARGET = $(RELEASE_DIR)/bin

# Debug settings
DEBUG_FLAGS = -g -O
DEBUG_DIR = $(TARGET_DIR)/debug
DEBUG_OBJ_DIR = $(DEBUG_DIR)/obj
DEBUG_OBJECTS = $(SOURCES:$(SRC_DIR)/%.cpp=$(DEBUG_OBJ_DIR)/%.o)
DEBUG_TARGET = $(DEBUG_DIR)/bin

.PHONY: all clean release debug

all: release

release: $(RELEASE_TARGET)

debug: $(DEBUG_TARGET)

$(RELEASE_TARGET): $(RELEASE_OBJECTS)
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) $(RELEASE_FLAGS) $^ -o $@

$(DEBUG_TARGET): $(DEBUG_OBJECTS)
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) $^ -o $@

$(RELEASE_OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) $(RELEASE_FLAGS) -c $< -o $@

$(DEBUG_OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp
	@mkdir -p $(dir $@)
	@$(CXX) $(CXXFLAGS) $(DEBUG_FLAGS) -c $< -o $@

clean:
	@rm -rf $(TARGET_DIR)
