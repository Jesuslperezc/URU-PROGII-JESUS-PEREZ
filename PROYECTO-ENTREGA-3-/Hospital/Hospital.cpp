#include "Hospital.hpp"
#include "Pacientes.hpp"
#include "GestionArchivos.hpp"
#include "Doctores.hpp"
#include "Historial.hpp"
#include "Citas.hpp"
#include "Utilidades.hpp"
#include <iostream>
#include <cstring>
#include <vector>
#include <ctime>
#include <iomanip>
#include <limits>

Hospital::Hospital() {
    std:: memset(nombre, 0, sizeof(nombre));
    std:: memset(direccion, 0, sizeof(direccion));
    std:: memset(telefono, 0, sizeof(telefono));
    siguienteIDPaciente = 1;
    siguienteIDDoctor = 1;
    siguienteIDCita = 1;
    siguienteIDConsulta = 1;
    totalPacientesRegistrados = 0;
    totalDoctoresRegistrados = 0;
    totalCitasAgendadas = 0;
    totalConsultasRealizadas = 0;
}

Hospital::Hospital(const char* nombre, const char* direccion, const char* telefono) {
    std:: strncpy(this->nombre, nombre, sizeof(this->nombre) - 1);
    std:: strncpy(this->direccion, direccion, sizeof(this->direccion) - 1);
    std:: strncpy(this->telefono, telefono, sizeof(this->telefono) - 1);
    siguienteIDPaciente = 1;
    siguienteIDDoctor = 1;
    siguienteIDCita = 1;
    siguienteIDConsulta = 1;
    totalPacientesRegistrados = 0;
    totalDoctoresRegistrados = 0;
    totalCitasAgendadas = 0;
    totalConsultasRealizadas = 0;
}

bool guardarHospital(const Hospital& hospital) {
 escribirRegistro<Hospital>("hospital.bin", hospital, 0);
    return true;
}



void Hospital::setSiguienteIDPaciente(int id) { siguienteIDPaciente = id;}
void Hospital::setSiguienteIDDoctor(int id) { siguienteIDDoctor = id; }
void Hospital::setSiguienteIDCita(int id) { siguienteIDCita = id; }
void Hospital::setSiguienteIDConsulta(int id) { siguienteIDConsulta = id; }
void Hospital::setTotalPacientesRegistrados(int total) { totalPacientesRegistrados = total; }
void Hospital::setTotalDoctoresRegistrados(int total) { totalDoctoresRegistrados = total; }
void Hospital::setTotalCitasAgendadas(int total) { totalCitasAgendadas = total; }
void Hospital::setTotalConsultasRealizadas(int total) { totalConsultasRealizadas = total; }
