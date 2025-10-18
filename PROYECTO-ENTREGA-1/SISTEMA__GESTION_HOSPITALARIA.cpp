#include <iostream>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <fstream>

using namespace std;

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

Paciente* crearPaciente(Hospital* hospital, const char* nombre, 
                                     const char* apellido, const char* cedula, int edad, char sexo) {
    //Verificar que la cedula no exista previamente en el sistema
    for (int i = 0; i < hospital->cantidadPacientes; i++) {
        if (strcmp(hospital->pacientes[i].cedula, cedula) == 0) {
            cout << "Error: La cédula ya existe en el sistema.\n";
            return nullptr;
        }
    }

    //  Redimensionar arreglo si está lleno
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

    //  Crear referencia al nuevo paciente
    Paciente* nuevoPaciente = &hospital->pacientes[hospital->cantidadPacientes];

    //  Asignar datos personales
    nuevoPaciente->id = hospital->siguienteIdPaciente++;
    strcpy(nuevoPaciente->nombre, nombre);
    strcpy(nuevoPaciente->apellido, apellido);
    strcpy(nuevoPaciente->cedula, cedula);
    nuevoPaciente->edad = edad;
    nuevoPaciente->sexo = sexo;
    nuevoPaciente->activo = true;

    // Inicializar arreglos dinámicos
    nuevoPaciente->capacidadHistorial = 5;
    nuevoPaciente->cantidadConsultas = 0;
    nuevoPaciente->historial = new HistorialMedico[nuevoPaciente->capacidadHistorial];

    nuevoPaciente->capacidadCitas = 5;
    nuevoPaciente->cantidadCitas = 0;
    nuevoPaciente->citasAgendadas = new int[nuevoPaciente->capacidadCitas]; // IDs de citas

    // Inicializar cadenas vacías
    strcpy(nuevoPaciente->alergias, "");
    strcpy(nuevoPaciente->observaciones, "");

    //  Agregar al hospital
    hospital->cantidadPacientes++;

    cout << "Paciente creado exitosamente con ID: " << nuevoPaciente->id << "\n";
    return nuevoPaciente;
}

#include <cctype> // para tolower

bool compararCaseInsensitive(const char* a, const char* b) {
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b))
            return false;
        ++a;
        ++b;
    }
    return *a == *b;
}

Paciente* buscarPacientePorCedula(Hospital* hospital, const char* cedula) {
    for (int i = 0; i < hospital->cantidadPacientes; i++) {
        if (compararCaseInsensitive(hospital->pacientes[i].cedula, cedula)) {
            return &hospital->pacientes[i]; //
        }
    }

    cout << "Paciente no encontrado.\n";
    return nullptr;
}

Paciente* buscarPacientePorId(Hospital* hospital, int id) {
    for (int i = 0; i < hospital->cantidadPacientes; i++) {
        if (hospital->pacientes[i].id == id) {
            return &hospital->pacientes[i];
        }
    }

    cout << "Paciente no encontrado.\n";
    return nullptr;
}

Paciente** buscarPacientesPorNombre(Hospital* hospital, 
 const char* nombre, int* cantidad) {
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

bool actualizarPaciente(Hospital* hospital, int id) {
    Paciente* paciente = buscarPacientePorId(hospital, id);
    if (paciente == nullptr) {
        cout << "Paciente no encontrado.\n";
        return false;
    }
    cout << "Actualizando datos del paciente ID: " << id << "\n";
    cout << "Nombre actual: " << paciente->nombre << "\n";
    cout << "Ingrese nuevo nombre (o presione Enter para mantener): ";
    char nuevoNombre[50];
    cin.ignore();
    cin.getline(nuevoNombre, 50);
    if (strlen(nuevoNombre) > 0) {
        strcpy(paciente->nombre, nuevoNombre);
    }
    cout << "Apellido actual: " << paciente->apellido << "\n";
    cout << "Ingrese nuevo apellido (o presione Enter para mantener): ";
    char nuevoApellido[50];

    cin.getline(nuevoApellido, 50);
    if (strlen(nuevoApellido) > 0) {
        strcpy(paciente->apellido, nuevoApellido);
    }
    cout << "Edad actual: " << paciente->edad << "\n";
    cout << "Ingrese nueva edad (o presione Enter para mantener): ";
    char nuevaEdadStr[10];
    cin.getline(nuevaEdadStr, 10);
    if (strlen(nuevaEdadStr) > 0) {
        int nuevaEdad = atoi(nuevaEdadStr);
        if (nuevaEdad > 0) {
            paciente->edad = nuevaEdad;
        } else {
            cout << "Edad invalida. Manteniendo valor actual.\n";
        }
    }
    cout << "Sexo actual: " << paciente->sexo << "\n";
    cout << "Ingrese nuevo sexo (M/F)
    char nuevoSexoStr[10];
    cin.getline(nuevoSexoStr, 10);
    if (strlen(nuevoSexoStr) > 0) {
        char nuevoSexo = toupper(nuevoSexoStr[0]);
        if (nuevoSexo == 'M' || nuevoSexo == 'F') {
            paciente->sexo = nuevoSexo;
        } else {
            cout << "Sexo invalido. Manteniendo valor actual.\n";
        }
    }
    cout << "Datos actualizados exitosamente.\n";
    return true;
}

bool eliminarPaciente(Hospital* hospital, int id) {
    for (int i = 0; i < hospital->cantidadPacientes; i++) {
        if (hospital->pacientes[i].id == id) {
            hospital->pacientes[i].activo = false;
            hospital->pacientes[i] = hospital->pacientes[hospital->cantidadPacientes - 1];
            hospital->cantidadPacientes--;
            cout << "Paciente eliminado exitosamente.\n";
            return true;
        }
    }
    cout << "Paciente no encontrado.\n";
    return false;
}
 void listarPacientes(Hospital* hospital){
 cout<<"=== Lista de Pacientes ===\n";
 for(int i=0;i<hospital->cantidadPacientes;i++){
 Paciente& p=hospital->pacientes[i];
 cout<<"ID:"<<p.id<<", Nombre:"<<p.nombre<<", Apellido:"<<p.apellido<<", Cedula:"<<p.cedula<<", Edad:"<<p.edad<<", Sexo:"<<p.sexo<<"\n";

 }
void agregarConsultaAlHistorial(Paciente* paciente, HistorialMedico consulta){
    // Verificar si el array está lleno
    if(paciente->cantidadConsultas>=paciente->capacidadHistorial){
    }
}

int main() {
    ofstream archivo("Registro de Hospitales.cpp");

    Hospital* ptr = new Hospital;
    RegistrarHospital(ptr);
    cout << ptr->nombre;

    return 0;
}

