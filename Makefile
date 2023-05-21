# Compiler and flags
CXX = g++
CXXFLAGS =	-g -Wall -Wextra -Werror -Wconversion -Wsign-conversion -Wformat-security -fstack-protector-all \
			-Wstack-protector --param ssp-buffer-size=4 -fPIE -pie -Wl,-z,relro,-z,now -ftrapv -Wpedantic -Wnull-dereference \
			-Wstrict-overflow=5

# Directories
INCLUDE_DIR = include
SRC_DIR = src
BIN_DIR = bin

# Source files
SRC_FILES = $(wildcard $(SRC_DIR)/*.cpp)
OBJ_FILES = $(patsubst $(SRC_DIR)/%.cpp, $(BIN_DIR)/%.o, $(SRC_FILES))

# External libraries
LIBS = -ljpeg

# Target executable
TARGET = $(BIN_DIR)/parser

# Rule to build the executable
$(TARGET): $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LIBS)

# Rule to build object files
$(BIN_DIR)/%.o: $(SRC_DIR)/%.cpp
	$(CXX) $(CXXFLAGS) -I$(INCLUDE_DIR) -c $< -o $@

# Clean rule
clean:
	rm -rf $(BIN_DIR)/*.o $(TARGET)

