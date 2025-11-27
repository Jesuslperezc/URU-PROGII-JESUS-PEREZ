#ifndef HOSPITAL_HPP
#define HOSPITAL_HPP
#include <fstream>
#include <cstring>
#include <ctime>
#include "../persistencia/GestionArchivos.hpp"
#include "../utilidades/utilidad.hpp"

class Hospital
{
    template<typename T>
    friend bool escribirRegistro(const char* nombreArchivo, const T &registro, int indice);

private:
    char nombre[100];
    char direccion[150];
    char telefono[15];
    
    // Contadores de IDs (auto-increment)
    int siguienteIDPaciente;
    int siguienteIDDoctor;
    int siguienteIDCita;
    int siguienteIDConsulta;
    
    // Estadísticas generales
    int totalPacientesRegistrados;
    int totalDoctoresRegistrados;
    int totalCitasAgendadas;
    int totalConsultasRealizadas;

public:
//Constructor
    Hospital();
    Hospital( const char* nombre, const char* direccion, const char* telefono);

    //Destructor
    ~Hospital();


    //Getters
const char* getNombre() const { return nombre; }
    const char* getDireccion() const { return direccion; }
    const char* getTelefono() const { return telefono; }

    int getSiguienteIDPaciente() const { return siguienteIDPaciente; }
    int getSiguienteIDDoctor() const { return siguienteIDDoctor; }
    int getSiguienteIDCita() const { return siguienteIDCita; }
    int getSiguienteIDConsulta() const { return siguienteIDConsulta; }

    int getTotalPacientesRegistrados() const { return totalPacientesRegistrados; }
    int getTotalDoctoresRegistrados() const { return totalDoctoresRegistrados; }
    int getTotalCitasAgendadas() const { return totalCitasAgendadas; }
    int getTotalConsultasRealizadas() const { return totalConsultasRealizadas; }

    // Incrementadores 
    void incrementarIDPaciente() { siguienteIDPaciente++; }
    void incrementarIDDoctor() { siguienteIDDoctor++; }
    void incrementarIDCita() { siguienteIDCita++; }
    void incrementarIDConsulta() { siguienteIDConsulta++; }

    void aumentarPacientesRegistrados() { totalPacientesRegistrados++; }
    void aumentarDoctoresRegistrados() { totalDoctoresRegistrados++; }
    void aumentarCitasAgendadas() { totalCitasAgendadas++; }
    void aumentarConsultasRealizadas() { totalConsultasRealizadas++; }

    //Setters
    void setSiguienteIDPaciente(int id);
    void setSiguienteIDDoctor(int id);
    void setSiguienteIDCita(int id);
    void setSiguienteIDConsulta(int id);

    void setTotalPacientesRegistrados(int total);
    void setTotalDoctoresRegistrados(int total);
    void setTotalCitasAgendadas(int total);
    void setTotalConsultasRealizadas(int total);
    bool guardarHospital(const Hospital& hospital);
    bool cargarHospital();


};

 #endif