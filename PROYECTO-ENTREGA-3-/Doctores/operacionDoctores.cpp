#include "Doctores.hpp"
#include "../Hospital/Hospital.hpp"
#include "../persistencia/GestionArchivos.hpp"
#include "../utilidades/utilidad.hpp"
#include <iostream>
#include <cstring>
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
        if (!d.isEliminado() && gestor.compararCaseInsensitive(d.getEspecialidad(), nombreBuscado)) {
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
    if (!gestor.validarNombreSinEspacios(nombre) || !gestor.validarNombreSinEspacios(apellido)) return {};
    if (!gestor.validarCedula(cedula)) return {};
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
   
    gestor.asegurarArchivo("doctores.bin");
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
    std::fstream archivo("doctores.bin", std::ios::binary | std::ios::in | std::ios::out);
    if (!archivo.is_open()) return false;

    ArchivoHeader header;
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));

    Doctor d{};
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.seekg(sizeof(ArchivoHeader) + i * sizeof(Doctor));
        archivo.read(reinterpret_cast<char*>(&d), sizeof(Doctor));
        if (d.getId() == id && !d.isEliminado()) {
            d.setEliminado(true);
            archivo.seekp(sizeof(ArchivoHeader) + i * sizeof(Doctor));
            archivo.write(reinterpret_cast<char*>(&d), sizeof(Doctor));
            archivo.close();
            std::cout << "Doctor eliminado.\n";
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

void listarDoctores(){
    ArchivoHeader header;
    GestorArchivos gestor;
    gestor.leerArchivoHeader("doctores.bin", header);
    
        for (int i = 0; i < header.cantidadRegistros; ++i) {
            Doctor d = leerRegistro<Doctor>("doctores.bin", i);
            if (!d.isEliminado()) {
                std:: cout << "ID: " << d.getId()
                    << " | Nombre: " << d.getNombre() << " " << d.getApellido()
                    << " | Especialidad: " << d.getEspecialidad()
                    << " | Costo Consulta: $" << d.getCostoConsulta()
                    << " | Disponible: " << (d.getDisponible() ? "Sí" : "No") 
                    << std:: endl;
            }
        }
}
void listarPacientesDeDoctor( int idDoctor){

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
        std::cout << "Doctor con ID " << idDoctor << " no encontrado.\n";
        return;
    }

    std::cout << "Pacientes del Doctor " << doctor.getNombre() << " " << doctor.getApellido() << ":\n";

    ArchivoHeader headerPacientes;
    gestor.leerArchivoHeader("pacientes.bin", headerPacientes);

    for (int i = 0; i < doctor.getCantidadPacientes(); ++i) {
        int pacienteID = doctor.getPacienteID(i);
        if (pacienteID != -1) {
            for (int j = 0; j < headerPacientes.cantidadRegistros; ++j) {
                Paciente p = leerRegistro<Paciente>("pacientes.bin", j);
                if (!p.isEliminado() && p.getId() == pacienteID) {
                    std::cout << "ID: " << p.getId()
                              << " | Nombre: " << p.getNombre() << " " << p.getApellido()
                              << " | Cédula: " << p.getCedula()
                              << std::endl;
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
        cout << "\n=======================================\n";
        cout << "||        GESTION DE DOCTORES        ||\n";
        cout << "=======================================\n";
        cout << "1. Registrar nuevo doctor\n";
        cout << "2. Buscar doctor por ID\n";
        cout << "3. Buscar doctores por especialidad\n";
        cout << "4. Asignar paciente a doctor\n";
        cout << "5. Ver pacientes asignados a doctor\n";
        cout << "6. Listar todos los doctores\n";
        cout << "7. Eliminar doctor\n";
        cout << "0. Volver al menú principal\n";
        cout << "Seleccione una opción: ";
        cin >> opDoct;

        switch (opDoct) {
            case 1:
                char nombre[50], apellido[50], cedula[20], especialidad[50];
                int aniosExperiencia;
                float costoConsulta;
                cout << "Ingrese nombre del doctor: ";
                cin.ignore(); // Limpiar el buffer
                cin.getline(nombre, sizeof(nombre));
                cout << "Ingrese apellido del doctor: ";
                cin.getline(apellido, sizeof(apellido));
                cout << "Ingrese cédula: ";
                cin.getline(cedula, sizeof(cedula));
                cout << "Ingrese especialidad: ";
                cin.getline(especialidad, sizeof(especialidad));
                cout << "Ingrese años de experiencia: ";
                cin >> aniosExperiencia;
                cout << "Ingrese costo de consulta: ";
                cin >> costoConsulta;
                cin.ignore(); // Limpiar el buffer después de leer un número
                crearDoctor(hospital,nombre, apellido, cedula, especialidad, aniosExperiencia, costoConsulta);
                break;

            case 2:
                int id;
                cout << "Ingrese ID del doctor a buscar: ";
                cin >> id;
                buscarRegistroPorID<Doctor>("doctores.bin", id);
                break;

            case 3:
                char nombreEspecialidad[50];
                cout << "Ingrese la especialidad a buscar: ";
                cin.ignore(); // Limpiar el buffer
                cin.getline(nombreEspecialidad, sizeof(nombreEspecialidad));
                buscarDoctoresPorEspecialidad(nombreEspecialidad);
                break;

            case 4: {
                int idDoc, idPac;
                cout << "Ingrese ID del doctor: ";
                cin >> idDoc;
                cout << "Ingrese ID del paciente: ";
                cin >> idPac;
                asignarPacienteADoctor(hospital, idDoc, idPac);
                break;
            }

            case 5:

                int idDoc;
                cout << "Ingrese ID del doctor: ";
                cin >> idDoc;
                listarPacientesDeDoctor(idDoc);
                break;

            case 6:
                listarDoctores();
                break;

            case 7:
                int ID;
                cout << "Ingrese ID del doctor a eliminar: ";
                cin >> ID;
                eliminarDoctor(ID);
                break;

            case 0:
                cout << "Volviendo al menú principal...\n";
                break;

            default:
                cout << "Opción inválida. Intente nuevamente.\n";
                break;
        }

        if (opDoct != 0) {
            cout << "\nPresione ENTER para continuar...";
            cin.ignore();
            cin.get();
        }

    } while (opDoct != 0);
}
