#include <gtest/gtest.h>
#include "CancionesDAO.h"
#include "Cancion.h"
#include <vector>
#include <string>
#include <filesystem>
#include <pwd.h> 
#include <unistd.h> 

namespace fileSys = std::filesystem;

TEST(DAO_Test, CreacionBD) {
    CancionesDAO dao(true);
    dao.crearBaseDeDatos();
    const char *homedir;
    if ((homedir = getenv("HOME")) == NULL) {
        homedir = getpwuid(getuid())->pw_dir;
    }

    fileSys::path ruta_bd = std::string(homedir) + "/.local/share/proyec_reproductorMP3/rolas.bd";
    ASSERT_TRUE(fileSys::exists(ruta_bd));
    ASSERT_TRUE(fileSys::is_regular_file(ruta_bd));
}

TEST(DAO_Test, AgregarCancion) {
    CancionesDAO dao(true);
    dao.crearBaseDeDatos();

    fileSys::path rutaFalsa = "/musica/cancion_prueba.mp3";
    Cancion c(rutaFalsa,"titulo_c","artista_c","genero_c", "album_C", 12, 2026);
    std::vector<Cancion> vectorCanciones;
    vectorCanciones.push_back(c);
    dao.agregarCanciones(vectorCanciones);


    sqlite3* bdAux = nullptr;
    const char *homedir = getenv("HOME");
    fileSys::path ruta_bd = std::string(homedir) + "/.local/share/proyec_reproductorMP3/rolas.bd";
    
    sqlite3_open(ruta_bd.c_str(), &bdAux);
    
    sqlite3_stmt* stmt = nullptr;
    std::string consulta = "SELECT * FROM rolas WHERE path = ?;";
    sqlite3_prepare_v2(bdAux, consulta.c_str(), -1, &stmt, nullptr);
    
    sqlite3_bind_text(stmt, 1, rutaFalsa.string().c_str(), -1, SQLITE_TRANSIENT);

    EXPECT_TRUE(sqlite3_step(stmt) == SQLITE_ROW);
    sqlite3_finalize(stmt);
    sqlite3_close(bdAux);  
}

TEST(DAO_Test, AgregarPerformer) {
    CancionesDAO dao(true);
    dao.crearBaseDeDatos();
    std::string nombre = "performer";
    dao.agregarPerformer(nombre);


    sqlite3* bdAux = nullptr;
    const char *homedir = getenv("HOME");
    fileSys::path ruta_bd = std::string(homedir) + "/.local/share/proyec_reproductorMP3/rolas.bd";
    
    sqlite3_open(ruta_bd.c_str(), &bdAux);
    
    sqlite3_stmt* stmt = nullptr;
    std::string consulta = "SELECT * FROM performers WHERE name = ?;";
    sqlite3_prepare_v2(bdAux, consulta.c_str(), -1, &stmt, nullptr);
    
    sqlite3_bind_text(stmt, 1, nombre.c_str(), -1, SQLITE_TRANSIENT);

    EXPECT_TRUE(sqlite3_step(stmt) == SQLITE_ROW);
    sqlite3_finalize(stmt);
    sqlite3_close(bdAux);  
}

TEST(DAO_Test, AgregarAlbum) {
    CancionesDAO dao(true);
    dao.crearBaseDeDatos();
    std::string nombreAlbum = "Albun_prueba";
    fileSys::path rutaFalsa = "/musica/Album_prueba";
    dao.agregarAlbun(rutaFalsa, nombreAlbum);


    sqlite3* bdAux = nullptr;
    const char *homedir = getenv("HOME");
    fileSys::path ruta_bd = std::string(homedir) + "/.local/share/proyec_reproductorMP3/rolas.bd";
    
    sqlite3_open(ruta_bd.c_str(), &bdAux);
    
    sqlite3_stmt* stmt = nullptr;
    std::string consulta = "SELECT * FROM albums WHERE name = ? AND path = ?;";
    sqlite3_prepare_v2(bdAux, consulta.c_str(), -1, &stmt, nullptr);
    
    sqlite3_bind_text(stmt, 1, nombreAlbum.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmt, 2, rutaFalsa.string().c_str(), -1, SQLITE_TRANSIENT);

    EXPECT_TRUE(sqlite3_step(stmt) == SQLITE_ROW);
    sqlite3_finalize(stmt);
    sqlite3_close(bdAux);  
}



