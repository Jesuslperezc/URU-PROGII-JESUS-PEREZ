#ifndef GESTIONARCHIVOS_HPP
#define GESTIONARCHIVOS_HPP
#include <fstream>
// Forward declarations to avoid circular includes and 'does not name a type' errors
class Hospital;
class Paciente;
class Cita;
class Historial;
class Doctor;
struct ArchivoHeader {
    int cantidadRegistros;      // Cantidad actual de registros
    int proximoID;              // Siguiente ID disponible
    int registrosActivos;       // Registros no eliminados
    int version;                // Versión del formato (ej: 1, 2, 3...)
};
class GestorArchivos{
private:
bool inicializarArchivo(const char* nombreArchivo);

public:
bool verificarArchivo(const char* nombreArchivo);
// Lee el header desde el archivo; devuelve true si tuvo éxito y llena 'header'.
bool leerArchivoHeader(const char* nombreArchivo, ArchivoHeader& header);
bool actualizarHeader(const char* nombreArchivo, ArchivoHeader header);
bool asegurarArchivo(const char* nombreArchivo);
void verificarArchivos();
bool guardarPaciente(const Paciente& paciente);
bool guardarHospital(const Hospital& hospital);
bool cargarHospital();
bool guardarCita(const Cita& cita);
bool guardarHistorial(const Historial& historial);
bool guardarDoctores (const Doctor& doctor);
void mostrarEstadisticasArchivos();
void hacerRespaldo();
void restaurarRespaldo();
bool compararCaseInsensitive(const char* a, const char* b);

};

#endif // GESTIONARCHIVOS_HPP
