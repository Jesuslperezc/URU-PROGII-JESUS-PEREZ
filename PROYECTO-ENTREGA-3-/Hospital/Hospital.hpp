#ifndef HOSPITAL_HPP
#define HOSPITAL_HPP

class Hospital
{
private:
    char nombre[100];
    char direccion[150];
    char telefono[15];
    
    // Contadores de IDs (auto-increment)
    int siguienteIDPaciente;
    int siguienteIDDoctor;
    int siguienteIDCita;
    int siguienteIDConsulta;
    
    // Estadísticas generales
    int totalPacientesRegistrados;
    int totalDoctoresRegistrados;
    int totalCitasAgendadas;
    int totalConsultasRealizadas;

public:
//Constructor
    Hospital();
    Hospital( const char* nombre, const char* direccion, const char* telefono);

    //Destructor
    ~Hospital();
};

Hospital::Hospital(/* args */)
{
}

Hospital::~Hospital()
{
}
 #endif