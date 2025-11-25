    #include "Pacientes.hpp"
    #include "../utilidades/utilidad.hpp"
    #include <cstring>
    #include <fstream>
    #include <iostream>

    Paciente::Paciente(){
        memset(nombre, 0, sizeof(nombre));
        memset(apellido, 0, sizeof(apellido));
        memset(cedula, 0, sizeof(cedula));
        memset(tipoSangre, 0, sizeof(tipoSangre));
        memset(telefono, 0, sizeof(telefono));
        memset(direccion, 0, sizeof(direccion));
        memset(email, 0, sizeof(email));
        memset(alergias, 0, sizeof(alergias));
        memset(observaciones, 0, sizeof(observaciones));
        id = 0;
        edad = 0;
        sexo = ' ';
        activo = true;
        cantidadConsultas = 0;
        primerConsultaID = -1;
        cantidadCitas = 0;
        memset(citasIDs, -1, sizeof(citasIDs));
        activo= true;
        eliminado = false;
        fechaCreacion = time(nullptr);
        fechaModificacion = time(nullptr);


    }

    Paciente::Paciente(  int id,const char* nombre, const char* apellido, const char* cedula,
    int edad, char sexo, const char* tipoSangre,const char* telefono, const char* direccion,
    const char* email, const char* alergias, const char* observaciones){

        this->id = id;
        std:: strncpy(this->nombre, nombre, sizeof(this->nombre) - 1);
        std:: strncpy(this->apellido, apellido, sizeof(this->apellido) - 1);
        std:: strncpy(this->cedula, cedula, sizeof(this->cedula) - 1);
        this->edad = edad;
        this->sexo = sexo;
        std:: strncpy(this->tipoSangre, tipoSangre, sizeof(this->tipoSangre) - 1);
        std:: strncpy(this->telefono, telefono, sizeof(this->telefono) - 1);
        std:: strncpy(this->direccion, direccion, sizeof(this->direccion) - 1);
        std:: strncpy(this->email, email, sizeof(this->email) - 1);
        std:: strncpy(this->alergias, alergias, sizeof(this->alergias) - 1);
        std:: strncpy(this->observaciones, observaciones, sizeof(this->observaciones) - 1);
        activo = true;
        cantidadConsultas = 0;
        primerConsultaID = -1;
        cantidadCitas = 0;
        memset(citasIDs, -1, sizeof(citasIDs));
        eliminado = false;
        fechaCreacion = time(nullptr);
        fechaModificacion = time(nullptr);
    }
    bool Paciente::guardarPaciente(const Paciente& paciente) {
        escribirRegistro<Paciente>("pacientes.bin",paciente ,0);
        return true;
    }
    
    
    
    //Setters
    void Paciente::setNombre(const char* nuevoNombre){
        std::strncpy(this->nombre, nuevoNombre, sizeof(this->nombre) - 1);
        this->nombre[sizeof(this->nombre) - 1] = '\0';
        fechaModificacion = time(nullptr);
        guardarPaciente(*this);
    }
    void Paciente::setApellido(const char* nuevoApellido){
        std::strncpy(this->apellido, nuevoApellido, sizeof(this->apellido) - 1);
        this->apellido[sizeof(this->apellido) - 1] = '\0';
        fechaModificacion = time(nullptr);
        guardarPaciente(*this);
    }
    void Paciente::setCedula(const char* nuevaCedula){
        std::strncpy(this->cedula, nuevaCedula, sizeof(this->cedula) - 1);
        this->cedula[sizeof(this->cedula) - 1] = '\0';
        fechaModificacion = time(nullptr);
        guardarPaciente(*this);
    }
    void Paciente::setEdad(int nuevaEdad){
        if(nuevaEdad < 0){
           std:: cout<<"Edad no puede ser negativa."<<std::endl;
           return;
        } 
        this->edad = nuevaEdad;
        fechaModificacion = time(nullptr);
       guardarPaciente(*this);

    }
    void Paciente::setSexo(char nuevoSexo){
        this->sexo = nuevoSexo;
        fechaModificacion = time(nullptr);
        guardarPaciente(*this);
    }
    void Paciente::setTipoSangre(const char* nuevoTipoSangre){
        std::strncpy(this->tipoSangre, nuevoTipoSangre, sizeof(this->tipoSangre) - 1);
        this->tipoSangre[sizeof(this->tipoSangre) - 1] = '\0';
        fechaModificacion = time(nullptr);
        guardarPaciente(*this);
    }
    void Paciente::setTelefono(const char* nuevoTelefono){
        std::strncpy(this->telefono, nuevoTelefono, sizeof(this->telefono) - 1);
        this->telefono[sizeof(this->telefono) - 1] = '\0';
        fechaModificacion = time(nullptr);
        guardarPaciente(*this);
    }
    void Paciente::setDireccion(const char* nuevaDireccion){
        std::strncpy(this->direccion, nuevaDireccion, sizeof(this->direccion) - 1);
        this->direccion[sizeof(this->direccion) - 1] = '\0';
        fechaModificacion = time(nullptr);
        guardarPaciente(*this);
    }
    void Paciente::setEmail(const char* nuevoEmail){
        std::strncpy(this->email, nuevoEmail, sizeof(this->email) - 1);
        this->email[sizeof(this->email) - 1] = '\0';
        fechaModificacion = time(nullptr);
        guardarPaciente(*this);
    }
    void Paciente::setAlergias(const char* nuevasAlergias){
        std::strncpy(this->alergias, nuevasAlergias, sizeof(this->alergias) - 1);
        this->alergias[sizeof(this->alergias) - 1] = '\0';
        fechaModificacion = time(nullptr);
        guardarPaciente(*this);
    }
    void Paciente::setObservaciones(const char* nuevasObservaciones){
        std::strncpy(this->observaciones, nuevasObservaciones, sizeof(this->observaciones) - 1);
        this->observaciones[sizeof(this->observaciones) - 1] = '\0';
        fechaModificacion = time(nullptr);
        guardarPaciente(*this);
    }
    void Paciente::setActivo(bool estado){
        this->activo = estado;
        fechaModificacion = time(nullptr);
        guardarPaciente(*this);}
    void Paciente::setEliminado(bool estado){
        this->eliminado = estado;
        fechaModificacion = time(nullptr);
        guardarPaciente(*this);
    }
    void Paciente::incrementarCantidadConsultas(int cantidad){
        this->cantidadConsultas += cantidad;
        fechaModificacion = time(nullptr);
        guardarPaciente(*this);
    }
    void Paciente::setPrimerConsultaID(int consultaID){
        this->primerConsultaID = consultaID;
        fechaModificacion = time(nullptr);
        guardarPaciente(*this);
    }
    void Paciente::agregarCitaID(int citaID){

        int slots = sizeof(this->citasIDs) / sizeof(this->citasIDs[0]);
        for(int i = 0; i < slots; ++i){
            if(this->citasIDs[i] == -1){
                this->citasIDs[i] = citaID;
                ++this->cantidadCitas;
                fechaModificacion = time(nullptr);
                guardarPaciente(*this);
                return;
                
            }
        }
    }
    void Paciente::eliminarCitaID(int citaID){
        int slots = sizeof(this->citasIDs) / sizeof(this->citasIDs[0]);
        for(int i = 0; i < slots; ++i){
            if(this->citasIDs[i] == citaID){
                this->citasIDs[i] = -1;
                if(this->cantidadCitas > 0) --this->cantidadCitas;
                fechaModificacion = time(nullptr);
                guardarPaciente(*this);
                return;
                
            }
        }
    }