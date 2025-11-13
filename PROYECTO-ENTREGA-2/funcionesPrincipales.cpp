#include "FuncionesPrincipales.hpp"
#include "funcionesArchivos.hpp"
#include <iostream>
#include <cstring>
#include <cctype>
#include <fstream>
#include "utilidades.hpp"
#include <iomanip>
#include <unistd.h>
#include <limits>
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
bool validarFormatoHora(const char* hora) {
    int hh, mm;
    return (sscanf(hora, "%2d:%2d", &hh, &mm) == 2 && hh >= 0 && hh < 24 && mm >= 0 && mm < 60);
}

Paciente crearPaciente(Hospital* hospital, const char* nombre,
                       const char* apellido, const char* cedula, int edad, char sexo) {
    // Validaciones
    if (!validarNombreSinEspacios(nombre) || !validarNombreSinEspacios(apellido)) return {};
    if (!validarCedula(cedula)) return {};
    if (!validarEdad(edad)) return {};
    if (!validarSexoChar(sexo)) return {};

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

    // Abrir archivo pacientes.bin
    fstream archivo("pacientes.bin", ios::binary | ios::in | ios::out);
    ArchivoHeader header{};

    if (!archivo.is_open()) {
        // Crear archivo si no existe
        archivo.open("pacientes.bin", ios::binary | ios::out);
        header.cantidadRegistros = 0;
        header.proximoID = 1;
        header.registrosActivos = 0;
        header.version = 1;
        archivo.write(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
        archivo.close();
        archivo.open("pacientes.bin", ios::binary | ios::in | ios::out);
        if (!archivo.is_open()) {
            cout << "No se pudo crear/abrir el archivo de pacientes.\n";
            return {};
        }
        return p;
    }

    // Leer header actual
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));

    // Asignar ID secuencial
    p.id = header.proximoID++;

    // Escribir paciente al final
    archivo.seekp(sizeof(ArchivoHeader) + header.cantidadRegistros * sizeof(Paciente));
    archivo.write(reinterpret_cast<char*>(&p), sizeof(Paciente));

    // Actualizar header
    header.cantidadRegistros++;
    header.registrosActivos++;
    archivo.seekp(0);
    archivo.write(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    archivo.close();

    // Actualizar hospital
    hospital->totalPacientesRegistrados++;
    guardarHospital(*hospital);

    cout << "Paciente creado exitosamente con ID: " << p.id << "\n";
    return p; // Devuelve el paciente creado
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
bool eliminarDoctor(int id) {
    fstream archivo("doctores.bin", ios::binary | ios::in | ios::out);
    if (!archivo.is_open()) return false;

    ArchivoHeader header;
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));

    Doctor d{};
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.seekg(sizeof(ArchivoHeader) + i * sizeof(Doctor));
        archivo.read(reinterpret_cast<char*>(&d), sizeof(Doctor));
        if (d.id == id && !d.eliminado) {
            d.eliminado = true;
            archivo.seekp(sizeof(ArchivoHeader) + i * sizeof(Doctor));
            archivo.write(reinterpret_cast<char*>(&d), sizeof(Doctor));
            archivo.close();
            cout << "Doctor eliminado.\n";
            return true;
        }
    }
    archivo.close();
    return false;
}
bool verificarDisponibilidad(Hospital* hospital, int idDoctor, const char* fecha, const char* hora) {
    for (int i = 0; i < hospital->totalCitasAgendadas; i++) {
        Cita cita= leerRegistro<Cita>("citas.bin", i);
        if (cita.doctorID == idDoctor && strcmp(cita.estado, "Agendada") == 0) {
            if (strcmp(cita.fecha, fecha) == 0 && strcmp(cita.hora, hora) == 0) {
                return false;  
            }
        }
    }
    return true; 
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

    ArchivoHeader header{};
    header.cantidadRegistros = 1;
    header.proximoID = hospital.siguienteIDPaciente; // o el que corresponda
    header.registrosActivos = 1;
    header.version = 1;


    archivo.write(reinterpret_cast<const char*>(&header), sizeof(ArchivoHeader));

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

Doctor crearDoctor(Hospital* hospital, const char* nombre,
                   const char* apellido, const char* cedula,
                   const char* especialidad, int aniosExperiencia,
                   float costoConsulta)
{
    // Validaciones básicas
    if (!validarNombreSinEspacios(nombre) || !validarNombreSinEspacios(apellido)) return {};
    if (!validarCedula(cedula)) return {};
    if (aniosExperiencia < 0 || costoConsulta < 0) return {};

    Doctor nuevoDoctor{};
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

    // Abrir archivo doctores.bin
    fstream archivo("doctores.bin", ios::binary | ios::in | ios::out);
    ArchivoHeader header{};
    if (!archivo.is_open()) {
        // Crear archivo si no existe
        archivo.open("doctores.bin", ios::binary | ios::out);
        header.cantidadRegistros = 0;
        header.proximoID = 1;
        header.registrosActivos = 0;
        header.version = 1;
        archivo.write(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
        archivo.close();
        archivo.open("doctores.bin", ios::binary | ios::in | ios::out);
        if (!archivo.is_open()) {
            cout << "No se pudo crear/abrir el archivo de doctores.\n";
            return {};
        }
    }

    // Leer header
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));

    // Asignar ID secuencial
    nuevoDoctor.id = header.proximoID++;

    // Escribir doctor al final del archivo
    archivo.seekp(sizeof(ArchivoHeader) + header.cantidadRegistros * sizeof(Doctor));
    archivo.write(reinterpret_cast<char*>(&nuevoDoctor), sizeof(Doctor));

    // Actualizar header
    header.cantidadRegistros++;
    header.registrosActivos++;
    archivo.seekp(0);
    archivo.write(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    archivo.close();

    // Actualizar hospital
    hospital->totalDoctoresRegistrados++;
    guardarHospital(*hospital);

    cout << "Doctor creado exitosamente con ID: " << nuevoDoctor.id << "\n";
    return nuevoDoctor;
}



bool asignarPacienteADoctor(int idDoctor, int idPaciente) {
    //  Buscar doctor en archivo
    Doctor doctor = buscarRegistroPorID<Doctor>("doctores.bin", idDoctor);
    if (doctor.id == 0) {
        cout << "Error: doctor no encontrado.\n";
        return false;
    }

    //  Verificar máximo de pacientes
    const int MAX_PACIENTES = 100; // Ajusta según tu struct Doctor
    if (doctor.cantidadPacientes >= MAX_PACIENTES) {
        cout << "Error: doctor ya tiene el máximo de pacientes.\n";
        return false;
    }

    //  Evitar duplicados
    for (int i = 0; i < doctor.cantidadPacientes; i++) {
        if (doctor.pacientesIDs[i] == idPaciente) {
            cout << "Error: paciente ya asignado a este doctor.\n";
            return false;
        }
    }

    // Asignar paciente
    doctor.pacientesIDs[doctor.cantidadPacientes] = idPaciente;
    doctor.cantidadPacientes++;

    // Guardar cambios en el archivo
    ArchivoHeader header = leerHeader("doctores.bin");
    bool guardado = false;

    for (int i = 0; i < header.cantidadRegistros; i++) {
        Doctor temp = leerRegistro<Doctor>("doctores.bin", i);
        if (temp.id == idDoctor) {
            guardado = escribirRegistro<Doctor>("doctores.bin", doctor, i);
            break;
        }
    }

    if (!guardado) {
        cout << "Error: no se pudo guardar la asignación en el archivo.\n";
        return false;
    }

    cout << "Paciente asignado correctamente al doctor " 
         << doctor.nombre << " " << doctor.apellido << ".\n";
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

void listarDoctores() {
    fstream archivo("doctores.bin", ios::binary | ios::in);
    if (!archivo.is_open()) {
        cout << "Error: no se pudo abrir el archivo de doctores.\n";
        return;
    }

    ArchivoHeader header{};
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));

    if (header.cantidadRegistros == 0) {
        cout << "No hay doctores registrados.\n";
        archivo.close();
        return;
    }

    cout << "\n=== Lista de Doctores ===\n";

    for (int i = 0; i < header.cantidadRegistros; i++) {
        Doctor d{};
        archivo.seekg(sizeof(ArchivoHeader) + i * sizeof(Doctor));
        archivo.read(reinterpret_cast<char*>(&d), sizeof(Doctor));

        cout << d.id << " - " << d.nombre << " " << d.apellido 
             << " - " << d.especialidad << "\n";
    }

    archivo.close();
}

