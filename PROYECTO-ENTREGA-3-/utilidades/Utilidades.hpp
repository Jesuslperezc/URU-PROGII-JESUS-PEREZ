#ifndef UTILIDADES_HPP
#define UTILIDADES_HPP

#include <iostream>
#include <iomanip>      
#include <type_traits>  
#include <fstream>      
#include <cstring>      
#include "GestionArchivos.hpp"
#include "Pacientes.hpp"
#include "Doctores.hpp"
#include "Citas.hpp"
#include "Historial.hpp"


// ===================================================================
// Función genérica que calcula la posición de un registro
// ===================================================================
template <typename T>
std::streamoff calcularPosicion(int indice) {
    return sizeof(ArchivoHeader) + static_cast<std::streamoff>(indice) * sizeof(T);
}

// ===================================================================
// Buscar registro por ID
// ===================================================================
template <typename T>
T buscarRegistroPorID(const char* nombreArchivo, int id) {
    std::fstream archivo(nombreArchivo, std::ios::binary | std::ios::in);
    if (!archivo.is_open()) {
        std::cout << "Error al abrir el archivo: " << nombreArchivo << std::endl;
        return T{}; // Objeto vacío
    }

    ArchivoHeader header{};
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));

    if (!archivo) {
        std::cout << "Error al leer el header de " << nombreArchivo << std::endl;
        archivo.close();
        return T{};
    }

    T registro{};
    for (int i = 0; i < header.cantidadRegistros; ++i) {
        archivo.seekg(sizeof(ArchivoHeader) + i * sizeof(T), std::ios::beg);
        archivo.read(reinterpret_cast<char*>(&registro), sizeof(T));

        if (!archivo) {
            std::cout << "Error al leer el registro " << i << std::endl;
            break;
        }

        if (!registro.eliminado && registro.id == id) {
            archivo.close();
            return registro;
        }
    }

    archivo.close();
    std::cout << "Registro con ID " << id << " no encontrado en " << nombreArchivo << std::endl;
    return T{};
}

// ===================================================================
// Buscar registro por nombre
// ===================================================================
template <typename T>
T buscarRegistroPorNombre(const char* nombreArchivo, const char* nombreBuscado) {
    std::fstream archivo(nombreArchivo, std::ios::binary | std::ios::in);
    if (!archivo.is_open()) {
        std::cout << "Error al abrir el archivo: " << nombreArchivo << std::endl;
        return T{};
    }

    ArchivoHeader header{};
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));

    if (!archivo) {
        std::cout << "Error al leer el header de " << nombreArchivo << std::endl;
        archivo.close();
        return T{};
    }

    T registro{};
    for (int i = 0; i < header.cantidadRegistros; ++i) {
        archivo.seekg(sizeof(ArchivoHeader) + i * sizeof(T), std::ios::beg);
        archivo.read(reinterpret_cast<char*>(&registro), sizeof(T));

        if (!archivo) break;

        if constexpr (std::is_same_v<T, Doctor>) {
            if (!registro.eliminado && registro.disponible &&
                std::strcmp(registro.nombre, nombreBuscado) == 0) {
                archivo.close();
                return registro;
            }
        } else if constexpr (std::is_same_v<T, Paciente>) {
            if (!registro.eliminado && std::strcmp(registro.nombre, nombreBuscado) == 0) {
                archivo.close();
                return registro;
            }
        } else if constexpr (std::is_same_v<T, Cita>) {
            if (!registro.eliminado && std::strcmp(registro.motivo, nombreBuscado) == 0) {
                archivo.close();
                return registro;
            }
        } else if constexpr (std::is_same_v<T, HistorialMedico>) {
            if (!registro.eliminado && std::strcmp(registro.diagnostico, nombreBuscado) == 0) {
                archivo.close();
                return registro;
            }
        }
    }

    archivo.close();
    std::cout << "Registro con nombre " << nombreBuscado << " no encontrado en " << nombreArchivo << std::endl;
    return T{};
}

// ===================================================================
// Escribir registro
// ===================================================================
template <typename T>
bool escribirRegistro(const char* nombreArchivo, const T& registro, int indice) {
    std::fstream archivo(nombreArchivo, std::ios::binary | std::ios::in | std::ios::out);
    if (!archivo.is_open()) {
        // Crear archivo con header si no existe
        archivo.open(nombreArchivo, std::ios::binary | std::ios::out);
        if (!archivo.is_open()) {
            std::cout << "Error: no se pudo crear el archivo " << nombreArchivo << std::endl;
            return false;
        }

        ArchivoHeader header{0, 1, 0, 1};
        archivo.write(reinterpret_cast<const char*>(&header), sizeof(ArchivoHeader));
        archivo.close();

        archivo.open(nombreArchivo, std::ios::binary | std::ios::in | std::ios::out);
        if (!archivo.is_open()) return false;
    }

    archivo.seekp(calcularPosicion<T>(indice), std::ios::beg);
    if (!archivo.good()) {
        std::cout << "Error al mover el puntero en " << nombreArchivo << std::endl;
        archivo.close();
        return false;
    }

    archivo.write(reinterpret_cast<const char*>(&registro), sizeof(T));
    if (!archivo.good()) {
        std::cout << "Error al escribir el registro en " << nombreArchivo << std::endl;
        archivo.close();
        return false;
    }

    archivo.close();
    return true;
}

