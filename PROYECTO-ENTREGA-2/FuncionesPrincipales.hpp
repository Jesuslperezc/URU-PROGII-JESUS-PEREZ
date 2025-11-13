#ifndef DECLARACIONES_HPP
#define DECLARACIONES_HPP
#include <iostream>
#include <cstring>
#include "structs.hpp"
using namespace std;
// ============================================
// DECLARACIONES DE FUNCIONES PACIENTES
// ============================================
bool validarCedula(const char* cedula);
bool validarNombreSinEspacios(const char* nombre);
bool validarEdad(int edad);
bool validarSexoChar(char sexo);
void RegistrarHospital(Hospital* hospital);
bool compararCaseInsensitive(const char* a, const char* b);
bool validarFormatoHora(const char* hora);
Paciente* crearPaciente(Hospital* hospital, const char* nombre,
                        const char* apellido, const char* cedula, int edad, char sexo);
bool eliminarPaciente(int id);
bool actualizarPaciente(int id);
bool agregarConsultaAlHistorial(int pacienteID, HistorialMedico nuevaConsulta);
bool guardarHospital(const Hospital& hospital);
HistorialMedico* leerHistorialCompleto(int pacienteID, int* cantidad);
Doctor crearDoctor(Hospital* hospital, const char* nombre,
                   const char* apellido, const char* cedula,
                   const char* especialidad, int aniosExperiencia,
                   float costoConsulta);
bool asignarPacienteADoctor(int idDoctor, int idPaciente);
bool removerPacienteDeDoctor(Doctor* doctor, int idPaciente);
void listarDoctores(Hospital* hospital);
bool validarFormatoFecha(const char* fecha);
void mostrarHistorialMedico(Paciente* paciente);
bool obtenerUltimaConsulta(Paciente* paciente, HistorialMedico& salida);
Cita agendarCita(Hospital* hospital, int idPaciente, int idDoctor,
                 const char* fecha, const char* hora, const char* motivo);
void listarPacientesDeDoctor(Hospital* hospital, int idDoctor);
bool atenderCita(Hospital* hospital, int idCita, const char* diagnostico,
                 const char* tratamiento, const char* medicamentos);
Cita* leerCitasDePaciente(int pacienteID, int* cantidad);
Hospital* cargarDatosHospital();
bool cancelarCita(Hospital* hospital, int idCita);
void obtenerCitasDeDoctor(int idDoctor);
void obtenerCitasPorFecha(const char* fechaBuscada);
void listarCitasPendientes();
Paciente buscarPacientePorCedula(const char* nombreArchivo, const char* cedulaBuscada);
void buscarPacientesPorNombre(const char* nombreBuscado);
void buscarDoctoresPorEspecialidad(const char* nombreBuscado);
bool eliminarDoctor(int id);
bool verificarDisponibilidad(Hospital* hospital, int idDoctor, const char* fecha, const char* hora);
Cita* leerCitasDoctor(int doctorID, int* cantidad);
Cita* obtenerCitasPorFecha(const char* fechaBuscada, int* cantidad);
Hospital* RegistrarHospital();
#endif // DECLARACIONES_HPP 