bool validarFormatoFecha(const char* fecha) {
    int anio, mes, dia;
    return (sscanf(fecha, "%4d-%2d-%2d", &anio, &mes, &dia) == 3 &&
            anio > 1900 && mes >= 1 && mes <= 12 && dia >= 1 && dia <= 31);
}

// Mostrar historial
void mostrarHistorialMedico(Paciente* paciente) {
    cout << "=== Historial Medico del Paciente ID: " << paciente->id << " ===\n";
    for (int i = 0; i < paciente->cantidadConsultas; i++) {
        HistorialMedico c = leerRegistro<HistorialMedico>("historiales.bin", paciente->citasIDs[i] - 1);
        cout << "Consulta ID: " << c.id
             << ", Fecha: " << c.fecha
             << ", Hora: " << c.hora
             << ", Diagnostico: " << c.diagnostico
             << ", Tratamiento: " << c.tratamiento
             << ", Medicamentos: " << c.medicamentos
             << ", ID Doctor: " << c.doctorID
             << ", Costo: " << c.costo << "\n";
    }
}


bool obtenerUltimaConsulta(Paciente* paciente, HistorialMedico& salida) {
    if (!paciente || paciente->cantidadConsultas == 0)
        return false;

    int indiceUltima = paciente->primerConsultaID + paciente->cantidadConsultas - 2;
    salida = leerRegistro<HistorialMedico>("historiales.bin", indiceUltima);
    return true;
}

