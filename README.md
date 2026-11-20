# Reproductor MP3 con Base de Datos e Interfaz Gráfica

Nombre: Gomez Ferro Ana Luisa

Materia: Modelado y Programación

## Descripción del proyecto.

Reproductor MP3 desarrollado en C++17 con interfaz gráfica basada en Qt6 (Widgets y Multimedia), que usa la biblioteca TagLib para la obtención de metadatos de los archivos MP3, y SQLite3 para la base de datos, ademas de implementar pruebas unitarias con Google Test.

## Tecnologías usadas.

* *Lenguajes:* C++17
* *Sistema de construcción:* CMake (v3.12+) y Make
* *Interfaz gráfica y multimedia:* Qt6 (Widgets y Multimedia)
* *Metadatos de audio:* TagLib
* *Base de datos:* SQLite3
* *Pruebas unitarias:* Google Test (GTest)
* *Compiladores:* g++

## Compilación y Ejecución (comandos).

Para preparar el entorno: cmake -B build

Para compilar: cmake --build build

Para ejecutar las pruebas unitarias individualmente: ./build/test_CancionesDAO y ./build/test_Minero

Para ejecutar el reproductor: ./build/Reproductor
