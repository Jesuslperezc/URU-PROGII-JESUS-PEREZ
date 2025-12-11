#include "Hospital/Hospital.hpp"
#include "persistencia/GestionArchivos.hpp"
#include "Pacientes/operacionesPaciente.hpp"
#include "Doctores/operacionDoctores.hpp"
#include "Citas/operacionCita.hpp"
#include "utilidades/formato.hpp"
#include <iostream>

int main() {
    Hospital hospital;

    int opMenu;
    do {
        system("cls"); // limpiar pantalla

        // Título del sistema
        Formato::titulo("SISTEMA DE GESTION HOSPITALARIA", '=');

        // Opciones del menú con colores
        Formato::mensaje("1. Gestion de Pacientes", Formato::CYAN);
        Formato::mensaje("2. Gestion de Doctores", Formato::CYAN);
        Formato::mensaje("3. Gestion de Citas", Formato::CYAN);
        Formato::mensaje("4. Mantenimiento", Formato::CYAN);
        Formato::mensaje("5. Salir", Formato::CYAN);

        Formato::mensaje("Seleccione una opcion: ", Formato::AMARILLO);
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
                mostrarMenuMantenimiento();
                std::cin.get();
                break;

            case 5:
                Formato::mensaje("Saliendo del sistema...", Formato::VERDE);
                break;

            default:
                Formato::mensaje("Opción inválida. Intente nuevamente.", Formato::ROJO);
                std::cin.get();
        }

    } while (opMenu != 5);

    return 0;
}

