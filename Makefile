# Makefile for Deadlock Aware Banking Solution
# C++17 Operating System Project
# Compiler: MinGW GCC on Windows 11

# Compiler and flags
CXX = g++
CXXFLAGS = -std=c++17 -Wall -Wextra -O2 -pthread
TARGET = banking.exe
SOURCES = main.cpp account.cpp scheduler.cpp logger.cpp deadlock_handler.cpp
OBJECTS = $(SOURCES:.cpp=.o)
HEADERS = account.h scheduler.h logger.h deadlock_handler.h

# Default target
all: $(TARGET)

# Build the main executable
$(TARGET): $(OBJECTS)
	@echo "Linking $(TARGET)..."
	$(CXX) $(OBJECTS) -o $(TARGET) $(CXXFLAGS)
	@echo "Build completed successfully!"
	@echo "Run with: ./$(TARGET)"

# Compile source files to object files
%.o: %.cpp $(HEADERS)
	@echo "Compiling $<..."
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean build artifacts
clean:
	@echo "Cleaning build artifacts..."
	rm -f $(OBJECTS) $(TARGET) transaction_log.txt
	@echo "Clean completed!"

# Run the program
run: $(TARGET)
	@echo "Running $(TARGET)..."
	./$(TARGET)

# Debug build
debug: CXXFLAGS += -g -DDEBUG
debug: $(TARGET)

# Release build
release: CXXFLAGS += -DNDEBUG
release: $(TARGET)

# Install dependencies (if needed)
install-deps:
	@echo "Installing dependencies..."
	@echo "Make sure MinGW is installed with pthread support"
	@echo "Required packages: gcc, g++, make"

# Check code style
check-style:
	@echo "Checking code style..."
	@echo "Note: Install clang-format for automatic formatting"

# Help target
help:
	@echo "Available targets:"
	@echo "  all      - Build the project (default)"
	@echo "  clean    - Remove build artifacts"
	@echo "  run      - Build and run the program"
	@echo "  debug    - Build with debug symbols"
	@echo "  release  - Build optimized release version"
	@echo "  help     - Show this help message"

# Phony targets
.PHONY: all clean run debug release install-deps check-style help

# Dependencies
main.o: main.cpp account.h scheduler.h logger.h deadlock_handler.h
account.o: account.cpp account.h
scheduler.o: scheduler.cpp scheduler.h
logger.o: logger.cpp logger.h
deadlock_handler.o: deadlock_handler.cpp deadlock_handler.h
#g++ main.cpp account.cpp scheduler.cpp logger.cpp deadlock_handler.cpp -o banking.exe -std=c++17 -pthread
#./banking.exe