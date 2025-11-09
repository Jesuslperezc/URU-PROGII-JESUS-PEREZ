
#ifndef STRUCTS_HPP
#define STRUCTS_HPP

#include <fstream>
using namespace std;
// ============================================
// ESTRUCTURAS
// ============================================
struct ArchivoHeader {
    int cantidadRegistros;      // Cantidad actual de registros
    int proximoID;              // Siguiente ID disponible
    int registrosActivos;       // Registros no eliminados
    int version;                // Versión del formato (ej: 1, 2, 3...)
};

struct HistorialMedico {
    // Datos originales
    int id;
    int pacienteID;                 // NUEVO: Referencia al paciente
    char fecha[11];
    char hora[6];
    char diagnostico[200];
    char tratamiento[200];
    char medicamentos[150];
    int doctorID;
    float costo;
    
    // NUEVO: Navegación enlazada
    int siguienteConsultaID;        // ID de siguiente consulta del mismo paciente
                                    // -1 si es la última
    
    // Metadata
    bool eliminado;
    time_t fechaRegistro;
};

struct Paciente {
    // Datos originales (sin cambios)
    int id;
    char nombre[50];
    char apellido[50];
    char cedula[20];
    int edad;
    char sexo;
    char tipoSangre[5];
    char telefono[15];
    char direccion[100];
    char email[50];
    char alergias[500];
    char observaciones[500];
    bool activo;
    
    // NUEVO: Índices para relaciones
    int cantidadConsultas;          // Total de consultas en historial
    int primerConsultaID;           // ID de primera consulta en historiales.bin
    
    int cantidadCitas;              // Total de citas agendadas
    int citasIDs[20];               // Array FIJO de IDs de citas (máx 20)
    
    // Metadata de registro
    bool eliminado;                 // Flag para borrado lógico
    time_t fechaCreacion;
    time_t fechaModificacion;
};

struct Doctor {
    // Datos originales
    int id;
    char nombre[50];
    char apellido[50];
    char cedulaProfesional[20];
    char especialidad[50];
    int aniosExperiencia;
    float costoConsulta;
    char horarioAtencion[50];
    char telefono[15];
    char email[50];
    bool disponible;
    
    // NUEVO: Relaciones con arrays fijos
    int cantidadPacientes;
    int pacientesIDs[50];           // Máximo 50 pacientes
    
    int cantidadCitas;
    int citasIDs[30];               // Máximo 30 citas
    
    // Metadata
    bool eliminado;
    time_t fechaCreacion;
    time_t fechaModificacion;
};

struct Cita {
    // Datos originales (sin cambios significativos)
    int id;
    int pacienteID;
    int doctorID;
    char fecha[11];
    char hora[6];
    char motivo[150];
    char estado[20];                // "Agendada", "Atendida", "Cancelada"
    char observaciones[200];
    bool atendida;
    
    // NUEVO: Referencia al historial
    int consultaID;                 // ID de consulta creada al atender
                                    // -1 si no ha sido atendida
    
    // Metadata
    bool eliminado;
    time_t fechaCreacion;
    time_t fechaModificacion;
};
struct Hospital {
    char nombre[100];
    char direccion[150];
    char telefono[15];
    
    // Contadores de IDs (auto-increment)
    int siguienteIDPaciente;
    int siguienteIDDoctor;
    int siguienteIDCita;
    int siguienteIDConsulta;
    
    // Estadísticas generales
    int totalPacientesRegistrados;
    int totalDoctoresRegistrados;
    int totalCitasAgendadas;
    int totalConsultasRealizadas;
};
#endif
