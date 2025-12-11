#include "Doctores.hpp"
#include "../Hospital/Hospital.hpp"
#include "../persistencia/GestionArchivos.hpp"
#include "../utilidades/utilidad.hpp"
#include "../utilidades/formato.hpp"
#include "../utilidades/validaciones.hpp"
#include <cctype>
#include <limits>
#include <iostream>
#include <cstring>
#include <string>
#include <fstream>
#include <ctime>

using namespace std;

void buscarDoctoresPorEspecialidad(const char* nombreBuscado){
 ArchivoHeader header;
 GestorArchivos gestor;
 gestor.leerArchivoHeader("doctores.bin", header);
    int coincidencias = 0;

    for (int i = 0; i < header.cantidadRegistros; ++i) {
        Doctor d = leerRegistro<Doctor>("doctores.bin", i);
        if (!d.isEliminado() && compararCaseInsensitive(d.getEspecialidad(), nombreBuscado)) {
            std:: cout << "Paciente encontrado: " 
                 << d.getNombre() << " " << d.getApellido() 
                 << " | Especialidad: " << d.getEspecialidad() <<std:: endl;
            coincidencias++;
        }
    }

   std:: cout << "Coincidencias encontradas: " << coincidencias << std::endl;
}
Doctor crearDoctor(Hospital* hospital,   const char* nombre,  const char* apellido, 
 const char* cedula, const char* especialidad, int aniosExperiencia,
 float costoConsulta) 
{
     GestorArchivos gestor;
    if (!validarNombreSinEspacios(nombre) || !validarNombreSinEspacios(apellido)) return {};
    if (!validarCedula(cedula)) return {};
    if (aniosExperiencia < 0 || costoConsulta < 0) return {};

    Doctor d;  // Constructor por defecto

    d.setNombre(nombre);
    d.setApellido(apellido);
    d.setCedulaProfesional(cedula);
    d.setEspecialidad(especialidad);
    d.setAniosExperiencia(aniosExperiencia);
    d.setCostoConsulta(costoConsulta);

    d.setDisponible(true);
    d.setCantidadPacientes(0);
    d.setCantidadCitas(0);

    // Inicializar listas de IDs
    for (int i = 0; i < 50; i++)
        d.inicializarPaciente(i);

    for (int i = 0; i < 30; i++)
        d.inicializarCita(i);
    // Timestamps
    time_t ahora = time(nullptr);
    d.setfechaCreacion(ahora);
    d.setfechaModificacion(ahora);
    d.setEliminado(false);
    d.setEliminado(false);

    // Archivo
   
    asegurarArchivo("doctores.bin");
    ArchivoHeader header; gestor.leerArchivoHeader("doctores.bin", header);

    d.setId(header.proximoID);

    // Guardar doctor en binario
    std::fstream archivo("doctores.bin", std::ios::binary | std::ios::in | std::ios::out);
    archivo.seekp(calcularPosicion<Doctor>(header.cantidadRegistros));
    archivo.write(reinterpret_cast<const char*>(&d), sizeof(Doctor));
    archivo.close();

    // Actualizar header
    header.cantidadRegistros++;
    header.registrosActivos++;
    header.proximoID++;
    gestor.actualizarHeader("doctores.bin", header);

    // Actualizar hospital
    hospital->setTotalDoctoresRegistrados(hospital->getTotalDoctoresRegistrados() + 1);
    std::fstream fh("hospital.bin", std::ios::binary | std::ios::in | std::ios::out);
    fh.seekp(sizeof(ArchivoHeader));
    fh.write(reinterpret_cast<const char*>(hospital), sizeof(Hospital));
    fh.close();
    std::cout << "Doctor creado exitosamente con ID: " << d.getId() << "\n";
    return d;
    return d;
}

