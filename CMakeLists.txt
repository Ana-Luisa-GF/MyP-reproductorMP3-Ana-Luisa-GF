cmake_minimum_required(VERSION 3.12)
project(proyecto_reprodutor DESCRIPTION "reproductor MP3 con base de datos e interfaz gráfica" LANGUAGES C CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#======================================================================================
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

#===========================================================================
find_package(Qt6 REQUIRED COMPONENTS Widgets)

find_package(PkgConfig REQUIRED)
pkg_check_modules(TAGLIB REQUIRED taglib)
find_package(SQLite3 REQUIRED)
find_package(GTest REQUIRED)


include_directories(${PROJECT_SOURCE_DIR}/include)

add_executable(Reproductor
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/Minero.cpp
    ${PROJECT_SOURCE_DIR}/include/Minero.h
    ${PROJECT_SOURCE_DIR}/src/CancionesDAO.cpp
    ${PROJECT_SOURCE_DIR}/include/CancionesDAO.h
    ${PROJECT_SOURCE_DIR}/include/Cancion.h
    ${PROJECT_SOURCE_DIR}/src/RepControlador.cpp
    ${PROJECT_SOURCE_DIR}/include/RepControlador.h
    ${PROJECT_SOURCE_DIR}/include/mainwindow.h
    ${PROJECT_SOURCE_DIR}/src/mainwindow.cpp
    ${PROJECT_SOURCE_DIR}/src/mainwindow.ui
    ${PROJECT_SOURCE_DIR}/iconos.qrc
)

target_include_directories(Reproductor PRIVATE
    ${SQLITE3_INCLUDE_DIRS}
    ${TAGLIB_INCLUDE_DIRS}
)

target_link_libraries(Reproductor PRIVATE
    Qt6::Widgets
#===========================================================================
    SQLite3::SQLite3
    ${TAGLIB_LIBRARIES}
)

target_compile_options(Reproductor PRIVATE ${TAGLIB_CFLAGS_OTHER})



# Test CancionesDAO
add_executable(test_CancionesDAO
    ${PROJECT_SOURCE_DIR}/test/test_CancionesDAO.cpp
    ${PROJECT_SOURCE_DIR}/src/CancionesDAO.cpp
    ${PROJECT_SOURCE_DIR}/include/Cancion.h
)
file(COPY ${PROJECT_SOURCE_DIR}/rolas.sql DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_include_directories(test_CancionesDAO PRIVATE
    ${PROJECT_SOURCE_DIR}/include
    ${SQLITE3_INCLUDE_DIRS}
    ${TAGLIB_INCLUDE_DIRS}
)

target_link_libraries(test_CancionesDAO PRIVATE
    GTest::gtest
    GTest::gtest_main
    pthread
    SQLite3::SQLite3
)


# Test Minero
add_executable(test_Minero
    ${PROJECT_SOURCE_DIR}/test/test_Minero.cpp
    ${PROJECT_SOURCE_DIR}/src/Minero.cpp
    ${PROJECT_SOURCE_DIR}/src/CancionesDAO.cpp
    ${PROJECT_SOURCE_DIR}/include/Cancion.h
)
file(COPY ${PROJECT_SOURCE_DIR}/Musica_tests DESTINATION ${CMAKE_CURRENT_BINARY_DIR})

target_include_directories(test_Minero PRIVATE
    ${PROJECT_SOURCE_DIR}/include
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


add_custom_target(correr_todas_las_pruebas ALL
    COMMAND $<TARGET_FILE:test_CancionesDAO> --gtest_color=yes
    COMMAND $<TARGET_FILE:test_Minero> --gtest_color=yes
    DEPENDS test_CancionesDAO test_Minero
    COMMENT "--- Ejecutando pruebas unitarias ---"
)

add_dependencies(Reproductor correr_todas_las_pruebas)