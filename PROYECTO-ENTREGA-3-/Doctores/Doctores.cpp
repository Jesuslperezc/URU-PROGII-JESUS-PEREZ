    #include "Doctores.hpp"
    #include "../utilidades/utilidad.hpp"
    #include "../persistencia/GestionArchivos.hpp"
    #include <cstring>
    #include <fstream>
    #include <iostream>

// Constructor por defecto
Doctor::Doctor() {
    memset(nombre, 0, sizeof(nombre));
    memset(apellido, 0, sizeof(apellido));
    memset(cedulaProfesional, 0, sizeof(cedulaProfesional));
    memset(especialidad, 0, sizeof(especialidad));
    memset(horarioAtencion, 0, sizeof(horarioAtencion));
    memset(telefono, 0, sizeof(telefono));
    memset(email, 0, sizeof(email));

    id = 0;
    aniosExperiencia = 0;
    costoConsulta = 0.0f;
    disponible = false;
}

// Constructor parametrizado
Doctor::Doctor(int id, const char* nombre, const char* apellido, const char* cedulaProfesional,
               const char* especialidad, int aniosExperiencia, float costoConsulta,
               const char* horarioAtencion, const char* telefono, const char* email,
               bool disponible) {
    this->id = id;
    this->aniosExperiencia = aniosExperiencia;
    this->costoConsulta = costoConsulta;
    this->disponible = disponible;

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