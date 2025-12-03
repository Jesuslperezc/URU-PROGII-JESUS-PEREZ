#ifndef OPERACIONES_PACIENTE_HPP
#define OPERACIONES_PACIENTE_HPP
#include <iostream>
#include <iomanip>
#include "../Pacientes/Pacientes.hpp"
#include "../hospital/Hospital.hpp"
#include "../persistencia/GestionArchivos.hpp"
#include "../utilidades/utilidad.hpp"

void mostrarHistorialMedico(Paciente* paciente);
bool obtenerUltimaConsulta(Paciente* paciente, Historial& salida);
void buscarPacientesPorNombre(const char* nombreBuscado);
Paciente buscarPacientePorCedula(const char* nombreArchivo, const char* cedulaBuscada);
bool actualizarPaciente(int id);
void mostrarMenuPacientes(Hospital* hospital);
//Regisrtar nuevo paciente esta en utilidades/utilidad.hpp ((PORQUE USA LA FUNCION GENERICA ESCRIBIRREGISTRO))
#endif