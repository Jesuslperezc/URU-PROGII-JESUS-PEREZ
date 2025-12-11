#include "operacionesPaciente.hpp"
#include "Pacientes.hpp"
#include "../persistencia/GestionArchivos.hpp"
#include "../utilidades/utilidad.hpp"
#include "../utilidades/formato.hpp"
#include "../utilidades/validaciones.hpp"
#include <iostream>
#include <iomanip>
#include <cstring>
#include <fstream>
#include <limits>

void mostrarHistorialMedico(Paciente* paciente) {
    if (!paciente) return;

    asegurarArchivo("historiales.bin");

    // Encabezado del historial
    Formato::encabezadoTabla("Historial Medico del Paciente ID: " 
                              + std::to_string(paciente->getId()) 
                              + " (" + paciente->getNombre() + ")");

    // Encabezado de la tabla
    std::cout << Formato::NEGRITA
              << std::left << std::setw(6)  << "ID"
              << std::setw(12) << "Fecha"
              << std::setw(10) << "Hora"
              << std::setw(30) << "Diagnostico"
              << std::setw(30) << "Tratamiento"
              << std::setw(30) << "Medicamentos"
              << std::setw(8)  << "DocID"
              << std::setw(10) << "Costo" << Formato::RESET << "\n";

    std::cout << std::string(140, '-') << "\n";

    int consultasMostradas = 0;
    int* citas = paciente->getCitasIDs();

    for (int i = 0; i < paciente->getCantidadCitas(); i++) {
        int idCita = citas[i];
        if (idCita <= 0) continue;

        Cita cita = buscarRegistroPorID<Cita>("citas.bin", idCita);
        if (cita.getId() == 0 || cita.getConsultaID() <= 0) continue;

        Historial h = buscarRegistroPorID<Historial>("historiales.bin", cita.getConsultaID());
        if (h.getHistorialID() == 0) continue;

        // Colores según estado: atendido en verde
        std::cout << Formato::VERDE
                  << std::left << std::setw(6)  << h.getHistorialID()
                  << std::setw(12) << h.getFecha()
                  << std::setw(10) << h.getHora()
                  << std::setw(30) << h.getDiagnostico()
                  << std::setw(30) << h.getTratamiento()
                  << std::setw(30) << h.getMedicamentos()
                  << std::setw(8)  << h.getDoctorID()
                  << std::setw(10) << h.getCosto() << Formato::RESET << "\n";

        consultasMostradas++;
    }

    if (consultasMostradas == 0)
        Formato::mensaje("No hay historial médico registrado para este paciente.", Formato::AMARILLO);

    std::cout << std::string(140, '-') << "\n";
    std::cout << Formato::NEGRITA << "Total consultas atendidas: " 
              << consultasMostradas << Formato::RESET << "\n";
}


