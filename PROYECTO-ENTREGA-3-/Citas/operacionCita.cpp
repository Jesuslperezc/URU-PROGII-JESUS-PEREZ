#include "operacionCita.hpp"
#include "../Hospital/Hospital.hpp"
#include "../persistencia/GestionArchivos.hpp"
#include "../utilidades/utilidad.hpp"
#include "../Historiales/Historial.hpp"
#include <limits>
#include <iostream>
#include <cstring>

void obtenerCitasDeDoctor(int idDoctor) {
    Doctor doctor = buscarRegistroPorID<Doctor>("doctores.bin", idDoctor);
    if (doctor.getId() == 0) {
        std:: cout << "Error: Doctor no encontrado.\n";
        return;
    }

    if (doctor.getCantidadCitas() == 0) {
        std:: cout << "El doctor no tiene citas registradas.\n";
        return;
    }

   std:: cout << "\n=== Citas del Doctor " << doctor.getNombre() << " ===\n";

    for (int i = 0; i < doctor.getCantidadCitas(); i++) {
        int idCita = doctor.getCitaID(i);
        Cita cita = buscarRegistroPorID<Cita>("citas.bin", idCita);

        if (cita.getId() == 0) {
            std:: cout << "[Aviso] Cita con ID " << idCita 
                      << " no encontrada en archivo.\n";
            continue;
        }

        std:: cout << "ID: " << cita.getId()
                  << " | Fecha: " << cita.getFecha()
                  << " | Hora: " << cita.getHora()
                  << " | Paciente ID: " << cita.getPacienteID()
                  << " | Estado: " << cita.getEstado() << "\n";
    }
}

void obtenerCitasDePaciente(int idPaciente) {
    Paciente paciente = buscarRegistroPorID<Paciente>("pacientes.bin", idPaciente);

    if (paciente.getId() == 0) {
        std:: cout << "Error: Paciente no encontrado.\n";
        return;
    }

    if (paciente.getCantidadCitas() == 0) {
        std:: cout << "El paciente no tiene citas registradas.\n";
        return;
    }

    std:: cout << "\n=== Citas del Paciente " << paciente.getNombre() << " ===\n";

    int* citas = paciente.getCitasIDs(); // más claro
    for (int i = 0; i < paciente.getCantidadCitas(); i++) {
        int idCita = citas[i];           // acceso correcto a arreglo
        Cita cita = buscarRegistroPorID<Cita>("citas.bin", idCita);

        if (cita.getId() != 0) {
            std:: cout << "ID: " << cita.getId()
                      << " | Fecha: " << cita.getFecha()
                      << " | Hora: " << cita.getHora()
                      << " | Doctor ID: " << cita.getDoctorID()
                      << " | Estado: " << cita.getEstado() << "\n";
        }
    }
}

Cita agendarCita(Hospital* hospital, int idPaciente, int idDoctor,
                 const char* fecha, const char* hora, const char* motivo)
{
    GestorArchivos gestor;

    ArchivoHeader headerCitas;
    gestor.leerArchivoHeader("citas.bin", headerCitas);

    Paciente paciente = buscarRegistroPorID<Paciente>("pacientes.bin", idPaciente);
    Doctor doctor     = buscarRegistroPorID<Doctor>("doctores.bin", idDoctor);

    if (paciente.getId() == 0 || paciente.isEliminado()) {
        std::cout << "Paciente no encontrado.\n";
        return {};
    }
    if (doctor.getId() == 0 || doctor.isEliminado()) {
        std::cout << "Doctor no encontrado.\n";
        return {};
    }

    if (!gestor.validarFormatoFecha(fecha)) {
        std::cout << "Formato de fecha inválido.\n";
        return {};
    }
    if (!gestor.validarFormatoHora(hora)) {
        std::cout << "Formato de hora inválido.\n";
        return {};
    }

    // ============================
    //  CREAR CITA
    // ============================

    Cita nuevaCita;

    nuevaCita.setId(headerCitas.proximoID);
    nuevaCita.setPacienteID(idPaciente);
    nuevaCita.setDoctorID(idDoctor);

    nuevaCita.setFecha(fecha);
    nuevaCita.setHora(hora);
    nuevaCita.setMotivo(motivo);

    nuevaCita.setEstado("Agendada");
    nuevaCita.setObservaciones("");

    nuevaCita.setAtendida(false);
    nuevaCita.setConsultaID(-1);
    nuevaCita.setEliminado(false);

    time_t ahora = time(nullptr);
    nuevaCita.setFechaCreacion(ahora);
    nuevaCita.setFechaModificacion(ahora);

    // ============================
    //   GUARDAR EN FILE
    // ============================

    escribirRegistro<Cita>("citas.bin", nuevaCita, headerCitas.cantidadRegistros);

    std::cout << "Cita agendada exitosamente con ID: " << nuevaCita.getId() << "\n";

    // ============================
    //  ACTUALIZAR PACIENTE
    // ============================

    if (paciente.getCantidadCitas() < 20) {

        int idx = paciente.getCantidadCitas();
        int* citas = paciente.getCitasIDs();

        citas[idx] = nuevaCita.getId();
        paciente.setCantidadCitas(idx + 1);
        paciente.setFechaModificacion(time(nullptr));

        int idxPaciente = encontrarIndicePorID<Paciente>("pacientes.bin", idPaciente);
        escribirRegistro<Paciente>("pacientes.bin", paciente, idxPaciente);
    }

    return nuevaCita;
}

