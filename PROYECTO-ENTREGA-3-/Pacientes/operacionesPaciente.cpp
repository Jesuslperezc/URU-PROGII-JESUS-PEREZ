#include "operacionesPaciente.hpp"
#include "Pacientes.hpp"
#include "../persistencia/GestionArchivos.hpp"
#include "../utilidades/utilidad.hpp"
#include <iostream>
#include <iomanip>
#include <cstring>
#include <fstream>

void mostrarHistorialMedico(Paciente* paciente) {
    std::cout << "=== Historial Médico del Paciente ID: " << paciente->getId() << " ===\n";
    std::cout << std::left << std::setw(6)  << "ID"
         << std::setw(12) << "Fecha"
         << std::setw(10) << "Hora"
         << std::setw(30) << "Diagnostico"
         << std::setw(30) << "Tratamiento"
         << std::setw(30) << "Medicamentos"
         << std::setw(8)  << "DocID"
         << std::setw(10) << "Costo" << "\n";
    std::cout << std::string(140, '-') << "\n";

    for (int i = 0; i < paciente->getCantidadConsultas(); i++) {
        // Buscar consulta por ID en historiales
        Historial c = buscarRegistroPorID<Historial>("historiales.bin", paciente->getCitasIDs()[i]);

        if (c.gethistorialID() != 0) {
            std::cout << std::left << std::setw(6)  << c.gethistorialID()
                      << std::setw(12) << c.getFecha()
                      << std::setw(10) << c.getHora()
                      << std::setw(30) << c.getDiagnostico()
                      << std::setw(30) << c.getTratamiento()
                      << std::setw(30) << c.getMedicamentos()
                      << std::setw(8)  << c.getDoctorID()
                      << std::setw(10) << c.getCosto() << "\n";
        }
    }

    std::cout << std::string(140, '-') << "\n";
    std::cout << "Total consultas: " << paciente->getCantidadConsultas() << "\n";
}
bool obtenerUltimaConsulta(Paciente* paciente, Historial& salida){

    if (paciente->getCantidadConsultas() == 0) {
        return false; // No hay consultas
    }

    int ultimaConsultaID = paciente->getCitasIDs()[paciente->getCantidadConsultas() - 1];
    Historial consulta = buscarRegistroPorID<Historial>("historiales.bin", ultimaConsultaID);

    if (consulta.gethistorialID() != 0) {
        salida = consulta;
        return true;
    }

    return false; // No se encontró la consulta
}
void buscarPacientesPorNombre(const char* nombreBuscado) {
    ArchivoHeader header;
    GestorArchivos gestor;

    if (!gestor.leerArchivoHeader("pacientes.bin", header)) {
        std::cout << "No se pudo leer el archivo de pacientes.\n";
        return;
    }

    int coincidencias = 0;

    for (int i = 0; i < header.cantidadRegistros; ++i) {
        Paciente p = leerRegistro<Paciente>("pacientes.bin", i);
        if (!p.isEliminado() && gestor.compararCaseInsensitive(p.getNombre(), nombreBuscado)) {
            std::cout << "Paciente encontrado: "
                      << p.getNombre() << " " << p.getApellido()
                      << " | Cedula: " << p.getCedula() << std::endl;
            coincidencias++;
        }
    }

    if (coincidencias == 0) {
        std::cout << "No se encontraron pacientes con el nombre: " << nombreBuscado << std::endl;
    } else {
        std::cout << "Coincidencias encontradas: " << coincidencias << std::endl;
    }
}
bool actualizarPaciente(int id){
    int indice = encontrarIndicePorID<Paciente>("pacientes.bin", id);
    if (indice == -1) {
        std::cout << "Paciente con ID " << id << " no encontrado.\n";
        return false;
    }

    Paciente paciente = leerRegistro<Paciente>("pacientes.bin", indice);

    std::cout << "Actualizando datos del paciente ID: " << paciente.getId() << "\n";

    char buffer[100];

    std::cout << "Nombre actual (" << paciente.getNombre() << "), ingrese nuevo o ENTER para mantener: ";
    std::cin.getline(buffer, sizeof(buffer));
    if (strlen(buffer) > 0) {
        paciente.setNombre(buffer);
    }

    std::cout << "Apellido actual (" << paciente.getApellido() << "), ingrese nuevo o ENTER para mantener: ";
    std::cin.getline(buffer, sizeof(buffer));
    if (strlen(buffer) > 0) {
        paciente.setApellido(buffer);
    }
    std::cout << "Cedula actual (" << paciente.getCedula() << "), ingrese nuevo o ENTER para mantener: ";
    std::cin.getline(buffer, sizeof(buffer));
    if (strlen(buffer) > 0) {
        paciente.setCedula(buffer);
    }
    std::cout << "Edad actual (" << paciente.getEdad() << "), ingrese nuevo o ENTER para mantener: ";
    std::cin.getline(buffer, sizeof(buffer));
    if (strlen(buffer) > 0) {
        paciente.setEdad(std::stoi(buffer));
    }
    std::cout << "Sexo actual (" << paciente.getSexo() << "), ingrese nuevo o ENTER para mantener: ";
    std::cin.getline(buffer, sizeof(buffer));
    if (strlen(buffer) > 0) {
        paciente.setSexo(buffer[0]);
    }
    std::cout << "Tipo de sangre actual (" << paciente.getTipoSangre() << "), ingrese nuevo o ENTER para mantener: ";
    std::cin.getline(buffer, sizeof(buffer));
    if (strlen(buffer) > 0) {
        paciente.setTipoSangre(buffer);
    }

    // Aquí se pueden agregar más campos según sea necesario

    if (!escribirRegistro<Paciente>("pacientes.bin", paciente, indice)) {
        std::cout << "Error al actualizar el paciente en el archivo.\n";
        return false;
    }

    std::cout << "Paciente actualizado exitosamente.\n";
    return true;    
}

