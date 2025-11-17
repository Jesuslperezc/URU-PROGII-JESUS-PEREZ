#ifndef CITAS_HPP
#define CITAS_HPP
#include <fstream>
#include <ctime>
class Cita{
private:
int id;
    int pacienteID;
    int doctorID;
    char fecha[11];
    char hora[6];
    char motivo[150];
    char estado[20];                // "Agendada", "Atendida", "Cancelada"
    char observaciones[200];
    bool atendida;
    
    // NUEVO: Referencia al historial
    int consultaID;                 // ID de consulta creada al atender
                                    // -1 si no ha sido atendida
    
    // Metadata
    bool eliminado;
    time_t fechaCreacion;
    time_t fechaModificacion;
public:
    //Constructor
    Cita();
    Cita( int id, int pacienteID, int doctorID, const char* fecha,
    const char* hora, const char* motivo, const char* estado,
    const char* observaciones, bool atendida);
    //Destructor
    ~Cita();
};
#endif // CITAS_HPP