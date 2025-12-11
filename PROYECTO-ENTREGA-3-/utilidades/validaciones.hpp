 #ifndef VALIDACIONES_HPP
#define VALIDACIONES_HPP
 void pausarPantalla();
bool compararCaseInsensitive(const char* a, const char* b);
bool validarCedula(const char* cedula);
bool validarNombreSinEspacios(const char* nombre);
bool validarEdad(int edad);
bool validarSexoChar(char sexo);
bool validarFormatoHora(const char* hora);
bool validarFormatoFecha(const char* fecha);
#endif // VALIDACIONES_HPP