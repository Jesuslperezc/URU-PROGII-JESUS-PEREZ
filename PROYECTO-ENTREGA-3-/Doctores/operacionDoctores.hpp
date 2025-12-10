
#ifndef OPERACIONDOCTORES_HPP
#define OPERACIONDOCTORES_HPP
#include "Doctores.hpp"
#include "../persistencia/GestionArchivos.hpp"
#include "../utilidades/utilidad.hpp"
#include <iostream>
#include <cstring>


void buscarDoctoresPorEspecialidad(const char* nombreBuscado);
Doctor crearDoctor(Hospital* hospital, const char* nombre,
                   const char* apellido, const char* cedula,
                   const char* especialidad, int aniosExperiencia,
                   float costoConsulta);
bool asignarPacienteADoctor(int idDoctor, int idPaciente);
bool removerPacienteDeDoctor(Doctor* doctor, int idPaciente);
void listarDoctores();
void listarPacientesDeDoctor( int idDoctor);
void obtenerCitasDeDoctor(int idDoctor);
bool eliminarDoctor(int id);
void mostrarMenuDoctor(Hospital* hospital);


#endif // OPERACIONDOCTORES_HPP