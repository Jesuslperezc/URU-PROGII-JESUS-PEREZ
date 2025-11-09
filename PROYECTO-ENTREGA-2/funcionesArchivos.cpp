#include "funcionesArchivos.hpp"
#include <iostream>
#include <fstream>
using namespace std;



// ============================================
// FUNCIONES DE ARCHIVOS BINARIOS
// ============================================

bool inicializarArchivo(const char* nombreArchivo) {
    fstream archivo(nombreArchivo, ios::binary | ios::out);
    if (!archivo.is_open()) {
        cout << "Error al crear el archivo: " << nombreArchivo << endl;
        return false;
    }

    ArchivoHeader header{0,1,0,1};
    archivo.write(reinterpret_cast<const char*>(&header), sizeof(ArchivoHeader));
    archivo.close();
    return true;
}

bool verificarArchivo(const char* nombreArchivo) {
    fstream archivo(nombreArchivo, ios::binary | ios::in);
    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo: " << nombreArchivo << endl;
        return false;
    }

    ArchivoHeader header{};
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    if (!archivo.good()) {
        cout << "Error al leer el encabezado del archivo." << endl;
        archivo.close();
        return false;
    }

    cout << "=== Información del archivo ===\n";
    cout << "Cantidad de registros: " << header.cantidadRegistros << endl;
    cout << "Próximo ID: " << header.proximoID << endl;
    cout << "Registros activos: " << header.registrosActivos << endl;
    cout << "Versión del archivo: " << header.version << endl;

    if (header.version != 1) {
        cout << "Versión de archivo no compatible: " << header.version << endl;
        archivo.close();
        return false;
    }

    archivo.close();
    return true;
}

ArchivoHeader leerHeader(const char* nombreArchivo) {
    fstream archivo(nombreArchivo, ios::binary | ios::in);
    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo: " << nombreArchivo << endl;
        return ArchivoHeader{0,0,0,0};
    }

    ArchivoHeader header{};
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    archivo.close();
    return header;
}

bool actualizarHeader(const char* nombreArchivo, ArchivoHeader header) {
    fstream archivo(nombreArchivo, ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo: " << nombreArchivo << endl;
        return false;
    }

    archivo.seekp(0, ios::beg);
    archivo.write(reinterpret_cast<const char*>(&header), sizeof(ArchivoHeader));
    archivo.close();
    return true;
}