Cita agendarCita(Hospital* hospital, int idPaciente, int idDoctor,
                 const char* fecha, const char* hora, const char* motivo) {

    // Abrir archivos en modo lectura/escritura, crear si no existen
    fstream archivoPacientes("pacientes.bin", ios::binary | ios::in | ios::out);
    fstream archivoDoctores("doctores.bin", ios::binary | ios::in | ios::out);
    fstream archivoCitas("citas.bin", ios::binary | ios::in | ios::out);

    ArchivoHeader headerPacientes{}, headerDoctores{}, headerCitas{};

    // Crear archivos si no existen
    if (!archivoPacientes.is_open()) {
        archivoPacientes.open("pacientes.bin", ios::binary | ios::out);
        headerPacientes = {0, 1, 0, 1};
        archivoPacientes.write(reinterpret_cast<char*>(&headerPacientes), sizeof(ArchivoHeader));
        archivoPacientes.close();
        archivoPacientes.open("pacientes.bin", ios::binary | ios::in | ios::out);
    }
    if (!archivoDoctores.is_open()) {
        archivoDoctores.open("doctores.bin", ios::binary | ios::out);
        headerDoctores = {0, 1, 0, 1};
        archivoDoctores.write(reinterpret_cast<char*>(&headerDoctores), sizeof(ArchivoHeader));
        archivoDoctores.close();
        archivoDoctores.open("doctores.bin", ios::binary | ios::in | ios::out);
    }
    if (!archivoCitas.is_open()) {
        archivoCitas.open("citas.bin", ios::binary | ios::out);
        headerCitas = {0, 1, 0, 1};
        archivoCitas.write(reinterpret_cast<char*>(&headerCitas), sizeof(ArchivoHeader));
        archivoCitas.close();
        archivoCitas.open("citas.bin", ios::binary | ios::in | ios::out);
    }

    // Leer headers actuales
    archivoPacientes.read(reinterpret_cast<char*>(&headerPacientes), sizeof(ArchivoHeader));
    archivoDoctores.read(reinterpret_cast<char*>(&headerDoctores), sizeof(ArchivoHeader));
    archivoCitas.read(reinterpret_cast<char*>(&headerCitas), sizeof(ArchivoHeader));

    // Validar IDs
    if (idPaciente < 1 || idPaciente > headerPacientes.cantidadRegistros) {
        cout << "Paciente no encontrado.\n";
        return {};
    }
    if (idDoctor < 1 || idDoctor > headerDoctores.cantidadRegistros) {
        cout << "Doctor no encontrado.\n";
        return {};
    }

    // Leer registros usando seek
    Paciente paciente{};
    Doctor doctor{};
    archivoPacientes.seekg(sizeof(ArchivoHeader) + (idPaciente - 1) * sizeof(Paciente));
    archivoPacientes.read(reinterpret_cast<char*>(&paciente), sizeof(Paciente));
    archivoDoctores.seekg(sizeof(ArchivoHeader) + (idDoctor - 1) * sizeof(Doctor));
    archivoDoctores.read(reinterpret_cast<char*>(&doctor), sizeof(Doctor));

    // Validar formatos
    if (!validarFormatoFecha(fecha)) { cout << "Formato de fecha inválido.\n"; return {}; }
    if (!validarFormatoHora(hora)) { cout << "Formato de hora inválido.\n"; return {}; }

    // Crear nueva cita
    Cita nuevaCita{};
    nuevaCita.id = headerCitas.proximoID++;
    nuevaCita.pacienteID = idPaciente;
    nuevaCita.doctorID = idDoctor;
    strncpy(nuevaCita.fecha, fecha, sizeof(nuevaCita.fecha)-1);
    strncpy(nuevaCita.hora, hora, sizeof(nuevaCita.hora)-1);
    strncpy(nuevaCita.motivo, motivo, sizeof(nuevaCita.motivo)-1);
    strncpy(nuevaCita.estado, "Agendada", sizeof(nuevaCita.estado)-1);
    nuevaCita.atendida = false;

    // Guardar la cita
    archivoCitas.seekp(sizeof(ArchivoHeader) + headerCitas.cantidadRegistros * sizeof(Cita));
    archivoCitas.write(reinterpret_cast<char*>(&nuevaCita), sizeof(Cita));
    headerCitas.cantidadRegistros++;
    archivoCitas.seekp(0);
    archivoCitas.write(reinterpret_cast<char*>(&headerCitas), sizeof(ArchivoHeader));

    // Actualizar paciente
    if (paciente.cantidadCitas < 20) {
        paciente.citasIDs[paciente.cantidadCitas++] = nuevaCita.id;
        archivoPacientes.seekp(sizeof(ArchivoHeader) + (idPaciente - 1) * sizeof(Paciente));
        archivoPacientes.write(reinterpret_cast<char*>(&paciente), sizeof(Paciente));
    }

    // Actualizar doctor
    if (doctor.cantidadCitas < 20) {
        doctor.citasIDs[doctor.cantidadCitas++] = nuevaCita.id;
        archivoDoctores.seekp(sizeof(ArchivoHeader) + (idDoctor - 1) * sizeof(Doctor));
        archivoDoctores.write(reinterpret_cast<char*>(&doctor), sizeof(Doctor));
    }

    // Actualizar hospital
    hospital->totalCitasAgendadas++;
    hospital->siguienteIDCita = nuevaCita.id + 1;
    guardarHospital(*hospital);

    // Cerrar archivos
    archivoPacientes.close();
    archivoDoctores.close();
    archivoCitas.close();

    cout << "Cita agendada exitosamente con ID: " << nuevaCita.id
         << " entre Dr. " << doctor.nombre
         << " y paciente " << paciente.nombre << ".\n";

    cin.ignore(numeric_limits<streamsize>::max(), '\n');
    cout << "Presione Enter para continuar...";
    cin.get();

    return nuevaCita;
}


