# Compiler
CXX = g++

# Compiler flags
# -std=c++17: Use C++17 standard
# -g: Include debugging information
# -Wall: Turn on all standard warnings
CXXFLAGS = -std=c++17 -g -Wall

# Executable name
TARGET = chess_engine

# Source files
SRCS = src/main.cpp src/board.cpp src/move.cpp

# Object files (derived from source files)
OBJS = $(SRCS:.cpp=.o)

# Default rule: Build the target executable
all: $(TARGET)

# Rule to link the executable
$(TARGET): $(OBJS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(OBJS)

# Rule to compile C++ source files into object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Rule to clean up build files
clean:
	rm -f $(OBJS) $(TARGET)

# Rule to run the program
run: all
	./$(TARGET)