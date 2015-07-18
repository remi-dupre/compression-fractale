CC=g++ # Compilateur
EXEC=fzip # Les executables à générer

FLAGS=-W -Wall -ansi -pedantic -O3 -s
FILTERS=-Wno-sign-compare -Wno-unused-variable # Les warnings à ignorer
LIBS=-lpthread # Les bibliothèques à linker

BUILD_DIR=build
SOURCE_DIR=code
SRC=$(wildcard $(SOURCE_DIR)/*.cpp) # La liste des .cpp
OBJ=$(patsubst $(SOURCE_DIR)/%.cpp,$(BUILD_DIR)/%.o,$(SRC))

all: $(EXEC)

# Nettoye les dossiers de compilation
clean:
	rm -rf $(BUILD_DIR)/
	rm -rf $(SOURCE_DIR)/lib/

# Création de l'executable
fzip: $(BUILD_DIR)/lodepng.o $(OBJ)
	$(CC) -o $@ $^ $(FLAGS) $(FILTERS) $(LIBS)

# Compilation des fichiers

$(BUILD_DIR)/lodepng.o:
	@mkdir -p $(SOURCE_DIR)/lib $(BUILD_DIR)
	curl -o $(SOURCE_DIR)/lib/lodepng.cpp "https://raw.githubusercontent.com/lvandeve/lodepng/master/lodepng.cpp"
	curl -o $(SOURCE_DIR)/lib/lodepng.h "https://raw.githubusercontent.com/lvandeve/lodepng/master/lodepng.h"
	$(CC) -c -o $(BUILD_DIR)/lodepng.o $(SOURCE_DIR)/lib/lodepng.cpp $(FLAGS) $(FILTERS)

$(BUILD_DIR)/%.o: $(SOURCE_DIR)/%.cpp
	@mkdir -p $(BUILD_DIR)
	$(CC) -c -o $@ $< $(FLAGS) $(FILTERS)
