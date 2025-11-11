#include "FuncionesPrincipales.hpp"
#include "funcionesArchivos.hpp"
#include <iostream>
#include <cstring>
#include <cctype>
#include <fstream>
#include "utilidades.hpp"
#include <iomanip>
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
    ArchivoHeader header=leerHeader("pacientes.bin");
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


bool actualizarPaciente(int id) {
    fstream archivo("pacientes.bin", ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) return false;

    ArchivoHeader header;
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));

    Paciente p{};
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.seekg(sizeof(ArchivoHeader) + i * sizeof(Paciente));
        archivo.read(reinterpret_cast<char*>(&p), sizeof(Paciente));
        if (p.id == id && !p.eliminado) {
            cout << "Actualizando paciente ID: " << id << "\n";
            // pedir nuevos datos...
            strcpy(p.nombre, "NuevoNombre"); // ejemplo
            p.fechaModificacion = time(nullptr);

            archivo.seekp(sizeof(ArchivoHeader) + i * sizeof(Paciente));
            archivo.write(reinterpret_cast<char*>(&p), sizeof(Paciente));
            archivo.close();
            return true;
        }
    }
    archivo.close();
    return false;
}

bool agregarConsultaAlHistorial(int pacienteID, HistorialMedico nuevaConsulta) {
    // 1. Leer paciente
    Paciente paciente = leerRegistro<Paciente>("pacientes.bin", pacienteID - 1);
    if (paciente.id == 0 || paciente.eliminado) {
        cout << "Error: Paciente no encontrado o eliminado.\n";
        return false;
    }

    // 2. Preparar nueva consulta
    nuevaConsulta.pacienteID = pacienteID;
    nuevaConsulta.fechaRegistro = time(nullptr);
    nuevaConsulta.eliminado = false;

    // 3. Asignar ID a la nueva consulta
    ArchivoHeader headerHistorial = leerHeader("historiales.bin");
    nuevaConsulta.id = headerHistorial.cantidadRegistros + 1;

    // 4. Enlazar consulta
    if (paciente.cantidadConsultas == 0) {
        nuevaConsulta.siguienteConsultaID = -1;
        paciente.primerConsultaID = nuevaConsulta.id;
    } else {
        int consultaIDActual = paciente.primerConsultaID;
        HistorialMedico consultaActual;

        while (consultaIDActual != -1) {
            consultaActual = leerRegistro<HistorialMedico>("historiales.bin", consultaIDActual - 1);
            if (consultaActual.siguienteConsultaID == -1) {
                consultaActual.siguienteConsultaID = nuevaConsulta.id;
                escribirRegistro<HistorialMedico>("historiales.bin", consultaActual, consultaIDActual - 1);
                break;
            }
            consultaIDActual = consultaActual.siguienteConsultaID;
        }
    }

    // 5. Guardar la nueva consulta
    if (!escribirRegistro<HistorialMedico>("historiales.bin", nuevaConsulta, nuevaConsulta.id - 1)) {
        cout << "Error al escribir la nueva consulta.\n";
        return false;
    }

    // 6. Actualizar contador de consultas en el paciente
    paciente.cantidadConsultas++;
    escribirRegistro<Paciente>("pacientes.bin", paciente, pacienteID - 1);

    // 7. ACTUALIZAR HEADER del archivo de historiales
    headerHistorial.cantidadRegistros++;
    headerHistorial.registrosActivos++;
    actualizarHeader("historiales.bin", headerHistorial);

    cout << "Consulta agregada exitosamente al historial del paciente.\n";
    return true;
}



bool guardarHospital(const Hospital& hospital) {
    fstream archivo("hospital.bin", ios::binary | ios::out | ios::trunc);
    if (!archivo.is_open()) {
        cout << "Error al guardar hospital.bin\n";
        return false;
    }
    archivo.write(reinterpret_cast<const char*>(&hospital), sizeof(Hospital));
    archivo.close();
    return true;
}

HistorialMedico* leerHistorialCompleto(int pacienteID, int* cantidad) {
    // 1. Leer paciente desde pacientes.bin
    Paciente paciente = leerRegistro<Paciente>("pacientes.bin", pacienteID - 1);

    // 2. Validar que tenga historial
    if (paciente.primerConsultaID == -1 || paciente.cantidadConsultas == 0) {
        *cantidad = 0;
        return nullptr;
    }

    // 3. Crear array dinámico
    HistorialMedico* historial = new HistorialMedico[paciente.cantidadConsultas];

    // 4. Recorrer las consultas enlazadas
    int consultaID = paciente.primerConsultaID;
    int index = 0;

    while (consultaID != -1 && index < paciente.cantidadConsultas) {
        HistorialMedico consulta = leerRegistro<HistorialMedico>("historiales.bin", consultaID - 1);
        historial[index++] = consulta;
        consultaID = consulta.siguienteConsultaID;
    }

    // 5. Devolver cantidad real de consultas leídas
    *cantidad = index;

    return historial;
}
// ============================================