void listarPacientesDeDoctor(int idDoctor) {
    // Leer el doctor directamente del archivo
    Doctor doctor = buscarRegistroPorID<Doctor>("doctores.bin", idDoctor);

    if (doctor.id == 0) {
        cout << "Error: doctor no encontrado.\n";
        return;
    }

    if (doctor.cantidadPacientes == 0) {
        cout << "El doctor no tiene pacientes asignados.\n";
        return;
    }

    cout << "\n=== Pacientes asignados al Doctor: "
         << doctor.nombre << " " << doctor.apellido << " ===\n";
    cout << left << setw(6) << "ID"
         << setw(20) << "Nombre"
         << setw(20) << "Apellido"
         << setw(8) << "Edad"
         << setw(15) << "Cedula" << "\n";
    cout << string(70, '-') << "\n";

    for (int i = 0; i < doctor.cantidadPacientes; i++) {
        int idPaciente = doctor.pacientesIDs[i];

        if (idPaciente != 0) {
            // Leer paciente
            Paciente paciente = buscarRegistroPorID<Paciente>("pacientes.bin", idPaciente);

            if (paciente.id != 0) {
                cout << left << setw(6) << paciente.id
                     << setw(20) << paciente.nombre
                     << setw(20) << paciente.apellido
                     << setw(8) << paciente.edad
                     << setw(15) << paciente.cedula << "\n";
            } else {
                cout << left << setw(6) << idPaciente
                     << setw(20) << "(no encontrado)" << "\n";
            }
        }
    }

    cout << string(70, '-') << "\n";
    cout << "Total de pacientes asignados: " << doctor.cantidadPacientes << "\n\n";
}

bool atenderCita(Hospital* hospital, int idCita, const char* diagnostico,
                 const char* tratamiento, const char* medicamentos) {
    
    // Buscar la cita
    Cita cita = buscarRegistroPorID<Cita>("citas.bin", idCita);
    if (cita.id == 0) {
        cout << "Cita no encontrada.\n";
        return false;
    }
    if (strcmp(cita.estado, "Agendada") != 0) {
        cout << "Error: la cita no está agendada.\n";
        return false;
    }

    // Marcar cita como atendida
    strcpy(cita.estado, "Atendida");

    // Buscar paciente y doctor
    Paciente paciente = buscarRegistroPorID<Paciente>("pacientes.bin", cita.pacienteID);
    Doctor doctor = buscarRegistroPorID<Doctor>("doctores.bin", cita.doctorID);

    if (paciente.id == 0 || doctor.id == 0) {
        cout << "Error: paciente o doctor no encontrados.\n";
        return false;
    }

    // Crear nueva consulta (historial médico)
    HistorialMedico nuevaConsulta;
    nuevaConsulta.id = hospital->siguienteIDConsulta++;
    strcpy(nuevaConsulta.fecha, cita.fecha);
    strcpy(nuevaConsulta.hora, cita.hora);
    strcpy(nuevaConsulta.diagnostico, diagnostico);
    strcpy(nuevaConsulta.tratamiento, tratamiento);
    strcpy(nuevaConsulta.medicamentos, medicamentos);
    nuevaConsulta.doctorID = doctor.id;
    nuevaConsulta.costo = doctor.costoConsulta;

    // Guardar consulta en el archivo de historiales
    escribirRegistro<HistorialMedico>("historiales.bin", nuevaConsulta, nuevaConsulta.id - 1);

    // Asociar la consulta al paciente
    if (paciente.cantidadConsultas < 100) {  // por ejemplo, máximo 100 consultas
        if (paciente.cantidadConsultas == 0)
            paciente.primerConsultaID = nuevaConsulta.id;
        paciente.cantidadConsultas++;
        escribirRegistro<Paciente>("pacientes.bin", paciente, paciente.id - 1);
    } else {
        cout << "Advertencia: paciente ha alcanzado el máximo de consultas.\n";
    }

    // Guardar los cambios de la cita y el hospital
    escribirRegistro<Cita>("citas.bin", cita, cita.id - 1);
    escribirRegistro<Hospital>("hospital.bin", *hospital, 0);

    cout << " Cita atendida correctamente y registrada en el historial médico.\n";
    return true;
}