bool obtenerUltimaConsulta(Paciente* paciente, Historial& salida){

    if (paciente->getCantidadConsultas() == 0) {
        return false; // No hay consultas
    }

    int ultimaConsultaID = paciente->getCitasIDs()[paciente->getCantidadConsultas() - 1];
    Historial consulta = buscarRegistroPorID<Historial>("historiales.bin", ultimaConsultaID);

    if (consulta.getHistorialID() != 0) {
        salida = consulta;
        return true;
    }

    return false; // No se encontró la consulta
}
void buscarPacientesPorNombre(const char* nombreBuscado) {
    ArchivoHeader header;
    GestorArchivos gestor;

    if (!gestor.leerArchivoHeader("pacientes.bin", header)) {
       Formato::mensaje("Error al leer el archivo de pacientes.", Formato::ROJO);
        return;
    }

    int coincidencias = 0;

    for (int i = 0; i < header.cantidadRegistros; ++i) {
        Paciente p = leerRegistro<Paciente>("pacientes.bin", i);
        if (!p.isEliminado() && compararCaseInsensitive(p.getNombre(), nombreBuscado)) {
            std::cout << "Paciente encontrado: "
                      << p.getNombre() << " " << p.getApellido()
                      << " | Cedula: " << p.getCedula() << std::endl;
            coincidencias++;
        }
    }

    if (coincidencias == 0) {
        Formato::mensaje("No se encontraron pacientes con el nombre especificado.", Formato::ROJO);
    } else {
        std::cout << "Coincidencias encontradas: " << coincidencias << std::endl;
    }
}
bool actualizarPaciente(int id) {
    int indice = encontrarIndicePorID<Paciente>("pacientes.bin", id);
    if (indice == -1) {
        Formato::mensaje("Paciente con ID " + std::to_string(id) + " no encontrado.", Formato::ROJO);
        return false;
    }

    Paciente paciente = leerRegistro<Paciente>("pacientes.bin", indice);

    Formato::titulo("Actualizando datos del paciente ID: " + std::to_string(paciente.getId()));

    char buffer[100];

    std::cout << Formato::SUBRAYADO << "Nombre actual (" << paciente.getNombre() << ")" 
              << Formato::RESET << ", ingrese nuevo o ENTER para mantener: ";
    std::cin.getline(buffer, sizeof(buffer));
    if (strlen(buffer) > 0) paciente.setNombre(buffer);

    std::cout << Formato::SUBRAYADO << "Apellido actual (" << paciente.getApellido() << ")" 
              << Formato::RESET << ", ingrese nuevo o ENTER para mantener: ";
    std::cin.getline(buffer, sizeof(buffer));
    if (strlen(buffer) > 0) paciente.setApellido(buffer);

    std::cout << Formato::SUBRAYADO << "Cedula actual (" << paciente.getCedula() << ")" 
              << Formato::RESET << ", ingrese nuevo o ENTER para mantener: ";
    std::cin.getline(buffer, sizeof(buffer));
    if (strlen(buffer) > 0) paciente.setCedula(buffer);

    std::cout << Formato::SUBRAYADO << "Edad actual (" << paciente.getEdad() << ")" 
              << Formato::RESET << ", ingrese nuevo o ENTER para mantener: ";
    std::cin.getline(buffer, sizeof(buffer));
    if (strlen(buffer) > 0) paciente.setEdad(std::stoi(buffer));

    std::cout << Formato::SUBRAYADO << "Sexo actual (" << paciente.getSexo() << ")" 
              << Formato::RESET << ", ingrese nuevo o ENTER para mantener: ";
    std::cin.getline(buffer, sizeof(buffer));
    if (strlen(buffer) > 0) paciente.setSexo(buffer[0]);

    std::cout << Formato::SUBRAYADO << "Tipo de sangre actual (" << paciente.getTipoSangre() << ")" 
              << Formato::RESET << ", ingrese nuevo o ENTER para mantener: ";
    std::cin.getline(buffer, sizeof(buffer));
    if (strlen(buffer) > 0) paciente.setTipoSangre(buffer);

    // Escribir cambios en archivo
    if (!escribirRegistro<Paciente>("pacientes.bin", paciente, indice)) {
        Formato::mensaje("Error al actualizar el paciente en el archivo.", Formato::ROJO);
        return false;
    }

    Formato::mensaje("Paciente actualizado exitosamente.", Formato::VERDE);
    return true;    
}


