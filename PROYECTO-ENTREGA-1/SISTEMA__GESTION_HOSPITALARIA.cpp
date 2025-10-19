#include <iostream>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <cctype> // para tolower

using namespace std;

// ============================================
// ESTRUCTURAS
// ============================================

struct HistorialMedico {
    int idConsulta;
    char fecha[11];
    char hora[6];
    char diagnostico[200];
    char tratamiento[200];
    char medicamentos[150];
    int idDoctor;
    float costo;
};

struct Paciente {
    int id;
    char nombre[50];
    char apellido[50];
    char cedula[20];
    int edad;
    char sexo;
    char tipoSangre[5];
    char telefono[15];
    char direccion[100];
    char email[50];
    
    HistorialMedico* historial;
    int cantidadConsultas;
    int capacidadHistorial;
    
    int* citasAgendadas;
    int cantidadCitas;
    int capacidadCitas;
    
    char alergias[500];
    char observaciones[500];
    
    bool activo;
};

struct Doctor {
    int id;
    char nombre[50];
    char apellido[50];
    char cedula[20];
    char especialidad[50];
    int aniosExperiencia;
    float costoConsulta;
    char horarioAtencion[50];
    char telefono[15];
    char email[50];
    
    int* pacientesAsignados;
    int cantidadPacientes;
    int capacidadPacientes;
    
    int* citasAgendadas;
    int cantidadCitas;
    int capacidadCitas;
    
    bool disponible;
};

struct Cita {
    int id;
    int idPaciente;
    int idDoctor;
    char fecha[11];
    char hora[6];
    char motivo[150];
    char estado[20];
    char observaciones[200];
    bool atendida;
};

struct Hospital {
    char nombre[100];
    char direccion[150];
    char telefono[15];
    
    Paciente* pacientes;
    int cantidadPacientes;
    int capacidadPacientes;
    
    Doctor* doctores;
    int cantidadDoctores;
    int capacidadDoctores;
    
    Cita* citas;
    int cantidadCitas;
    int capacidadCitas;
    
    int siguienteIdPaciente;
    int siguienteIdDoctor;
    int siguienteIdCita;
    int siguienteIdConsulta;
}; 
// Redimensionar array de enteros (IDs de pacientes o citas)
int* redimensionarArrayInt(int* array, int& capacidadActual) {
    int nuevaCapacidad = capacidadActual * 2;
    int* nuevoArray = new int[nuevaCapacidad];
    for (int i = 0; i < capacidadActual; i++) {
        nuevoArray[i] = array[i];
    }
    delete[] array;
    capacidadActual = nuevaCapacidad;
    return nuevoArray;
}

//  Redimensionar array de HistorialMedico
HistorialMedico* redimensionarArrayHistorial(HistorialMedico* array, int& capacidadActual) {
    int nuevaCapacidad = capacidadActual * 2;
    HistorialMedico* nuevoArray = new HistorialMedico[nuevaCapacidad];
    for (int i = 0; i < capacidadActual; i++) {
        nuevoArray[i] = array[i];
    }
    delete[] array;
    capacidadActual = nuevaCapacidad;
    return nuevoArray;
}


// ============================================
// FUNCIONES DE HOSPITAL
// ============================================

// Requisito: Inicializar hospital con datos y arrays dinámicos
void RegistrarHospital(Hospital* hospital) {
    cout << "=== Registro del Hospital ===\n";
    cout << "Ingrese el nombre del hospital: ";
    cin.getline(hospital->nombre, 100);
    cout << "Ingrese la direccion del hospital: ";
    cin.getline(hospital->direccion, 150);
    cout << "Ingrese el telefono del hospital: ";
    cin.getline(hospital->telefono, 15);

    hospital->cantidadPacientes = 0;
    hospital->capacidadPacientes = 10;
    hospital->pacientes = new Paciente[hospital->capacidadPacientes];

    hospital->cantidadDoctores = 0;
    hospital->capacidadDoctores = 10;
    hospital->doctores = new Doctor[hospital->capacidadDoctores];

    hospital->cantidadCitas = 0;
    hospital->capacidadCitas = 20;
    hospital->citas = new Cita[hospital->capacidadCitas];

    hospital->siguienteIdPaciente = 1;
    hospital->siguienteIdDoctor = 1;
    hospital->siguienteIdCita = 1;
    hospital->siguienteIdConsulta = 1;

    cout << "\n Hospital registrado exitosamente.\n\n";
}

