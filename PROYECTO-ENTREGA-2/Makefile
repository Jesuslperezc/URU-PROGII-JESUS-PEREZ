# Nombre del ejecutable
TARGET = hospital_v2.exe

# Todos los archivos fuente
SOURCES = *.cpp

# Compilador y flags
CXX = g++
CXXFLAGS = -Wall -Wextra -g3

# Regla por defecto
all: $(TARGET)

$(TARGET): $(SOURCES)
	$(CXX) $(CXXFLAGS) $(SOURCES) -o $(TARGET)

# Limpiar archivos objeto
clean:
	del *.o $(TARGET)  # Windows
