#include "hospital/Hospital.hpp"
#include "persistencia/GestionArchivos.hpp"
#include "pacientes/operacionesPaciente.hpp"
#include <iostream>


int main() {
    using namespace std;

    cout << "Sistema de Gestión Hospitalaria\n";
    // Aquí podría ir la lógica para iniciar el sistema, mostrar menús, etc
   Hospital hospital;
   Hospital();
   hospital.cargarHospital();
   mostrarMenuPacientes(&hospital);
    return 0;
}