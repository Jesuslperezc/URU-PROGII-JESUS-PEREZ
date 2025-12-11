#include "operacionCita.hpp"
#include "../Hospital/Hospital.hpp"
#include "../persistencia/GestionArchivos.hpp"
#include "../utilidades/utilidad.hpp"
#include "../Historiales/Historial.hpp"
#include "../utilidades/formato.hpp"
#include "../utilidades/validaciones.hpp"
#include "Citas.hpp"
#include <limits>
#include <iostream>
#include <cstring>

void obtenerCitasDeDoctor(int idDoctor) {
    Doctor doctor = buscarRegistroPorID<Doctor>("doctores.bin", idDoctor);
    if (doctor.getId() == 0) {
        Formato::mensaje("Error: Doctor no encontrado.", Formato::ROJO);
        return;
    }

    if (doctor.getCantidadCitas() == 0) {
        Formato::mensaje("El doctor no tiene citas registradas.", Formato::AMARILLO);
        return;
    }

    Formato::encabezadoTabla(std::string("Citas del Doctor ") + doctor.getNombre());

    for (int i = 0; i < doctor.getCantidadCitas(); i++) {
        int idCita = doctor.getCitaID(i);
        Cita cita = buscarRegistroPorID<Cita>("citas.bin", idCita);

        if (cita.getId() == 0) {
            Formato::mensaje("[Aviso] Cita con ID " + std::to_string(idCita) + " no encontrada en archivo.", Formato::AMARILLO);
            continue;
        }

        std::cout << Formato::NEGRITA
                  << "ID: " << cita.getId()
                  << " | Fecha: " << cita.getFecha()
                  << " | Hora: " << cita.getHora()
                  << " | Paciente ID: " << cita.getPacienteID()
                  << " | Estado: " << cita.getEstado()
                  << Formato::RESET << "\n";
    }
}

void obtenerCitasDePaciente(int idPaciente) {
    Paciente paciente = buscarRegistroPorID<Paciente>("pacientes.bin", idPaciente);

    if (paciente.getId() == 0) {
        Formato::mensaje("Error: Paciente no encontrado.", Formato::ROJO);
        return;
    }

    if (paciente.getCantidadCitas() == 0) {
        Formato::mensaje("El paciente no tiene citas registradas.", Formato::AMARILLO);
        return;
    }

    Formato::encabezadoTabla(std::string("Citas del Paciente ") + paciente.getNombre());

    int* citas = paciente.getCitasIDs();
    for (int i = 0; i < paciente.getCantidadCitas(); i++) {
        int idCita = citas[i];
        Cita cita = buscarRegistroPorID<Cita>("citas.bin", idCita);

        if (cita.getId() != 0) {
            std::cout << Formato::NEGRITA
                      << "ID: " << cita.getId()
                      << " | Fecha: " << cita.getFecha()
                      << " | Hora: " << cita.getHora()
                      << " | Doctor ID: " << cita.getDoctorID()
                      << " | Estado: " << cita.getEstado()
                      << Formato::RESET << "\n";
        } else {
            Formato::mensaje("[Aviso] Cita con ID " + std::to_string(idCita) + " no encontrada en archivo.", Formato::AMARILLO);
        }
    }
}