// ============================================
// FUNCIONES PACIENTES
// ============================================

// Requisito: crear paciente, verificar cedula, redimensionar arrays dinámicos
Paciente* crearPaciente(Hospital* hospital, const char* nombre, 
                        const char* apellido, const char* cedula, int edad, char sexo) {
    // Verificar que la cedula no exista previamente
    for (int i = 0; i < hospital->cantidadPacientes; i++) {
        if (strcmp(hospital->pacientes[i].cedula, cedula) == 0) {
            cout << "Error: La cédula ya existe en el sistema.\n";
            return nullptr;
        }
    }

    // Redimensionar arreglo si está lleno
    if (hospital->cantidadPacientes >= hospital->capacidadPacientes) {
        int nuevaCapacidad = hospital->capacidadPacientes * 2;
        Paciente* nuevoArray = new Paciente[nuevaCapacidad];
        for (int i = 0; i < hospital->cantidadPacientes; i++) {
            nuevoArray[i] = hospital->pacientes[i];
        }
        delete[] hospital->pacientes;
        hospital->pacientes = nuevoArray;
        hospital->capacidadPacientes = nuevaCapacidad;
    }

    Paciente* nuevoPaciente = &hospital->pacientes[hospital->cantidadPacientes];

    // Asignar datos personales
    nuevoPaciente->id = hospital->siguienteIdPaciente++;
    strcpy(nuevoPaciente->nombre, nombre);
    strcpy(nuevoPaciente->apellido, apellido);
    strcpy(nuevoPaciente->cedula, cedula);
    nuevoPaciente->edad = edad;
    nuevoPaciente->sexo = sexo;
    nuevoPaciente->activo = true;

    // Inicializar arrays dinámicos
    nuevoPaciente->capacidadHistorial = 5;
    nuevoPaciente->cantidadConsultas = 0;
    nuevoPaciente->historial = new HistorialMedico[nuevoPaciente->capacidadHistorial];

    nuevoPaciente->capacidadCitas = 5;
    nuevoPaciente->cantidadCitas = 0;
    nuevoPaciente->citasAgendadas = new int[nuevoPaciente->capacidadCitas];

    strcpy(nuevoPaciente->alergias, "");
    strcpy(nuevoPaciente->observaciones, "");

    hospital->cantidadPacientes++;

    cout << "Paciente creado exitosamente con ID: " << nuevoPaciente->id << "\n";
    return nuevoPaciente;
}

// Comparación case-insensitive
bool compararCaseInsensitive(const char* a, const char* b) {
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b))
            return false;
        ++a;
        ++b;
    }
    return *a == *b;
}

// Buscar paciente por cédula (case-insensitive)
Paciente* buscarPacientePorCedula(Hospital* hospital, const char* cedula) {
    for (int i = 0; i < hospital->cantidadPacientes; i++) {
        if (compararCaseInsensitive(hospital->pacientes[i].cedula, cedula)) {
            return &hospital->pacientes[i]; 
        }
    }
    cout << "Paciente no encontrado.\n";
    return nullptr;
}

// Buscar paciente por ID
Paciente* buscarPacientePorId(Hospital* hospital, int id) {
    for (int i = 0; i < hospital->cantidadPacientes; i++) {
        if (hospital->pacientes[i].id == id) {
            return &hospital->pacientes[i];
        }
    }
    cout << "Paciente no encontrado.\n";
    return nullptr;
}

// Buscar pacientes por nombre (parcial, case-insensitive)
Paciente** buscarPacientesPorNombre(Hospital* hospital, const char* nombre, int* cantidad) {
    Paciente** resultados = new Paciente*[hospital->cantidadPacientes];
    *cantidad = 0;

    for (int i = 0; i < hospital->cantidadPacientes; i++) {
        if (compararCaseInsensitive(hospital->pacientes[i].nombre, nombre)) {
            resultados[*cantidad] = &hospital->pacientes[i];
            (*cantidad)++;
        }
    }

    if (*cantidad == 0) {
        delete[] resultados;
        return nullptr;
    }

    return resultados;
}