Cita* leerCitasDePaciente(int pacienteID, int* cantidad) {
    *cantidad = 0; // Inicializar por seguridad
    // 1. Leer paciente desde el archivo
    Paciente paciente = leerRegistro<Paciente>("pacientes.bin", pacienteID - 1);
    // Validar que el paciente exista
    if (paciente.id == 0) {
        cout << "Error: paciente no encontrado.\n";
        return nullptr;
    }
    // 2. Obtener la cantidad de citas y sus IDs
    int totalCitas = paciente.cantidadCitas;
    if (totalCitas == 0) {
        cout << "El paciente no tiene citas registradas.\n";
        return nullptr;
    }
    int* citasIDs = paciente.citasIDs;
    // 3. Crear un array dinámico para almacenar las citas válidas
    Cita* citasArray = new Cita[totalCitas];
    int contador = 0;
    // 4. Leer cada cita desde el archivo
    for (int i = 0; i < totalCitas; i++) {
        int idCita = citasIDs[i];
        if (idCita == 0) continue; // ID vacío

        Cita cita = leerRegistro<Cita>("citas.bin", idCita - 1);

        // Si la cita existe y no está eliminada
        if (cita.id != 0 && !cita.eliminado) {
            citasArray[contador++] = cita;
        }
    }
    // 5. Actualizar la cantidad real de citas válidas
    *cantidad = contador;
    // Si no se encontró ninguna válida, liberar memoria
    if (contador == 0) {
        delete[] citasArray;
        return nullptr;
    }
    return citasArray;
}
Cita* leerCitasDoctor(int doctorID, int* cantidad) {
    *cantidad = 0; // Inicializar por seguridad
    // 1. Leer paciente desde el archivo
    Doctor d = leerRegistro<Doctor>("doctores.bin", doctorID - 1);
    // Validar que el paciente exista
    if (d.id == 0) {
        cout << "Error: Doctor no encontrado.\n";
        return nullptr;
    }
    // 2. Obtener la cantidad de citas y sus IDs
    int totalCitas = d.cantidadCitas;
    if (totalCitas == 0) {
        cout << "El Doctor no tiene citas registradas.\n";
        return nullptr;
    }
    int* citasIDs = d.citasIDs;
    // 3. Crear un array dinámico para almacenar las citas válidas
    Cita* citasArray = new Cita[totalCitas];
    int contador = 0;
    // 4. Leer cada cita desde el archivo
    for (int i = 0; i < totalCitas; i++) {
        int idCita = citasIDs[i];
        if (idCita == 0) continue; // ID vacío

        Cita cita = leerRegistro<Cita>("citas.bin", idCita - 1);

        // Si la cita existe y no está eliminada
        if (cita.id != 0 && !cita.eliminado) {
            citasArray[contador++] = cita;
        }
    }
    // 5. Actualizar la cantidad real de citas válidas
    *cantidad = contador;
    // Si no se encontró ninguna válida, liberar memoria
    if (contador == 0) {
        delete[] citasArray;
        return nullptr;
    }
    return citasArray;
}
Hospital* cargarDatosHospital() {
    // 1. Verificar existencia de todos los archivos .bin
    if (!verificarArchivo("hospital.bin")) {
        inicializarArchivo("hospital.bin");
    }
    if (!verificarArchivo("pacientes.bin")) {
        inicializarArchivo("pacientes.bin");
    }
    if (!verificarArchivo("doctores.bin")) {
        inicializarArchivo("doctores.bin");
    }
    if (!verificarArchivo("citas.bin")) {
        inicializarArchivo("citas.bin");
    }
    if (!verificarArchivo("historiales.bin")) {
        inicializarArchivo("historiales.bin");
    }

    fstream archivo("hospital.bin", ios::binary | ios::in);
    if (!archivo.is_open()) {
        cout << "Error al abrir hospital.bin\n";
        return nullptr;
    }

    archivo.seekg(sizeof(ArchivoHeader), ios::beg);

    Hospital* hospital = new Hospital();
    archivo.read(reinterpret_cast<char*>(hospital), sizeof(Hospital));

    archivo.close();

    if (!archivo.good()) {
        cout << "Error al leer hospital.bin\n";
        delete hospital;
        return nullptr;
    }

    cout << "Datos del hospital cargados correctamente: " 
         << hospital->nombre << "\n";

    return hospital;
}
bool cancelarCita(Hospital* hospital, int idCita) {
    // 1. Leer la cita desde el archivo
    Cita cita = buscarRegistroPorID<Cita>("citas.bin", idCita);

    if (cita.id == 0) {
        cout << "Error: cita no encontrada.\n";
        return false;
    }

    // 2. Verificar que esté agendada
    if (strcmp(cita.estado, "Agendada") != 0) {
        cout << "Error: solo se pueden cancelar citas agendadas.\n";
        return false;
    }

    // 3. Marcar la cita como cancelada
    strcpy(cita.estado, "Cancelada");
    cita.atendida = false;

    // 4. Guardar la cita modificada en el archivo binario
    escribirRegistro<Cita>("citas.bin", cita, cita.id - 1);

    // 5. Quitar la referencia en el paciente
    Paciente paciente = buscarRegistroPorID<Paciente>("pacientes.bin", cita.pacienteID);
    if (paciente.id != 0) {
        for (int j = 0; j < paciente.cantidadCitas; j++) {
            if (paciente.citasIDs[j] == idCita) {
                // Desplazar elementos hacia la izquierda
                for (int k = j; k < paciente.cantidadCitas - 1; k++) {
                    paciente.citasIDs[k] = paciente.citasIDs[k + 1];
                }
                paciente.cantidadCitas--;
                break;
            }
        }
        escribirRegistro<Paciente>("pacientes.bin", paciente, paciente.id - 1);
    }

    // 6. Quitar la referencia en el doctor
    Doctor doctor = buscarRegistroPorID<Doctor>("doctores.bin", cita.doctorID);
    if (doctor.id != 0) {
        for (int i = 0; i < doctor.cantidadCitas;i ++) {
            if (doctor.citasIDs[i] == idCita) {
                for (int j = i; j < doctor.cantidadCitas - 1; j++) {
                    doctor.citasIDs[j] = doctor.citasIDs[j + 1];
                }
                doctor.cantidadCitas--;
                break;
            }
        }
        escribirRegistro<Doctor>("doctores.bin", doctor, doctor.id - 1);
    }

    // 7. Actualizar estadísticas del hospital
    hospital->totalCitasAgendadas--;
    guardarHospital(*hospital);

    cout << " Cita con ID " << idCita << " cancelada correctamente.\n";
    return true;
}
void obtenerCitasDeDoctor(int idDoctor) {
    Doctor doctor = buscarRegistroPorID<Doctor>("doctores.bin", idDoctor);
    if (doctor.id == 0) {
        cout << "Error: Doctor no encontrado.\n";
        return;
    }

    if (doctor.cantidadCitas == 0) {
        cout << "El doctor no tiene citas registradas.\n";
        return;
    }

    cout << "\n=== Citas del Doctor " << doctor.nombre << " ===\n";
    for (int i = 0; i < doctor.cantidadCitas; i++) {
        int idCita = doctor.citasIDs[i];
        Cita cita = buscarRegistroPorID<Cita>("citas.bin", idCita);

        if (cita.id != 0) {
            cout << "ID: " << cita.id
                 << " | Fecha: " << cita.fecha
                 << " | Hora: " << cita.hora
                 << " | Paciente ID: " << cita.pacienteID
                 << " | Estado: " << cita.estado << "\n";
        }
    }
}
void obtenerCitasPorFecha(const char* fechaBuscada) {
    ArchivoHeader header = leerHeader("citas.bin");
    if (header.cantidadRegistros == 0) {
        cout << "No hay citas registradas.\n";
        return;
    }

    fstream archivo("citas.bin", ios::binary | ios::in);
    if (!archivo.is_open()) {
        cout << "Error al abrir citas.bin\n";
        return;
    }

    cout << "\n=== Citas en la fecha " << fechaBuscada << " ===\n";
    int contador = 0;

    Cita cita;
    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.read(reinterpret_cast<char*>(&cita), sizeof(Cita));
        if (!archivo) break;

        if (strcmp(cita.fecha, fechaBuscada) == 0) {
            cout << "ID: " << cita.id
                 << " | Doctor ID: " << cita.doctorID
                 << " | Paciente ID: " << cita.pacienteID
                 << " | Hora: " << cita.hora
                 << " | Estado: " << cita.estado << "\n";
            contador++;
        }
    }
    archivo.close();

    if (contador == 0)
        cout << "No se encontraron citas en esa fecha.\n";
}
void listarCitasPendientes() {
    ArchivoHeader header = leerHeader("citas.bin");
    if (header.cantidadRegistros == 0) {
        cout << "No hay citas registradas.\n";
        return;
    }

    fstream archivo("citas.bin", ios::binary | ios::in);
    if (!archivo.is_open()) {
        cout << "Error al abrir citas.bin\n";
        return;
    }

    cout << "\n=== Citas Pendientes ===\n";
    cout << left << setw(6) << "ID"
         << setw(12) << "Fecha"
         << setw(8) << "Hora"
         << setw(10) << "ID_Doc"
         << setw(12) << "ID_Pac"
         << setw(25) << "Motivo" << "\n";
    cout << string(75, '-') << "\n";

    Cita cita;
    int contador = 0;

    archivo.seekg(sizeof(ArchivoHeader), ios::beg);
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.read(reinterpret_cast<char*>(&cita), sizeof(Cita));
        if (!archivo) break;

        if (strcmp(cita.estado, "Agendada") == 0) {
            cout << left << setw(6) << cita.id
                 << setw(12) << cita.fecha
                 << setw(8) << cita.hora
                 << setw(10) << cita.doctorID
                 << setw(12) << cita.pacienteID
                 << setw(25) << cita.motivo << "\n";
            contador++;
        }
    }

    archivo.close();

    if (contador == 0)
        cout << "No hay citas pendientes.\n";
    else
        cout << string(75, '-') << "\nTotal de citas pendientes: " << contador << "\n";
}


