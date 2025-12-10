#include "operacionCita.hpp"
#include "../Hospital/Hospital.hpp"
#include "../persistencia/GestionArchivos.hpp"
#include "../utilidades/utilidad.hpp"
#include <iostream>
#include <cstring>

void obtenerCitasDeDoctor(int idDoctor) {
    Doctor doctor = buscarRegistroPorID<Doctor>("doctores.bin", idDoctor);
    if (doctor.getId() == 0) {
        std::cout << "Error: Doctor no encontrado.\n";
        return;
    }

    if (doctor.getCantidadCitas() == 0) {
        std::cout << "El doctor no tiene citas registradas.\n";
        return;
    }

    std::cout << "\n=== Citas del Doctor " << doctor.getNombre() << " ===\n";

    for (int i = 0; i < doctor.getCantidadCitas(); i++) {
        int idCita = doctor.getCitaID(i);
        Cita cita = buscarRegistroPorID<Cita>("citas.bin", idCita);

        if (cita.getId() == 0) {
            std::cout << "[Aviso] Cita con ID " << idCita 
                      << " no encontrada en archivo.\n";
            continue;
        }

        std::cout << "ID: " << cita.getId()
                  << " | Fecha: " << cita.getFecha()
                  << " | Hora: " << cita.getHora()
                  << " | Paciente ID: " << cita.getPacienteID()
                  << " | Estado: " << cita.getEstado() << "\n";
    }
}

void obtenerCitasDePaciente(int idPaciente) {
    Paciente paciente = buscarRegistroPorID<Paciente>("pacientes.bin", idPaciente);

    if (paciente.getId() == 0) {
        std::cout << "Error: Paciente no encontrado.\n";
        return;
    }

    if (paciente.getCantidadCitas() == 0) {
        std::cout << "El paciente no tiene citas registradas.\n";
        return;
    }

    std::cout << "\n=== Citas del Paciente " << paciente.getNombre() << " ===\n";

    int* citas = paciente.getCitasIDs(); // más claro
    for (int i = 0; i < paciente.getCantidadCitas(); i++) {
        int idCita = citas[i];           // acceso correcto a arreglo
        Cita cita = buscarRegistroPorID<Cita>("citas.bin", idCita);

        if (cita.getId() != 0) {
            std::cout << "ID: " << cita.getId()
                      << " | Fecha: " << cita.getFecha()
                      << " | Hora: " << cita.getHora()
                      << " | Doctor ID: " << cita.getDoctorID()
                      << " | Estado: " << cita.getEstado() << "\n";
        }
    }
}
