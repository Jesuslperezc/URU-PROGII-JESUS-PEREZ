#include "Hospital.hpp"
#include "../persistencia/GestionArchivos.hpp"
#include "../Doctores/Doctores.hpp"
#include "../Historiales/Historial.hpp"
#include "../Citas/Citas.hpp"
#include "../utilidades/utilidad.hpp"
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
    std:: memset(this->nombre, 0, sizeof(this->nombre));
    std:: strncpy(this->nombre, nombre, sizeof(this->nombre) - 1);
    std:: memset(this->direccion, 0, sizeof(this->direccion));
    std:: strncpy(this->direccion, direccion, sizeof(this->direccion) - 1);
    std:: memset(this->telefono, 0, sizeof(this->telefono));
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


void Hospital::setSiguienteIDPaciente(int id) { siguienteIDPaciente = id; guardarHospital(*this); }
void Hospital::setSiguienteIDDoctor(int id) { siguienteIDDoctor = id;guardarHospital(*this); }
void Hospital::setSiguienteIDCita(int id) { siguienteIDCita = id; guardarHospital(*this); }
void Hospital::setSiguienteIDConsulta(int id) { siguienteIDConsulta = id;guardarHospital(*this); }
void Hospital::setTotalPacientesRegistrados(int total) { totalPacientesRegistrados = total;guardarHospital(*this); }
void Hospital::setTotalDoctoresRegistrados(int total) { totalDoctoresRegistrados = total; guardarHospital(*this); }
void Hospital::setTotalCitasAgendadas(int total) { totalCitasAgendadas = total;guardarHospital(*this); }
void Hospital::setTotalConsultasRealizadas(int total) { totalConsultasRealizadas = total; guardarHospital(*this); }

bool Hospital::cargarHospital() {
    std::fstream archivo("hospital.bin", std::ios::binary | std::ios::in | std::ios::out);
    if (!archivo.is_open()) {
        std::cout << "Error al abrir hospital.bin\n";
        *this = Hospital();  // reinicializa el objeto actual
        return false;
    }

    archivo.seekg(sizeof(ArchivoHeader), std::ios::beg);
    archivo.read(reinterpret_cast<char*>(this), sizeof(Hospital));

    if (archivo.gcount() != sizeof(Hospital)) {
        std::cout << "Hospital vacío, inicializando...\n";
        *this = Hospital();
        escribirRegistro<Hospital>("hospital.bin", *this, 0);
    }

    archivo.close();
    std::cout << "Datos del hospital cargados correctamente: " << getNombre() << "\n";
    return true;
}
