# Compiler & flags
CXX := g++
CXXFLAGS := -Wall -Wextra -Iinclude -MMD -MP
LIBS := -lsfml-graphics -lsfml-window -lsfml-system

SRC := $(shell find src -name '*.cpp')
OBJ_DEBUG := $(SRC:src/%.cpp=build/debug/%.o)
OBJ_RELEASE := $(SRC:src/%.cpp=build/release/%.o)

BIN_DEBUG := bin/collision-sim-debug
BIN_RELEASE := bin/collision-sim

# Default
all: release

# Debug build
debug: CXXFLAGS += -g -O0
debug: $(BIN_DEBUG)

$(BIN_DEBUG): $(OBJ_DEBUG)
	@mkdir -p $(dir $@)
	$(CXX) $(OBJ_DEBUG) -o $@ $(LIBS)

build/debug/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Release build
release: CXXFLAGS += -O2 -DNDEBUG
release: $(BIN_RELEASE)

$(BIN_RELEASE): $(OBJ_RELEASE)
	@mkdir -p $(dir $@)
	$(CXX) $(OBJ_RELEASE) -o $@ $(LIBS)

build/release/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean
clean:
