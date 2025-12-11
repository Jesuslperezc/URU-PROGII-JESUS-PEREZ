#ifndef OPERACIONCITA_HPP
#define OPERACIONCITA_HPP
#include "Citas.hpp"
#include "../persistencia/GestionArchivos.hpp"
#include "../utilidades/utilidad.hpp"
#include "../Doctores/Doctores.hpp"
#include "../Pacientes/Pacientes.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>
#include <cstring>

Cita agendarCita(Hospital* hospital, int idPaciente, int idDoctor,
                 const char* fecha, const char* hora, const char* motivo);

bool eliminarCita( int idCita);
void obtenerCitasPorFecha(const char* fechaBuscada);
void listarCitasPendientes();
bool atenderCita(Hospital* hospital, int idCita, const char* diagnostico,
   const char* tratamiento, const char* medicamentos);

void obtenerCitasDeDoctor(int idDoctor);
void obtenerCitasDePaciente(int idPaciente);

void mostrarMenuCitas(Hospital* hospital) ;
            
#endif // OPERACIONCITA_HPP