Cita agendarCita(Hospital* hospital, int idPaciente, int idDoctor,
                 const char* fecha, const char* hora, const char* motivo)
{
    (void)*hospital; // evitar warning
    GestorArchivos gestor;

    ArchivoHeader headerCitas;
    gestor.leerArchivoHeader("citas.bin", headerCitas);

    Paciente paciente = buscarRegistroPorID<Paciente>("pacientes.bin", idPaciente);
    Doctor doctor     = buscarRegistroPorID<Doctor>("doctores.bin", idDoctor);

    if (paciente.getId() == 0 || paciente.isEliminado()) {
     Formato::mensaje("Paciente no encontrado.\n", Formato::ROJO);
        return {};
    }
    if (doctor.getId() == 0 || doctor.isEliminado()) {
        Formato::mensaje("Doctor no encontrado.\n", Formato::ROJO);
        return {};
    }

    if (!validarFormatoFecha(fecha)) {
        Formato::mensaje("Formato de fecha inválido.\n", Formato::ROJO);
        return {};
    }
    if (!validarFormatoHora(hora)) {
        Formato::mensaje("Formato de hora inválido.\n", Formato::ROJO);
        return {};
    }

    // Crear nueva cita
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

    // Guardar cita en archivo
    if (!escribirRegistro<Cita>("citas.bin", nuevaCita, headerCitas.cantidadRegistros)) {
        Formato::mensaje("Error al escribir la cita.\n", Formato::ROJO);
        return {};
    }

    // Actualizar header
    headerCitas.cantidadRegistros++;
    headerCitas.registrosActivos++;
    headerCitas.proximoID++;
    gestor.actualizarHeader("citas.bin", headerCitas);

    Formato::mensaje("Cita agendada exitosamente con ID: " + std::to_string(nuevaCita.getId()) + "\n", Formato::VERDE);

    // Actualizar paciente usando la función segura
    paciente.agregarCitaID(nuevaCita.getId());
    int idxPaciente = encontrarIndicePorID<Paciente>("pacientes.bin", idPaciente);
    escribirRegistro<Paciente>("pacientes.bin", paciente, idxPaciente);

    // Actualizar doctor
    if (doctor.getCantidadCitas() < 30) {
        int idxD = doctor.getCantidadCitas();
        doctor.setCitaID(idxD, nuevaCita.getId());
        doctor.setCantidadCitas(idxD + 1);
        doctor.setfechaModificacion(time(nullptr));

        int idxDoctor = encontrarIndicePorID<Doctor>("doctores.bin", idDoctor);
        escribirRegistro<Doctor>("doctores.bin", doctor, idxDoctor);
    } else {
        Formato::mensaje("Aviso: doctor ya alcanzó máximo de citas, no se guardó en doctor.\n", Formato::AMARILLO);
    }

    return nuevaCita;
}

bool eliminarCita(int id) {
    ArchivoHeader header;
    std::fstream archivo("citas.bin", std::ios::binary | std::ios::in | std::ios::out);
    if (!archivo.is_open()) return false;

    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));

    Cita c{};
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.seekg(sizeof(ArchivoHeader) + i * sizeof(Cita));
        archivo.read(reinterpret_cast<char*>(&c), sizeof(Cita));
        if (c.getId() == id && !c.isEliminado()) {
            c.setEliminado(true);

            // Opcional: eliminar historial asociado
            if (c.getConsultaID() > 0) {
                Historial h = buscarRegistroPorID<Historial>("historiales.bin", c.getConsultaID());
                if (h.getHistorialID() > 0) {
                    h.setEliminado(true);
                    int idxH = encontrarIndicePorID<Historial>("historiales.bin", h.getHistorialID());
                    escribirRegistro<Historial>("historiales.bin", h, idxH);
                }
            }

            archivo.seekp(sizeof(ArchivoHeader) + i * sizeof(Cita));
            archivo.write(reinterpret_cast<char*>(&c), sizeof(Cita));
            archivo.close();
           Formato::mensaje("Cita eliminada correctamente.\n", Formato::VERDE);
            return true;
        }
    }
    archivo.close();
    return false;
}

