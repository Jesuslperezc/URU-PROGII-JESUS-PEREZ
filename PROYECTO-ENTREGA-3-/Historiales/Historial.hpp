#ifndef HISTORIAL_HPP
#define HISTORIAL_HPP
#include <fstream>
#include <ctime>
class Historial{
private: 
 // Datos originales
    int id;
    int pacienteID;                 // NUEVO: Referencia al paciente
    char fecha[11];
    char hora[6];
    char diagnostico[200];
    char tratamiento[200];
    char medicamentos[150];
    int doctorID;
    float costo;
    
    // NUEVO: Navegación enlazada
    int siguienteConsultaID;        // ID de siguiente consulta del mismo paciente
                                    // -1 si es la última
    
    // Metadata
    bool eliminado;
    time_t fechaRegistro;
public:
    //Constructor
    Historial();
    Historial( int id, int pacienteID, const char* fecha, const char* hora,
    const char* diagnostico, const char* tratamiento, const char* medicamentos);
    //Getters}
    int gethistorialID(){return id;}
    int getpacienteID(){return pacienteID;}
    const char* getFecha(){return fecha;}
    const char* getHora(){return hora;}
    const char* getDiagnostico(){return diagnostico;}
    const char* getTratamiento () {return tratamiento;}
    const char* getMedicamentos(){return medicamentos;}
 
   // Setters
    void setHistorialID(int nuevoID);
    void setPacienteID(int nuevoPacienteID);
    void setFecha(const char* nuevaFecha);

    void setHora(const char* nuevaHora);
    void setDiagnostico(const char* nuevoDiagnostico);

    void setTratamiento(const char* nuevoTratamiento);

    void setMedicamentos(const char* nuevosMedicamentos);

    //Destructor
    ~Historial();
};
#endif // HISTORIAL_HPP