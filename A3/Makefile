CXX := g++
OUTPUT := A3

SFML_DIR := /opt/homebrew/Cellar/sfml/2.6.1

CXX_FLAGS := -O3 -std=c++20 -Wno-unused-result
INCLUDES := -I./src -I./src/imgui -I$(SFML_DIR)/include
LDFLAGS := -O3 -lsfml-graphics -lsfml-window -lsfml-system -lsfml-audio -framework OpenGL -L$(SFML_DIR)/lib

SRC_FILES := $(wildcard src/*.cpp src/imgui/*.cpp)
OBJ_FILES := $(SRC_FILES:.cpp=.o)

all:$(OUTPUT)

$(OUTPUT):$(OBJ_FILES) Makefile
	$(CXX) $(OBJ_FILES) $(LDFLAGS) -o ./bin/$@

.cpp.o:
	$(CXX) -c $(CXX_FLAGS) $(INCLUDES) $< -o $@

clean:
	rm -f $(OBJ_FILES) ./bin/A3

run: $(OUTPUT)
	cd bin && ./A3 && cd ..
