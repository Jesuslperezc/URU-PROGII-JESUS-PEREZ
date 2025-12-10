#include "Hospital/Hospital.hpp"
#include "persistencia/GestionArchivos.hpp"
#include "Pacientes/operacionesPaciente.hpp"
#include "Doctores/operacionDoctores.hpp"
#include <iostream>


int main() {
    using namespace std;

    cout << "Sistema de Gestión Hospitalaria\n";

    Hospital hospital;           // Se crea el objeto
    hospital.cargarHospital();   // Se carga la info real

   // Se pasa al menú

    return 0;
}
