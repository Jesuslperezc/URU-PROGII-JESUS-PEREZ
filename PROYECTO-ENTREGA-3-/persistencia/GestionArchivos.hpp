#ifndef GESTIONARCHIVOS_HPP
#define GESTIONARCHIVOS_HPP
#include <fstream>
struct ArchivoHeader {
    int cantidadRegistros;      // Cantidad actual de registros
    int proximoID;              // Siguiente ID disponible
    int registrosActivos;       // Registros no eliminados
    int version;                // Versión del formato (ej: 1, 2, 3...)
};
bool inicializarArchivo(const char* nombreArchivo);
bool verificarArchivo(const char* nombreArchivo);
// Lee el header desde el archivo; devuelve true si tuvo éxito y llena 'header'.
bool leerArchivoHeader(const char* nombreArchivo, ArchivoHeader& header);
bool actualizarHeader(const char* nombreArchivo, ArchivoHeader header);
bool asegurarArchivo(const char* nombreArchivo);
void verificarArchivos();

#endif // GESTIONARCHIVOS_HPP
