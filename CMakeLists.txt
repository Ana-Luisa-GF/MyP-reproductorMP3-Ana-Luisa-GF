
project(proyecto_reprodutor DESCRIPTION "reproductor MP3 con base de datos e interfaz gráfica" LANGUAGES C CXX)

find_package(SQLite3 REQUIRED)
find_package(Taglib REQUIRED)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Minero
${PROJECT_SOURCE_DIR}/minero.cpp
${PROJECT_SOURCE_DIR}/minero.h
)

target_include_directories(Minero PRIVATE
    ${SQLITE3_INCLUDE_DIRS}
    ${TAGLIB_INCLUDE_DIRS}
)

target_link_libraries(Minero PRIVATE
    SQLite::SQLite3
    taglib
)