bool eliminarDoctor(int id) {

    ArchivoHeader header;
    std::fstream archivo("doctores.bin", std::ios::binary | std::ios::in | std::ios::out);
    if (!archivo.is_open()) return false;

    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));

    Doctor d{};
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.seekg(sizeof(ArchivoHeader) + i * sizeof(Doctor));
        archivo.read(reinterpret_cast<char*>(&d), sizeof(Doctor));
        if (d.getId() == id && !d.isEliminado()) {
            d.setEliminado(true);

            // Marcar todas sus citas como eliminadas
            for (int j = 0; j < d.getCantidadCitas(); j++) {
                int idCita = d.getCitaID(j);
                if (idCita <= 0) continue;
                d.eliminarCitaID(idCita);
            }

            archivo.seekp(sizeof(ArchivoHeader) + i * sizeof(Doctor));
            archivo.write(reinterpret_cast<char*>(&d), sizeof(Doctor));
            archivo.close();
            std::cout << "Doctor eliminado correctamente.\n";
            return true;
        }
    }
    archivo.close();
    return false;
}

bool asignarPacienteADoctor(Hospital* hospital, int idDoctor, int idPaciente) {
    if (!hospital) return false;

    GestorArchivos gestor;

    // ---------- LEER HEADER DOCTORES ----------
    ArchivoHeader headerDoctores;
    gestor.leerArchivoHeader("doctores.bin", headerDoctores);

    // ---------- BUSCAR DOCTOR ----------
    Doctor doctor{};
    int indiceDoctor = -1;

    for (int i = 0; i < headerDoctores.cantidadRegistros; ++i) {
        Doctor d = leerRegistro<Doctor>("doctores.bin", i);
        if (!d.isEliminado() && d.getId() == idDoctor) {
            doctor = d;
            indiceDoctor = i;
            break;
        }
    }

    if (indiceDoctor == -1) {
        std::cout << "Doctor con ID " << idDoctor << " no encontrado.\n";
        return false;
    }

    // ---------- LEER HEADER PACIENTES ----------
    ArchivoHeader headerPacientes;
    gestor.leerArchivoHeader("pacientes.bin", headerPacientes);

    // ---------- BUSCAR PACIENTE ----------
    Paciente paciente{};
    bool pacienteEncontrado = false;

    for (int i = 0; i < headerPacientes.cantidadRegistros; ++i) {
        Paciente p = leerRegistro<Paciente>("pacientes.bin", i);
        if (!p.isEliminado() && p.getId() == idPaciente) {
            paciente = p;
            pacienteEncontrado = true;
            break;
        }
    }

    if (!pacienteEncontrado) {
        std::cout << "Paciente con ID " << idPaciente << " no encontrado.\n";
        return false;
    }

    // ---------- ASIGNAR PACIENTE ----------
    if (!doctor.setPacienteID(paciente.getId())) { // aquí usamos tu método que busca un slot libre
        std::cout << "Doctor ya tiene todos los pacientes asignados.\n";
        return false;
    }

    // ---------- GUARDAR DOCTOR ACTUALIZADO ----------
    if (!escribirRegistro<Doctor>("doctores.bin", doctor, indiceDoctor)) {
        std::cout << "Error al guardar doctor actualizado.\n";
        return false;
    }

    std::cout << "Paciente con ID " << paciente.getId()
              << " asignado al doctor con ID " << doctor.getId() << "\n";

    return true;
}


bool removerPacienteDeDoctor(Doctor* doctor, int idPaciente) {
    if (!doctor) return false;

    if (!doctor->removerPaciente(idPaciente)) {
        std::cout << "El doctor no tiene asignado al paciente con ID "
                  << idPaciente << ".\n";
        return false;
    }

    GestorArchivos gestor;
    ArchivoHeader header;
    gestor.leerArchivoHeader("doctores.bin", header);

    for (int i = 0; i < header.cantidadRegistros; ++i) {
        Doctor d = leerRegistro<Doctor>("doctores.bin", i);
        if (d.getId() == doctor->getId()) {
            doctor->setfechaModificacion(time(nullptr));
            if (!escribirRegistro("doctores.bin", *doctor, i)) {
                std::cout << "Error al actualizar doctor en archivo.\n";
                return false;
            }

            std::cout << "Paciente removido y cambios guardados.\n";
            return true;
        }
    }

    std::cout << "Error: no se encontró el doctor dentro del archivo.\n";
    return false;
}

