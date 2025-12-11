    #include "Doctores.hpp"
    #include "../utilidades/utilidad.hpp"
    #include "../persistencia/GestionArchivos.hpp"
    #include <cstring>
    #include <fstream>
    #include <iostream>

// Constructor por defecto
Doctor::Doctor() {
    // Limpiar cadenas
    memset(nombre, 0, sizeof(nombre));
    memset(apellido, 0, sizeof(apellido));
    memset(cedulaProfesional, 0, sizeof(cedulaProfesional));
    memset(especialidad, 0, sizeof(especialidad));
    memset(horarioAtencion, 0, sizeof(horarioAtencion));
    memset(telefono, 0, sizeof(telefono));
    memset(email, 0, sizeof(email));

    // Valores base
    id = 0;
    aniosExperiencia = 0;
    costoConsulta = 0.0f;
    disponible = false;

    cantidadPacientes = 0;
    cantidadCitas = 0;

    // Inicializar arrays de IDs a -1
    for (int i = 0; i < 50; i++)
        pacientesIDs[i] = -1;

    for (int i = 0; i < 30; i++)
        citasIDs[i] = -1;

    // Metadata
    eliminado = false;
    fechaCreacion = time(nullptr);
    fechaModificacion = time(nullptr);
}


Doctor::Doctor(int id, const char* nombre, const char* apellido, const char* cedulaProfesional,
               const char* especialidad, int aniosExperiencia, float costoConsulta,
               const char* horarioAtencion, const char* telefono, const char* email,
               bool disponible)
{
    this->id = id;
    this->aniosExperiencia = aniosExperiencia;
    this->costoConsulta = costoConsulta;
    this->disponible = disponible;

    // Copia segura de cadenas
    std::strncpy(this->nombre, nombre, sizeof(this->nombre) - 1);
    this->nombre[sizeof(this->nombre) - 1] = '\0';

    std::strncpy(this->apellido, apellido, sizeof(this->apellido) - 1);
    this->apellido[sizeof(this->apellido) - 1] = '\0';

    std::strncpy(this->cedulaProfesional, cedulaProfesional, sizeof(this->cedulaProfesional) - 1);
    this->cedulaProfesional[sizeof(this->cedulaProfesional) - 1] = '\0';

    std::strncpy(this->especialidad, especialidad, sizeof(this->especialidad) - 1);
    this->especialidad[sizeof(this->especialidad) - 1] = '\0';

    std::strncpy(this->horarioAtencion, horarioAtencion, sizeof(this->horarioAtencion) - 1);
    this->horarioAtencion[sizeof(this->horarioAtencion) - 1] = '\0';

    std::strncpy(this->telefono, telefono, sizeof(this->telefono) - 1);
    this->telefono[sizeof(this->telefono) - 1] = '\0';

    std::strncpy(this->email, email, sizeof(this->email) - 1);
    this->email[sizeof(this->email) - 1] = '\0';




    cantidadPacientes = 0;
    cantidadCitas = 0;

    // Limpia arrays de IDs
    for (int i = 0; i < 50; i++) pacientesIDs[i] = -1;
    for (int i = 0; i < 30; i++) citasIDs[i] = -1;

    // Metadata
    eliminado = false;
    fechaCreacion = time(nullptr);
    fechaModificacion = time(nullptr);
}

// Setters
void Doctor::setId(int nuevoId) {
    id = nuevoId;
}

void Doctor::setNombre(const char* nuevoNombre) {
    std::strncpy(nombre, nuevoNombre, sizeof(nombre) - 1);
    nombre[sizeof(nombre) - 1] = '\0';
}

void Doctor::setApellido(const char* nuevoApellido) {
    std::strncpy(apellido, nuevoApellido, sizeof(apellido) - 1);
    apellido[sizeof(apellido) - 1] = '\0';
}

void Doctor::setCedulaProfesional(const char* nuevaCedula) {
    std::strncpy(cedulaProfesional, nuevaCedula, sizeof(cedulaProfesional) - 1);
    cedulaProfesional[sizeof(cedulaProfesional) - 1] = '\0';
}

void Doctor::setEspecialidad(const char* nuevaEspecialidad) {
    std::strncpy(especialidad, nuevaEspecialidad, sizeof(especialidad) - 1);
    especialidad[sizeof(especialidad) - 1] = '\0';
}

void Doctor::setAniosExperiencia(int nuevosAnios) {
    aniosExperiencia = nuevosAnios;
}

void Doctor::setCostoConsulta(float nuevoCosto) {
    costoConsulta = nuevoCosto;
}

void Doctor::setHorarioAtencion(const char* nuevoHorario) {
    std::strncpy(horarioAtencion, nuevoHorario, sizeof(horarioAtencion) - 1);
    horarioAtencion[sizeof(horarioAtencion) - 1] = '\0';
}

void Doctor::setTelefono(const char* nuevoTelefono) {
    std::strncpy(telefono, nuevoTelefono, sizeof(telefono) - 1);
    telefono[sizeof(telefono) - 1] = '\0';
}

void Doctor::setEmail(const char* nuevoEmail) {
    std::strncpy(email, nuevoEmail, sizeof(email) - 1);
    email[sizeof(email) - 1] = '\0';
}

void Doctor::setDisponible(bool nuevoDisponible) {
    disponible = nuevoDisponible;
}

void Doctor::setCantidadPacientes(int nuevaCantidad) {
    this->cantidadPacientes = nuevaCantidad;
}

void Doctor::setCitaID(int index, int citaID) {
    if (index >= 0) {
        this->citasIDs[index] = citaID;
    }
}

void Doctor::setCantidadCitas(int nuevaCantidad) {
    this->cantidadCitas = nuevaCantidad;
}
bool Doctor::setPacienteID( int pacienteID) {
const int MAX_PACIENTES = 30;

    for (int i = 0; i < MAX_PACIENTES; ++i) {
        if (pacientesIDs[i] == -1) {
            pacientesIDs[i] = pacienteID;
            cantidadPacientes++;
            return true;
        }
    }

    return false;
}

void Doctor::setEliminado(bool nuevoEliminado) {
    eliminado = nuevoEliminado;
}

void Doctor::setfechaCreacion(time_t nuevaFecha) {
    fechaCreacion = nuevaFecha;
}

void Doctor::setfechaModificacion(time_t nuevaFecha) {
    fechaModificacion = nuevaFecha;
}
void Doctor::inicializarPaciente(int index) {
    if (index >= 0 && index < 50)
        pacientesIDs[index] = -1;
}

void Doctor::inicializarCita(int index) {
    if (index >= 0 && index < 30)
        citasIDs[index] = -1;
}
bool Doctor::removerPaciente(int idPaciente) {
    for (int i = 0; i < 50; ++i) {
        if (pacientesIDs[i] == idPaciente) {
            pacientesIDs[i] = -1;           // liberar slot
            if (cantidadPacientes > 0)
                cantidadPacientes--;        // actualizar contador
            return true;                    // paciente removido
        }
    }
    return false; // paciente no encontrado
}
    void Doctor::eliminarCitaID(int citaID){
        int slots = sizeof(this->citasIDs) / sizeof(this->citasIDs[0]);
        for(int i = 0; i < slots; ++i){
            if(this->citasIDs[i] == citaID){
                this->citasIDs[i] = -1;
                if(this->cantidadCitas > 0) --this->cantidadCitas;
                fechaModificacion = time(nullptr);
                
                return;
                
            }
        }
    }


Doctor::~Doctor() {
    // No se requiere limpieza especial por uso de arrays fijos
}