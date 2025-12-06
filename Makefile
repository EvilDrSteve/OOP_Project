# === Compiler and flags ===
CXX = g++
CXXFLAGS = -std=c++17 -Wall -I include
LDFLAGS = -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio

# === Directories ===
SRC_DIR = src
INC_DIR = include
OBJ_DIR = obj
BUILD_DIR = build
BIN = $(BUILD_DIR)/game

# === Source and object files ===
SRC = $(wildcard $(SRC_DIR)/*.cpp)
OBJ = $(patsubst $(SRC_DIR)/%.cpp, $(OBJ_DIR)/%.o, $(SRC))
MAIN_OBJ = $(OBJ_DIR)/main.o

# === Default target ===
all: $(BIN)

# === Link all object files into the final executable ===
$(BIN): $(OBJ) $(MAIN_OBJ) | $(BUILD_DIR)
	$(CXX) $(OBJ) $(MAIN_OBJ) -o $@ $(LDFLAGS)

# === Compile .cpp files from src/ into obj/ ===
$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c $< -o $@

# === Compile main.cpp (in root folder) ===
$(MAIN_OBJ): main.cpp | $(OBJ_DIR)
	$(CXX) $(CXXFLAGS) -c main.cpp -o $(MAIN_OBJ)

# === Ensure obj and build directories exist ===
$(OBJ_DIR):
	mkdir -p $(OBJ_DIR)

$(BUILD_DIR):
	mkdir -p $(BUILD_DIR)

# === Clean build artifacts ===
clean:
	rm -rf $(OBJ_DIR) $(BUILD_DIR)
