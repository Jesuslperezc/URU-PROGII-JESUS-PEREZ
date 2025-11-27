#ifndef DOCTORES_HPP
#define DOCTORES_HPP
#include <fstream>
class Doctor{
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
    Doctor();
    Doctor( int id, const char* nombre, const char* apellido, const char* cedulaProfesional,
    const char* especialidad, int aniosExperiencia, float costoConsulta, const char* horarioAtencion,
    const char* telefono, const char* email, bool disponible);
   // Getters
    int getId() const { return id; }
    const char* getNombre() const { return nombre; }
    const char* getApellido() const { return apellido; }
    const char* getCedulaProfesional() const { return cedulaProfesional; }
    const char* getEspecialidad() const { return especialidad; }
    int getAniosExperiencia() const { return aniosExperiencia; }
    float getCostoConsulta() const { return costoConsulta; }
    const char* getHorarioAtencion() const { return horarioAtencion; }
    const char* getTelefono() const { return telefono; }
    const char* getEmail() const { return email; }
    bool getDisponible() const { return disponible; }

    //Setters
    // Setters
    void setId(int nuevoId);
    void setNombre(const char* nuevoNombre);
    void setApellido(const char* nuevoApellido);
    void setCedulaProfesional(const char* nuevaCedula);
    void setEspecialidad(const char* nuevaEspecialidad);
    void setAniosExperiencia(int nuevosAnios);
    void setCostoConsulta(float nuevoCosto);
    void setHorarioAtencion(const char* nuevoHorario);
    void setTelefono(const char* nuevoTelefono);
    void setEmail(const char* nuevoEmail);
    void setDisponible(bool nuevoDisponible);

    //Destructor
    ~Doctor();
};
#endif // DOCTORES_HPP