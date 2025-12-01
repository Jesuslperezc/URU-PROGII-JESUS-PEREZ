#include "GestionArchivos.hpp"
#include <fstream>
#include <iostream>
#include <iomanip>
#include "../Pacientes/Pacientes.hpp"
#include "../Doctores/Doctores.hpp"
#include "../Hospital/Hospital.hpp"
#include <filesystem>
#include <vector>
namespace fs = std::filesystem;
using namespace std;

bool GestorArchivos:: inicializarArchivo(const char* nombreArchivo) {
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

bool GestorArchivos:: verificarArchivo(const char* nombreArchivo) {
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

bool GestorArchivos:: leerArchivoHeader(const char* nombreArchivo,ArchivoHeader&header) {
    fstream archivo(nombreArchivo, ios::binary | ios::in);
    if (!archivo.is_open()) {
        cout << "Error al abrir el archivo: " << nombreArchivo << endl;
        header= {0,0,0,0};
        return false;

    }
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    archivo.close();
    return true;
}

bool GestorArchivos:: actualizarHeader(const char* nombreArchivo, ArchivoHeader header) {
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
bool GestorArchivos::asegurarArchivo(const char* nombreArchivo) {
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
void GestorArchivos::verificarArchivos() {
    cout << "\n=== Verificación de Archivos ===\n";
    verificarArchivo("hospital.bin");
    verificarArchivo("pacientes.bin");
    verificarArchivo("doctores.bin");
    verificarArchivo("citas.bin");
    verificarArchivo("historiales.bin");
    
}



void GestorArchivos:: hacerRespaldo() {
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
void GestorArchivos:: restaurarRespaldo() {
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
void GestorArchivos::mostrarEstadisticasArchivos() {
    cout << "\n=== Estadísticas de Archivos ===\n";

    std::vector<const char*> archivos = {
        "hospital.bin",
        "pacientes.bin",
        "doctores.bin",
        "citas.bin",
        "historiales.bin"
    };

    for (const char* nombreArchivo : archivos) {
        ArchivoHeader header;
        if (leerArchivoHeader(nombreArchivo, header)) {
            cout << nombreArchivo << ":\n";
            cout << "  Registros totales: " << header.cantidadRegistros << "\n";
            cout << "  Registros activos: " << header.registrosActivos << "\n";
            cout << "  Próximo ID: " << header.proximoID << "\n";
            cout << "  Versión: " << header.version << "\n\n";
        } else {
            cout << nombreArchivo << ": no se pudieron leer estadísticas.\n\n";
        }
    }
}

bool GestorArchivos::guardarPaciente(const Paciente& paciente) {
    // Leer header
    ArchivoHeader header{};
    if (!leerArchivoHeader("pacientes.bin", header)) {
        std::cout << "Error al leer header de pacientes.bin" << std::endl;
        return false;
    }

    // Usar cantidadRegistros como índice
    int indice = header.cantidadRegistros;

    // Escribir registro
    if (!escribirRegistro<Paciente>("pacientes.bin", paciente, indice)) {
        return false;
    }

    // Actualizar header
    header.cantidadRegistros++;
    header.proximoID = paciente.getId()+1;
    header.registrosActivos++;
    if (!actualizarHeader("pacientes.bin", header)) {
        std::cout << "Error al actualizar header de pacientes.bin" << std::endl;
        return false;
    }

    return true;
}

bool GestorArchivos::guardarCita(const Cita& cita) {
    // Leer el header actual
    ArchivoHeader header{};
    if (!leerArchivoHeader("citas.bin", header)) {
        std::cout << "Error al leer header de citas.bin" << std::endl;
        return false;
    }

    // Usar cantidadRegistros como índice (escribir al final)
    int indice = header.cantidadRegistros;

    // Escribir la cita en el archivo
    if (!escribirRegistro<Cita>("citas.bin", cita, indice)) {
        std::cout << "Error al escribir cita en citas.bin" << std::endl;
        return false;
    }

    // Actualizar el header
    header.cantidadRegistros++;
    header.proximoID = cita.getId() + 1;   // siguiente ID disponible
    header.registrosActivos++;

    if (!actualizarHeader("citas.bin", header)) {
        std::cout << "Error al actualizar header de citas.bin" << std::endl;
        return false;
    }
      return true;
}

    bool GestorArchivos:: guardarHistorial(const Historial& historial) {
           ArchivoHeader header{};
    if (!leerArchivoHeader("historiales.bin", header)) {
        std::cout << "Error al leer header de historiales.bin" << std::endl;
        return false;
    }

    // Usar cantidadRegistros como índice
    int indice = header.cantidadRegistros;

    // Escribir registro
    if (!escribirRegistro<Historial>("historiales.bin", historial, indice)) {
        return false;
    }

    // Actualizar header
    header.cantidadRegistros++;
    header.proximoID = historial.gethistorialID()+1;
    header.registrosActivos++;
    if (!actualizarHeader("historiales.bin", header)) {
        std::cout << "Error al actualizar header de historiales.bin" << std::endl;
        return false;
    }
    return true;
}
bool GestorArchivos:: guardarDoctores (const Doctor& doctor){
                   ArchivoHeader header{};
    if (!leerArchivoHeader("doctores.bin", header)) {
        std::cout << "Error al leer header de doctores.bin" << std::endl;
        return false;
    }

    // Usar cantidadRegistros como índice
    int indice = header.cantidadRegistros;

    // Escribir registro
    if (!escribirRegistro<Doctor>("doctores.bin", doctor, indice)) {
        return false;
    }

    // Actualizar header
    header.cantidadRegistros++;
    header.proximoID = doctor.getId()+1;
    header.registrosActivos++;
    if (!actualizarHeader("doctores.bin", header)) {
        std::cout << "Error al actualizar header de pacientes.bin" << std::endl;
        return false;
    }

    return true;
}
  
bool GestorArchivos::compararCaseInsensitive(const char* a, const char* b) {
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b))
            return false;
        ++a;
        ++b;
    }
    return *a == *b;
}
    