void listarDoctores() {
    GestorArchivos gestor;
    ArchivoHeader header;
    gestor.leerArchivoHeader("doctores.bin", header);

    Formato::titulo("LISTADO DE DOCTORES");

    for (int i = 0; i < header.cantidadRegistros; ++i) {
        Doctor d = leerRegistro<Doctor>("doctores.bin", i);
        std::cout << Formato::NEGRITA << "ID: " << d.getId() << Formato::RESET
                  << " | Nombre: " << Formato::acentos(d.getNombre()) << " "
                  << Formato::acentos(d.getApellido())
                  << " | Especialidad: " << Formato::acentos(d.getEspecialidad())
                  << " | Costo Consulta: " << Formato::AMARILLO << "$"
                  << d.getCostoConsulta() << Formato::RESET
                  << " | Disponible: "
                  << (d.getDisponible() ? (std::string(Formato::VERDE) + "Sí") : (std::string(Formato::ROJO) + "No"))
                  << Formato::RESET
                  << "\n\n";
    }
}

void listarPacientesDeDoctor(int idDoctor) {
    GestorArchivos gestor;
    ArchivoHeader headerDoctores;
    gestor.leerArchivoHeader("doctores.bin", headerDoctores);

    Doctor doctor{};
    int indiceDoctor = -1;

    for (int i = 0; i < headerDoctores.cantidadRegistros; ++i) {
        Doctor d = leerRegistro<Doctor>("doctores.bin", i);
        if (!d.isEliminado() && d.getId() == idDoctor) {
            doctor = d;
            indiceDoctor = i;
            break;
        }
    }

    if (indiceDoctor == -1) {
        Formato::mensaje("Doctor con ID " + std::to_string(idDoctor) + " no encontrado.", Formato::ROJO);
        return;
    }

    // Construir el título como std::string antes de pasarlo a Formato::titulo
    std::string nombreCompleto = std::string(doctor.getNombre()) + " " + std::string(doctor.getApellido());
    Formato::titulo(std::string("PACIENTES DEL DOCTOR ") + Formato::acentos(nombreCompleto));

    ArchivoHeader headerPacientes;
    gestor.leerArchivoHeader("pacientes.bin", headerPacientes);

    for (int i = 0; i < doctor.getCantidadPacientes(); ++i) {
        int pacienteID = doctor.getPacienteID(i);
        if (pacienteID != -1) {
            for (int j = 0; j < headerPacientes.cantidadRegistros; ++j) {
                Paciente p = leerRegistro<Paciente>("pacientes.bin", j);
                if (!p.isEliminado() && p.getId() == pacienteID) {
                    std::cout << Formato::NEGRITA << "ID: " << p.getId() << Formato::RESET
                              << " | Nombre: " << Formato::acentos(p.getNombre()) << " " 
                              << Formato::acentos(p.getApellido())
                              << " | Cedula: " << p.getCedula()
                              << "\n";
                    break;
                }
            }
        }
    }
}

