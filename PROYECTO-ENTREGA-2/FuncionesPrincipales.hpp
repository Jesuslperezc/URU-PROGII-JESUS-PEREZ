#ifndef DECLARACIONES_HPP
#define DECLARACIONES_HPP
#include <iostream>
#include <cstring>
#include "structs.hpp"
using namespace std;
// ============================================
// DECLARACIONES DE FUNCIONES PACIENTES
// ============================================
bool validarSexoChar(char sexo);
bool validarEdad(int edad);
bool validarNombreSinEspacios(const char* nombre);
bool validarCedula(const char* cedula);
void RegistrarHospital(Hospital* hospital);
Paciente buscarPacientePorID(int id);
bool agregarPaciente(Paciente nuevoPaciente);
bool actualizarPaciente(int id);
bool eliminarPaciente(int id);
void listarPacientes();
Paciente* crearPaciente(Hospital* hospital, const char* nombre, const char* apellido, const char* cedula, int edad, char sexo);

bool compararCaseInsensitive(const char* a, const char* b);

#endif // DECLARACIONES_HPP 