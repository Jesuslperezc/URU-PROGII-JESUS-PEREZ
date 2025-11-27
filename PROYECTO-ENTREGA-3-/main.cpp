#include "hospital/Hospital.hpp"

#include <iostream>

using namespace std;

// TODO: Declarar funciones de menús
void mostrarMenuPrincipal();
void menuPacientes(Hospital& hospital);
void menuDoctores(Hospital& hospital);
void menuCitas(Hospital& hospital);
void menuHistorial();

int main() {
    // TODO: 1. Inicializar sistema de archivos
    if (!GestorArchivos::inicializarSistemaArchivos()) {
        cerr << "Error al inicializar archivos" << endl;
        return 1;
    }
    
    // TODO: 2. Cargar o crear Hospital
    Hospital hospital;
    if (!GestorArchivos::cargarHospital(hospital)) {
        // Crear hospital por defecto si no existe
        hospital = Hospital("Hospital General", "Calle Principal #123", "555-1234");
        GestorArchivos::guardarHospital(hospital);
    }
    
    // TODO: 3. Loop principal con menú
    int opcion;
    do {
        mostrarMenuPrincipal();
        cin >> opcion;
        cin.ignore();
        
        switch(opcion) {
            case 1:
                menuPacientes(hospital);
                break;
            case 2:
                menuDoctores(hospital);
                break;
            case 3:
                menuCitas(hospital);
                break;
            case 4:
                menuHistorial();
                break;
            case 5:
                // Guardar y salir
                GestorArchivos::guardarHospital(hospital);
                cout << "\n✓ Datos guardados. Hasta pronto!" << endl;
                break;
            default:
                cout << "Opción inválida" << endl;
        }
    } while(opcion != 5);
    
    return 0;
}

void mostrarMenuPrincipal() {
    cout << "\n╔════════════════════════════════════════╗" << endl;
    cout << "║   SISTEMA DE GESTIÓN HOSPITALARIA v3   ║" << endl;
    cout << "║      (POO y Modularización)            ║" << endl;
    cout << "╚════════════════════════════════════════╝" << endl;
    cout << "\n1. Gestión de Pacientes" << endl;
    cout << "2. Gestión de Doctores" << endl;
    cout << "3. Gestión de Citas" << endl;
    cout << "4. Historial Médico" << endl;
    cout << "5. Guardar y Salir" << endl;
    cout << "\nOpción: ";
}

void menuPacientes(Hospital& hospital) {
    int opcion;
    do {
        cout << "\n=== GESTIÓN DE PACIENTES ===" << endl;
        cout << "1. Registrar nuevo paciente" << endl;
        cout << "2. Buscar por ID" << endl;
        cout << "3. Buscar por cédula" << endl;
        cout << "4. Modificar datos" << endl;
        cout << "5. Eliminar" << endl;
        cout << "6. Listar todos" << endl;
        cout << "7. Ver historial médico" << endl;
        cout << "8. Volver" << endl;
        cout << "\nOpción: ";
        cin >> opcion;
        cin.ignore();
        
        // IMPORTANTE: Solo llamamos a las funciones, NO implementamos lógica aquí
        switch(opcion) {
            case 1:
                registrarPaciente(hospital);  // Función de operacionesPacientes.cpp
                break;
            case 2:
                buscarPacientePorID();  // Función de operacionesPacientes.cpp
                break;
            case 3:
                buscarPacientePorCedula();  // Función de operacionesPacientes.cpp
                break;
            case 4:
                modificarPaciente();
                break;
            case 5:
                eliminarPaciente();
                break;
            case 6:
                listarTodosPacientes();
                break;
            case 7:
                verHistorialPaciente();
                break;
            case 8:
                cout << "Volviendo al menú principal..." << endl;
                break;
            default:
                cout << "Opción inválida" << endl;
        }
    } while(opcion != 8);
}

void menuDoctores(Hospital& hospital) {
    int opcion;
    do {
        cout << "\n=== GESTIÓN DE DOCTORES ===" << endl;
        cout << "1. Registrar nuevo doctor" << endl;
        cout << "2. Buscar por ID" << endl;
        cout << "3. Listar todos" << endl;
        cout << "4. Listar por especialidad" << endl;
        cout << "5. Modificar datos" << endl;
        cout << "6. Volver" << endl;
        cout << "\nOpción: ";
        cin >> opcion;
        cin.ignore();
        
        switch(opcion) {
            case 1:
                registrarDoctor(hospital);  // Función de operacionesDoctores.cpp
                break;
            case 2:
                buscarDoctorPorID();
                break;
            case 3:
                listarTodosDoctores();
                break;
            case 4:
                listarDoctoresPorEspecialidad();
                break;
            case 5:
                modificarDoctor();
                break;
            case 6:
                cout << "Volviendo..." << endl;
                break;
            default:
                cout << "Opción inválida" << endl;
        }
    } while(opcion != 6);
}