void mostrarMenuDoctor(Hospital* hospital) {
    int opDoct;

    do {
        system("cls");

        // Título
        Formato::titulo("GESTION DE DOCTORES", '=');

        // Opciones
        Formato::mensaje("1. Registrar nuevo doctor", Formato::CYAN);
        Formato::mensaje("2. Buscar doctor por ID", Formato::CYAN);
        Formato::mensaje("3. Buscar doctores por especialidad", Formato::CYAN);
        Formato::mensaje("4. Asignar paciente a doctor", Formato::CYAN);
        Formato::mensaje("5. Ver pacientes asignados a doctor", Formato::CYAN);
        Formato::mensaje("6. Listar todos los doctores", Formato::CYAN);
        Formato::mensaje("7. Eliminar doctor", Formato::CYAN);
        Formato::mensaje("0. Volver al menú principal", Formato::CYAN);

        Formato::mensaje("Seleccione una opción: ", Formato::AMARILLO);
        std::cin >> opDoct;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (opDoct) {

            // ------------------------------------------------------------------
            case 1: {
                char nombre[50], apellido[50], cedula[20], especialidad[50];
                int aniosExperiencia;
                float costoConsulta;

                Formato::mensaje("Ingrese nombre del doctor: ", Formato::AMARILLO);
                std::cin.getline(nombre, sizeof(nombre));

                Formato::mensaje("Ingrese apellido del doctor: ", Formato::AMARILLO);
                std::cin.getline(apellido, sizeof(apellido));

                Formato::mensaje("Ingrese cedula: ", Formato::AMARILLO);
                std::cin.getline(cedula, sizeof(cedula));

                Formato::mensaje("Ingrese especialidad: ", Formato::AMARILLO);
                std::cin.getline(especialidad, sizeof(especialidad));

                Formato::mensaje("Ingrese años de experiencia: ", Formato::AMARILLO);
                std::cin >> aniosExperiencia;

                Formato::mensaje("Ingrese costo de consulta: ", Formato::AMARILLO);
                std::cin >> costoConsulta;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                crearDoctor(hospital, nombre, apellido, cedula, especialidad, aniosExperiencia, costoConsulta);

               pausarPantalla();
                break;
            }

            // ------------------------------------------------------------------
            case 2: {
                int id;
                Formato::mensaje("Ingrese ID del doctor a buscar: ", Formato::AMARILLO);
                std::cin >> id;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                buscarRegistroPorID<Doctor>("doctores.bin", id);

                pausarPantalla();
                break;
            }

            // ------------------------------------------------------------------
            case 3: {
                char nombreEspecialidad[50];
                Formato::mensaje("Ingrese la especialidad a buscar: ", Formato::AMARILLO);
                std::cin.getline(nombreEspecialidad, sizeof(nombreEspecialidad));

                buscarDoctoresPorEspecialidad(nombreEspecialidad);

                pausarPantalla();
                break;
            }

            // ------------------------------------------------------------------
            case 4: {
                int idDoc, idPac;

                Formato::mensaje("Ingrese ID del doctor: ", Formato::AMARILLO);
                std::cin >> idDoc;

                Formato::mensaje("Ingrese ID del paciente: ", Formato::AMARILLO);
                std::cin >> idPac;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                asignarPacienteADoctor(hospital, idDoc, idPac);

                pausarPantalla();
                break;
            }

            // ------------------------------------------------------------------
            case 5: {
                int idDoc;
                Formato::mensaje("Ingrese ID del doctor: ", Formato::AMARILLO);
                std::cin >> idDoc;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                listarPacientesDeDoctor(idDoc);

                pausarPantalla();
                break;
            }

            // ------------------------------------------------------------------
            case 6:
                listarDoctores();
                pausarPantalla();
                break;

            // ------------------------------------------------------------------
            case 7: {
                int id;
                Formato::mensaje("Ingrese ID del doctor a eliminar: ", Formato::AMARILLO);
                std::cin >> id;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                eliminarDoctor(id);

                pausarPantalla();
                break;
            }

            // ------------------------------------------------------------------
            case 0:
                Formato::mensaje("Volviendo al menú principal...", Formato::VERDE);
                break;

            default:
                Formato::mensaje("Opción inválida. Intente nuevamente.", Formato::ROJO);
                pausarPantalla();
                break;
        }

    } while (opDoct != 0);
}
