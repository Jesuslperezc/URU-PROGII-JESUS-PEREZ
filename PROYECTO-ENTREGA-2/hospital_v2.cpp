#include <iostream>
#include <cstring>
#include <ctime>
#include <iomanip>
#include <fstream>
#include <cctype> // para tolower, isdigit
#include <limits>
#include "structs.hpp"
#include "utilidades.hpp"
#include "funcionesArchivos.hpp"
#include "FuncionesPrincipales.hpp"
using namespace std;



int main() {

    asegurarArchivo("hospital.bin");
    asegurarArchivo("pacientes.bin");
    asegurarArchivo("doctores.bin");
    asegurarArchivo("citas.bin");
    asegurarArchivo("historiales.bin");
    // Cargar hospital

    Hospital* hospital = cargarDatosHospital();

   


    int opMenu;
    do {
        system("cls");
        cout << "===========================================\n";
        cout << "||   SISTEMA DE GESTION HOSPITALARIA      ||\n";
        cout << "===========================================\n";
        cout << "1. Gestion de Pacientes\n";
        cout << "2. Gestion de Doctores\n";
        cout << "3. Gestion de Citas\n";
        cout<< "Mantenimiento\n";
        cout << "5. Salir\n";
        cout << "Seleccione una opcion: ";
        cin >> opMenu;

        switch (opMenu) {
            case 1: { // Gestion de Pacientes
                int opPaciente;
                do {
                    system("cls");
                    cout << "\n=======================================\n";
                    cout << "||        GESTION DE PACIENTES        ||\n";
                    cout << "=======================================\n";
                    cout << "1. Registrar nuevo paciente\n";
                    cout << "2. Buscar paciente por cedula\n";
                    cout << "3. Buscar paciente por nombre\n";
                    cout << "4. Ver historial medico completo\n";
                    cout << "5. Actualizar datos del paciente\n";
                    cout << "6. Listar todos los pacientes\n";
                    cout << "7. Eliminar paciente.\n";
                    cout << "0. Volver al menu principal\n";
                    cout << "Seleccione una opcion: ";
                    cin >> opPaciente;

                    switch (opPaciente) {
                      case 1: { // Registrar nuevo paciente
                            system("cls");

                            char nombre[50], apellido[50], cedula[20];
                            int edad;
                            char sexo;

                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Ingrese nombre (sin espacios): ";
                            cin.getline(nombre, 50);
                            if (!validarNombreSinEspacios(nombre)) { cin.get(); break; }

                            cout << "Ingrese apellido (sin espacios): ";
                            cin.getline(apellido, 50);
                            if (!validarNombreSinEspacios(apellido)) { cin.get(); break; }

                            cout << "Ingrese cedula (solo numeros): ";
                            cin.getline(cedula, 20);
                            if (!validarCedula(cedula)) { cin.get(); break; }

                            cout << "Ingrese edad: ";
                            cin >> edad;
                            if (!validarEdad(edad)) { cin.ignore(numeric_limits<streamsize>::max(), '\n'); cin.get(); break; }

                            cout << "Ingrese sexo (M/F): ";
                            cin >> sexo;
                            if (!validarSexoChar(sexo)) { cin.ignore(numeric_limits<streamsize>::max(), '\n'); cin.get(); break; }

                            // Crear paciente y registrar
                            Paciente p = crearPaciente(hospital, nombre, apellido, cedula, edad, sexo);

                            if (p.id != 0) {
                                cout << "Paciente registrado correctamente. ID: " << p.id << "\n";
                            } else {
                                cout << "Error al registrar el paciente.\n";
                            }

                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cin.get(); // Pausa
                        }
                        break;

                        case 2: { // Buscar por cedula
                            system("cls");
                            char cedula[20];
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Ingrese cedula: "; cin.getline(cedula, 20);
                            if (!validarCedula(cedula)) { cin.get(); break; }
                            buscarPacientePorCedula("pacientes.bin", cedula); 
                            cin.get();
                            break;
                        }

                        case 3: { // Buscar por nombre
                            system("cls");
                            char nombre[50];
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Ingrese nombre: "; cin.getline(nombre, 50);
                            buscarPacientesPorNombre(nombre);
                            cin.get();
                            break;
                        }

                        case 4: { // Historial medico
                            system("cls");
                            int id;
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Ingrese id del paciente: "; 
                            cin >> id;

                            Paciente p = buscarRegistroPorID<Paciente>("pacientes.bin", id);

                            if (p.cantidadConsultas == 0) {
                                cout << "No hay historial médico.\n";
                            } else {
                                mostrarHistorialMedico(&p);  //
                            }

                            cin.get();
                            cin.get(); // para pausar
                            break;
                        }


                        case 5: { // Actualizar paciente
                            system("cls");
                            char cedula[20];
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Ingrese cedula del paciente a actualizar: "; cin.getline(cedula, 20);
                            if (!validarCedula(cedula)) { cin.get(); break; }
                            Paciente pac= buscarPacientePorCedula("pacientes.bin", cedula);

                            char nuevoNombre[50], nuevoApellido[50]; int nuevaEdad = 0; char nuevoSexo = 'X';
                            cout << "\nNuevos datos (deje vacio o X para mantener):\n";
                            cout << "Nuevo nombre (sin espacios): "; cin.getline(nuevoNombre, 50);
                            if (strlen(nuevoNombre) > 0 && !validarNombreSinEspacios(nuevoNombre)) { cin.get(); break; }

                            cout << "Nuevo apellido (sin espacios): "; cin.getline(nuevoApellido, 50);
                            if (strlen(nuevoApellido) > 0 && !validarNombreSinEspacios(nuevoApellido)) { cin.get(); break; }

                            cout << "Nueva edad (0 = igual): "; cin >> nuevaEdad;
                            if (nuevaEdad != 0 && !validarEdad(nuevaEdad)) { cin.ignore(numeric_limits<streamsize>::max(), '\n'); cin.get(); break; }

                            cout << "Nuevo sexo (M/F, X = igual): "; cin >> nuevoSexo;
                            if (nuevoSexo != 'X' && nuevoSexo != 'x' && !validarSexoChar(nuevoSexo)) { cin.ignore(numeric_limits<streamsize>::max(), '\n'); cin.get(); break; }

                            if (strlen(nuevoNombre) > 0) strcpy(pac.nombre, nuevoNombre);
                            if (strlen(nuevoApellido) > 0) strcpy(pac.apellido, nuevoApellido);
                            if (nuevaEdad > 0) pac.edad = nuevaEdad;
                            char sx = toupper(nuevoSexo);
                            if (sx == 'M' || sx == 'F') pac.sexo = sx;
                            actualizarPaciente(pac.id);
                            cout << "Actualizado correctamente.\n";
                            cin.ignore(numeric_limits<streamsize>::max(), '\n'); cin.get();
                            break;
                        }

                        case 6: { // Listar pacientes
                            system("cls");
                            listarRegistros<Paciente>("pacientes.bin");
                            cout << "\nPresione Enter para continuar...";
                            cin.ignore(numeric_limits<streamsize>::max(), '\n'); cin.get();
                            break;
                        }

                        case 7: { // Eliminar paciente
                            system("cls");
                            char cedula[20];
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Ingrese cedula del paciente a eliminar: "; cin.getline(cedula, 20);
                            if (!validarCedula(cedula)) { cin.get(); break; }
                            Paciente paciente = buscarPacientePorCedula("pacientes.bin", cedula);

                            eliminarPaciente(paciente.id);
                            cout << "Paciente eliminado correctamente.\n";
                            cin.get();
                            break;
                        }
                    }
                } while (opPaciente != 0);
                break;
            }

            case 2: { // Gestion de Doctores
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
                    cout << "0. Volver al menu principal\n";
                    cout << "Seleccione una opcion: ";
                    cin >> opDoct;

                    switch (opDoct) {
                    case 1: { // Registrar nuevo doctor
                        system("cls");
                        char nombre[50], apellido[50], cedula[20], especialidad[50];
                        int aniosExperiencia;
                        float costoConsulta;

                        cin.ignore(numeric_limits<streamsize>::max(), '\n');

                        cout << "Ingrese nombre (sin espacios): "; 
                        cin.getline(nombre, 50);
                        if (!validarNombreSinEspacios(nombre)) { 
                            cout << "Nombre inválido.\n"; 
                            cin.get(); 
                            break; 
                        }

                        cout << "Ingrese apellido (sin espacios): "; 
                        cin.getline(apellido, 50);
                        if (!validarNombreSinEspacios(apellido)) { 
                            cout << "Apellido inválido.\n"; 
                            cin.get(); 
                            break; 
                        }

                        cout << "Ingrese cedula profesional (solo numeros): "; 
                        cin.getline(cedula, 20);
                        if (!validarCedula(cedula)) { 
                            cout << "Cédula inválida.\n"; 
                            cin.get(); 
                            break; 
                        }

                        cout << "Ingrese especialidad: "; 
                        cin.getline(especialidad, 50);

                        cout << "Ingrese años de experiencia: "; 
                        cin >> aniosExperiencia;
                        if (aniosExperiencia < 0) { 
                            cout << "Años inválidos.\n"; 
                            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                            cin.get(); 
                            break; 
                        }

                        cout << "Ingrese costo de consulta: "; 
                        cin >> costoConsulta;
                        if (costoConsulta < 0.0f) { 
                            cout << "Costo inválido.\n"; 
                            cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                            cin.get(); 
                            break; 
                        }

                        Doctor d = crearDoctor(hospital, nombre, apellido, cedula, especialidad, aniosExperiencia, costoConsulta);
                        if (d.id != 0) {
                            cout << "Doctor registrado correctamente. ID: " << d.id << "\n";
                        } else {
                            cout << "No se pudo registrar el doctor.\n";
                        }

                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); 
                        cin.get(); // Pausa
                        break;
                    }


                        case 2: { // Buscar doctor por ID
                            system("cls");
                            int idDoc; cout << "Ingrese ID del doctor: "; cin >> idDoc;
                            Doctor doc = buscarRegistroPorID<Doctor>("doctores.bin", idDoc);

                             cout << "Doctor: " << doc.nombre << " " << doc.apellido << " | Especialidad: " << doc.especialidad << "\n";
                        
                            cin.get();
                            break;
                        }

                        case 3: { // Buscar doctores por especialidad
                            system("cls");
                            char especialidad[50];
                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cout << "Ingrese especialidad: "; cin.getline(especialidad, 50);
                            buscarDoctoresPorEspecialidad(especialidad);
                            cin.get();
                            break;
                        }

                        case 4: { // Asignar paciente a doctor
                            system("cls");
                            int idDoc, idPac;
                            cout << "Ingrese ID del doctor: "; cin >> idDoc;
                            cout << "Ingrese ID del paciente: "; cin >> idPac;

                            // Buscar doctor y paciente en los archivos binarios
                            Doctor doctor = buscarRegistroPorID<Doctor>("doctores.bin", idDoc);
                            Paciente paciente = buscarRegistroPorID<Paciente>("pacientes.bin", idPac);

                            if (doctor.id != 0 && paciente.id != 0) {
                                // Llamamos a tu función para asignar el paciente
                                if (asignarPacienteADoctor(idDoc, idPac)) {
                                    cout << "Paciente asignado correctamente.\n";
                                } else {
                                    cout << "No se pudo asignar el paciente.\n";
                                }
                            } else {
                                cout << "Doctor o paciente no encontrado.\n";
                            }

                            cin.ignore(numeric_limits<streamsize>::max(), '\n');
                            cin.get(); // Pausa
                            break;
                        }


                        case 5: { // Ver pacientes de doctor
                            system("cls");
                            int idDoc;
                            while (true) {
                                cout << "Ingrese ID del doctor: ";
                                if (!(cin >> idDoc)) {
                                    cin.clear();
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    cout << "Entrada invalida. Ingrese un numero entero.\n";
                                    continue;
                                }
                                cin.ignore(numeric_limits<streamsize>::max(), '\n'); // consumir resto de la linea
                                Doctor d = buscarRegistroPorID<Doctor>("doctores.bin", idDoc);
                                if (d.id==0) {
                                    cout << "Doctor no encontrado. Desea reintentar? (s/n): ";
                                    char opc;
                                    if (!(cin >> opc)) {
                                        cin.clear();
                                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                        break;
                                    }
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    if (opc == 's' || opc == 'S') continue;
                                    break;
                                }
                                listarPacientesDeDoctor(idDoc);
                                cout << "\nPresione Enter para continuar...";
                                cin.get();
                                break;
                            }
                            break;
                        }

                        case 6: { // Listar todos los doctores
                            system("cls");
                            listarDoctores();
                            cout << "\nPresione Enter para continuar...";
                            cin.ignore(numeric_limits<streamsize>::max(), '\n'); cin.get();
                            break;
                        }

                        case 7: { // Eliminar doctor
                            system("cls");
                            int idDoc; cout << "Ingrese ID del doctor a eliminar: "; cin >> idDoc;
                            eliminarDoctor(idDoc);
                            cin.get();
                            break;
                        }

                        case 0: break;
                        default: cout << "Opcion no valida.\n"; cin.get(); break;
                    }
                } while (opDoct != 0);
                break;
            }

                        case 3: { // Gestion de citas
                            int opCita;
                            do {
                                system("cls");
                                cout << "\n=======================================\n";
                                cout << "||        GESTION DE CITAS           ||\n";
                                cout << "=======================================\n";
                                cout << "1. Agendar nueva cita\n";
                                cout << "2. Cancelar cita\n";
                                cout << "3. Atender cita\n";
                                cout << "4. Ver citas de un paciente\n";
                                cout << "5. Ver citas de un doctor\n";
                                cout << "6. Ver citas de una fecha\n";
                                cout << "7. Ver citas pendientes\n";
                                cout << "0. Volver al menu principal\n";
                                cout << "Seleccione una opcion: ";
                                cin >> opCita;
            
                                switch (opCita) {
                                    case 1: { // Agendar cita
                                        system("cls");
                                        int idPac, idDoc;
                                        char fecha[11], hora[6], motivo[100];
            
                                        cout << "Ingrese ID del paciente: "; cin >> idPac;
                                        cout << "Ingrese ID del doctor: "; cin >> idDoc;
                                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                        cout << "Ingrese fecha (YYYY-MM-DD): "; cin.getline(fecha, 11);
                                        cout << "Ingrese hora (HH:MM): "; cin.getline(hora, 6);
                                        cout << "Ingrese motivo de la cita: "; cin.getline(motivo, 100);
            
                                        if (!verificarDisponibilidad(hospital, idDoc, fecha, hora)) {
                                            cout << "El doctor no está disponible en esa fecha y hora.\n";
                                        } else {
                                            Cita cita = agendarCita(hospital, idPac, idDoc, fecha, hora, motivo);
                                            if (cita.id != 0) {
                                                cout << "Cita registrada correctamente.\n";
                                            } else {
                                                cout << "Error al registrar la cita.\n";
                                            }
                                        }
            
                                        cout << "\nPresione Enter para continuar...";
                                        cin.get();
                                        break;
                                    }
            
                                    case 2: { // Cancelar cita
                                        system("cls");
                                        int idCita; cout << "Ingrese ID de la cita a cancelar: "; cin >> idCita;
                                        cancelarCita(hospital, idCita);
                                        cin.ignore(numeric_limits<streamsize>::max(), '\n'); cin.get();
                                        break;
                                    }
                                    case 3: { // Atender cita
                                        system("cls");
                                        int idCita;
                                        char diagnostico[200], tratamiento[200], medicamentos[200];

                                        cout << "Ingrese ID de la cita a atender: ";
                                        cin >> idCita;
                                        cin.ignore(numeric_limits<streamsize>::max(), '\n');

                                        cout << "Ingrese diagnostico: ";
                                        cin.getline(diagnostico, 200);

                                        cout << "Ingrese tratamiento: ";
                                        cin.getline(tratamiento, 200);

                                        cout << "Ingrese medicamentos: ";
                                        cin.getline(medicamentos, 200);

                                        // Aquí verificamos el resultado de la función
                                        if (atenderCita(hospital, idCita, diagnostico, tratamiento, medicamentos)) {
                                            cout << " Cita atendida correctamente.\n";
                                        } else {
                                            cout << " Error al atender la cita.\n";
                                        }

                                        cin.get(); // Pausa
                                        break;
                                    }
                        
                                    case 4: { // Ver citas de un paciente
                                        system("cls");
                                        int idPac, cantidad = 0;
                                        cout << "Ingrese ID del paciente: "; cin >> idPac;
                                        Cita* citas = leerCitasDePaciente(idPac, &cantidad);
            
                                        if (citas && cantidad > 0) {
                                            cout << left << setw(6) << "ID"
                                                << setw(12) << "Fecha"
                                                << setw(10) << "Hora"
                                                << setw(8)  << "DocID"
                                                << setw(8)  << "PacID"
                                                << setw(25) << "Motivo" << "\n";
                                            cout << string(70, '-') << "\n";
            
                                            for (int i = 0; i < cantidad; i++) {
                                                cout << left << setw(6)  << citas[i].id
                                                    << setw(12) << citas[i].fecha
                                                    << setw(10) << citas[i].hora
                                                    << setw(8)  << citas[i].doctorID
                                                    << setw(8)  << citas[i].pacienteID
                                                    << setw(25) << citas[i].motivo << "\n";
                                            }
            
                                            delete[] citas; // liberar memoria
                                        } else {
                                            cout << "No se encontraron citas para este paciente.\n";
                                        }
            
                                        cout << string(70, '-') << "\n";
                                        cout << "Total de citas encontradas: " << cantidad << "\n";
            
                                        system("pause");
                                        break;
                                    }
            
                                    case 5: { // Ver citas de un doctor
                                        system("cls");
                                        int idDoc, cantidad = 0;
                                        cout << "Ingrese ID del doctor: "; cin >> idDoc;
                                        Cita* citas = leerCitasDoctor(idDoc, &cantidad);
            
                                        if (citas && cantidad > 0) {
                                            cout << left << setw(6) << "ID"
                                                << setw(12) << "Fecha"
                                                << setw(10) << "Hora"
                                                << setw(8)  << "DocID"
                                                << setw(8)  << "PacID"
                                                << setw(25) << "Motivo" << "\n";
                                            cout << string(70, '-') << "\n";
            
                                            for (int i = 0; i < cantidad; i++) {
                                                cout << left << setw(6)  << citas[i].id
                                                    << setw(12) << citas[i].fecha
                                                    << setw(10) << citas[i].hora
                                                    << setw(8)  << citas[i].doctorID
                                                    << setw(8)  << citas[i].pacienteID
                                                    << setw(25) << citas[i].motivo << "\n";
                                            }
            
                                            delete[] citas;
                                        } else {
                                            cout << "No se encontraron citas para este doctor.\n";
                                        }
            
                                        cout << string(70, '-') << "\n";
                                        cout << "Total de citas encontradas: " << cantidad << "\n";
            
                                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                        cin.get();
                                        break;
                                    }
            
                                    case 6: { // Buscar citas por fecha
                                        system("cls");
                                        char fecha[12]; // formato: "dd/mm/yyyy"
                                        cout << "Ingrese fecha (dd/mm/yyyy): ";
                                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                        cin.getline(fecha, 12);
            
                                        int cantidad = 0;
                                        Cita* citas = obtenerCitasPorFecha(fecha, &cantidad);
            
                                        if (citas == nullptr) {
                                            cout << "No se encontraron citas para la fecha " << fecha << ".\n";
                                        } else {
                                            cout << "\nCitas encontradas: " << cantidad << "\n";
                                            for (int i = 0; i < cantidad; i++) {
                                                Cita c = citas[i];
                                                cout << "ID: " << c.id
                                                    << " | DoctorID: " << c.doctorID
                                                    << " | PacienteID: " << c.pacienteID
                                                    << " | Hora: " << c.hora
                                                    << " | Motivo: " << c.motivo << "\n";
                                            }
                                            delete[] citas;
                                        }
            
                                        system("pause");
                                        break;
                                    }
            
                                    case 7: { // Citas pendientes
                                        system("cls");
                                        listarCitasPendientes();
                                        cout << "\nPresione Enter para continuar...";
                                        cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                        cin.get();
                                        break;
                                    }
            
                                    case 0:
                                        break;
            
                                    default:
                                        cout << "Opción no válida.\n";
                                        cin.get();
                                        break;
                                } // end switch(opCita)
            
                            } while (opCita != 0);
            
                            break;
                        }
            
                        // --- Submenú de Mantenimiento ---
                        case 4: {
                            int subop;
                            do {
                                system("cls");
                                cout << "MANTENIMIENTO DE ARCHIVOS:\n";
                                cout << "1. Verificar integridad de archivos\n";
                                cout << "2. Hacer respaldo de datos\n";
                                cout << "3. Restaurar desde respaldo\n";
                                cout << "4. Estadísticas de uso de archivos\n";
                                cout << "5. Compactar\n";
                                 cout << "6. salir\n";
                                cout << "Opción: ";
                                cin >> subop;
            
                                switch(subop) {
                                    case 1: verificarArchivos(); break;
                                    case 2: hacerRespaldo(); break;
                                    case 3: restaurarRespaldo(); break;
                                    case 4: mostrarEstadisticasArchivos(); break;
                                    case 5: 
                                        compactarArchivoDoctores();
                                        compactarArchivoPacientes();
                                        break;
                                    case 6: /* volver */ break;
                                    default: cout << "Opción inválida.\n"; break;
                                }
            
                                if (subop != 5) {
                                    cout << "\nPresione Enter para continuar...";
                                    cin.ignore(numeric_limits<streamsize>::max(), '\n');
                                    cin.get();
                                }
            
                            } while (subop != 5);
            
                            break;
                        }
            
                        default:
                            cout << "Opcion no valida.\n";
                            cin.get();
                            break;
                    } // end switch(opMenu)
                } while (opMenu != 4);
            
                // --- Guardar hospital antes de salir ---
                if (hospital) {
                    if (guardarHospital(*hospital)) {
                        cout << "Datos del hospital guardados correctamente en hospital.bin.\n";
                    } else {
                        cout << "Error al guardar los datos del hospital.\n";
                    }
                    delete hospital;
                }
            
                return 0;
            }
            