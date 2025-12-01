    #include "../Pacientes/Pacientes.hpp"
    #include "../utilidades/utilidad.hpp"
    #include "Historial.hpp"
    #include <cstring>
    #include <fstream>
    #include <iostream>
    #include <cstring>
    #include <ctime>

// Constructor por defecto
Historial::Historial() {
    id = 0;
    pacienteID = 0;
    std::memset(fecha, 0, sizeof(fecha));
    std::strncpy(fecha, "0000-00-00", sizeof(fecha) - 1);
    std::memset(hora, 0, sizeof(hora));
    std::strncpy(hora, "00:00", sizeof(hora) - 1);
    std::memset(diagnostico, 0, sizeof(diagnostico));
    std::strncpy(diagnostico, "", sizeof(diagnostico) - 1);
    std::memset(tratamiento, 0, sizeof(tratamiento));
    std::strncpy(tratamiento, "", sizeof(tratamiento) - 1);
    std::memset(medicamentos, 0, sizeof(medicamentos));
    std::strncpy(medicamentos, "", sizeof(medicamentos) - 1);
    doctorID = 0;
    costo = 0.0f;
    siguienteConsultaID = -1; // Indica que no hay siguiente consulta
    eliminado = false;
    fechaRegistro = std::time(nullptr);
}
 Historial::Historial( int id,int siguienteConsultaID,int pacienteID, int doctorID, const char* fecha, const char* hora,
    const char* diagnostico, const char* tratamiento, const char* medicamentos, float costo){
    this->id = 0; // Se asignará al guardar
    this->pacienteID = pacienteID;
    std::strncpy(this->fecha, fecha, sizeof(this->fecha) - 1);
    this->fecha[sizeof(this->fecha) - 1] = '\0';
    std::strncpy(this->hora, hora, sizeof(this->hora) - 1);
    this->hora[sizeof(this->hora) - 1] = '\0';
    std::strncpy(this->diagnostico, diagnostico, sizeof(this->diagnostico) - 1);
    this->diagnostico[sizeof(this->diagnostico) - 1] = '\0';
    std::strncpy(this->tratamiento, tratamiento, sizeof(this->tratamiento) -
        1);
    this->tratamiento[sizeof(this->tratamiento) - 1] = '\0';
    std::strncpy(this->medicamentos, medicamentos, sizeof(this->medicamentos) - 1);
    this->medicamentos[sizeof(this->medicamentos) - 1] = '\0';
    this->doctorID = doctorID;
    this->costo = costo;
    this->siguienteConsultaID = siguienteConsultaID;
    this->eliminado = false;
    this->fechaRegistro = std::time(nullptr);
    
    }
   // Setters
    void Historial::setHistorialID(int nuevoID) { id = nuevoID; }
    void Historial::setPacienteID(int nuevoPacienteID) { pacienteID = nuevoPacienteID; }

    void Historial::setFecha(const char* nuevaFecha) {std::strncpy(fecha, nuevaFecha, sizeof(fecha) - 1);
    fecha[sizeof(fecha) - 1] = '\0'; }

    void Historial::setHora(const char* nuevaHora) {std::strncpy(hora, nuevaHora, sizeof(hora) - 1);
    hora[sizeof(hora) - 1] = '\0'; }
    void Historial::setDiagnostico(const char* nuevoDiagnostico) {std::strncpy(diagnostico, nuevoDiagnostico, sizeof(diagnostico) - 1);
    diagnostico[sizeof(diagnostico) - 1] = '\0';
    }

    void Historial::setTratamiento(const char* nuevoTratamiento) {std::strncpy(tratamiento, nuevoTratamiento, sizeof(tratamiento) - 1);
    tratamiento[sizeof(tratamiento) - 1] = '\0';
    }

    void Historial::setMedicamentos(const char* nuevosMedicamentos) {std::strncpy(medicamentos, nuevosMedicamentos, sizeof(medicamentos) - 1);
    medicamentos[sizeof(medicamentos) - 1] = '\0';
    }