Paciente buscarPacientePorCedula(const char* nombreArchivo, const char* cedulaBuscada) {
    Paciente resultado{};  // Devuelve un registro vacío si no se encuentra
    if (cedulaBuscada == nullptr) {
        cout << "Error: cedula invalida.\n";
        return resultado;
    }

    ArchivoHeader header = leerHeader(nombreArchivo);
    for (int i = 0; i < header.cantidadRegistros; ++i) {
        Paciente p = leerRegistro<Paciente>(nombreArchivo, i);

        // Solo considerar registros no eliminados
        if (!p.eliminado && compararCaseInsensitive(p.cedula, cedulaBuscada)) {
            cout << "Paciente encontrado: " 
                      << p.nombre << " " << p.apellido
                      << " | Cedula: " << p.cedula << std::endl;
            return p;
        }
    }

    cout << "Paciente no encontrado.\n";
    return resultado;
}
void buscarPacientesPorNombre(const char* nombreBuscado) {
    ArchivoHeader header = leerHeader("pacientes.bin");
    int coincidencias = 0;

    for (int i = 0; i < header.cantidadRegistros; ++i) {
        Paciente p = leerRegistro<Paciente>("pacientes.bin", i);
        if (!p.eliminado && compararCaseInsensitive(p.nombre, nombreBuscado)) {
            cout << "Paciente encontrado: " 
                 << p.nombre << " " << p.apellido 
                 << " | Cedula: " << p.cedula << endl;
            coincidencias++;
        }
    }

    cout << "Coincidencias encontradas: " << coincidencias << endl;
}