Paciente crearPaciente(Hospital* hospital, const char* nombre,
                       const char* apellido, const char* cedula,const char* alergias, int edad, char sexo) {


    Paciente p{};
    p.setNombre(nombre);
    p.setApellido(apellido);
    p.setCedula(cedula);

    p.setEdad(edad);
    p.setSexo(toupper((unsigned char)sexo));
    p.setEliminado(false);
    p.setAlergias(alergias);

    p.setCantidadCitas(0);
    p.setPrimerConsultaID(-1);
    p.setCantidadCitas(0);
    for (int i = 0; i < 20; i++) p.setCitasID(i, -1);
    ArchivoHeader header; GestorArchivos gestor;
    gestor.asegurarArchivo("pacientes.bin");
    gestor.leerArchivoHeader("pacientes.bin",header);
    int nuevoId = header.proximoID; // Guardamos el ID que se asignará a este paciente

    std::fstream archivo("pacientes.bin", std::ios::binary | std::ios::in | std::ios::out);
    archivo.seekp(calcularPosicion<Paciente>(header.cantidadRegistros), std::ios::beg);
    archivo.write(reinterpret_cast<const char*>(&p), sizeof(Paciente));
    archivo.close();

    header.cantidadRegistros++;
    header.registrosActivos++;
    header.proximoID++;
    gestor.actualizarHeader("pacientes.bin", header);

    // Actualizar hospital
    hospital->setTotalPacientesRegistrados(hospital->getTotalPacientesRegistrados() + 1);
    std::fstream fh("hospital.bin", std::ios::binary | std::ios::in | std::ios::out);
    if (fh.is_open()) {
        fh.seekp(sizeof(ArchivoHeader), std::ios::beg);
        fh.write(reinterpret_cast<const char*>(hospital), sizeof(Hospital));
        fh.close();
    }

    std::cout << "Paciente creado exitosamente con ID: " << nuevoId << "\n";
    return p;
}

using namespace std;

