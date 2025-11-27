#ifndef PACIENTES_HPP
#define PACIENTES_HPP
#include <fstream>
#include <ctime>
#include <cstring>

class Paciente
{

    template<typename T>
    friend bool escribirRegistro(const char* nombreArchivo, const T &registro, int indice);
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
    Paciente();

    Paciente(  int id,const char* nombre, const char* apellido, const char* cedula,
    int edad, char sexo, const char* tipoSangre,const char* telefono, const char* direccion,
    const char* email, const char* alergias, const char* observaciones);

    //Getters
    const char* getNombre(){return nombre;}
    const char* getApellido(){return apellido;}
    const char* getCedula(){return cedula;}
    int getId()const{return id;}
    bool isActivo(){return activo;}
    bool isEliminado(){return eliminado;}
    int getCantidadConsultas(){return cantidadConsultas;}
    const char* getAlergias(){return alergias;}
    const char* getObservaciones(){return observaciones;}
    const char* getTelefono(){return telefono;}
    const char* getEmail(){return email;}
    int getCantidadCitas(){return cantidadCitas;}
    int* getCitasIDs(){return citasIDs;}
    int getEdad(){return edad;}
    char getSexo(){return sexo;}
    const char* getTipoSangre(){return tipoSangre;}
    //Setters

    void setNombre(const char* nuevoNombre);
    void setApellido(const char* nuevoApellido);
    void setCedula(const char* nuevaCedula);
    void setEdad(int nuevaEdad);
    void setSexo(char nuevoSexo);
    void setTipoSangre(const char* nuevoTipoSangre);
    void setTelefono(const char* nuevoTelefono);
    void setDireccion(const char* nuevaDireccion);
    void setEmail(const char* nuevoEmail);
    void setAlergias(const char* nuevasAlergias);
    void setObservaciones(const char* nuevasObservaciones);
    void setActivo(bool estado);
    void setEliminado(bool estado);
    void incrementarCantidadConsultas(int cantidad);
    void setPrimerConsultaID(int consultaID);
    void agregarCitaID(int citaID);
    void eliminarCitaID(int citaID);




    //Destructor
    ~Paciente();
};



#endif