// Actualizar datos del paciente
bool actualizarPaciente(Hospital* hospital, int id) {
    Paciente* paciente = buscarPacientePorId(hospital, id);
    if (!paciente) return false;

    cout << "Actualizando datos del paciente ID: " << id << "\n";

    // Nombre
    cout << "Nombre actual: " << paciente->nombre << "\n";
    cout << "Ingrese nuevo nombre (Enter para mantener): ";
    char nuevoNombre[50];
    cin.ignore();
    cin.getline(nuevoNombre, 50);
    if (strlen(nuevoNombre) > 0) strcpy(paciente->nombre, nuevoNombre);

    // Apellido
    cout << "Apellido actual: " << paciente->apellido << "\n";
    cout << "Ingrese nuevo apellido (Enter para mantener): ";
    char nuevoApellido[50];
    cin.getline(nuevoApellido, 50);
    if (strlen(nuevoApellido) > 0) strcpy(paciente->apellido, nuevoApellido);

    // Edad
    cout << "Edad actual: " << paciente->edad << "\n";
    cout << "Ingrese nueva edad (Enter para mantener): ";
    char nuevaEdadStr[10];
    cin.getline(nuevaEdadStr, 10);
    if (strlen(nuevaEdadStr) > 0) {
        int nuevaEdad = atoi(nuevaEdadStr);
        if (nuevaEdad >= 0 && nuevaEdad <= 120) paciente->edad = nuevaEdad;
    }

    // Sexo
    cout << "Sexo actual: " << paciente->sexo << "\n";
    cout << "Ingrese nuevo sexo (M/F, Enter para mantener): ";
    char nuevoSexoStr[10];
    cin.getline(nuevoSexoStr, 10);
    if (strlen(nuevoSexoStr) > 0) {
        char nuevoSexo = toupper(nuevoSexoStr[0]);
        if (nuevoSexo == 'M' || nuevoSexo == 'F') paciente->sexo = nuevoSexo;
    }

    cout << "Datos actualizados exitosamente.\n";
    return true;
}

// Eliminar paciente (libera memoria de arrays dinámicos)
bool eliminarPaciente(Hospital* hospital, int id) {
    for (int i = 0; i < hospital->cantidadPacientes; i++) {
        if (hospital->pacientes[i].id == id) {
            Paciente& p = hospital->pacientes[i];

            delete[] p.historial;
            delete[] p.citasAgendadas;

            // Compactar array
            hospital->pacientes[i] = hospital->pacientes[hospital->cantidadPacientes - 1];
            hospital->cantidadPacientes--;

            cout << "Paciente eliminado exitosamente.\n";
            return true;
        }
    }
    cout << "Paciente no encontrado.\n";
    return false;
}

// Listar pacientes
void listarPacientes(Hospital* hospital) {
    cout << "=== Lista de Pacientes ===\n";
    for (int i = 0; i < hospital->cantidadPacientes; i++) {
        Paciente& p = hospital->pacientes[i];
        cout << "ID:" << p.id << ", Nombre:" << p.nombre
             << ", Apellido:" << p.apellido
             << ", Cedula:" << p.cedula
             << ", Edad:" << p.edad
             << ", Sexo:" << p.sexo << "\n";
    }
}

// Agregar consulta al historial
void agregarConsultaAlHistorial(Paciente* paciente, HistorialMedico consulta) {
    if (paciente->cantidadConsultas >= paciente->capacidadHistorial) {
        int nuevaCapacidad = paciente->capacidadHistorial * 2;
        HistorialMedico* nuevoArray = new HistorialMedico[nuevaCapacidad];
        for (int i = 0; i < paciente->cantidadConsultas; i++) {
            nuevoArray[i] = paciente->historial[i];
        }
        delete[] paciente->historial;
        paciente->historial = nuevoArray;
        paciente->capacidadHistorial = nuevaCapacidad;
    }

    paciente->historial[paciente->cantidadConsultas] = consulta;
    paciente->cantidadConsultas++;
    cout << "Consulta agregada exitosamente al historial del paciente ID: "
         << paciente->id << "\n";
}

// Obtener historial completo
HistorialMedico* obtenerHistorialCompleto(Paciente* paciente, int* cantidad) {
    if (!paciente || paciente->cantidadConsultas == 0) {
        *cantidad = 0;
        return nullptr;
    }
    *cantidad = paciente->cantidadConsultas;
    return paciente->historial;
}

// Mostrar historial
void mostrarHistorialMedico(Paciente* paciente) {
    cout << "=== Historial Medico del Paciente ID: " << paciente->id << " ===\n";
    for (int i = 0; i < paciente->cantidadConsultas; i++) {
        HistorialMedico& c = paciente->historial[i];
        cout << "Consulta ID: " << c.idConsulta
             << ", Fecha: " << c.fecha
             << ", Hora: " << c.hora
             << ", Diagnostico: " << c.diagnostico
             << ", Tratamiento: " << c.tratamiento
             << ", Medicamentos: " << c.medicamentos
             << ", ID Doctor: " << c.idDoctor
             << ", Costo: " << c.costo << "\n";
    }
}

