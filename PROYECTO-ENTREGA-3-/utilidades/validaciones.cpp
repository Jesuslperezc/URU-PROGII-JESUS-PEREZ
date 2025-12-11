#include <iostream>
#include <limits>
#include "validaciones.hpp"
#include <cstring>
using namespace std;
 void pausarPantalla() {
    std::cout << "\nPresione ENTER para continuar...";
    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
    std::cin.get();
}
bool  compararCaseInsensitive(const char* a, const char* b) {
    while (*a && *b) {
        if (tolower((unsigned char)*a) != tolower((unsigned char)*b))
            return false;
        ++a;
        ++b;
    }
    return *a == *b;
}
bool  validarCedula(const char* cedula){
    if (cedula == nullptr) return false;
    if (cedula[0] == '\0') return false;
    size_t len = strlen(cedula);
    if (len > 19) { // deja espacio para el terminador
        cout << "Excede el limite de caracteres para cédula.\n";
        return false;
    }
    // Solo dígitos
    for (size_t i = 0; i < len; ++i) {
        if (!isdigit((unsigned char)cedula[i])) {
            cout << "La cédula debe contener solo números.\n";
            return false;
        }
    }
    return true;
}

bool  validarNombreSinEspacios(const char* nombre) {
    if (nombre == nullptr) return false;
    if (nombre[0] == '\0') return false;
    // No permitir espacios en blanco
    for (size_t i = 0; i < strlen(nombre); ++i) {
        if (isspace((unsigned char)nombre[i])) {
            cout << "El nombre/apellido no debe contener espacios.\n";
            return false;
        }
    }
    return true;
}

bool  validarEdad(int edad) {
    if (edad < 0) {
        cout << "La edad no puede ser negativa.\n";
        return false;
    }
    if (edad > 120) {
        cout << "La edad supera el límite razonable (120).\n";
        return false;
    }
    return true;
}

bool  validarFormatoHora(const char* hora) {
    int hh, mm;
    return (sscanf(hora, "%2d:%2d", &hh, &mm) == 2 && hh >= 0 && hh < 24 && mm >= 0 && mm < 60);
}

bool  validarFormatoFecha(const char* fecha) {
    int anio, mes, dia;
    return (sscanf(fecha, "%4d-%2d-%2d", &anio, &mes, &dia) == 3 &&
            anio > 1900 && mes >= 1 && mes <= 12 && dia >= 1 && dia <= 31);
}
bool  validarSexoChar(char sexo) {
    char s = toupper((unsigned char)sexo);
    if (s == 'M' || s == 'F') return true;
    cout << "Sexo inválido. Use M o F.\n";
    return false;
}