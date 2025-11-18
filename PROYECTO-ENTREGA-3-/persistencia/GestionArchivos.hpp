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
#endif // GESTIONARCHIVOS_HPP