Paciente crearPaciente(Hospital* hospital, const char* nombre,const char* apellido, const char* cedula,
 const char* alergias, int edad, char sexo) {

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
    for (int i = 0; i < 20; i++) p.setCitasID(i, -1);

    ArchivoHeader header;
    GestorArchivos gestor;
    asegurarArchivo("pacientes.bin");
    gestor.leerArchivoHeader("pacientes.bin", header);

    int nuevoId = header.proximoID;
    p.setId(nuevoId);

    // Guardar paciente en archivo
    std::fstream archivo("pacientes.bin", std::ios::binary | std::ios::in | std::ios::out);
    archivo.seekp(calcularPosicion<Paciente>(header.cantidadRegistros), std::ios::beg);
    archivo.write(reinterpret_cast<const char*>(&p), sizeof(Paciente));
    archivo.close();

    // Actualizar header
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

bool eliminarPaciente(int id) {

    ArchivoHeader header;
    std::fstream archivo("pacientes.bin", std::ios::binary | std::ios::in | std::ios::out);
    if (!archivo.is_open()) return false;

    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));

    Paciente p{};
    for (int i = 0; i < header.cantidadRegistros; i++) {
        archivo.seekg(sizeof(ArchivoHeader) + i * sizeof(Paciente));
        archivo.read(reinterpret_cast<char*>(&p), sizeof(Paciente));
        if (p.getId() == id && !p.isEliminado()) {
            // Marcar paciente como eliminado
            p.setEliminado(true);

            // Eliminar todas sus citas
            int* citas = p.getCitasIDs();
            for (int j = 0; j < p.getCantidadCitas(); j++) {
                int idCita = citas[j];
                if (idCita <= 0) continue;
                p.eliminarCitaID(idCita); // función de abajo
            }

            archivo.seekp(sizeof(ArchivoHeader) + i * sizeof(Paciente));
            archivo.write(reinterpret_cast<char*>(&p), sizeof(Paciente));
            archivo.close();
            Formato::mensaje("Paciente con ID " + std::to_string(id) + " eliminado exitosamente.", Formato::VERDE);
            return true;
        }
    }
    archivo.close();
    return false;
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

// ===================================================================


Paciente buscarPacientePorCedula(const char* nombreArchivo, const char* cedulaBuscada){
    std::fstream archivo(nombreArchivo, std::ios::binary | std::ios::in);
    if (!archivo.is_open()) {
        std::cout << "No se pudo abrir el archivo de pacientes.\n";
        return Paciente{};
    }

    ArchivoHeader header{};
    archivo.read(reinterpret_cast<char*>(&header), sizeof(ArchivoHeader));
    if (!archivo.good()) {
        archivo.close();
        std::cout << "Error al leer el header del archivo de pacientes.\n";
        return Paciente{};
    }

    Paciente reg{};
    for (int i = 0; i < header.cantidadRegistros; ++i) {
        archivo.seekg(calcularPosicion<Paciente>(i), std::ios::beg);
        archivo.read(reinterpret_cast<char*>(&reg), sizeof(Paciente));
        if (!archivo.good()) break;

        if (!reg.isEliminado() && std::strcmp(reg.getCedula(), cedulaBuscada) == 0) {
            archivo.close();
            std::cout << "Paciente encontrado: " << reg.getNombre() << " " << reg.getApellido() << "\n";
            return reg;
        }
    }

    archivo.close();
    std::cout << "Paciente con cedula " << cedulaBuscada << " no encontrado.\n";
    return Paciente{}; // no encontrado

}


