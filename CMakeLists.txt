cmake_minimum_required(VERSION 3.12)
project(proyecto_reprodutor DESCRIPTION "reproductor MP3 con base de datos e interfaz gráfica" LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(TAGLIB REQUIRED taglib)

find_package(SQLite3 REQUIRED)

add_executable(Reproductor
    ${PROJECT_SOURCE_DIR}/main.cpp
    ${PROJECT_SOURCE_DIR}/Minero.cpp
    ${PROJECT_SOURCE_DIR}/Minero.h
    ${PROJECT_SOURCE_DIR}/CancionesDAO.cpp
    ${PROJECT_SOURCE_DIR}/CancionesDAO.h
    ${PROJECT_SOURCE_DIR}/Cancion.h
)

target_include_directories(Reproductor PRIVATE
    ${SQLITE3_INCLUDE_DIRS}
    ${TAGLIB_INCLUDE_DIRS}
)

target_link_libraries(Reproductor PRIVATE
    SQLite3::SQLite3
    ${TAGLIB_LIBRARIES}
)

target_compile_options(Reproductor PRIVATE ${TAGLIB_CFLAGS_OTHER})