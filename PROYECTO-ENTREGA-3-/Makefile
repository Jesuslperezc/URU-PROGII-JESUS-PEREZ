APP_NAME := PROYECTO-ENTREGA-3

CXX      := g++
CXXFLAGS := -std=c++17 -Wall -Wextra -O2
INCLUDES := -I. \
            -ICitas \
            -IDoctores \
            -IHistoriales \
            -IHospital \
            -IPacientes \
            -Ipersistencia \
            -Iutilidades

BUILD_DIR := build
BIN_DIR   := bin

SRC := \
  main.cpp \
  Citas/Citas.cpp \
  Doctores/Doctores.cpp \
  Historiales/Historial.cpp \
  Hospital/Hospital.cpp \
  Pacientes/Pacientes.cpp \
  Pacientes/operacionesPaciente.cpp \
  persistencia/GestionArchivos.cpp \
  utilidades/utilidad.cpp

OBJ := $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(SRC))
APP := $(BIN_DIR)/$(APP_NAME).exe

.PHONY: all clean run

all: $(APP)

$(APP): $(OBJ)
	mkdir -p $(BIN_DIR)
	$(CXX) $(CXXFLAGS) $(OBJ) -o $(APP)

$(BUILD_DIR)/%.o: %.cpp
	mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

run: $(APP)
	./$(APP)

clean:
	rm -rf $(BUILD_DIR) $(BIN_DIR)