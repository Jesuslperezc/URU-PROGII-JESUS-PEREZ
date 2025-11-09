#ifndef UTILIDADES_HPP
#define UTILIDADES_HPP

#include <fstream>
#include "structs.hpp"  
using namespace std;

// Función genérica que calcula la posición de un registro
template <typename T>
streamoff calcularPosicion(int indice) {
    return sizeof(ArchivoHeader) + static_cast<streamoff>(indice) * sizeof(T);
}

// Función genérica para buscar un registro por ID en un archivo binario
template <typename T>
T buscarRegistroPorID(const char* nombreArchivo, int id) {
    fstream archivo(nombreArchivo, ios::binary | ios::in);
    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo: " << nombreArchivo << endl;
        return T{}; // Objeto vacío en caso de error
    }

    ArchivoHeader header;
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    if (!archivo) {
        cout << "Error al leer el header de " << nombreArchivo << endl;
        archivo.close();
        return T{};
    }

    T registro;
    for (int i = 0; i < header.cantidadRegistros; ++i) {
        archivo.seekg(sizeof(ArchivoHeader) + i * sizeof(T), ios::beg);
        archivo.read(reinterpret_cast<char*>(&registro), sizeof(T));

        if (!archivo) {
            cout << "Error al leer el registro " << i << endl;
            break;
        }

        if (!registro.eliminado && registro.id == id) {
            archivo.close();
            return registro;
        }
    }

    archivo.close();
    cout << "Registro con ID " << id << " no encontrado en " << nombreArchivo << endl;
    return T{};
}

// Función genérica para escribir un registro en una posición específica
template <typename T>
bool escribirRegistro(const char* nombreArchivo, const T& registro, int indice) {
    fstream archivo(nombreArchivo, ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) return false;
    archivo.seekp(calcularPosicion<T>(indice));
    archivo.write(reinterpret_cast<const char*>(&registro), sizeof(T));
    archivo.close();
    return true;
}

// Leer un registro genérico por índice
template <typename T>
T leerRegistro(const char* nombreArchivo, int indice) {
    fstream archivo(nombreArchivo, ios::binary | ios::in);
    T registro{};
    if (archivo.is_open()) {
        archivo.seekg(calcularPosicion<T>(indice));
        archivo.read(reinterpret_cast<char*>(&registro), sizeof(T));
        archivo.close();
    }
    return registro;
}
// Crear un nuevo paciente en el archivo binario
Paciente crearPaciente(const char* nombreArchivo, const Paciente& p) {
    ArchivoHeader header = leerHeader(nombreArchivo);
    fstream archivo(nombreArchivo, ios::binary | ios::in | ios::out);

    if (!archivo.is_open()) return Paciente{};

    // Asignar ID automáticamente
    Paciente nuevoPaciente = p;
    nuevoPaciente.id = header.proximoID;
    nuevoPaciente.activo = true;

    archivo.seekp(calcularPosicion<Paciente>(header.cantidadRegistros));
    archivo.write(reinterpret_cast<const char*>(&nuevoPaciente), sizeof(Paciente));
    archivo.close();

    // Actualizar header
    header.cantidadRegistros++;
    header.proximoID++;
    actualizarHeader(nombreArchivo, header);

    return nuevoPaciente;
}

#endif