///===========================================
//             D O C T O R E S
// ===========================================
Doctor crearDoctor(Hospital* hospital, const char* nombre,
                   const char* apellido, const char* cedula,
                   const char* especialidad, int aniosExperiencia,
                   float costoConsulta)
{
    // Validaciones
    if (!validarNombreSinEspacios(nombre) || !validarNombreSinEspacios(apellido))
        return Doctor{};
    if (!validarCedula(cedula))
        return Doctor{};
    if (aniosExperiencia < 0) {
        cout << "Años de experiencia no puede ser negativo.\n";
        return Doctor{};
    }
    if (costoConsulta < 0) {
        cout << "Costo de consulta no puede ser negativo.\n";
        return Doctor{};
    }

    // Crear el registro directamente en la pila
    Doctor nuevoDoctor{};
    nuevoDoctor.id = hospital->totalDoctoresRegistrados + 1;
    strcpy(nuevoDoctor.nombre, nombre);
    strcpy(nuevoDoctor.apellido, apellido);
    strcpy(nuevoDoctor.cedulaProfesional, cedula);
    strcpy(nuevoDoctor.especialidad, especialidad);
    nuevoDoctor.aniosExperiencia = aniosExperiencia;
    nuevoDoctor.costoConsulta = costoConsulta;
    nuevoDoctor.disponible = true;
    nuevoDoctor.eliminado = false;
    nuevoDoctor.fechaCreacion = time(nullptr);
    nuevoDoctor.cantidadPacientes = 0;
    nuevoDoctor.cantidadCitas = 0;

    // Guardar el doctor directamente en el archivo binario
    escribirRegistro("doctores.bin", nuevoDoctor, hospital->totalDoctoresRegistrados);

    // Actualizar el hospital
    hospital->totalDoctoresRegistrados++;
    guardarHospital(*hospital);

    cout << "Doctor creado exitosamente con ID: " << nuevoDoctor.id << "\n";
    return nuevoDoctor;
}

bool asignarPacienteADoctor(Doctor* doctor, int idPaciente) {
    if (doctor == nullptr) {
        cout << "Error: doctor no existe.\n";
        return false;
    }

    // Evitar duplicados
    for (int i = 0; i < doctor->cantidadPacientes; i++) {
        if (doctor->pacientesIDs[i] == idPaciente) {
            cout << "Error: paciente ya asignado.\n";
            return false;
        }
    }


    // Asignar paciente
    doctor->pacientesIDs[doctor->cantidadPacientes] = idPaciente;
    doctor->cantidadPacientes++;
    escribirRegistro<Doctor>("doctores.bin", *doctor, doctor->id - 1);
    cout << "Paciente asignado correctamente al doctor " << doctor->nombre << " " << doctor->apellido << ".\n";
    return true;
}

// 
bool removerPacienteDeDoctor(Doctor* doctor, int idPaciente) {
    if (doctor == nullptr) {
        cout << "Error: doctor no existe.\n";
        return false;
    }

    bool encontrado = false;
    for (int i = 0; i < doctor->cantidadPacientes; i++) {
        if (doctor->pacientesIDs[i] == idPaciente) {
            // Compactar el array
            for (int j = i; j < doctor->cantidadPacientes - 1; j++) {
                doctor->pacientesIDs[j] = doctor->pacientesIDs[j + 1];
            }
            doctor->cantidadPacientes--;
            encontrado = true;
            cout << "Paciente eliminado correctamente del doctor "
                 << doctor->nombre << " " << doctor->apellido << ".\n";
            break;
        }
    }
   
    if(encontrado) {
        escribirRegistro<Doctor>("doctores.bin", *doctor, doctor->id - 1);
    }

    if (!encontrado) {
        cout << "Error: paciente no encontrado en la lista del doctor.\n";
    }

    return encontrado;
}

void listarDoctores(Hospital* hospital) {
    if (hospital->totalDoctoresRegistrados == 0) {
        cout << "No hay doctores registrados en el hospital.\n";
        return;
    }

    // Abrir archivo una sola vez
    fstream archivo("doctores.bin", ios::binary | ios::in);
    if (!archivo.is_open()) {
        cout << "Error al abrir doctores.bin\n";
        return;
    }

    // Crear un array dinámico para todos los doctores
    Doctor* doctores = new Doctor[hospital->totalDoctoresRegistrados];
    archivo.read(reinterpret_cast<char*>(doctores), sizeof(Doctor) * hospital->totalDoctoresRegistrados);
    archivo.close();

    cout << "\n=== Lista de Doctores ===\n";
    cout << left << setw(6) << "ID"
         << setw(20) << "Nombre"
         << setw(20) << "Apellido"
         << setw(15) << "Cédula"
         << setw(20) << "Especialidad"
         << setw(8) << "Exper."
         << setw(10) << "Costo"
         << setw(12) << "Disponible" << "\n";
    cout << string(111, '-') << "\n";

    for (int i = 0; i < hospital->totalDoctoresRegistrados; i++) {
        Doctor& d = doctores[i];
        cout << left << setw(6) << d.id
             << setw(20) << d.nombre
             << setw(20) << d.apellido
             << setw(15) << d.cedulaProfesional
             << setw(20) << d.especialidad
             << setw(8) << d.aniosExperiencia
             << setw(10) << fixed << setprecision(2) << d.costoConsulta
             << setw(12) << (d.disponible ? "Sí" : "No") << "\n";
    }

    cout << string(111, '-') << "\n";
    cout << "Total de doctores registrados: " << hospital->totalDoctoresRegistrados << "\n\n";

    delete[] doctores;
}
bool validarFormatoFecha(const char* fecha) {
    int anio, mes, dia;
    return (sscanf(fecha, "%4d-%2d-%2d", &anio, &mes, &dia) == 3 &&
            anio > 1900 && mes >= 1 && mes <= 12 && dia >= 1 && dia <= 31);
}