void buscarDoctoresPorEspecialidad(const char* nombreBuscado) {
    ArchivoHeader header = leerHeader("doctores.bin");
    int coincidencias = 0;

    for (int i = 0; i < header.cantidadRegistros; ++i) {
        Doctor d = leerRegistro<Doctor>("doctores.bin", i);
        if (!d.eliminado && compararCaseInsensitive(d.nombre, nombreBuscado)) {
            cout << "Paciente encontrado: " 
                 << d.nombre << " " << d.apellido 
                 << " | Especialidad: " << d.especialidad << endl;
            coincidencias++;
        }
    }

    cout << "Coincidencias encontradas: " << coincidencias << endl;
}
bool verificarYRepararIntegridadReferencial() {
    bool integridadOk = true;

    // --- 1. Revisar citas ---
    ArchivoHeader headerCitas = leerHeader("citas.bin");
    for (int i = 0; i < headerCitas.cantidadRegistros; i++) {
        Cita c = leerRegistro<Cita>("citas.bin", i);
        if (c.eliminado || c.id == 0) continue;

        bool citaValida = true;

        // Verificar paciente
        Paciente p = leerRegistro<Paciente>("pacientes.bin", c.pacienteID - 1);
        if (p.id == 0 || p.eliminado) {
            cout << "Cita ID " << c.id << " tiene pacienteID inválido: " << c.pacienteID << "\n";
            citaValida = false;
            integridadOk = false;
        }

        // Verificar doctor
        Doctor d = leerRegistro<Doctor>("doctores.bin", c.pacienteID - 1);
        if (d.id == 0 || d.eliminado) {
            cout << "Cita ID " << c.id << " tiene doctorID inválido: " << c.pacienteID << "\n";
            citaValida = false;
            integridadOk = false;
        }

        // Reparar: marcar como eliminada si no es válida
        if (!citaValida) {
            c.eliminado = true;
            escribirRegistro<Cita>("citas.bin", c, i);
            cout << "Cita ID " << c.id << " eliminada automáticamente.\n";
        }
    }

    // --- 2. Revisar historiales ---
    ArchivoHeader headerHist = leerHeader("historiales.bin");
    for (int i = 0; i < headerHist.cantidadRegistros; i++) {
        HistorialMedico h = leerRegistro<HistorialMedico>("historiales.bin", i);
        if (h.eliminado || h.id == 0) continue;

        Paciente p = leerRegistro<Paciente>("pacientes.bin", h.pacienteID - 1);
        if (p.id == 0 || p.eliminado) {
            cout << "Historial ID " << h.id << " tiene pacienteID inválido: " << h.pacienteID << "\n";
            h.eliminado = true;
            escribirRegistro<HistorialMedico>("historiales.bin", h, i);
            cout << "Historial ID " << h.id << " eliminado automáticamente.\n";
            integridadOk = false;
        }
    }

    // --- 3. Revisar pacientes y sus citas ---
    ArchivoHeader headerPac = leerHeader("pacientes.bin");
    for (int i = 0; i < headerPac.cantidadRegistros; i++) {
        Paciente pac = leerRegistro<Paciente>("pacientes.bin", i);
        if (pac.eliminado || pac.id == 0) continue;

        bool cambios = false;
        for (int j = 0; j < pac.cantidadCitas; j++) {
            int idCita = pac.citasIDs[j];
            if (idCita == 0) continue;

            Cita c = leerRegistro<Cita>("citas.bin", idCita - 1);
            if (c.id == 0 || c.eliminado) {
                cout << "Paciente ID " << pac.id << " tiene cita inválida ID " << idCita << "\n";
                pac.citasIDs[j] = 0; // Reparar
                cambios = true;
                integridadOk = false;
            }
        }

        if (cambios) {
            escribirRegistro<Paciente>("pacientes.bin", pac, i);
            cout << "Paciente ID " << pac.id << " actualizado.\n";
        }
    }

    if (integridadOk)
        cout << "\nIntegridad referencial OK. No se encontraron errores.\n";
    else
        cout << "\nSe repararon automáticamente las referencias rotas.\n";

    return integridadOk;
}
Cita* obtenerCitasPorFecha(const char* fechaBuscada, int* cantidad) {
    *cantidad = 0; // Inicializar cantidad

    ArchivoHeader header = leerHeader("citas.bin");
    if (header.cantidadRegistros == 0) {
        cout << "No hay citas registradas.\n";
        return nullptr;
    }

    // Array temporal para almacenar todas las coincidencias (máx: cantidad de registros)
    Cita* temp = new Cita[header.cantidadRegistros];
    int contador = 0;

    for (int i = 0; i < header.cantidadRegistros; i++) {
        Cita c = leerRegistro<Cita>("citas.bin", i);
        if (c.eliminado || c.id == 0) continue;

        // Comparar la fecha (case-sensitive, puedes hacer case-insensitive si quieres)
        if (strcmp(c.fecha, fechaBuscada) == 0) {
            temp[contador++] = c;
        }
    }

    // Ajustar array final
    if (contador == 0) {
        delete[] temp;
        return nullptr;
    }

    Cita* resultado = new Cita[contador];
    for (int i = 0; i < contador; i++) resultado[i] = temp[i];
    delete[] temp;

    *cantidad = contador;
    return resultado;
}
Hospital* RegistrarHospital() {
    Hospital* hospital = new Hospital;

    // Pedir datos
    cout << "=== Registro del Hospital ===\n";
    cout << "Nombre: ";
    cin.getline(hospital->nombre, 100);
    cout << "Direccion: ";
    cin.getline(hospital->direccion, 150);
    cout << "Telefono: ";
    cin.getline(hospital->telefono, 15);

    // Inicializar contadores e IDs
    hospital->siguienteIDPaciente = 1;
    hospital->siguienteIDDoctor = 1;
    hospital->siguienteIDCita = 1;
    hospital->siguienteIDConsulta = 1;

    hospital->totalPacientesRegistrados = 0;
    hospital->totalDoctoresRegistrados = 0;
    hospital->totalCitasAgendadas = 0;
    hospital->totalConsultasRealizadas = 0;

    // Guardar inmediatamente en disco
    fstream archivo("hospital.bin", ios::binary | ios::out | ios::trunc);
    if (!archivo.is_open()) {
        cout << "Error: no se pudo crear hospital.bin\n";
        delete hospital;
        return nullptr;
    }

    archivo.write(reinterpret_cast<const char*>(hospital), sizeof(Hospital));
    archivo.close();

    cout << "\nHospital registrado y guardado correctamente.\n\n";
    return hospital;
}
//g++ -Wall -Wextra -g3 `
 //   ..\hospital_v2.cpp `
 //   ..\funcionesPrincipales.cpp `
 //   ..\funcionesArchivos.cpp `
//    -o hospital_v2.exe
