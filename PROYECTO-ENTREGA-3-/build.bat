@echo off
REM =================================================
REM Build script robusto para proyecto C++ (MSYS2/MinGW)
REM =================================================

REM Crear carpetas build y bin si no existen
if not exist build mkdir build
if not exist bin mkdir bin

REM Limpiar objetos antiguos
echo Limpiando objetos antiguos...
del build\*.o >nul 2>&1

REM Limpiar ejecutable antiguo
if exist bin\miPrograma.exe del bin\miPrograma.exe

REM Compilar todos los .cpp recursivamente
echo =================================================
echo Compilando archivos fuente...
echo =================================================
set compileError=0

for /R %%f in (*.cpp) do (
    echo Compilando %%f
    g++ -std=c++17 -Wall -c "%%f" -I. -o "build\%%~nf.o"
    if errorlevel 1 (
        echo ERROR compilando %%f
        set compileError=1
    )
)

REM Verificar si hubo errores de compilación
if %compileError%==1 (
    echo =================================================
    echo Se encontraron errores de compilación. Abortando enlace.
    echo =================================================
    pause
    exit /b 1
)

REM Enlazar todos los objetos en bin\miPrograma.exe
echo =================================================
echo Enlazando objetos...
echo =================================================
g++ build\*.o -o bin\miPrograma.exe
if errorlevel 1 (
    echo =================================================
    echo ERROR durante el enlace. Abortando ejecución.
    echo =================================================
    pause
    exit /b 1
)

REM Ejecutar el programa
echo =================================================
echo Ejecución exitosa:
echo =================================================
.\bin\miPrograma.exe

pause
