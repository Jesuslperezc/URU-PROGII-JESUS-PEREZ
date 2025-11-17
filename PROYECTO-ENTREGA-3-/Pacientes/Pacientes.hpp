#ifndef PACIENTES_HPP
#define PACIENTES_HPP

#include <fstream>
#include <ctime>

class Pacientes
{
private:
    int id;
    char nombre[50];
    char apellido[50];
    char cedula[20];
    int edad;
    char sexo;
    char tipoSangre[5];
    char telefono[15];
    char direccion[100];
    char email[50];
    char alergias[500];
    char observaciones[500];
    bool activo;
    
    // NUEVO: Índices para relaciones
    int cantidadConsultas;          // Total de consultas en historial
    int primerConsultaID;           // ID de primera consulta en historiales.bin
    
    int cantidadCitas;              // Total de citas agendadas
    int citasIDs[20];               // Array FIJO de IDs de citas (máx 20)
    
    // Metadata de registro
    bool eliminado;                 // Flag para borrado lógico
    time_t fechaCreacion;
    time_t fechaModificacion;

public:
    //Constructor
    Pacientes();

    Pacientes(  int id,const char* nombre, const char* apellido, const char* cedula,
    int edad, char sexo, const char* tipoSangre,const char* telefono, const char* direccion,
    const char* email, const char* alergias, const char* observaciones);

    //Destructor
    ~Pacientes();
};

Pacientes::Pacientes(/* args */)
{
}

Pacientes::~Pacientes()
{
}

    

#endif
