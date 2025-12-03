@echo off
setlocal

set APP_NAME=PROYECTO-ENTREGA-3.exe
set BUILD_DIR=build
set BIN_DIR=bin

if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
if not exist "%BIN_DIR%" mkdir "%BIN_DIR%"

g++ -std=c++17 -Wall -Wextra -O2 -ICitas -IDoctores -IHistoriales -IHospital -IPacientes -Ipersistencia -Iutilidades -c main.cpp -o %BUILD_DIR%/main.o
g++ -std=c++17 -Wall -Wextra -O2 -ICitas -IDoctores -IHistoriales -IHospital -IPacientes -Ipersistencia -Iutilidades -c Citas/Citas.cpp -o %BUILD_DIR%/Citas.o
g++ -std=c++17 -Wall -Wextra -O2 -ICitas -IDoctores -IHistoriales -IHospital -IPacientes -Ipersistencia -Iutilidades -c Doctores/Doctores.cpp -o %BUILD_DIR%/Doctores.o
g++ -std=c++17 -Wall -Wextra -O2 -ICitas -IDoctores -IHistoriales -IHospital -IPacientes -Ipersistencia -Iutilidades -c Historiales/Historial.cpp -o %BUILD_DIR%/Historial.o
g++ -std=c++17 -Wall -Wextra -O2 -ICitas -IDoctores -IHistoriales -IHospital -IPacientes -Ipersistencia -Iutilidades -c Hospital/Hospital.cpp -o %BUILD_DIR%/Hospital.o
g++ -std=c++17 -Wall -Wextra -O2 -ICitas -IDoctores -IHistoriales -IHospital -IPacientes -Ipersistencia -Iutilidades -c Pacientes/Pacientes.cpp -o %BUILD_DIR%/Pacientes.o
g++ -std=c++17 -Wall -Wextra -O2 -ICitas -IDoctores -IHistoriales -IHospital -IPacientes -Ipersistencia -Iutilidades -c Pacientes/operacionesPaciente.cpp -o %BUILD_DIR%/operacionesPaciente.o
g++ -std=c++17 -Wall -Wextra -O2 -ICitas -IDoctores -IHistoriales -IHospital -IPacientes -Ipersistencia -Iutilidades -c persistencia/GestionArchivos.cpp -o %BUILD_DIR%/GestionArchivos.o

g++ ^
  %BUILD_DIR%/main.o ^
  %BUILD_DIR%/Citas.o ^
  %BUILD_DIR%/Doctores.o ^
  %BUILD_DIR%/Historial.o ^
  %BUILD_DIR%/Hospital.o ^
  %BUILD_DIR%/Pacientes.o ^
  %BUILD_DIR%/operacionesPaciente.o ^
  %BUILD_DIR%/GestionArchivos.o ^
  -o %BIN_DIR%/%APP_NAME%

echo.
echo ================================================
echo Compilación terminada. Ejecutando el programa...
echo ================================================
echo.

%BIN_DIR%/%APP_NAME%

endlocal