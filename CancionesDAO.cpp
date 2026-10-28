#include "CancionesDAO.h"
#include <cstdlib>
#include <cstdio>
#include <string>
#include <fstream>
#include <cstring>
#include <iostream>
#include <unistd.h>
#include <sys/stat.h>
#include <pwd.h>
//#include <sys/types.h>
//#include <sstream>
//#include <ctime>


using namespace std;
namespace fileSys= std::filesystem;

CancionesDAO:: ~CancionesDAO(){
    if (baseDeDatos != nullptr) 
        sqlite3_close(baseDeDatos);
}

void CancionesDAO::crearBaseDeDatos(){
    const char *homedir;

    printf("ENTRAMOS AL METODO \n");

    if ((homedir = getenv("HOME")) == NULL) {
        homedir = getpwuid(getuid())->pw_dir;
    }

    fileSys::path ruta_bd = homedir;
    ruta_bd /= ".local/share/proyec_reproductorMP3";

    std::cout << "Ruta evaluada: " << ruta_bd.string() << std::endl;

    if(!fileSys::exists(ruta_bd) || !fileSys::is_directory(ruta_bd)){
          printf("No existe\n");
        if (mkdir(ruta_bd.c_str(), 0755) != 0){
            fprintf(stderr,"Fallo al crear el directorio para la base de datos: %s\n", strerror(errno));
            exit(1);
        } 
    }
  
        
    fileSys::path archivoBD = ruta_bd / "rolas.bd";
    
    bool archivoYaExistia = fileSys::exists(archivoBD);

    int codigo_error = sqlite3_open(archivoBD.c_str(), &baseDeDatos);

    if (codigo_error != SQLITE_OK){ 
        fprintf(stderr,"Error al abrir la base de datos:: %s\n",  sqlite3_errmsg(baseDeDatos)); 
        exit(1);
    }

    if(!archivoYaExistia){
        ifstream archivoSql("rolas.sql");

        if (!archivoSql.is_open()) {
            fprintf(stderr, "No se pudo abrir el archivo rolas.sql para inicializar las tablas.\n");
            sqlite3_close(baseDeDatos);
            exit(1);
        }

        std::stringstream buffer;
        buffer << archivoSql.rdbuf();
        string consultaSql = buffer.str();
        archivoSql.close();

        char* mensajeError = nullptr;
        int mensaje = sqlite3_exec(baseDeDatos, consultaSql.c_str(), 0, 0, &mensajeError);

        if (mensaje != SQLITE_OK) {
            fprintf(stderr, "Error al crear las tablas: %s\n", mensajeError);
            sqlite3_free(mensajeError);
            sqlite3_close(baseDeDatos);
            exit(1);
        }
    }

}


void CancionesDAO::agregarCanciones(const std::vector<Cancion>& listCanciones){
    string peticion1 = "INSERT OR IGNORE INTO rolas (id_performer, id_album, path, " 
                       "title, track, year, genre) VALUES (?, ?, ?, ?, ?, ?, ?);";
    sqlite3_stmt* stmtInsert = nullptr;

    if (sqlite3_prepare_v2(baseDeDatos, peticion1.c_str(), -1, &stmtInsert, nullptr) != SQLITE_OK)
        return;
    

    string peticion2 = "SELECT * FROM rolas WHERE path = ?;";
    sqlite3_stmt* stmtSelect = nullptr;
    if(sqlite3_prepare_v2(baseDeDatos, peticion2.c_str(), -1, &stmtSelect, nullptr) != SQLITE_OK){
        sqlite3_finalize(stmtInsert);
        return;
    }
    
    int performer;
    int album;

    for(const Cancion& cancion : listCanciones){
        sqlite3_bind_text(stmtSelect, 1, cancion.ruta.string().c_str(), -1, SQLITE_TRANSIENT);
        if(sqlite3_step(stmtSelect) == SQLITE_ROW){
            sqlite3_reset(stmtSelect);
            continue;
        }
            

        performer = agregarPerformer(cancion.artista);
        if(performer == -1)
            continue;
        album = agregarAlbun(cancion.ruta.parent_path().string(),cancion.album);
        if(album == -1)
        continue;
        
        sqlite3_bind_int(stmtInsert, 1, performer);
        sqlite3_bind_int(stmtInsert, 2, album);
        sqlite3_bind_text(stmtInsert, 3, cancion.ruta.string().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmtInsert, 4, cancion.titulo.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmtInsert, 5, cancion.track);
        sqlite3_bind_int(stmtInsert, 6, cancion.anio);
        sqlite3_bind_text(stmtInsert, 7, cancion.genero.c_str(), -1, SQLITE_TRANSIENT);

        if (sqlite3_step(stmtInsert) != SQLITE_DONE)
            fprintf(stderr, "Error al insertar la canción: %s\n", sqlite3_errmsg(baseDeDatos));
        
        sqlite3_reset(stmtInsert);
        sqlite3_reset(stmtSelect);
    }

    sqlite3_finalize(stmtInsert);
    sqlite3_finalize(stmtSelect);
}

