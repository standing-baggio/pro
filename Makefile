# Compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -std=c++11

# Raylib and other libraries
LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11

# Source files
SRCS = main.cpp ping.cpp snake.cpp menu.cpp conway.cpp

# Object files
OBJS = $(SRCS:.cpp=.o)

# Executable name
TARGET = main

# Build target
$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS)

# Rule for building object files
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJS) $(TARGET)