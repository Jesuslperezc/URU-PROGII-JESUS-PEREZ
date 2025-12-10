#ifndef OPERACIONCITA_HPP
#define OPERACIONCITA_HPP
#include "Citas/Citas.hpp"
#include "../persistencia/GestionArchivos.hpp"
#include "../utilidades/utilidad.hpp"
#include "../Doctores/Doctores.hpp"
#include "../Pacientes/Pacientes.hpp"
#include <iostream>
#include <iomanip>
#include <fstream>

Cita agendarCita(Hospital* hospital, int idPaciente, int idDoctor,
                 const char* fecha, const char* hora, const char* motivo);

Cita* leerCitasDePaciente(int pacienteID, int* cantidad);
bool cancelarCita(Hospital* hospital, int idCita);
void obtenerCitasPorFecha(const char* fechaBuscada);
void listarCitasPendientes();
bool atenderCita(Hospital* hospital, int idCita, const char* diagnostico,
                 const char* tratamiento, const char* medicamentos);

void obtenerCitasDeDoctor(int idDoctor);
void obtenerCitasDePaciente(int idPaciente);









                           //   do {
                             ////cout << "\n=======================================\n"
                             //cout << "||        GESTION DE CITAS           ||\n";
                                //////////cout// << "=======================================\n";
                                //cout// //<< "1. Agendar nueva cita\n";
                                //cout << "2. Cancelar cita\n";
                                //cout << "3. Atender cita\n";
                                //cout << "4. Ver citas de un paciente\n";
                                //cout << "5. Ver citas de un doctor\n";
                                //cout << "6. Ver citas de una fecha\n";
                                //cout << "7. Ver citas pendientes\n";
                                //cout << "0. Volver al menu principal\n";
                                //cout << "Seleccione una opcion: ";
                                //cin //>> opCita;
            










#endif // OPERACIONCITA_HPP