void mostrarMenuPacientes(Hospital* hospital) {
    int opPaciente = -1;

    do {
        system("cls");

        // Título
        Formato::titulo("GESTION DE PACIENTES", '=');

        // Opciones del menú
        Formato::mensaje("1. Registrar nuevo paciente", Formato::CYAN);
        Formato::mensaje("2. Buscar paciente por cedula", Formato::CYAN);
        Formato::mensaje("3. Buscar paciente por nombre", Formato::CYAN);
        Formato::mensaje("4. Ver historial medico completo", Formato::CYAN);
        Formato::mensaje("5. Actualizar datos del paciente", Formato::CYAN);
        Formato::mensaje("6. Listar todos los pacientes", Formato::CYAN);
        Formato::mensaje("7. Eliminar paciente", Formato::CYAN);
        Formato::mensaje("0. Volver al menu principal", Formato::CYAN);

        Formato::mensaje("Seleccione una opcion: ", Formato::AMARILLO);
        std::cin >> opPaciente;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        switch (opPaciente) {

            // ---------------------------------------------------------
            case 1: {
                Formato::mensaje(">>> Registrar nuevo paciente...", Formato::VERDE);

                char nombre[50], apellido[50], cedula[20], alergias[100];
                int edad;
                char sexo;

                Formato::mensaje("Ingrese nombre: ", Formato::AMARILLO);
                std::cin.getline(nombre, sizeof(nombre));

                Formato::mensaje("Ingrese apellido: ", Formato::AMARILLO);
                std::cin.getline(apellido, sizeof(apellido));

                Formato::mensaje("Ingrese cedula: ", Formato::AMARILLO);
                std::cin.getline(cedula, sizeof(cedula));

                Formato::mensaje("Ingrese alergias: ", Formato::AMARILLO);
                std::cin.getline(alergias, sizeof(alergias));

                Formato::mensaje("Ingrese edad: ", Formato::AMARILLO);
                std::cin >> edad;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                Formato::mensaje("Ingrese sexo (M/F): ", Formato::AMARILLO);
                std::cin >> sexo;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                crearPaciente(hospital, nombre, apellido, cedula, alergias, edad, sexo);

                pausarPantalla();
                break;
            }

            // ---------------------------------------------------------
            case 2: {
                Formato::mensaje(">>> Buscar paciente por cedula...", Formato::VERDE);
                
                char cedula[20];
                Formato::mensaje("Ingrese cedula: ", Formato::AMARILLO);
                std::cin.getline(cedula, sizeof(cedula));

                buscarPacientePorCedula("pacientes.bin", cedula);

                pausarPantalla();
                break;
            }

            // ---------------------------------------------------------
            case 3: {
                Formato::mensaje(">>> Buscar paciente por nombre...", Formato::VERDE);

                char nombre[50];
                Formato::mensaje("Ingrese nombre: ", Formato::AMARILLO);
                std::cin.getline(nombre, sizeof(nombre));

                buscarPacientesPorNombre(nombre);

                pausarPantalla();
                break;
            }

            // ---------------------------------------------------------
            case 4: {
                Formato::mensaje(">>> Ver historial medico completo...", Formato::VERDE);

                int pacienteID;
                Formato::mensaje("Ingrese ID del paciente: ", Formato::AMARILLO);
                std::cin >> pacienteID;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                int indice = encontrarIndicePorID<Paciente>("pacientes.bin", pacienteID);
                if (indice != -1) {
                    Paciente p = leerRegistro<Paciente>("pacientes.bin", indice);
                    mostrarHistorialMedico(&p);
                } else {
                    Formato::mensaje("Paciente no encontrado.", Formato::ROJO);
                }

                pausarPantalla();
                break;
            }

            // ---------------------------------------------------------
            case 5: {
                Formato::mensaje(">>> Actualizar datos del paciente...", Formato::VERDE);

                int pacienteID;
                Formato::mensaje("Ingrese ID del paciente a actualizar: ", Formato::AMARILLO);
                std::cin >> pacienteID;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                actualizarPaciente(pacienteID);

                pausarPantalla();
                break;
            }

            // ---------------------------------------------------------
            case 6:
                Formato::mensaje(">>> Listar todos los pacientes...", Formato::VERDE);

                listarRegistros<Paciente>("pacientes.bin");

                pausarPantalla();
                break;

            // ---------------------------------------------------------
            case 7: {
                Formato::mensaje(">>> Eliminar paciente...", Formato::VERDE);

                int pacienteID;
                Formato::mensaje("Ingrese ID del paciente a eliminar: ", Formato::AMARILLO);
                std::cin >> pacienteID;
                std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

                if (!eliminarPaciente(pacienteID))
                    Formato::mensaje("No se pudo eliminar el paciente.", Formato::ROJO);

                pausarPantalla();
                break;
            }

            // ---------------------------------------------------------
            case 0:
                Formato::mensaje("Volviendo al menu principal...", Formato::VERDE);
                break;

            // ---------------------------------------------------------
            default:
                Formato::mensaje("Opcion invalida. Intente de nuevo.", Formato::ROJO);
                pausarPantalla();
                break;
        }

    } while (opPaciente != 0);
}
