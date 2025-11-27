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
    // Constructores
    Cita();
    Cita(int id, int pacienteID, int doctorID, const char* fecha,
         const char* hora, const char* motivo, const char* estado,
         const char* observaciones, bool atendida);

    // Getters
    int getId() const { return id; }
    int getPacienteID() const { return pacienteID; }
    int getDoctorID() const { return doctorID; }
    const char* getFecha() const { return fecha; }
    const char* getHora() const { return hora; }
    const char* getMotivo() const { return motivo; }
    const char* getEstado() const { return estado; }
    const char* getObservaciones() const { return observaciones; }
    bool getAtendida() const { return atendida; }
    int getConsultaID() const { return consultaID; }
    bool getEliminado() const { return eliminado; }
    time_t getFechaCreacion() const { return fechaCreacion; }
    time_t getFechaModificacion() const { return fechaModificacion; }

    //Setters
    void setId(int nuevoId);
    void setPacienteID(int nuevoPacienteID);
    void setDoctorID(int nuevoDoctorID);
    void setFecha(const char* nuevaFecha);
    void setHora(const char* nuevaHora);
    void setMotivo(const char* nuevoMotivo);
    void setEstado(const char* nuevoEstado);
    void setObservaciones(const char* nuevasObservaciones);
    void setAtendida(bool estadoAtendida);
    void setConsultaID(int nuevoConsultaID);
    void setEliminado(bool estadoEliminado);
    void setFechaCreacion(time_t nuevaFechaCreacion);
    void setFechaModificacion(time_t nuevaFechaModificacion);
};

#endif // CITAS_HPP