bool cancelarCita(Hospital* hospital, int idCita){


    int indiceCita = encontrarIndicePorID<Cita>("citas.bin", idCita);
    if (indiceCita == -1) {
        std::cout << "Cita no encontrada.\n";
        return false;
    }

    Cita cita = leerRegistro<Cita>("citas.bin", indiceCita);

    if (cita.isEliminado()) {
        std::cout << "Cita ya ha sido eliminada.\n";
        return false;
    }

    cita.setEstado("Cancelada");
    cita.setFechaModificacion(time(nullptr));

    escribirRegistro<Cita>("citas.bin", cita, indiceCita);
    std::cout << "Cita con ID " << idCita << " ha sido cancelada.\n";
    return true;
}
void obtenerCitasPorFecha(const char* fechaBuscada){
    GestorArchivos gestor;

    ArchivoHeader headerCitas;
    gestor.leerArchivoHeader("citas.bin", headerCitas);

    std:: cout << "\n=== Citas para la fecha " << fechaBuscada << " ===\n";

    for (int i = 0; i < headerCitas.cantidadRegistros; i++) {
        Cita cita = leerRegistro<Cita>("citas.bin", i);

        if (!cita.isEliminado() && strcmp(cita.getFecha(), fechaBuscada) == 0) {
            std:: cout << "ID: " << cita.getId()
                      << " | Hora: " << cita.getHora()
                      << " | Paciente ID: " << cita.getPacienteID()
                      << " | Doctor ID: " << cita.getDoctorID()
                      << " | Estado: " << cita.getEstado() << "\n";
        }
    }
}
void listarCitasPendientes(){
    GestorArchivos gestor;

    ArchivoHeader headerCitas;
    gestor.leerArchivoHeader("citas.bin", headerCitas);

    std:: cout << "\n=== Citas Pendientes ===\n";

    for (int i = 0; i < headerCitas.cantidadRegistros; i++) {
        Cita cita = leerRegistro<Cita>("citas.bin", i);

        if (!cita.isEliminado() && strcmp(cita.getEstado(), "Agendada") == 0) {
            std:: cout << "ID: " << cita.getId()
                      << " | Fecha: " << cita.getFecha()
                      << " | Hora: " << cita.getHora()
                      << " | Paciente ID: " << cita.getPacienteID()
                      << " | Doctor ID: " << cita.getDoctorID() << "\n";
        }
    }
}
bool atenderCita(Hospital* hospital, int idCita, const char* diagnostico,
                 const char* tratamiento, const char* medicamentos) {
    GestorArchivos gestor;
    int indiceCita = encontrarIndicePorID<Cita>("citas.bin", idCita);

    if (indiceCita == -1) {
        std::cout << "Cita no encontrada.\n";
        return false;
    }

    Cita cita = leerRegistro<Cita>("citas.bin", indiceCita);

    if (cita.isEliminado()) {
        std::cout << "Cita ya ha sido eliminada.\n";
        return false;
    }

    if (cita.getAtendida()) {
        std::cout << "Cita ya ha sido atendida.\n";
        return false;
    }

    // === Crear historial ===
    Historial nuevoHistorial;
    ArchivoHeader headerHistoriales;
    gestor.leerArchivoHeader("historiales.bin",headerHistoriales);

    nuevoHistorial.setHistorialID(headerHistoriales.proximoID);
    nuevoHistorial.setPacienteID(cita.getPacienteID());
    nuevoHistorial.setDoctorID(cita.getDoctorID());
    nuevoHistorial.setFecha(cita.getFecha());
    nuevoHistorial.setDiagnostico(diagnostico);
    nuevoHistorial.setTratamiento(tratamiento);
    nuevoHistorial.setMedicamentos(medicamentos);
    nuevoHistorial.setEliminado(false);

    time_t ahora = time(nullptr);
    nuevoHistorial.setFechaRegistro(ahora);
   

    // === Guardar historial ===
    std::fstream archivoHistoriales("historiales.bin",
                                    std::ios::binary | std::ios::in | std::ios::out);

    archivoHistoriales.seekp(
        calcularPosicion<Historial>(headerHistoriales.cantidadRegistros),
        std::ios::beg);

    archivoHistoriales.write(
        reinterpret_cast<const char*>(&nuevoHistorial),
        sizeof(Historial));

    archivoHistoriales.close();

    // Actualizar header
    headerHistoriales.cantidadRegistros++;
    headerHistoriales.registrosActivos++;
    headerHistoriales.proximoID++;
    gestor.actualizarHeader("historiales.bin", headerHistoriales);

    // === Actualizar Cita ===
    cita.setAtendida(true);
    cita.setEstado("Atendida");
    cita.setConsultaID(nuevoHistorial.getHistorialID());
    cita.setFechaModificacion(time(nullptr));

    // Guardar cita actualizada
    escribirRegistro<Cita>("citas.bin", cita, indiceCita);

    return true;
}
void mostrarMenuCitas(Hospital* hospital) {
    int opcion;

    do {
        std::cout << "\n=======================================\n";
        std::cout << "||          GESTION DE CITAS         ||\n";
        std::cout << "=======================================\n";
        std::cout << "1. Agendar nueva cita\n";
        std::cout << "2. Cancelar cita\n";
        std::cout << "3. Atender cita\n";
        std::cout << "4. Ver citas de un paciente\n";
        std::cout << "5. Ver citas de un doctor\n";
        std::cout << "6. Ver citas de una fecha\n";
        std::cout << "7. Ver citas pendientes\n";
        std::cout << "0. Volver al menu principal\n";
        std::cout << "Seleccione una opcion: ";
        std::cin >> opcion;
        std::cin.ignore();

        switch (opcion) {

        case 1: {
            int idP, idD;
            char fecha[11], hora[6], motivo[256];

            std::cout << "\nID del paciente: ";
            std::cin >> idP;
            std::cout << "ID del doctor: ";
            std::cin >> idD;

            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

            std::cout << "Fecha (aaaa-mm-dd): ";
            std::cin.getline(fecha, 11);

            std::cout << "Hora (HH:MM): ";
            std::cin.getline(hora, 6);

            std::cout << "Motivo: ";
            std::cin.getline(motivo, 256);

            agendarCita(hospital, idP, idD, fecha, hora, motivo);
            break;
        }


        case 2: {
            int idC;
            std::cout << "\nID de la cita a cancelar: ";
            std::cin >> idC;

            cancelarCita(hospital, idC);
            break;
        }

        case 3: {
            int idC;
            char diag[256], trat[256], med[256];
            std::cout << "\nID de la cita a atender: ";
            std::cin >> idC;
            std::cin.ignore();
            std::cout << "Diagnóstico: ";
            std::cin.getline(diag, sizeof(diag));
            std::cout << "Tratamiento: ";
            std::cin.getline(trat, sizeof(trat));
            std::cout << "Medicamentos: ";
            std::cin.getline(med, sizeof(med));


            atenderCita(hospital, idC, diag, trat, med);
            break;
        }

        case 4: {
            int idP;
            std::cout << "\nID del paciente: ";
            std::cin >> idP;
            obtenerCitasDePaciente(idP);
            break;
        }

        case 5: {
            int idD;
            std::cout << "\nID del doctor: ";
            std::cin >> idD;
            obtenerCitasDeDoctor(idD);
            break;
        }

        case 6: {
            std::string fecha;
            std::cin.ignore();
            std::cout << "\nFecha a consultar (aaaa-mm-dd): ";
            std::getline(std::cin, fecha);
            obtenerCitasPorFecha(fecha.c_str());
            break;
        }

        case 7:
            listarCitasPendientes();
            break;

        case 0:
            std::cout << "Volviendo al menu principal...\n";
            break;

        default:
            std::cout << "Opción inválida.\n";
        }

    } while (opcion != 0);
}