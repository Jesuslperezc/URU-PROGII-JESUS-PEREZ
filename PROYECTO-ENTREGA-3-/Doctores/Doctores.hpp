#ifndef DOCTORES_HPP
#define DOCTORES_HPP
#include <fstream>
class Doctores{
private:
// Datos originales
    int id;
    char nombre[50];
    char apellido[50];
    char cedulaProfesional[20];
    char especialidad[50];
    int aniosExperiencia;
    float costoConsulta;
    char horarioAtencion[50];
    char telefono[15];
    char email[50];
    bool disponible;
    
    // NUEVO: Relaciones con arrays fijos
    int cantidadPacientes;
    int pacientesIDs[50];           // Máximo 50 pacientes
    
    int cantidadCitas;
    int citasIDs[30];               // Máximo 30 citas
    
    // Metadata
    bool eliminado;
    time_t fechaCreacion;
    time_t fechaModificacion;
public:
    //Constructor
    Doctores();
    Doctores( int id, const char* nombre, const char* apellido, const char* cedulaProfesional,
    const char* especialidad, int aniosExperiencia, float costoConsulta, const char* horarioAtencion,
    const char* telefono, const char* email, bool disponible);
    //Destructor
    ~Doctores();
};
#endif // DOCTORES_HPP