CXX := g++
CXXFLAGS := -Wall -Wextra -O2 -Iinclude -MMD -MP

# Find all cpp files recursively under src/
SRC := $(shell find src -name '*.cpp')
OBJ := $(SRC:src/%.cpp=build/%.o)
DEP := $(OBJ:.o=.d)

# Final binary
BIN := bin/collision-sim

# Default target
all: $(BIN)

# Link step
$(BIN): $(OBJ)
	@mkdir -p $(dir $@)
	$(CXX) $(OBJ) -o $@

# Compile step (mirror src/ folder inside build/)
build/%.o: src/%.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Auto-include dependencies
-include $(DEP)

.PHONY: clean run

clean:
	rm -rf build bin

run: $(BIN)
	./$(BIN)
