#ifndef UTILIDADES_HPP
#define UTILIDADES_HPP
#include <iostream>
#include <type_traits>
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

// ===== === == ===== = = == = ====== ==== ======= == == ==== ===== ==
//    TEMPLATE: Buscar registro por nombre en un archivo binario
// === = = = = ========== ========== == == === ====== === == == == ===
template <typename T>
T buscarRegistroPorNombre(const char* nombreArchivo, const char* nombreBuscado) {
    fstream archivo(nombreArchivo, ios::binary | ios::in);
    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo: " << nombreArchivo << endl;
        return T{};  // Devuelve objeto vacío
    }

    ArchivoHeader header{};
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    if (!archivo) {
        cout << "Error al leer el header de " << nombreArchivo << endl;
        archivo.close();
        return T{};
    }

    T registro{};
    for (int i = 0; i < header.cantidadRegistros; ++i) {
        archivo.seekg(sizeof(ArchivoHeader) + i * sizeof(T), ios::beg);
        archivo.read(reinterpret_cast<char*>(&registro), sizeof(T));

        if (!archivo) {
            cout << "Error al leer el registro " << i << " de " << nombreArchivo << endl;
            break;
        }

        // -------------------------------------------------------------
        // CASOS SEGÚN EL TIPO DE ESTRUCTURA
        // -------------------------------------------------------------
        if constexpr (is_same_v<T, Doctor>) {
            if (!registro.eliminado && registro.disponible &&
                strcmp(registro.nombre, nombreBuscado) == 0) {
                archivo.close();
                return registro;
            }
        }
        else if constexpr (is_same_v<T, Paciente>) {
            if (!registro.eliminado && strcmp(registro.nombre, nombreBuscado) == 0) {
                archivo.close();
                return registro;
            }
        }
        else if constexpr (is_same_v<T, Cita>) {
            if (!registro.eliminado && strcmp(registro.motivo, nombreBuscado) == 0) {
                archivo.close();
                return registro;
            }
        }
        else if constexpr (is_same_v<T, HistorialMedico>) {
            if (!registro.eliminado && strcmp(registro.diagnostico, nombreBuscado) == 0) {
                archivo.close();
                return registro;
            }
        }
    }

    archivo.close();
    cout << "Registro con nombre " << nombreBuscado << " no encontrado en " << nombreArchivo << endl;
    return T{};
}


// Función genérica para escribir un registro en una posición específica
template <typename T>
bool escribirRegistro(const char* nombreArchivo, const T& registro, int indice) {
    // Verificar si el archivo existe
    fstream archivo(nombreArchivo, ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) {
        // Si no existe, lo creamos con un header inicial
        archivo.open(nombreArchivo, ios::binary | ios::out);
        if (!archivo.is_open()) {
            cout << "Error: no se pudo crear el archivo " << nombreArchivo << endl;
            return false;
        }
        ArchivoHeader header{};
        header.cantidadRegistros = 0;
        header.proximoID = 1;
        header.registrosActivos = 0;
        header.version = 1;
        archivo.write(reinterpret_cast<const char*>(&header), sizeof(ArchivoHeader));
        archivo.close();

        // Reabrimos ahora sí en modo lectura/escritura
        archivo.open(nombreArchivo, ios::binary | ios::in | ios::out);
        if (!archivo.is_open()) return false;
    }

    // Posicionar el puntero de escritura en el registro indicado
    archivo.seekp(calcularPosicion<T>(indice), ios::beg);
    if (!archivo.good()) {
        cout << "Error al mover el puntero en " << nombreArchivo << endl;
        archivo.close();
        return false;
    }

    // Escribir el registro
    archivo.write(reinterpret_cast<const char*>(&registro), sizeof(T));
    if (!archivo.good()) {
        cout << "Error al escribir el registro en " << nombreArchivo << endl;
        archivo.close();
        return false;
    }

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

