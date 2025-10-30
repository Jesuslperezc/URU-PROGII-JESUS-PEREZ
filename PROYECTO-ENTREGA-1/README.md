===========================================README==================================================

Descripción del proyecto
Sistema de gestión hospitalaria escrito en C++ que permite administrar de forma local: datos del hospital, pacientes, doctores, citas y historiales médicos. Implementa manejo manual de memoria con arrays dinámicos y operaciones completas de CRUD para las entidades principales.

Tecnologías y requisitos
- Lenguaje: C++ (estilo compatible con C; usa headers estándar).
- Compilador recomendado: g++ o clang++ (soporta C++98/11).
- Plataforma: cualquiera con compilador C++ y consola.
- Requisitos: no usa bibliotecas externas; solo la STL mínima (iostream, cstring, ctime, iomanip, fstream, cctype, limits).

Archivos
- main.cpp (o el archivo fuente que contenga el código proporcionado)
- README.txt (este documento)

Cómo compilar y ejecutar
- Renombrar el archivo fuente a main.cpp si fuera necesario: g++ -o hospital main.cpp
- Ejecutar el binario generado: ./hospital

Estructura de datos y diseño
- Hospital
- Campos: nombre, dirección, teléfono.
- Arrays dinámicos: pacientes, doctores, citas.
- Contadores y siguientes identificadores: siguienteIdPaciente, siguienteIdDoctor, siguienteIdCita, siguienteIdConsulta.
- Paciente
- Campos personales: id, nombre, apellido, cédula, edad, sexo, tipo de sangre, teléfono, dirección, email.
- Historial: puntero dinámico a HistorialMedico con redimensionamiento.
- Citas: array dinámico de IDs de citas con redimensionamiento.
- Campos adicionales: alergias, observaciones, activo.
- Doctor
- Campos personales y profesionales: id, nombre, apellido, cédula, especialidad, años de experiencia, costo de consulta, horario, teléfono, email.
- Pacientes asignados: array dinámico de IDs.
- Citas agendadas: array dinámico de IDs.
- Disponible: flag booleano.
- Cita
- id, idPaciente, idDoctor, fecha, hora, motivo, estado, observaciones, atendida.
- HistorialMedico
- idConsulta, fecha, hora, diagnóstico, tratamiento, medicamentos, idDoctor, costo.

Funcionalidades implementadas
- Registro inicial del hospital con inicialización de arrays dinámicos.
- Pacientes
- Crear paciente nuevo verificando duplicidad de cédula.
- Buscar por cédula (case-insensitive), por ID y por nombre exacto (case-insensitive).
- Actualizar campos básicos (nombre, apellido, edad, sexo).
- Eliminar paciente liberando su historial y citas agendadas.
- Listar todos los pacientes con datos básicos.
- Manejo dinámico de historial y lista de citas con redimensionamiento automátic o.
- Doctores
- Crear doctor verificando duplicidad de cédula.
- Buscar por ID y por especialidad (case-insensitive).
- Asignar y remover pacientes a un doctor (evita duplicados).
- Listar doctores con formato tabular y estado de disponibilidad.
- Eliminar doctor cancelando sus citas y liberando arrays asociados.
- Citas
- Agendar cita con validación de formato fecha YYYY-MM-DD y hora HH:MM.
- Verificar disponibilidad del doctor (evita solapamiento exacto fecha+hora).
- Cancelar cita actualizando su estado y removiendo referencias en paciente y doctor.
- Atender cita: registrar diagnóstico/tratamiento/medicamentos; almacenar en historial del paciente y tomar el costo desde el doctor; marcar cita como atendida.
- Consultas por paciente, doctor y por fecha; listar citas pendientes.
- Validaciones básicas
- Formato de fecha y hora.
- Verificación de duplicidad de cédula al crear paciente o doctor.
- Comprobación de existencia de IDs antes de operar.

Flujo de uso recomendado
- Ejecutar el programa y completar los datos del hospital.
- Registrar doctores antes de agendar citas para que las citas tomen costo y disponibilidad.
- Registrar pacientes (cédula única) antes de agendar citas.
- Agendar citas usando ID paciente y ID doctor; verificar que la función de disponibilidad retorne true.
- Atender citas para generar entradas en el historial médico y computar costos.
- Consultar historiales, listar citas pendientes o cancelar citas desde el menú.

Casos de validación y ejemplos
- Fecha válida: 2025-11-03
- Hora válida: 14:30
- Sexo válido: M o F
- Al crear un paciente con cédula ya existente el sistema impide duplicados.
- Si se intenta agendar una cita en fecha y hora donde ya existe una cita agendada para el mismo doctor, la operación es rechazada.

Limitaciones actuales
- No existe persistencia en disco; todos los datos se pierden al cerrar la ejecución.
- Manejo manual de memoria con new/delete: riesgo de leaks o copias superficiales si el código se modifica.
- Uso de buffers con tamaños fijos para strings; pueden requerir sanitización adicional para entradas largas.
- Validaciones básicas únicamente (cédula, formatos); no hay validaciones por país, formato telefónico o correo.
- No hay control de concurrencia ni multiusuario; aplicación de consola de un solo usuario.

Riesgos y consideraciones técnicas
- Al redimensionar arrays se hacen copias superficiales de estructuras que contienen punteros; si se llegase a introducir gestión más compleja (por ejemplo punteros a memoria interna), habría que implementar copias profundas o migrar a std::vector y std::string.
- Los métodos que retornen arrays dinámicos (por ejemplo obtenerCitasDePaciente) delegan la liberación al llamador; documentar ese contrato para evitar leaks.

Recomendaciones de refactorización prioritarias
- Migrar arrays a std::vector y cadenas a std::string para seguridad y simplicidad.
- Separar el código en módulos: hospital.h/hospital.cpp, paciente.h/paciente.cpp, doctor.h/doctor.cpp, cita.h/cita.cpp, main.cpp.
- Implementar persistencia: guardar/leer estructuras a archivo binario o formato JSON/CSV.
- Añadir validaciones robustas y manejo de errores consistente (códigos de retorno, logs).
- Escribir pruebas unitarias para funciones críticas: crearPaciente, crearDoctor, agendarCita, atenderCita, cancelarCita.
- Considerar reemplazar sistema de IDs automáticos por gestión centralizada con verificación al deserializar datos guardados.

Mejoras funcionales sugeridas
- Exportar historiales y listados a CSV.
- Integración con calendario externo o envío de recordatorios por correo.
- Interfaz gráfica con Qt o migración a aplicación web con backend REST.
- Gestión de usuarios y roles (administrador, recepcionista, médico) con permisos.