void obtenerCitasPorFecha(const char* fechaBuscada) {
    GestorArchivos gestor;

    ArchivoHeader headerCitas;
    gestor.leerArchivoHeader("citas.bin", headerCitas);

    if (headerCitas.cantidadRegistros == 0) {
        std::cout << "No hay citas registradas.\n";
        return;
    }

    std::cout << "\n=== Citas para la fecha " << fechaBuscada << " ===\n";

    bool encontrada = false;

    for (int i = 0; i < headerCitas.cantidadRegistros; i++) {
        Cita cita = leerRegistro<Cita>("citas.bin", i);

        if (!cita.isEliminado() && strcmp(cita.getFecha(), fechaBuscada) == 0) {
            encontrada = true;

            std::cout << "ID: " << cita.getId()
                      << " | Hora: " << cita.getHora()
                      << " | Paciente ID: " << cita.getPacienteID()
                      << " | Doctor ID: " << cita.getDoctorID()
                      << " | Estado: " << cita.getEstado() << "\n";
        }
    }

    if (!encontrada)
        std::cout << "No existen citas para esa fecha.\n";
}
void listarCitasPendientes() {
    GestorArchivos gestor;
    ArchivoHeader headerCitas;
    gestor.leerArchivoHeader("citas.bin", headerCitas);

    if (headerCitas.cantidadRegistros == 0) {
        Formato::mensaje("No hay citas registradas.\n", Formato::ROJO);
        return;
    }

    Formato::titulo("CITAS PENDIENTES", '=');

    std::cout << std::left
              << std::setw(6)  << "ID"
              << std::setw(12) << "Fecha"
              << std::setw(8)  << "Hora"
              << std::setw(12) << "PacienteID"
              << std::setw(10) << "DoctorID" << "\n";
    std::cout << std::string(50, '-') << "\n";

    bool pendientes = false;

    for (int i = 0; i < headerCitas.cantidadRegistros; i++) {
        Cita cita = leerRegistro<Cita>("citas.bin", i);

        if (!cita.isEliminado() && strcmp(cita.getEstado(), "Agendada") == 0) {
            pendientes = true;

            std::cout << std::left
                      << std::setw(6)  << cita.getId()
                      << std::setw(12) << cita.getFecha()
                      << std::setw(8)  << cita.getHora()
                      << std::setw(12) << cita.getPacienteID()
                      << std::setw(10) << cita.getDoctorID() << "\n";
        }
    }

    if (!pendientes)
        Formato::mensaje("No hay citas pendientes.\n", Formato::AMARILLO);

    std::cout << std::string(50, '-') << "\n";
}

bool atenderCita(Hospital* hospital, int idCita, const char* diagnostico,
                 const char* tratamiento, const char* medicamentos) {

    (void)*hospital;
    GestorArchivos gestor;
    int indiceCita = encontrarIndicePorID<Cita>("citas.bin", idCita);

    if (indiceCita == -1) {
        Formato::mensaje("Cita no encontrada.", Formato::ROJO);
        return false;
    }

    Cita cita = leerRegistro<Cita>("citas.bin", indiceCita);

    if (cita.isEliminado()) {
        Formato::mensaje("Cita ya ha sido eliminada.", Formato::AMARILLO);
        return false;
    }

    if (cita.getAtendida()) {
        Formato::mensaje("Cita ya ha sido atendida.", Formato::AMARILLO);
        return false;
    }

    // === Crear o asegurar historiales.bin ===
    ArchivoHeader headerHistoriales;
    if (!gestor.leerArchivoHeader("historiales.bin", headerHistoriales)) {
        headerHistoriales = {0, 0, 1, 1};
        std::fstream archivo("historiales.bin", std::ios::binary | std::ios::out);
        archivo.write(reinterpret_cast<const char*>(&headerHistoriales), sizeof(ArchivoHeader));
        archivo.close();
    }

    // === Crear historial ===
    Historial h;
    h.setHistorialID(headerHistoriales.proximoID);
    h.setPacienteID(cita.getPacienteID());
    h.setDoctorID(cita.getDoctorID());
    h.setFecha(cita.getFecha());
    h.setHora(cita.getHora());
    h.setDiagnostico(diagnostico);
    h.setTratamiento(tratamiento);
    h.setMedicamentos(medicamentos);
    h.setEliminado(false);
    h.setFechaRegistro(time(nullptr));

    if (!escribirRegistro<Historial>("historiales.bin", h, headerHistoriales.cantidadRegistros)) {
        Formato::mensaje("Error al guardar historial.", Formato::ROJO);
        return false;
    }

    // Actualizar header historiales
    headerHistoriales.cantidadRegistros++;
    headerHistoriales.registrosActivos++;
    headerHistoriales.proximoID++;
    gestor.actualizarHeader("historiales.bin", headerHistoriales);

    // === Actualizar cita ===
    cita.setAtendida(true);
    cita.setEstado("Atendida");
    cita.setConsultaID(h.getHistorialID());
    cita.setFechaModificacion(time(nullptr));

    if (!escribirRegistro<Cita>("citas.bin", cita, indiceCita)) {
        Formato::mensaje("Error al actualizar cita.", Formato::ROJO);
        return false;
    }

    // Mensaje de éxito
    Formato::mensaje("=== Cita atendida y historial registrado correctamente ===", Formato::VERDE);
    return true;
}