int CancionesDAO::agregarPerformer(std::string nombre){

    string peticion1 = "SELECT * FROM performers WHERE name = ?";
    sqlite3_stmt* stmtSelect = nullptr;
    if(sqlite3_prepare_v2(baseDeDatos, peticion1.c_str(), -1, &stmtSelect, nullptr) != SQLITE_OK)
        return -1;

    sqlite3_bind_text(stmtSelect, 1, nombre.c_str(), -1, SQLITE_TRANSIENT);
    if(sqlite3_step(stmtSelect) == SQLITE_ROW){
            int id_performer = sqlite3_column_int(stmtSelect, 0);
            sqlite3_finalize(stmtSelect);
            return id_performer;
        }

    sqlite3_finalize(stmtSelect);


    string peticion2 = "INSERT OR IGNORE INTO performers (id_type, name) VALUES (?, ?);";
    sqlite3_stmt* stmtInsert = nullptr;

    if (sqlite3_prepare_v2(baseDeDatos, peticion2.c_str(), -1, &stmtInsert, nullptr) != SQLITE_OK)
        return -1;
    

    sqlite3_bind_int(stmtInsert, 1, 2);
    sqlite3_bind_text(stmtInsert, 2, nombre.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmtInsert) != SQLITE_DONE){
        fprintf(stderr, "Error al insertar performer: %s\n", sqlite3_errmsg(baseDeDatos));
        sqlite3_finalize(stmtInsert);
        return -1;
    }

    int id_performer = static_cast<int>(sqlite3_last_insert_rowid(baseDeDatos));
    
    sqlite3_finalize(stmtInsert);

    return id_performer;
}

int CancionesDAO::agregarAlbun(std::filesystem::path ruta, std::string nombre){

    string peticion1 = "SELECT * FROM albums WHERE name = ? AND path = ?";
    sqlite3_stmt* stmtSelect = nullptr;
    if(sqlite3_prepare_v2(baseDeDatos, peticion1.c_str(), -1, &stmtSelect, nullptr) != SQLITE_OK)
        return -1;

    sqlite3_bind_text(stmtSelect, 1, nombre.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmtSelect, 2, ruta.string().c_str(), -1, SQLITE_TRANSIENT);

    if(sqlite3_step(stmtSelect) == SQLITE_ROW){
            int id_album = sqlite3_column_int(stmtSelect, 0);
            sqlite3_finalize(stmtSelect);
            return id_album;
        }

    sqlite3_finalize(stmtSelect);


    string peticion2 = "INSERT OR IGNORE INTO albums (path, name) VALUES (?, ?);";
    sqlite3_stmt* stmtInsert = nullptr;

    if (sqlite3_prepare_v2(baseDeDatos, peticion2.c_str(), -1, &stmtInsert, nullptr) != SQLITE_OK)
        return -1;
    

    sqlite3_bind_text(stmtInsert, 1, ruta.string().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmtInsert, 2, nombre.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmtInsert) != SQLITE_DONE){
        fprintf(stderr, "Error al insertar album: %s\n", sqlite3_errmsg(baseDeDatos));
        sqlite3_finalize(stmtInsert);
        return -1;
    }

    int id_album = static_cast<int>(sqlite3_last_insert_rowid(baseDeDatos));
    
    sqlite3_finalize(stmtInsert);

    return id_album;
}