// ===================================================================
// Leer registro
// ===================================================================
template <typename T>
T leerRegistro(const char* nombreArchivo, int indice) {
    std::fstream archivo(nombreArchivo, std::ios::binary | std::ios::in);
    T registro{};
    if (archivo.is_open()) {
        archivo.seekg(calcularPosicion<T>(indice));
        archivo.read(reinterpret_cast<char*>(&registro), sizeof(T));
        archivo.close();
    }
    return registro;
}

// ===================================================================
// Leer header (no-template)
// ===================================================================
 ArchivoHeader leerHeader(const char* nombreArchivo);

// ===================================================================
// Listar registros
// ===================================================================
template <typename T>
void listarRegistros(const char* nombreArchivo) {
    ArchivoHeader header = leerHeader(nombreArchivo);
    int total = header.cantidadRegistros, activos = 0;

    // Encabezados dinámicos según el tipo
    if constexpr (std::is_same_v<T, Paciente>) {
        std::cout << std::left << std::setw(6) << "ID"
                  << std::setw(20) << "Nombre"
                  << std::setw(20) << "Apellido"
                  << std::setw(8)  << "Edad"
                  << std::setw(15) << "Cedula" << "\n";
    } else if constexpr (std::is_same_v<T, Doctor>) {
        std::cout << std::left << std::setw(6) << "ID"
                  << std::setw(20) << "Nombre"
                  << std::setw(20) << "Apellido"
                  << std::setw(15) << "Especialidad" << "\n";
    } else if constexpr (std::is_same_v<T, Cita>) {
        std::cout << std::left << std::setw(6) << "ID"
                  << std::setw(12) << "Fecha"
                  << std::setw(10) << "Hora"
                  << std::setw(8)  << "DocID"
                  << std::setw(8)  << "PacID"
                  << std::setw(25) << "Motivo" << "\n";
    }

    std::cout << std::string(80, '-') << "\n";

    // Mostrar registros activos
    for (int i = 0; i < total; ++i) {
        T r = leerRegistro<T>(nombreArchivo, i);
        if (!r.eliminado && r.id != 0) {
            activos++;

            if constexpr (std::is_same_v<T, Paciente>) {
                std::cout << std::left << std::setw(6) << r.id
                          << std::setw(20) << r.nombre
                          << std::setw(20) << r.apellido
                          << std::setw(8)  << r.edad
                          << std::setw(15) << r.cedula << "\n";
            } else if constexpr (std::is_same_v<T, Doctor>) {
                std::cout << std::left << std::setw(6) << r.id
                          << std::setw(20) << r.nombre
                          << std::setw(20) << r.apellido
                          << std::setw(15) << r.especialidad << "\n";
            } else if constexpr (std::is_same_v<T, Cita>) {
                std::cout << std::left << std::setw(6)  << r.id
                          << std::setw(12) << r.fecha
                          << std::setw(10) << r.hora
                          << std::setw(8)  << r.idDoctor
                          << std::setw(8)  << r.idPaciente
                          << std::setw(25) << r.motivo << "\n";
            }
        }
    }

    std::cout << std::string(80, '-') << "\n";
    std::cout << "Total de registros activos: " << activos << "\n\n";
}

template <typename T>
int encontrarIndicePorID(const char* nombreArchivo, int idBuscado) {
    std::fstream archivo(nombreArchivo, std::ios::binary | std::ios::in);
    if (!archivo.is_open()) return -1;

    ArchivoHeader header{};
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    if (!archivo.good()) { archivo.close(); return -1; }

    T reg{};
    for (int i = 0; i < header.cantidadRegistros; ++i) {
        archivo.seekg(calcularPosicion<T>(i), std::ios::beg);
        archivo.read(reinterpret_cast<char*>(&reg), sizeof(T));
        if (!archivo.good()) break;

        if (!reg.eliminado && reg.id == idBuscado) {
            archivo.close();
            return i; // índice real en el archivo
        }
    }

    archivo.close();
    return -1; // no encontrado
}


#endif // UTILIDADES_HPP
