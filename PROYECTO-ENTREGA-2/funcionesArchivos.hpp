#ifndef FUNCIONESARCHIVOS_HPP
#define FUNCIONESARCHIVOS_HPP

#include <iostream>
#include <fstream>
#include "structs.hpp"
using namespace std;

// ============================================
// DECLARACIONES DE FUNCIONES DE ARCHIVOS BINARIOS
// ============================================

bool inicializarArchivo(const char* nombreArchivo);
bool verificarArchivo(const char* nombreArchivo);
ArchivoHeader leerHeader(const char* nombreArchivo);
bool actualizarHeader(const char* nombreArchivo, ArchivoHeader header);
bool asegurarArchivo(const char* nombreArchivo);

#endif // FUNCIONESARCHIVOS_HPP
