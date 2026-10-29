cmake_minimum_required(VERSION 3.12)
project(proyecto_reprodutor DESCRIPTION "reproductor MP3 con base de datos e interfaz gráfica" LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(TAGLIB REQUIRED taglib)
find_package(SQLite3 REQUIRED)
find_package(GTest REQUIRED)

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

#Test.

#Test clase CancionesDAO
add_executable(test_CancionesDAO 
    ${PROJECT_SOURCE_DIR}/test/test_CancionesDAO.cpp
    ${PROJECT_SOURCE_DIR}/CancionesDAO.cpp
    ${PROJECT_SOURCE_DIR}/Cancion.h
)
file(COPY ${PROJECT_SOURCE_DIR}/rolas.sql DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_include_directories(test_CancionesDAO PRIVATE
    ${PROJECT_SOURCE_DIR}
    ${SQLITE3_INCLUDE_DIRS}
    ${TAGLIB_INCLUDE_DIRS}
)

target_link_libraries(test_CancionesDAO PRIVATE 
    GTest::gtest 
    GTest::gtest_main 
    pthread
    SQLite3::SQLite3)

# Ejecución de los test.
add_custom_command(
    TARGET test_CancionesDAO POST_BUILD
    COMMAND $<TARGET_FILE:test_CancionesDAO>
    COMMENT "Ejecutando pruebas de cancionesDAO..."
)

add_dependencies(Reproductor test_CancionesDAO)

#Test clase Minero
add_executable(test_Minero 
    ${PROJECT_SOURCE_DIR}/test/test_Minero.cpp
    ${PROJECT_SOURCE_DIR}/Minero.cpp
    ${PROJECT_SOURCE_DIR}/CancionesDAO.cpp
    ${PROJECT_SOURCE_DIR}/Cancion.h
)
file(COPY ${PROJECT_SOURCE_DIR}/Musica_tests DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_include_directories(test_Minero PRIVATE
    ${PROJECT_SOURCE_DIR}
    ${SQLITE3_INCLUDE_DIRS}
    ${TAGLIB_INCLUDE_DIRS}
)

target_link_libraries(test_Minero PRIVATE 
    GTest::gtest 
    GTest::gtest_main 
    pthread
    SQLite3::SQLite3  
    ${TAGLIB_LIBRARIES} 
)

# Ejecución de los test.
add_custom_command(
    TARGET test_Minero POST_BUILD
    COMMAND $<TARGET_FILE:test_Minero>
    COMMENT "Ejecutando pruebas de Minero..."
)

add_dependencies(test_Minero test_CancionesDAO)