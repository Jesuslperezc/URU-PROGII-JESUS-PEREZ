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
bool asegurarArchivo(const char* nombreArchivo) {
    // Intentar abrir el archivo en modo lectura
    fstream archivo(nombreArchivo, ios::binary | ios::in);
    if (archivo.is_open()) {
        archivo.close();
        return true; // El archivo ya existe
    }

    // Si no existe, lo creamos con un header inicial
    fstream nuevo(nombreArchivo, ios::binary | ios::out);
    if (!nuevo.is_open()) {
        cout << "Error al crear el archivo: " << nombreArchivo << endl;
        return false;
    }

    // Header inicial
    ArchivoHeader header{};
    header.cantidadRegistros = 0;
    header.proximoID = 1;
    header.registrosActivos = 0;
    header.version = 1;

    nuevo.write(reinterpret_cast<const char*>(&header), sizeof(ArchivoHeader));
    nuevo.close();

    return true;
}
void verificarArchivos() {
    cout << "\n=== Verificación de Archivos ===\n";
    verificarArchivo("hospital.bin");
    verificarArchivo("pacientes.bin");
    verificarArchivo("doctores.bin");
    verificarArchivo("citas.bin");
    verificarArchivo("historiales.bin");
    
}
#include <filesystem>
#include <vector>
namespace fs = std::filesystem;

void hacerRespaldo() {
    cout << "\n=== Creando respaldo ===\n";
    fs::create_directory("backup");

    vector<string> archivos = {
        "hospital.bin", "pacientes.bin", "doctores.bin", "citas.bin", "historiales.bin"
    };

    for (auto& nombre : archivos) {
        fs::copy_file(nombre, "backup/" + nombre, fs::copy_options::overwrite_existing);
        cout << "Respaldo creado: " << nombre << "\n";
    }
}
void restaurarRespaldo() {
    cout << "\n=== Restaurando respaldo ===\n";
    vector<string> archivos = {
        "hospital.bin", "pacientes.bin", "doctores.bin", "citas.bin", "historiales.bin"
    };

    for (auto& nombre : archivos) {
        string origen = "backup/" + nombre;
        if (fs::exists(origen)) {
            fs::copy_file(origen, nombre, fs::copy_options::overwrite_existing);
            cout << "Archivo restaurado: " << nombre << "\n";
        } else {
            cout << "No existe respaldo para: " << nombre << "\n";
        }
    }
}
void mostrarEstadisticasArchivos() {
    cout << "\n=== Estadísticas de Archivos ===\n";

    auto mostrar = [](const char* nombreArchivo) {
        ArchivoHeader header = leerHeader(nombreArchivo);
        cout << nombreArchivo << ":\n";
        cout << "  Registros totales: " << header.cantidadRegistros << "\n";
        cout << "  Registros activos: " << header.registrosActivos << "\n";
        cout << "  Próximo ID: " << header.proximoID << "\n";
        cout << "  Versión: " << header.version << "\n\n";
    };

    mostrar("hospital.bin");
    mostrar("pacientes.bin");
    mostrar("doctores.bin");
    mostrar("citas.bin");
    mostrar("historiales.bin");
}//===========================================================
bool compactarArchivoPacientes() {
    ifstream original("pacientes.bin", ios::binary);
    ofstream temporal("pacientes_temp.bin", ios::binary);

    if (!original.is_open() || !temporal.is_open()) {
        cerr << "ERROR: No se pudo abrir archivos para compactar.\n";
        return false;
    }

    ArchivoHeader header;
    original.read((char*)&header, sizeof(ArchivoHeader));

    ArchivoHeader nuevoHeader = {0, 1, 0, header.version};
    temporal.write((char*)&nuevoHeader, sizeof(ArchivoHeader));

    Paciente p;
    for (int i = 0; i < header.cantidadRegistros; i++) {
        original.read((char*)&p, sizeof(Paciente));
        if (!p.eliminado) {
            p.id = nuevoHeader.proximoID++;
            temporal.write((char*)&p, sizeof(Paciente));
            nuevoHeader.cantidadRegistros++;
            nuevoHeader.registrosActivos++;
        }
    }

    temporal.seekp(0);
    temporal.write((char*)&nuevoHeader, sizeof(ArchivoHeader));

    original.close();
    temporal.close();

    remove("pacientes.bin");
    rename("pacientes_temp.bin", "pacientes.bin");

    cout << "Archivo compactado exitosamente.\n";
    return true;
}
bool compactarArchivoDoctores() {
    ifstream original("doctores.bin", ios::binary);
    ofstream temporal("doctores_temp.bin", ios::binary);

    if (!original.is_open() || !temporal.is_open()) {
        cerr << "ERROR: No se pudo abrir archivos para compactar.\n";
        return false;
    }

    ArchivoHeader header;
    original.read((char*)&header, sizeof(ArchivoHeader));

    ArchivoHeader nuevoHeader = {0, 1, 0, header.version};
    temporal.write((char*)&nuevoHeader, sizeof(ArchivoHeader));

    Doctor d;
    for (int i = 0; i < header.cantidadRegistros; i++) {
        original.read((char*)&d, sizeof(Paciente));
        if (!d.eliminado) {
            d.id = nuevoHeader.proximoID++;
            temporal.write((char*)&d, sizeof(Paciente));
            nuevoHeader.cantidadRegistros++;
            nuevoHeader.registrosActivos++;
        }
    }

    temporal.seekp(0);
    temporal.write((char*)&nuevoHeader, sizeof(ArchivoHeader));

    original.close();
    temporal.close();

    remove("doctores.bin");
    rename("doctores_temp.bin", "doctores.bin");

    cout << "Archivo compactado exitosamente.\n";
    return true;
}