// Última consulta
HistorialMedico* obtenerUltimaConsulta(Paciente* paciente) {
    if (!paciente || paciente->cantidadConsultas == 0) return nullptr;
    return &paciente->historial[paciente->cantidadConsultas - 1];
}

// ============================================
// FUNCIONES DOCTORES
// ============================================

// Crear doctor
Doctor* crearDoctor(Hospital* hospital, const char* nombre, 
                    const char* apellido, const char* cedula,
                    const char* especialidad, int aniosExperiencia, 
                    float costoConsulta) {
    for (int i = 0; i < hospital->cantidadDoctores; i++) {
        if (strcmp(hospital->doctores[i].cedula, cedula) == 0) {
            cout << "Error: La cédula ya existe en el sistema.\n";
            return nullptr;
        }
    }

    if (hospital->cantidadDoctores >= hospital->capacidadDoctores) {
        int nuevaCapacidad = hospital->capacidadDoctores * 2;
        Doctor* nuevoArray = new Doctor[nuevaCapacidad];
        for (int i = 0; i < hospital->cantidadDoctores; i++) {
            nuevoArray[i] = hospital->doctores[i];
        }
        delete[] hospital->doctores;
        hospital->doctores = nuevoArray;
        hospital->capacidadDoctores = nuevaCapacidad;
    }

    Doctor* nuevoDoctor = &hospital->doctores[hospital->cantidadDoctores];
    nuevoDoctor->id = hospital->siguienteIdDoctor++;
    strcpy(nuevoDoctor->nombre, nombre);
    strcpy(nuevoDoctor->apellido, apellido);
    strcpy(nuevoDoctor->cedula, cedula);
    strcpy(nuevoDoctor->especialidad, especialidad);
    nuevoDoctor->aniosExperiencia = aniosExperiencia;
    nuevoDoctor->costoConsulta = costoConsulta;
    nuevoDoctor->disponible = true;

    nuevoDoctor->capacidadPacientes = 5;
    nuevoDoctor->cantidadPacientes = 0;
    nuevoDoctor->pacientesAsignados = new int[nuevoDoctor->capacidadPacientes];

    nuevoDoctor->capacidadCitas = 5;
    nuevoDoctor->cantidadCitas = 0;
    nuevoDoctor->citasAgendadas = new int[nuevoDoctor->capacidadCitas];

    hospital->cantidadDoctores++;
    cout << "Doctor creado exitosamente con ID: " << nuevoDoctor->id << "\n";
    return nuevoDoctor;
}

// Buscar doctor por ID
Doctor* buscarDoctorPorId(Hospital* hospital, int id) {
    for (int i = 0; i < hospital->cantidadDoctores; i++) {
        if (hospital->doctores[i].id == id) return &hospital->doctores[i];
    }
    cout << "Doctor no encontrado.\n";
    return nullptr;
}

// Buscar doctores por especialidad
Doctor** buscarDoctoresPorEspecialidad(Hospital* hospital, const char* especialidad, int* cantidad) {
    Doctor** resultados = new Doctor*[hospital->cantidadDoctores];
    *cantidad = 0;

    for (int i = 0; i < hospital->cantidadDoctores; i++) {
        if (compararCaseInsensitive(hospital->doctores[i].especialidad, especialidad)) {
            resultados[*cantidad] = &hospital->doctores[i];
            (*cantidad)++;
        }
    }
    if (*cantidad == 0) {
        delete[] resultados;
        return nullptr;
    }
    return resultados;
}

