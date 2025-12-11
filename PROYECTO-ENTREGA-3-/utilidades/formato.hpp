#ifndef FORMATO_HPP
#define FORMATO_HPP
#include <string>
#include <iomanip>
#include <iostream>
#include <limits>

class Formato {
public:
    // Colores de texto
    static constexpr const char* RESET       = "\033[0m";
    static constexpr const char* ROJO        = "\033[31m";
    static constexpr const char* VERDE       = "\033[32m";
    static constexpr const char* AMARILLO    = "\033[33m";
    static constexpr const char* AZUL        = "\033[34m";
    static constexpr const char* MAGENTA     = "\033[35m";
    static constexpr const char* CYAN        = "\033[36m";
    static constexpr const char* BLANCO      = "\033[37m";

    // Negrita y subrayado
    static constexpr const char* NEGRITA     = "\033[1m";
    static constexpr const char* SUBRAYADO   = "\033[4m";

    // Fondos
    static constexpr const char* BG_ROJO     = "\033[41m";
    static constexpr const char* BG_VERDE    = "\033[42m";
    static constexpr const char* BG_AMARILLO = "\033[43m";
    static constexpr const char* BG_AZUL     = "\033[44m";
    static constexpr const char* BG_MAGENTA  = "\033[45m";
    static constexpr const char* BG_CYAN     = "\033[46m";
    static constexpr const char* BG_BLANCO   = "\033[47m";

    // Función para imprimir título centrado
    static void titulo(const std::string& texto, char relleno = '=') {
        int ancho = 80; // ancho de la consola
        int padding = (ancho - texto.size()) / 2;
        std::cout << std::string(padding, relleno)
                  << texto
                  << std::string(padding, relleno) << "\n";
    }

    // Función para imprimir subtítulo
    static void subtitulo(const std::string& texto) {
        std::cout << NEGRITA << texto << RESET << "\n";
    }

    // Función para imprimir tabla con color en encabezados
    static void encabezadoTabla(const std::string& texto) {
        std::cout << BG_AZUL << BLANCO << NEGRITA
                  << " " << texto << " " << RESET << "\n";
    }

    // Función para resaltar mensaje
    static void mensaje(const std::string& texto, const char* color = VERDE) {
        std::cout << color << texto << RESET << "\n";
    }

    // Función para acentos y caracteres especiales
    static std::string acentos(const std::string& texto) {
        std::string resultado = texto;
        // Reemplazos simples: puedes agregar más si quieres
        replaceAll(resultado, "á", "\xE1");
        replaceAll(resultado, "é", "\xE9");
        replaceAll(resultado, "í", "\xED");
        replaceAll(resultado, "ó", "\xF3");
        replaceAll(resultado, "ú", "\xFA");
        replaceAll(resultado, "ñ", "\xF1");
        replaceAll(resultado, "Ñ", "\xD1");
        return resultado;
    }
   


private:
    static void replaceAll(std::string &str, const std::string &from, const std::string &to) {
        size_t start_pos = 0;
        while((start_pos = str.find(from, start_pos)) != std::string::npos) {
            str.replace(start_pos, from.length(), to);
            start_pos += to.length();
        }
    }
};














#endif // FORMATO_HPP