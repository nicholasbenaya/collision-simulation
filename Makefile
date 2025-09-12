CXX := g++
CXXFLAGS := -Wall -Wextra -O2 -Iinclude -MMD -MP
LIBS := -lsfml-graphics -lsfml-window -lsfml-system

# Find all cpp files recursively under src/
SRC := $(shell find src -name '*.cpp')
OBJ := $(SRC:src/%.cpp=build/%.o)
DEP := $(OBJ:.o=.d)

# Final binary
BIN := bin/collision-sim

# Default target
all: $(BIN)

# Link step (now with SFML)
$(BIN): $(OBJ)
	@mkdir -p $(dir $@)
	$(CXX) $(OBJ) -o $@ $(LIBS)

# Compile step (unchanged)
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
