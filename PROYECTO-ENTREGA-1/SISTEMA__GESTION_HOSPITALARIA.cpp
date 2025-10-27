#include <iostream>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <cctype> // para tolower
#include <limits>

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
//VALIDAR CEDULA
bool validarCedula(const char* cedula){

    if (cedula[0]=='\0'||cedula==nullptr){
        return false;

    }
    if(strlen(cedula)>20){
        cout<<"Excede el limite de caracteres";
        return false;
    }
    return true;
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

void destruirHospital(Hospital* hospital) {
    if (hospital == nullptr)
        return;

    //  Liberar memoria de cada PACIENTE
    for (int i = 0; i < hospital->cantidadPacientes; i++) {
        Paciente* p = &hospital->pacientes[i];

        // Liberar historial médico
        if (p->historial != nullptr) {
            delete[] p->historial;
            p->historial = nullptr;
        }

        // Liberar citas agendadas
        if (p->citasAgendadas != nullptr) {
            delete[] p->citasAgendadas;
            p->citasAgendadas = nullptr;
        }
    }

    //  Liberar array de PACIENTES
    if (hospital->pacientes != nullptr) {
        delete[] hospital->pacientes;
        hospital->pacientes = nullptr;
    }

    //️ Liberar memoria de cada DOCTOR
    for (int i = 0; i < hospital->cantidadDoctores; i++) {
        Doctor* d = &hospital->doctores[i];

        // Liberar pacientes asignados
        if (d->pacientesAsignados != nullptr) {
            delete[] d->pacientesAsignados;
            d->pacientesAsignados = nullptr;
        }

        // Liberar citas agendadas
        if (d->citasAgendadas != nullptr) {
            delete[] d->citasAgendadas;
            d->citasAgendadas = nullptr;
        }
    }

    // Liberar array de DOCTORES
    if (hospital->doctores != nullptr) {
        delete[] hospital->doctores;
        hospital->doctores = nullptr;
    }

    // Liberar array de CITAS generales
    if (hospital->citas != nullptr) {
        delete[] hospital->citas;
        hospital->citas = nullptr;
    }

    //Finalmente liberar la estructura Hospital
    delete hospital;
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
    if (hospital == nullptr || cedula == nullptr) {
        cout << "Error: datos inválidos.\n";
        cin.get();
        return nullptr;
    }

    for (int i = 0; i < hospital->cantidadPacientes; i++) {
        if (compararCaseInsensitive(hospital->pacientes[i].cedula, cedula)) {
            cout << "Paciente encontrado: " 
                 << hospital->pacientes[i].nombre << " " 
                 << hospital->pacientes[i].apellido 
                 << " | Cedula: " << hospital->pacientes[i].cedula << endl;
            cin.get();
            return &hospital->pacientes[i]; 
        }
    }

    cout << "Paciente no encontrado.\n";
    cin.get();
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
            cout << "Paciente encontrado: " 
                 << hospital->pacientes[i].nombre << " " 
                 << hospital->pacientes[i].apellido 
                 << " | Cedula: " << hospital->pacientes[i].cedula << endl;
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
    cout << "Ingrese nuevo sexo (, Enter para mantener): ";
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
    if (hospital == nullptr) {
        cout << "Error: el puntero al hospital es nulo.\n";
        return;
    }

    if (hospital->cantidadPacientes == 0) {
        cout << "No hay pacientes registrados.\n";
        return;
    }

    cout << "=====================================\n";
    cout << "        LISTA DE PACIENTES\n";
    cout << "=====================================\n";

    for (int i = 0; i < hospital->cantidadPacientes; i++) {
        Paciente& p = hospital->pacientes[i];
        cout << "Paciente #" << i + 1 << endl;
        cout << "  ID:       " << p.id << endl;
        cout << "  Nombre:   " << p.nombre << " " << p.apellido << endl;
        cout << "  Cedula:   " << p.cedula << endl;
        cout << "  Edad:     " << p.edad << endl;
        cout << "  Sexo:     " << p.sexo << endl;
        cout << "-------------------------------------\n";
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
//PARTE 4 GESTION DE CITAS
Cita* buscarCitaPorId(Hospital* hospital, int id) {
    for (int i = 0; i < hospital->cantidadCitas; i++) {
        if (hospital->citas[i].id == id){

         return &hospital->citas[i];
        }
    }
    cout << "Cita no encontrada.\n";
    return nullptr;
}
//validar fecha
bool validarFormatoFecha(const char* fecha) {
    int anio, mes, dia;
    return (sscanf(fecha, "%4d-%2d-%2d", &anio, &mes, &dia) == 3 &&
            anio > 1900 && mes >= 1 && mes <= 12 && dia >= 1 && dia <= 31);
}
 
// Validar formato de hora (HH:MM)
bool validarFormatoHora(const char* hora) {
    int hh, mm;
    return (sscanf(hora, "%2d:%2d", &hh, &mm) == 2 && hh >= 0 && hh < 24 && mm >= 0 && mm < 60);
}
//Redimensionar con tipo de dato citas
Cita* redimensionarArrayCita(Cita* array, int& capacidadActual) {
    int nuevaCapacidad = capacidadActual * 2;
    Cita* nuevoArray = new Cita[nuevaCapacidad];
    for (int i = 0; i < capacidadActual; i++) {
        nuevoArray[i] = array[i];
    }
    delete[] array;
    capacidadActual = nuevaCapacidad;
    return nuevoArray;
}

Cita* agendarCita(Hospital* hospital, int idPaciente, int idDoctor,
const char* fecha, const char* hora, const char* motivo){

    Paciente*paciente= buscarPacientePorId(hospital,idPaciente);
    Doctor*doctor=buscarDoctorPorId(hospital,idDoctor);
   
    if (!validarFormatoFecha(fecha)) {
        cout << "Error: formato de fecha inválido. Use YYYY-MM-DD.\n";
        return nullptr;
    }
    if (!validarFormatoHora(hora)) {
        cout << "Error: formato de hora inválido. Use HH:MM.\n";
        return nullptr;
    }

     if (hospital->cantidadCitas >= hospital->capacidadCitas) {
        hospital->citas = redimensionarArrayCita(hospital->citas, hospital->capacidadCitas);
     }

 Cita* nuevaCita = &hospital->citas[hospital->cantidadCitas];
    nuevaCita->id = hospital->siguienteIdCita++;
    nuevaCita->idPaciente = idPaciente;
    nuevaCita->idDoctor = idDoctor;
    strcpy(nuevaCita->fecha, fecha);
    strcpy(nuevaCita->hora, hora);
    strcpy(nuevaCita->motivo, motivo);
    strcpy(nuevaCita->estado, "Agendada");
    strcpy(nuevaCita->observaciones, "");
    nuevaCita->atendida = false;
      if (paciente->cantidadCitas >= paciente->capacidadCitas) {
        paciente->citasAgendadas = redimensionarArrayInt(paciente->citasAgendadas, paciente->capacidadCitas);
      }

      if (doctor->cantidadCitas>=doctor->capacidadCitas){
        doctor->citasAgendadas=redimensionarArrayInt(doctor->citasAgendadas,doctor->capacidadCitas);
      }
      hospital->cantidadCitas++;

    cout << "Cita agendada exitosamente con ID: " << nuevaCita->id
         << " entre el Dr. " << doctor->nombre << " y el paciente " << paciente->nombre << ".\n";

    return nuevaCita;
}
bool cancelarCita(Hospital* hospital, int idCita){

    for (int i = 0; i < hospital->cantidadCitas; i++) {
        if (hospital->citas[i].id == idCita) {
            // Marcar la cita como cancelada
            strcpy(hospital->citas[i].estado, "Cancelada");
            hospital->citas[i].atendida = false;

            // Remover referencia de la cita en el paciente (si existe)
            int idPaciente = hospital->citas[i].idPaciente;
            Paciente* paciente = buscarPacientePorId(hospital, idPaciente);
            if (paciente != nullptr) {
                for (int j = 0; j < paciente->cantidadCitas; j++) {
                    if (paciente->citasAgendadas[j] == idCita) {
                        for (int k = j; k < paciente->cantidadCitas - 1; k++) {
                            paciente->citasAgendadas[k] = paciente->citasAgendadas[k + 1];
                        }
                        paciente->cantidadCitas--;
                        break;
                    }
                }
            }

            // Remover referencia de la cita en el doctor (si existe)
            int idDoctor = hospital->citas[i].idDoctor;
            Doctor* doctor = buscarDoctorPorId(hospital, idDoctor);
            if (doctor != nullptr) {
                for (int j = 0; j < doctor->cantidadCitas; j++) {
                    if (doctor->citasAgendadas[j] == idCita) {
                        for (int k = j; k < doctor->cantidadCitas - 1; k++) {
                            doctor->citasAgendadas[k] = doctor->citasAgendadas[k + 1];
                        }
                        doctor->cantidadCitas--;
                        break;
                    }
                }
            }

            cout << "Cita con ID " << idCita << " cancelada.\n";
            return true;
        }
    }

    cout << "Cita no encontrada.\n";
    return false;
}

bool atenderCita(Hospital* hospital, int idCita, const char* diagnostico,
const char* tratamiento, const char* medicamentos){
 Cita*cita= buscarCitaPorId(hospital,idCita);
 if (cita==nullptr){
    return false;
 }
 if (strcmp(cita->estado, "Agendada") != 0) {
    cout<< "Error la cita no esta agendada.\n";
    return false;
 }
 strcpy(cita->estado,"Atendida");

 Paciente* paciente= buscarPacientePorId(hospital,cita->idPaciente);
 Doctor*doctor=buscarDoctorPorId(hospital,cita->idDoctor);
 
 if(paciente==nullptr||doctor==nullptr){
    cout<<"Error. Paciente o doctor no encontrados.\n";
    return false;
 }
 HistorialMedico nuevaConsulta;

    nuevaConsulta.idConsulta = hospital->siguienteIdConsulta;
    strcpy(nuevaConsulta.fecha, cita->fecha);
    strcpy(nuevaConsulta.hora, cita->hora);
    strcpy(nuevaConsulta.diagnostico, diagnostico);
    strcpy(nuevaConsulta.tratamiento, tratamiento);
    strcpy(nuevaConsulta.medicamentos, medicamentos);
    nuevaConsulta.idDoctor = cita->idDoctor;

    // Obtener el costo de la consulta desde el doctor correspondiente
     doctor = buscarDoctorPorId(hospital, cita->idDoctor);
    if (doctor == nullptr) {
        cout << "Error: Doctor asociado a la cita no encontrado.\n";
        return false;
    }
    nuevaConsulta.costo = doctor->costoConsulta;

    //  Agregar la nueva consulta al historial del paciente
     paciente = buscarPacientePorId(hospital, cita->idPaciente);
    if (paciente == nullptr) {
        cout << "Error: Paciente asociado a la cita no encontrado.\n";
        return false;
    }

    agregarConsultaAlHistorial(paciente, nuevaConsulta);

    // Incrementar el contador global de consultas
    hospital->siguienteIdConsulta++;

    cout << " Cita atendida correctamente y registrada en el historial médico.\n";
    return true;
}
Cita** obtenerCitasDePaciente(Hospital* hospital, int idPaciente, int* cantidad) {
    //  Inicializar cantidad
    *cantidad = 0;

    //  Buscar al paciente por ID
    Paciente* paciente = buscarPacientePorId(hospital, idPaciente);
    if (paciente == nullptr) {
        cout << "Error: Paciente no encontrado.\n";
        return nullptr;
    }

    //  Verificar si el paciente tiene citas registradas
    if (paciente->cantidadCitas == 0) {
        cout << "El paciente no tiene citas registradas.\n";
        return nullptr;
    }

    //  Crear un arreglo dinámico de punteros a Cita
    Cita** resultados = new Cita*[paciente->cantidadCitas];

    //  Recorrer todas las citas del paciente
    for (int i = 0; i < paciente->cantidadCitas; i++) {
        int idCita = paciente->citasAgendadas[i];

        // Buscar la cita dentro del hospital
        Cita* cita = buscarCitaPorId(hospital, idCita);

        // Si existe, agregarla al arreglo de resultados
        if (cita != nullptr) {
            resultados[*cantidad] = cita;
            (*cantidad)++;
        }
    }

    //  Si no se encontró ninguna cita válida
    if (*cantidad == 0) {
        delete[] resultados;
        cout << "No se encontraron citas válidas para este paciente.\n";
        return nullptr;
    }

    // Retornar el arreglo dinámico con punteros a las citas válidas
    return resultados;
}
Cita** obtenerCitasDeDoctor(Hospital* hospital, int idDoctor, int* cantidad) {
    *cantidad = 0;
    Doctor* doctor = buscarDoctorPorId(hospital, idDoctor);
    if (doctor == nullptr) {
        cout << "Error: Doctor no encontrado.\n";
        return nullptr;
    }

    if (doctor->cantidadCitas == 0) {
        cout << "El doctor no tiene citas registradas.\n";
        return nullptr;
    }

    Cita** resultados = new Cita*[doctor->cantidadCitas];

    for (int i = 0; i < doctor->cantidadCitas; i++) {
        int idCita = doctor->citasAgendadas[i];
        Cita* cita = buscarCitaPorId(hospital, idCita);
        if (cita != nullptr) {
            resultados[*cantidad] = cita;
            (*cantidad)++;
        }
    }

    if (*cantidad == 0) {
        delete[] resultados;
        return nullptr;
    }

    return resultados;
}
Cita** obtenerCitasPorFecha(Hospital* hospital, const char* fecha, int* cantidad) {
    *cantidad = 0;
    Cita** resultados = new Cita*[hospital->cantidadCitas];

    for (int i = 0; i < hospital->cantidadCitas; i++) {
        if (strcmp(hospital->citas[i].fecha, fecha) == 0) {
            resultados[*cantidad] = &hospital->citas[i];
            (*cantidad)++;
        }
    }

    if (*cantidad == 0) {
        delete[] resultados;
        return nullptr;
    }

    return resultados;
}
void mostrarCitas(Cita** citas, int cantidad) {
    if (citas == nullptr || cantidad == 0) {
        cout << "No hay citas para mostrar.\n";
        return;
    }

    for (int i = 0; i < cantidad; i++) {
        Cita* c = citas[i];
        if (c == nullptr) continue;
        cout << "Cita #" << i + 1 << ":\n";
        cout << "ID: " << c->id << "\n";
        cout << "Paciente ID: " << c->idPaciente << "\n";
        cout << "Doctor ID: " << c->idDoctor << "\n";
        cout << "Fecha: " << c->fecha << "\n";
        cout << "Hora: " << c->hora << "\n";
        cout << "Motivo: " << c->motivo << "\n";
        cout << "Estado: " << c->estado << "\n";
        cout << "----------------------------------------\n";
    }
}


void listarCitasPendientes(Hospital* hospital) {
    cout << "\n=== Citas Pendientes ===\n";
    cout << left << setw(6) << "ID"
         << setw(12) << "Fecha"
         << setw(8) << "Hora"
         << setw(10) << "ID_Doc"
         << setw(12) << "ID_Pac"
         << setw(25) << "Motivo" << "\n";
    cout << string(75, '-') << "\n";

    int contador = 0;

    for (int i = 0; i < hospital->cantidadCitas; i++) {
        Cita& c = hospital->citas[i];
        if (strcmp(c.estado, "Agendada") == 0) {
            cout << left << setw(6) << c.id
                 << setw(12) << c.fecha
                 << setw(8) << c.hora
                 << setw(10) << c.idDoctor
                 << setw(12) << c.idPaciente
                 << setw(25) << c.motivo << "\n";
            contador++;
        }
    }

    if (contador == 0)
        cout << "No hay citas pendientes.\n";
    else
        cout << string(75, '-') << "\nTotal de citas pendientes: " << contador << "\n";
}
bool verificarDisponibilidad(Hospital* hospital, int idDoctor, const char* fecha, const char* hora) {
    for (int i = 0; i < hospital->cantidadCitas; i++) {
        Cita& cita = hospital->citas[i];
        if (cita.idDoctor == idDoctor && strcmp(cita.estado, "Agendada") == 0) {
            if (strcmp(cita.fecha, fecha) == 0 && strcmp(cita.hora, hora) == 0) {
                return false;  
            }
        }
    }
    return true; 
}



int main() {
    Hospital* hospital = new Hospital;
    RegistrarHospital(hospital);

    int opMenu;
    do {
        system("cls");
        cout << "===========================================\n";
        cout << "||   SISTEMA DE GESTION HOSPITALARIA      ||\n";
        cout << "===========================================\n";
        cout << "1. Gestion de Pacientes\n";
        cout << "2. Gestion de Doctores\n";
        cout << "3. Gestion de Citas\n";
        cout << "4. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opMenu;

        switch (opMenu) {
            case 1: { // Gestion de Pacientes
                int opPaciente;
                do {
                    system("cls");
                    cout << "\n=======================================\n";
                    cout << "||        GESTION DE PACIENTES        ||\n";
                    cout << "=======================================\n";
                    cout << "1. Registrar nuevo paciente\n";
                    cout << "2. Buscar paciente por cedula\n";
                    cout << "3. Buscar paciente por nombre\n";
                    cout << "4. Ver historial medico completo\n";
                    cout << "5. Actualizar datos del paciente\n";
                    cout << "6. Listar todos los pacientes\n";
                    cout << "7. Eliminar paciente.\n";
                    cout << "0. Volver al menu principal\n";
                    cout << "Seleccione una opcion: ";
                    cin >> opPaciente;

                    switch (opPaciente) {
                        case 1: { // Registrar nuevo paciente
                            system("cls");
                            char nombre[50], apellido[50], cedula[20];
                            int edad;
                            char sexo;

                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Ingrese nombre: "; cin.getline(nombre, 50);
                            cout << "Ingrese apellido: "; cin.getline(apellido, 50);
                            cout << "Ingrese cedula: "; cin.getline(cedula, 20);

                            if (!validarCedula(cedula)) {
                                cout << "Cedula invalida.\n"; cin.get(); break;
                            }

                            cout << "Ingrese edad: "; cin >> edad;
                            cout << "Ingrese sexo (M/F): "; cin >> sexo;

                            Paciente* paciente = crearPaciente(hospital, nombre, apellido, cedula, edad, sexo);
                            if (paciente) cout << "Paciente registrado correctamente.\n";
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cin.get();
                            break;
                        }

                        case 2: { // Buscar por cedula
                            system("cls");
                            char cedula[20];
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Ingrese cedula: "; cin.getline(cedula, 20);
                            Paciente* p = buscarPacientePorCedula(hospital, cedula);
                            if (!p) cout << "Paciente no encontrado.\n";
                            cin.get();
                            break;
                        }

                        case 3: { // Buscar por nombre
                            system("cls");
                            char nombre[50];
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Ingrese nombre: "; cin.getline(nombre, 50);

                            int cantidad = 0;
                            Paciente** lista = buscarPacientesPorNombre(hospital, nombre, &cantidad);
                            cout << "Coincidencias: " << cantidad << endl;
                            if (lista) delete[] lista;
                            cin.get();
                            break;
                        }

                        case 4: { // Historial medico
                            system("cls");
                            char cedula[20];
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Ingrese cedula del paciente: "; cin.getline(cedula, 20);
                            Paciente* p = buscarPacientePorCedula(hospital, cedula);
                            if (!p) { cout << "Paciente no encontrado.\n"; cin.get(); break; }

                            int cant = 0;
                            HistorialMedico* hist = obtenerHistorialCompleto(p, &cant);
                            if (cant == 0) cout << "No hay historial.\n";
                            else {
                                for (int i = 0; i < cant; i++)
                                    cout << hist[i].fecha << " - " << hist[i].diagnostico << endl;
                            }
                            cin.get();
                            break;
                        }

                        case 5: { // Actualizar paciente
                            system("cls");
                            char cedula[20];
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Ingrese cedula del paciente a actualizar: "; cin.getline(cedula, 20);
                            Paciente* pac = buscarPacientePorCedula(hospital, cedula);
                            if (!pac) { cout << "Paciente no encontrado.\n"; cin.get(); break; }

                            char nuevoNombre[50], nuevoApellido[50]; int nuevaEdad = 0; char nuevoSexo = 'X';
                            cout << "\nNuevos datos (deje vacio o X para mantener):\n";
                            cout << "Nuevo nombre: "; cin.getline(nuevoNombre, 50);
                            cout << "Nuevo apellido: "; cin.getline(nuevoApellido, 50);
                            cout << "Nueva edad (0 = igual): "; cin >> nuevaEdad;
                            cout << "Nuevo sexo (M/F, X = igual): "; cin >> nuevoSexo;

                            if (strlen(nuevoNombre) > 0) strcpy(pac->nombre, nuevoNombre);
                            if (strlen(nuevoApellido) > 0) strcpy(pac->apellido, nuevoApellido);
                            if (nuevaEdad > 0) pac->edad = nuevaEdad;
                            char sx = toupper(nuevoSexo);
                            if (sx == 'M' || sx == 'F') pac->sexo = sx;

                            cout << "Actualizado correctamente.\n";
                            cin.ignore(numeric_limits<streamsize>::max(), '\n'); cin.get();
                            break;
                        }

                        case 6: { // Listar pacientes
                            system("cls");
                            listarPacientes(hospital);
                            cout << "\nPresione Enter para continuar...";
                            cin.ignore(numeric_limits<streamsize>::max(), '\n'); cin.get();
                            break;
                        }

                        case 7: { // Eliminar paciente
                            system("cls");
                            char cedula[20];
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Ingrese cedula del paciente a eliminar: "; cin.getline(cedula, 20);
                            Paciente* paciente = buscarPacientePorCedula(hospital, cedula);
                            if (!paciente) { cout << "Paciente no encontrado.\n"; cin.get(); break; }

                            eliminarPaciente(hospital, paciente->id);
                            cout << "Paciente eliminado correctamente.\n";
                            cin.get();
                            break;
                        }
                    }
                } while (opPaciente != 0);
                break;
            }

            case 2: { // Gestion de Doctores
                int opDoct;
                do {
                    system("cls");
                    cout << "\n=======================================\n";
                    cout << "||        GESTION DE DOCTORES        ||\n";
                    cout << "=======================================\n";
                    cout << "1. Registrar nuevo doctor\n";
                    cout << "2. Buscar doctor por ID\n";
                    cout << "3. Buscar doctores por especialidad\n";
                    cout << "4. Asignar paciente a doctor\n";
                    cout << "5. Ver pacientes asignados a doctor\n";
                    cout << "6. Listar todos los doctores\n";
                    cout << "7. Eliminar doctor\n";
                    cout << "0. Volver al menu principal\n";
                    cout << "Seleccione una opcion: ";
                    cin >> opDoct;

                    switch (opDoct) {
                        case 1: { // Registrar doctor
                            system("cls");
                            char nombre[50], apellido[50], cedula[20], especialidad[50];
                            int aniosExperiencia;
                            float costoConsulta;

                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Ingrese nombre: "; cin.getline(nombre, 50);
                            cout << "Ingrese apellido: "; cin.getline(apellido, 50);
                            cout << "Ingrese cedula: "; cin.getline(cedula, 20);
                            cout << "Ingrese especialidad: "; cin.getline(especialidad, 50);
                            cout << "Ingrese anios de experiencia: "; cin >> aniosExperiencia;
                            cout << "Ingrese costo de consulta: "; cin >> costoConsulta;

                            Doctor* nuevo = crearDoctor(hospital, nombre, apellido, cedula, especialidad, aniosExperiencia, costoConsulta);
                            if (nuevo) cout << "Doctor registrado correctamente.\n";
                            cin.ignore(numeric_limits<streamsize>::max(), '\n'); cin.get();
                            break;
                        }

                        case 2: { // Buscar doctor por ID
                            system("cls");
                            int idDoc; cout << "Ingrese ID del doctor: "; cin >> idDoc;
                            Doctor* doc = buscarDoctorPorId(hospital, idDoc);
                            if (!doc) cout << "Doctor no encontrado.\n";
                            cin.get();
                            break;
                        }

                        case 3: { // Buscar doctores por especialidad
                            system("cls");
                            char especialidad[50];
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Ingrese especialidad: "; cin.getline(especialidad, 50);

                            int cantidad = 0;
                            Doctor** lista = buscarDoctoresPorEspecialidad(hospital, especialidad, &cantidad);
                            cout << "Coincidencias: " << cantidad << endl;
                            if (lista) delete[] lista;
                            cin.get();
                            break;
                        }

                        case 4: { // Asignar paciente a doctor
                            system("cls");
                            int idDoc, idPac;
                            cout << "Ingrese ID del doctor: "; cin >> idDoc;
                            cout << "Ingrese ID del paciente: "; cin >> idPac;

                            Doctor* doc = buscarDoctorPorId(hospital, idDoc);
                            Paciente* pac = buscarPacientePorId(hospital, idPac);
                            if (doc && pac) {
                                asignarPacienteADoctor(doc, idPac);
                                cout << "Paciente asignado correctamente.\n";
                            } else cout << "Doctor o paciente no encontrado.\n";
                            cin.get();
                            break;
                        }

                        case 5: { // Ver pacientes de doctor
                            system("cls");
                            int idDoc; cout << "Ingrese ID del doctor: "; cin >> idDoc;
                            listarPacientesDeDoctor(hospital, idDoc);
                            cin.get();
                            break;
                        }

                        case 6: { // Listar todos los doctores
                            system("cls");
                            listarDoctores(hospital);
                            cout << "\nPresione Enter para continuar...";
                            cin.ignore(numeric_limits<streamsize>::max(), '\n'); cin.get();
                            break;
                        }

                        case 7: { // Eliminar doctor
                            system("cls");
                            int idDoc; cout << "Ingrese ID del doctor a eliminar: "; cin >> idDoc;
                            eliminarDoctor(hospital, idDoc);
                            cin.get();
                            break;
                        }

                        case 0: break;
                        default: cout << "Opcion no valida.\n"; cin.get(); break;
                    }
                } while (opDoct != 0);
                break;
            }

            case 3: { // Gestion de citas
                int opCita;
                do {
                    system("cls");
                    cout << "\n=======================================\n";
                    cout << "||        GESTION DE CITAS           ||\n";
                    cout << "=======================================\n";
                    cout << "1. Agendar nueva cita\n";
                    cout << "2. Cancelar cita\n";
                    cout << "3. Atender cita\n";
                    cout << "4. Ver citas de un paciente\n";
                    cout << "5. Ver citas de un doctor\n";
                    cout << "6. Ver citas de una fecha\n";
                    cout << "7. Ver citas pendientes\n";
                    cout << "0. Volver al menu principal\n";
                    cout << "Seleccione una opcion: ";
                    cin >> opCita;

                    switch (opCita) {
                        case 1: { // Agendar cita
                            system("cls");
                            int idPac, idDoc;
                            char fecha[11], hora[6], motivo[100];
                            cout << "Ingrese ID del paciente: "; cin >> idPac;
                            cout << "Ingrese ID del doctor: "; cin >> idDoc;
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Ingrese fecha (YYYY-MM-DD): "; cin.getline(fecha, 11);
                            cout << "Ingrese hora (HH:MM): "; cin.getline(hora, 6);
                            cout << "Ingrese motivo de la cita: "; cin.getline(motivo, 100);

                            if (!verificarDisponibilidad(hospital, idDoc, fecha, hora))
                                cout << "El doctor no esta disponible en esa fecha y hora.\n";
                            else {
                                agendarCita(hospital, idPac, idDoc, fecha, hora, motivo);
                                cout << "Cita agendada correctamente.\n";
                            }
                            cin.get();
                            break;
                        }

                        case 2: { // Cancelar cita
                            system("cls");
                            int idCita; cout << "Ingrese ID de la cita a cancelar: "; cin >> idCita;
                            cancelarCita(hospital, idCita);
                            cin.ignore(numeric_limits<streamsize>::max(), '\n'); cin.get();
                            break;
                        }

                        case 3: { // Atender cita
                            system("cls");
                            int idCita; char diagnostico[200], tratamiento[200], medicamentos[200];
                            cout << "Ingrese ID de la cita a atender: "; cin >> idCita;
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Ingrese diagnostico: "; cin.getline(diagnostico, 200);
                            cout << "Ingrese tratamiento: "; cin.getline(tratamiento, 200);
                            cout << "Ingrese medicamentos: "; cin.getline(medicamentos, 200);

                            atenderCita(hospital, idCita, diagnostico, tratamiento, medicamentos);
                            cout << "Cita atendida correctamente.\n";
                            cin.get();
                            break;
                        }

                        case 4: { // Ver citas de un paciente
                            system("cls");
                            int idPac, cantidad = 0;
                            cout << "Ingrese ID del paciente: "; cin >> idPac;
                            Cita** citas = obtenerCitasDePaciente(hospital, idPac, &cantidad);
                            mostrarCitas(citas, cantidad);
                            delete[] citas;
                            system("pause");
                            break;
                        }

                        case 5: { // Ver citas de un doctor
                            system("cls");
                            int idDoc, cantidad = 0;
                            cout << "Ingrese ID del doctor: "; cin >> idDoc;
                            Cita** citas = obtenerCitasDeDoctor(hospital, idDoc, &cantidad);
                            mostrarCitas(citas, cantidad);
                            delete[] citas;
                            system("pause");
                            break;
                        }

                        case 6: { // Ver citas de una fecha
                            system("cls");
                            char fecha[11]; int cantidad = 0;
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Ingrese fecha (YYYY-MM-DD): "; cin.getline(fecha, 11);
                            Cita** citas = obtenerCitasPorFecha(hospital, fecha, &cantidad);
                            mostrarCitas(citas, cantidad);
                            delete[] citas;
                            system("pause");
                            break;
                        }

                        case 7: { // Citas pendientes
                            system("cls");
                            listarCitasPendientes(hospital);
                            cout << "\nPresione Enter para continuar...";
                            cin.ignore(numeric_limits<streamsize>::max(), '\n'); cin.get();
                            break;
                        }

                        case 0: break;
                        default: cout << "Opcion no valida.\n"; cin.get(); break;
                    }

                } while (opCita != 0);
                break;
            }

            case 4:
                cout << "Saliendo del sistema...\n";
                break;

            default:
                cout << "Opcion no valida.\n";
                cin.get();
                break;
        }

    } while (opMenu != 4);

    // Liberar memoria del hospital si es necesario

    return 0;
}
