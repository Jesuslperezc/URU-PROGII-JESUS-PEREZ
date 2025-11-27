    #include "../Pacientes/Pacientes.hpp"
    #include "../utilidades/utilidad.hpp"
    #include "Historial.hpp"
    #include <cstring>
    #include <fstream>
    #include <iostream>
    #include <cstring>
    #include <ctime>

// Constructor por defecto
Cita::Cita() {
    memset(fecha, 0, sizeof(fecha));
    memset(hora, 0, sizeof(hora));
    memset(motivo, 0, sizeof(motivo));
    memset(estado, 0, sizeof(estado));
    memset(observaciones, 0, sizeof(observaciones));

    id = 0;
    pacienteID = 0;
    doctorID = 0;
    consultaID = -1;       // por defecto no atendida
    atendida = false;
    eliminado = false;

    fechaCreacion = std::time(nullptr);
    fechaModificacion = fechaCreacion;
}

// Constructor parametrizado
Cita::Cita(int id, int pacienteID, int doctorID, const char* fecha,
           const char* hora, const char* motivo, const char* estado,
           const char* observaciones, bool atendida) {
    this->id = id;
    this->pacienteID = pacienteID;
    this->doctorID = doctorID;

    std::strncpy(this->fecha, fecha, sizeof(this->fecha) - 1);
    this->fecha[sizeof(this->fecha) - 1] = '\0';

    std::strncpy(this->hora, hora, sizeof(this->hora) - 1);
    this->hora[sizeof(this->hora) - 1] = '\0';

    std::strncpy(this->motivo, motivo, sizeof(this->motivo) - 1);
    this->motivo[sizeof(this->motivo) - 1] = '\0';

    std::strncpy(this->estado, estado, sizeof(this->estado) - 1);
    this->estado[sizeof(this->estado) - 1] = '\0';

    std::strncpy(this->observaciones, observaciones, sizeof(this->observaciones) - 1);
    this->observaciones[sizeof(this->observaciones) - 1] = '\0';

    this->atendida = atendida;
    this->consultaID = -1;   // aún no atendida
    this->eliminado = false;

    this->fechaCreacion = std::time(nullptr);
    this->fechaModificacion = fechaCreacion;
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