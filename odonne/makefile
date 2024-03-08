# Define compiler
CXX = g++

# Compiler flags
CXXFLAGS = -Wall -Wextra -std=c++11

# Target executable
TARGET = app

# Source files
SOURCES = $(wildcard *.cpp)

# Header files
HEADERS = $(wildcard *.h)

# List of file names
FILES = a1 a2 a3 a4 a5 a6 a7 a8

# Rule to build the target
$(TARGET): $(SOURCES) $(HEADERS)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

# Phony target to clean up
.PHONY: clean
clean:
	rm -f $(TARGET)

# Target to run the executable for all files
.PHONY: all
all: $(TARGET)
	@for file in $(FILES); do \
		echo "Running with $$file:"; \
		./$(TARGET) source-code-inputs/$$file; \
	done

# Target to run the executable with a specific file
.PHONY: a1 a2 a3 a4 a5 a6 a7 a8
a1 a2 a3 a4 a5 a6 a7 a8: $(TARGET)
	@echo "Running with $@:"
	./$(TARGET) source-code-inputs/$@
