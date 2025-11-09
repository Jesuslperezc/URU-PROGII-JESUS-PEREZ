#include "FuncionesPrincipales.hpp"
#include "funcionesArchivos.hpp"
#include <iostream>
#include <cstring>
#include <cctype>
#include <fstream>
using namespace std;

// VALIDACIONES adicionales
bool validarCedula(const char* cedula){
    if (cedula == nullptr) return false;
    if (cedula[0] == '\0') return false;
    size_t len = strlen(cedula);
    if (len > 19) { // deja espacio para el terminador
        cout << "Excede el limite de caracteres para cédula.\n";
        return false;
    }
    // Solo dígitos
    for (size_t i = 0; i < len; ++i) {
        if (!isdigit((unsigned char)cedula[i])) {
            cout << "La cédula debe contener solo números.\n";
            return false;
        }
    }
    return true;
}

bool validarNombreSinEspacios(const char* nombre) {
    if (nombre == nullptr) return false;
    if (nombre[0] == '\0') return false;
    // No permitir espacios en blanco
    for (size_t i = 0; i < strlen(nombre); ++i) {
        if (isspace((unsigned char)nombre[i])) {
            cout << "El nombre/apellido no debe contener espacios.\n";
            return false;
        }
    }
    return true;
}

bool validarEdad(int edad) {
    if (edad < 0) {
        cout << "La edad no puede ser negativa.\n";
        return false;
    }
    if (edad > 120) {
        cout << "La edad supera el límite razonable (120).\n";
        return false;
    }
    return true;
}

bool validarSexoChar(char sexo) {
    char s = toupper((unsigned char)sexo);
    if (s == 'M' || s == 'F') return true;
    cout << "Sexo inválido. Use M o F.\n";
    return false;
}

// Requisito: Inicializar hospital con datos
void RegistrarHospital(Hospital* hospital) {
    cout << "=== Registro del Hospital ===\n";
    cout << "Ingrese el nombre del hospital: ";
    cin.getline(hospital->nombre, 100);
    cout << "Ingrese la direccion del hospital: ";
    cin.getline(hospital->direccion, 150);
    cout << "Ingrese el telefono del hospital: ";
    cin.getline(hospital->telefono, 15);

    cout << "\n Hospital registrado exitosamente.\n\n";
}
bool compararCaseInsensitive(const char* a, const char* b) {
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b))
            return false;
        ++a;
        ++b;
    }
    return *a == *b;
}

// ============================================
// DEFINICIONES DE FUNCIONES PACIENTES
// ============================================
Paciente* crearPaciente(Hospital* hospital, const char* nombre,
                        const char* apellido, const char* cedula, int edad, char sexo) {
                            
    if (!validarNombreSinEspacios(nombre) || !validarNombreSinEspacios(apellido)) return nullptr;
    if (!validarCedula(cedula)) return nullptr;
    if (!validarEdad(edad)) return nullptr;
    if (!validarSexoChar(sexo)) return nullptr;

    Paciente p{};
    strcpy(p.nombre, nombre);
    strcpy(p.apellido, apellido);
    strcpy(p.cedula, cedula);
    p.edad = edad;
    p.sexo = toupper(sexo);
    p.activo = true;
    p.eliminado = false;
    p.fechaCreacion = time(nullptr);
    p.cantidadConsultas = 0;
    p.primerConsultaID = -1;
    p.cantidadCitas = 0;

    // Guardar el paciente directamente en el archivo "pacientes.bin"
    fstream archivo("pacientes.bin", ios::binary | ios::in | ios::out);
    ArchivoHeader header{};
    if (!archivo.is_open()) {
        // Si no existe, crear y escribir header inicial
        archivo.open("pacientes.bin", ios::binary | ios::out);
        header.cantidadRegistros = 0;
        archivo.write(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
        archivo.close();
        // volver a abrir en modo lectura/escritura
        archivo.open("pacientes.bin", ios::binary | ios::in | ios::out);
        if (!archivo.is_open()) {
            cout << "No se pudo crear/abrir el archivo de pacientes.\n";
            return nullptr;
        }
    }

    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    // Asignar ID secuencial
    p.id = header.cantidadRegistros + 1;
    // Escribir paciente al final (después del header y de los registros existentes)
    archivo.seekp(sizeof(ArchivoHeader) + header.cantidadRegistros * sizeof(Paciente));
    archivo.write(reinterpret_cast<char*>(&p), sizeof(Paciente));
    // Actualizar header en el archivo
    header.cantidadRegistros++;
    archivo.seekp(0);
    archivo.write(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    archivo.close();

    hospital->totalPacientesRegistrados++;
    actualizarHeader("hospital.bin", *reinterpret_cast<ArchivoHeader*>(&hospital));

    cout << "Paciente creado exitosamente con ID: " << p.id << "\n";
    return nullptr; // ya no devolvemos puntero dinámico
}

bool eliminarPaciente(int id) {
    fstream archivo("pacientes.bin", ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) return false;

    ArchivoHeader header;
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));

    Paciente p{};
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.seekg(sizeof(ArchivoHeader) + i * sizeof(Paciente));
        archivo.read(reinterpret_cast<char*>(&p), sizeof(Paciente));
        if (p.id == id && !p.eliminado) {
            p.eliminado = true;
            archivo.seekp(sizeof(ArchivoHeader) + i * sizeof(Paciente));
            archivo.write(reinterpret_cast<char*>(&p), sizeof(Paciente));
            archivo.close();
            cout << "Paciente eliminado.\n";
            return true;
        }
    }
    archivo.close();
    return false;
}

