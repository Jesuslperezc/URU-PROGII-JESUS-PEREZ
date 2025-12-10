#include "Hospital/Hospital.hpp"
#include "persistencia/GestionArchivos.hpp"
#include "Pacientes/operacionesPaciente.hpp"
#include "Doctores/operacionDoctores.hpp"
#include "Citas/operacionCita.hpp"
#include <iostream>


int main() {
    using namespace std;

    cout << "Sistema de Gestión Hospitalaria\n";

    Hospital hospital;           // Se crea el objeto
    hospital.cargarHospital();   // Se carga la info real
    int opMenu;

    do {
        system("cls"); // limpiar pantalla
        std::cout << "===========================================\n";
        std::cout << "||   SISTEMA DE GESTION HOSPITALARIA     ||\n";
        std::cout << "===========================================\n";
        std::cout << "1. Gestion de Pacientes\n";
        std::cout << "2. Gestion de Doctores\n";
        std::cout << "3. Gestion de Citas\n";
        std::cout << "4. Mantenimiento\n";
        std::cout << "5. Salir\n";
        std::cout << "Seleccione una opcion: ";
        std::cin >> opMenu;
        std::cin.ignore(); // limpiar buffer

        switch (opMenu) {
            case 1:
                mostrarMenuPacientes(&hospital);
                break;

            case 2:
                mostrarMenuDoctor(&hospital);
                break;

            case 3:
                mostrarMenuCitas(&hospital);
                break;

            case 4:
                std::cout << "Mantenimiento no implementado aún.\n";
                std::cout << "Presione Enter para continuar...";
                std::cin.get();
                break;

            case 5:
                std::cout << "Saliendo del sistema...\n";
                break;

            default:
                std::cout << "Opción inválida. Intente nuevamente.\n";
                std::cin.get();
        }

    } while (opMenu != 5);

    return 0;
}
   // Se pasa al menú
