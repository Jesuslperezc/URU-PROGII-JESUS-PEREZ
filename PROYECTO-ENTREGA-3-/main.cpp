#include "hospital/Hospital.hpp"
#include "persistencia/GestionArchivos.hpp"
#include "pacientes/operacionesPaciente.hpp"
#include <iostream>


int main() {
    using namespace std;

    cout << "Sistema de Gestión Hospitalaria\n";

    Hospital hospital;           // Se crea el objeto
    hospital.cargarHospital();   // Se carga la info real

    mostrarMenuPacientes(&hospital); // Se pasa al menú

    return 0;
}