//
bool asignarPacienteADoctor(Doctor* doctor, int idPaciente) {
    if (doctor == nullptr) {
        cout << "Error: doctor no existe.\n";
        return false;
    }

    // Evitar duplicados
    for (int i = 0; i < doctor->cantidadPacientes; i++) {
        if (doctor->pacientesAsignados[i] == idPaciente) {
            cout << "Error: paciente ya asignado.\n";
            return false;
        }
    }

    // Redimensionar array si está lleno
    if (doctor->cantidadPacientes >= doctor->capacidadPacientes) {
        int nuevaCapacidad = doctor->capacidadPacientes * 2;
        int* nuevoArray = new int[nuevaCapacidad];
        for (int i = 0; i < doctor->cantidadPacientes; i++) {
            nuevoArray[i] = doctor->pacientesAsignados[i];
        }
        delete[] doctor->pacientesAsignados;
        doctor->pacientesAsignados = nuevoArray;
        doctor->capacidadPacientes = nuevaCapacidad;
    }

    // Asignar paciente
    doctor->pacientesAsignados[doctor->cantidadPacientes] = idPaciente;
    doctor->cantidadPacientes++;

    cout << "Paciente asignado correctamente al doctor "
         << doctor->nombre << " " << doctor->apellido << ".\n";
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
        if (doctor->pacientesAsignados[i] == idPaciente) {
            // Compactar el array
            for (int j = i; j < doctor->cantidadPacientes - 1; j++) {
                doctor->pacientesAsignados[j] = doctor->pacientesAsignados[j + 1];
            }
            doctor->cantidadPacientes--;
            encontrado = true;
            cout << "Paciente eliminado correctamente del doctor "
                 << doctor->nombre << " " << doctor->apellido << ".\n";
            break;
        }
    }

    if (!encontrado) {
        cout << "Error: paciente no encontrado en la lista del doctor.\n";
    }

    return encontrado;
}


void listarPacientesDeDoctor(Hospital* hospital, int idDoctor) {
    Doctor* doctor = buscarDoctorPorId(hospital, idDoctor);
    if (doctor == nullptr) {
        cout << "Error: doctor no encontrado.\n";
        return;
    }

    if (doctor->cantidadPacientes == 0) {
        cout << "El doctor no tiene pacientes asignados.\n";
        return;
    }

    cout << "\n=== Pacientes asignados al Doctor: " 
         << doctor->nombre << " " << doctor->apellido << " ===\n";
    cout << left << setw(6) << "ID"
         << setw(20) << "Nombre"
         << setw(20) << "Apellido"
         << setw(8) << "Edad"
         << setw(15) << "Cédula" << "\n";
    cout << string(70, '-') << "\n";

    for (int i = 0; i < doctor->cantidadPacientes; i++) {
        int idPaciente = doctor->pacientesAsignados[i];
        Paciente* paciente = buscarPacientePorId(hospital, idPaciente);

        if (paciente != nullptr) {
            cout << left << setw(6) << paciente->id
                 << setw(20) << paciente->nombre
                 << setw(20) << paciente->apellido
                 << setw(8) << paciente->edad
                 << setw(15) << paciente->cedula << "\n";
        } else {
            cout << "ID: " << idPaciente << " (Paciente no encontrado)\n";
        }
    }

    cout << string(70, '-') << "\n";
    cout << "Total de pacientes asignados: " << doctor->cantidadPacientes << "\n\n";
}
void listarDoctores(Hospital* hospital) {
    if (hospital->cantidadDoctores == 0) {
        cout << "No hay doctores registrados en el hospital.\n";
        return;
    }

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

    for (int i = 0; i < hospital->cantidadDoctores; i++) {
        Doctor& d = hospital->doctores[i];
        cout << left << setw(6) << d.id
             << setw(20) << d.nombre
             << setw(20) << d.apellido
             << setw(15) << d.cedula
             << setw(20) << d.especialidad
             << setw(8) << d.aniosExperiencia
             << setw(10) << fixed << setprecision(2) << d.costoConsulta
             << setw(12) << (d.disponible ? "Sí" : "No") << "\n";
    }

    cout << string(111, '-') << "\n";
    cout << "Total de doctores registrados: " << hospital->cantidadDoctores << "\n\n";
}
bool eliminarDoctor(Hospital* hospital, int id) {
    for (int i = 0; i < hospital->cantidadDoctores; i++) {
        if (hospital->doctores[i].id == id) {
            Doctor& d = hospital->doctores[i];

            // Cancelar citas del doctor
            for (int j = 0; j < hospital->cantidadCitas; j++) {
                if (hospital->citas[j].idDoctor == id) {
                    strcpy(hospital->citas[j].estado, "Cancelada");
                    hospital->citas[j].atendida = false;
                }
            }

            // Liberar memoria de arrays dinámicos
            delete[] d.pacientesAsignados;
            delete[] d.citasAgendadas;

            // Compactar array de doctores
            hospital->doctores[i] = hospital->doctores[hospital->cantidadDoctores - 1];
            hospital->cantidadDoctores--;

            cout << "Doctor eliminado exitosamente.\n";
            return true;
        }
    }
    cout << "Doctor no encontrado.\n";
    return false;
}