void mostrarMenuCitas(Hospital* hospital) {
    int opcion;

    do {
        system("cls");

        // Título del menú
        Formato::titulo("GESTION DE CITAS", '=');

        // Opciones del menú con color
        Formato::mensaje("1. Agendar nueva cita", Formato::CYAN);
        Formato::mensaje("2. Cancelar cita", Formato::CYAN);
        Formato::mensaje("3. Atender cita", Formato::CYAN);
        Formato::mensaje("4. Ver citas de un paciente", Formato::CYAN);
        Formato::mensaje("5. Ver citas de un doctor", Formato::CYAN);
        Formato::mensaje("6. Ver citas de una fecha", Formato::CYAN);
        Formato::mensaje("7. Ver citas pendientes", Formato::CYAN);
        Formato::mensaje("0. Volver al menu principal", Formato::CYAN);

        Formato::mensaje("Seleccione una opcion: ", Formato::AMARILLO);
        std::cin >> opcion;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (opcion) {
            case 1: {
                int idP, idD;
                char fecha[11], hora[6], motivo[256];

                Formato::mensaje("ID del paciente: ", Formato::AMARILLO);
                std::cin >> idP;
                Formato::mensaje("ID del doctor: ", Formato::AMARILLO);
                std::cin >> idD;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                Formato::mensaje("Fecha (aaaa-mm-dd): ", Formato::AMARILLO);
                std::cin.getline(fecha, sizeof(fecha));
                Formato::mensaje("Hora (HH:MM): ", Formato::AMARILLO);
                std::cin.getline(hora, sizeof(hora));
                Formato::mensaje("Motivo: ", Formato::AMARILLO);
                std::cin.getline(motivo, sizeof(motivo));

                agendarCita(hospital, idP, idD, fecha, hora, motivo);
                pausarPantalla();
                break;
            }

            case 2: {
                int idC;
                Formato::mensaje("ID de la cita a cancelar: ", Formato::AMARILLO);
                std::cin >> idC;
                std::cin.ignore();
                eliminarCita(idC);
                pausarPantalla();
                break;
            }

            case 3: {
                int idC;
                char diag[256], trat[256], med[256];
                Formato::mensaje("ID de la cita a atender: ", Formato::AMARILLO);
                std::cin >> idC;
                std::cin.ignore();

                Formato::mensaje("Diagnostico: ", Formato::AMARILLO);
                std::cin.getline(diag, sizeof(diag));
                Formato::mensaje("Tratamiento: ", Formato::AMARILLO);
                std::cin.getline(trat, sizeof(trat));
                Formato::mensaje("Medicamentos: ", Formato::AMARILLO);
                std::cin.getline(med, sizeof(med));

                atenderCita(hospital, idC, diag, trat, med);
                pausarPantalla();
                break;
            }

            case 4: {
                int idP;
                Formato::mensaje("ID del paciente: ", Formato::AMARILLO);
                std::cin >> idP;
                std::cin.ignore();
                obtenerCitasDePaciente(idP);
                pausarPantalla();
                break;
            }

            case 5: {
                int idD;
                Formato::mensaje("ID del doctor: ", Formato::AMARILLO);
                std::cin >> idD;
                std::cin.ignore();
                obtenerCitasDeDoctor(idD);
                pausarPantalla();
                break;
            }

            case 6: {
                std::string fecha;
                Formato::mensaje("Fecha a consultar (aaaa-mm-dd): ", Formato::AMARILLO);
                std::getline(std::cin, fecha);
                obtenerCitasPorFecha(fecha.c_str());
                pausarPantalla();
                break;
            }

            case 7:
                listarCitasPendientes();
                pausarPantalla();
                break;

            case 0:
                Formato::mensaje("Volviendo al menu principal...", Formato::VERDE);
                pausarPantalla();
                break;

            default:
                Formato::mensaje("Opción inválida.", Formato::ROJO);
                pausarPantalla();
        }

    } while (opcion != 0);
}