// Prototipo de la función (puede ir en el .h)
Paciente crearPaciente(Hospital* hospital,
                       const char* nombre,
                       const char* apellido,
                       const char* cedula,
                       const char* alergias,
                       int edad,
                       char sexo);

void mostrarMenuPacientes(Hospital* hospital) {
    int opPaciente = -1;

    do {
        cout << "\n=======================================\n";
        cout << "||        GESTION DE PACIENTES        ||\n";
        cout << "=======================================\n";
        cout << "1. Registrar nuevo paciente\n";
        cout << "2. Buscar paciente por cedula\n";
        cout << "3. Buscar paciente por nombre\n";
        cout << "4. Ver historial medico completo\n";
        cout << "5. Actualizar datos del paciente\n";
        cout << "6. Listar todos los pacientes\n";
        cout << "7. Eliminar paciente\n";
        cout << "0. Volver al menu principal\n";
        cout << "Seleccione una opcion: ";
        cin >> opPaciente;
        cin.ignore(); // limpiar buffer

        switch (opPaciente) {
            case 1: {
                cout << ">>> Registrar nuevo paciente...\n";

                char nombre[50], apellido[50], cedula[20], alergias[100];
                int edad;
                char sexo;

                cout << "Ingrese nombre: ";
                cin.getline(nombre, sizeof(nombre));

                cout << "Ingrese apellido: ";
                cin.getline(apellido, sizeof(apellido));

                cout << "Ingrese cedula: ";
                cin.getline(cedula, sizeof(cedula));

                cout << "Ingrese alergias: ";
                cin.getline(alergias, sizeof(alergias));

                cout << "Ingrese edad: ";
                cin >> edad;
                cin.ignore();

                cout << "Ingrese sexo (M/F): ";
                cin >> sexo;
                cin.ignore();

                // Llamamos a tu función crearPaciente
                Paciente nuevo = crearPaciente(hospital, nombre, apellido, cedula, alergias, edad, sexo);
                break;
            }
            case 2:
                cout << ">>> Buscar paciente por cedula...\n";
                char cedula[20];
                cout << "Ingrese cedula: ";
                cin.ignore(); // limpiar buffer
                cin.getline(cedula, sizeof(cedula));
                buscarPacientePorCedula("pacientes.bin", cedula);
                break;
            case 3:
                cout << ">>> Buscar paciente por nombre...\n";
            
                char nombre[50];
                cout << "Ingrese nombre: ";
                cin.ignore(); // limpiar buffer
                cin.getline(nombre, sizeof(nombre));
                buscarPacientesPorNombre(nombre);
                break;
            case 4:
                cout << ">>> Ver historial medico completo...\n";
                int pacienteID;
                cout << "Ingrese ID del paciente: ";
                cin >> pacienteID;
                cin.ignore(); // limpiar buffer
                {
                    int indice = encontrarIndicePorID<Paciente>("pacientes.bin", pacienteID);
                    if (indice != -1) {
                        Paciente p = leerRegistro<Paciente>("pacientes.bin", indice);
                        mostrarHistorialMedico(&p);
                    } else {
                        cout << "Paciente con ID " << pacienteID << " no encontrado.\n";
                    }
                }
                break;
            case 5:
                cout << ">>> Actualizar datos del paciente...\n";
                cout << "Ingrese ID del paciente a actualizar: ";
                cin >> pacienteID;
                cin.ignore(); // limpiar buffer
                actualizarPaciente(pacienteID);
                break;
            case 6:
                cout << ">>> Listar todos los pacientes...\n";
                listarRegistros<Paciente>("pacientes.bin");
                break;
            case 7:
                cout << ">>> Eliminar paciente...\n";
                cout << "Ingrese ID del paciente a eliminar: ";
                cin >> pacienteID;
                cin.ignore(); // limpiar buffer

                
                break;
            case 0:
                cout << "Volviendo al menu principal...\n";
                break;
            default:
                cout << "Opcion invalida. Intente de nuevo.\n";
                break;
        }

    } while (opPaciente != 0);
}