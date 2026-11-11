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

using namespace std;
namespace fileSys= std::filesystem;

CancionesDAO:: ~CancionesDAO(){
    if (baseDeDatos != nullptr) 
        sqlite3_close(baseDeDatos);
}

void CancionesDAO::crearBaseDeDatos(){
    fileSys::path archivoBD;
    bool archivoYaExistia = false;

    if(usarRutaTests){
        archivoBD = ":memory:";
    }else{
        const char *homedir;
        if ((homedir = getenv("HOME")) == NULL) {
            homedir = getpwuid(getuid())->pw_dir;
        }

        fileSys::path ruta_bd = homedir;
        ruta_bd /= ".local/share/proyec_reproductorMP3";

        std::cout << "Ruta evaluada: " << ruta_bd.string() << std::endl;

        if(!fileSys::exists(ruta_bd) || !fileSys::is_directory(ruta_bd)){
            if (mkdir(ruta_bd.c_str(), 0755) != 0){
                fprintf(stderr,"Fallo al crear el directorio para la base de datos: %s\n", strerror(errno));
                exit(1);
            } 
        }
    
            
        archivoBD = ruta_bd / "rolas.bd";
        archivoYaExistia = fileSys::exists(archivoBD);
    }

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

        mensajeError = nullptr;
        consultaSql = "ALTER TABLE rolas ADD COLUMN portada TEXT;";
        mensaje = sqlite3_exec(baseDeDatos, consultaSql.c_str(), 0, 0, &mensajeError);

        if (mensaje != SQLITE_OK) {
            fprintf(stderr, "Error al crear las tablas: %s\n", mensajeError);
            sqlite3_free(mensajeError);
            sqlite3_close(baseDeDatos);
            exit(1);
        }

        mensajeError = nullptr;
        consultaSql = "ALTER TABLE albums ADD COLUMN portada TEXT;";
        mensaje = sqlite3_exec(baseDeDatos, consultaSql.c_str(), 0, 0, &mensajeError);

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
                       "title, track, year, genre, portada) VALUES (?, ?, ?, ?, ?, ?, ?, ?);";
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
        album = agregarAlbun(cancion.ruta.parent_path().string(),cancion.album, cancion.portada);
        if(album == -1)
        continue;
        
        sqlite3_bind_int(stmtInsert, 1, performer);
        sqlite3_bind_int(stmtInsert, 2, album);
        sqlite3_bind_text(stmtInsert, 3, cancion.ruta.string().c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmtInsert, 4, cancion.titulo.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_int(stmtInsert, 5, cancion.track);
        sqlite3_bind_int(stmtInsert, 6, cancion.anio);
        sqlite3_bind_text(stmtInsert, 7, cancion.genero.c_str(), -1, SQLITE_TRANSIENT);
        sqlite3_bind_text(stmtInsert, 8, cancion.portada.string().c_str(), -1, SQLITE_TRANSIENT);

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

int CancionesDAO::agregarAlbun(std::filesystem::path ruta, std::string nombre, std::string portada){

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


    string peticion2 = "INSERT OR IGNORE INTO albums (path, name,year,portada) VALUES (?, ?, 2026, ?);";
    sqlite3_stmt* stmtInsert = nullptr;

    if (sqlite3_prepare_v2(baseDeDatos, peticion2.c_str(), -1, &stmtInsert, nullptr) != SQLITE_OK)
        return -1;
    

    sqlite3_bind_text(stmtInsert, 1, ruta.string().c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmtInsert, 2, nombre.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_text(stmtInsert, 3, portada.c_str(), -1, SQLITE_TRANSIENT);

    if (sqlite3_step(stmtInsert) != SQLITE_DONE){
        fprintf(stderr, "Error al insertar album: %s\n", sqlite3_errmsg(baseDeDatos));
        sqlite3_finalize(stmtInsert);
        return -1;
    }

    int id_album = static_cast<int>(sqlite3_last_insert_rowid(baseDeDatos));
    
    sqlite3_finalize(stmtInsert);

    return id_album;
}


std::vector<Cancion> CancionesDAO::getCanciones(){
    vector<Cancion> rolas;
    std::string peticion = "SELECT r.path, r.title, r.track, r.year, r.genre, r.portada, p.name, a.name, r.id_rola, a.id_album, r.id_performer "
                           "FROM rolas r "
                           "JOIN performers p ON r.id_performer = p.id_performer "
                           "JOIN albums a ON r.id_album = a.id_album;";

    sqlite3_stmt* stmtSelect = nullptr;
    if(sqlite3_prepare_v2(baseDeDatos, peticion.c_str(), -1, &stmtSelect, nullptr) != SQLITE_OK)
      return rolas;


    while(sqlite3_step(stmtSelect) == SQLITE_ROW){

        const unsigned char* c_ruta = sqlite3_column_text(stmtSelect, 0);
        std::string ruta = c_ruta ? reinterpret_cast<const char*>(c_ruta) : "";

        const unsigned char* c_titulo = sqlite3_column_text(stmtSelect, 1);
        std::string titulo = c_titulo ? reinterpret_cast<const char*>(c_titulo) : "";

        int track = sqlite3_column_int(stmtSelect, 2);
        int anio = sqlite3_column_int(stmtSelect, 3);

        const unsigned char* c_genero = sqlite3_column_text(stmtSelect, 4);
        std::string genero = c_genero ? reinterpret_cast<const char*>(c_genero) : "";

        const unsigned char* c_portada = sqlite3_column_text(stmtSelect, 5);
        std::string portada = c_portada ? reinterpret_cast<const char*>(c_portada) : "";

        const unsigned char* c_artista = sqlite3_column_text(stmtSelect, 6);
        std::string artista = c_artista ? reinterpret_cast<const char*>(c_artista) : "Unknown";

        const unsigned char* c_album = sqlite3_column_text(stmtSelect, 7);
        std::string album = c_album ? reinterpret_cast<const char*>(c_album) : "Unknown";

        int id = sqlite3_column_int(stmtSelect, 8);
        int id_album = sqlite3_column_int(stmtSelect, 9);
        int id_performer = sqlite3_column_int(stmtSelect, 10);

        Cancion c(ruta,titulo,artista,genero,album,track,anio,portada);
        c.setId(id);
        c.setIdAlbum(id_album);

        rolas.push_back(c);
    }
    sqlite3_finalize(stmtSelect);
    return rolas;
}

bool CancionesDAO::modificarColumna(const std::string &columna, const int &id, const std::string &nuevoValor){
    if (columna != "genre" && columna != "title" && columna != "year" & columna != "track")
        return false;

    std::string peticion = "UPDATE rolas SET " + columna + " = ? WHERE id_rola = ?";

    sqlite3_stmt* stmtSelect = nullptr;
    if(sqlite3_prepare_v2(baseDeDatos, peticion.c_str(), -1, &stmtSelect, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_text(stmtSelect, 1, nuevoValor.c_str(), -1, SQLITE_TRANSIENT);
    sqlite3_bind_int(stmtSelect, 2, id);

    bool exito = (sqlite3_step(stmtSelect) == SQLITE_DONE);

    sqlite3_finalize(stmtSelect);
    return exito;
}

Album CancionesDAO::getAlbum(const int &id){
    Album album;
    string peticion = "SELECT id_album, path, name, year, portada FROM albums WHERE id_album = ?";
    sqlite3_stmt* stmtSelect = nullptr;
    if(sqlite3_prepare_v2(baseDeDatos, peticion.c_str(), -1, &stmtSelect, nullptr) != SQLITE_OK){
        album.album_valido=false;
        return album;
    }

    sqlite3_bind_int(stmtSelect, 1, id);

    if(sqlite3_step(stmtSelect) == SQLITE_ROW){
        int albumId = sqlite3_column_int(stmtSelect, 0);

        const unsigned char* pathPtr = sqlite3_column_text(stmtSelect, 1);
        std::string ruta = pathPtr ? reinterpret_cast<const char*>(pathPtr) : "";

        const unsigned char* namePtr = sqlite3_column_text(stmtSelect, 2);
        std::string nombre = namePtr ? reinterpret_cast<const char*>(namePtr) : "";

        int anio = sqlite3_column_int(stmtSelect, 3);

        const unsigned char* portadaPtr = sqlite3_column_text(stmtSelect, 4);
        std::string portada = portadaPtr ? reinterpret_cast<const char*>(portadaPtr) : "";

        sqlite3_finalize(stmtSelect);
        return Album(albumId, ruta, nombre, portada, anio, true);
    }
    sqlite3_finalize(stmtSelect);
    album.album_valido=false;
    return album;
}

bool CancionesDAO::modificarColumna_album(const int &id, const std::string &columna,const int &anio, const std::string &nuevoValor){
    if (columna != "name" && columna != "year")
        return false;

    std::string peticion = "UPDATE albums SET " + columna + " = ? WHERE id_album = ?";

    sqlite3_stmt* stmtSelect = nullptr;
    if(sqlite3_prepare_v2(baseDeDatos, peticion.c_str(), -1, &stmtSelect, nullptr) != SQLITE_OK)
        return false;

    if(columna == "name")
        sqlite3_bind_text(stmtSelect, 1, nuevoValor.c_str(), -1, SQLITE_TRANSIENT);
    else
        sqlite3_bind_int(stmtSelect, 1, anio);

    sqlite3_bind_int(stmtSelect, 2, id);

    bool exito = (sqlite3_step(stmtSelect) == SQLITE_DONE);

    sqlite3_finalize(stmtSelect);
    return exito;
}

std::vector<Album> CancionesDAO::getAlbums(){
    vector<Album> albums;
    std::string peticion = "SELECT id_album, path, name, year, portada FROM albums;";

    sqlite3_stmt* stmtSelect = nullptr;
    if(sqlite3_prepare_v2(baseDeDatos, peticion.c_str(), -1, &stmtSelect, nullptr) != SQLITE_OK)
        return albums;


    while(sqlite3_step(stmtSelect) == SQLITE_ROW){

        int id = sqlite3_column_int(stmtSelect, 0);

        const unsigned char* c_ruta = sqlite3_column_text(stmtSelect, 1);
        std::string ruta = c_ruta ? reinterpret_cast<const char*>(c_ruta) : "";

        const unsigned char* c_nombre = sqlite3_column_text(stmtSelect, 2);
        std::string nombre = c_nombre ? reinterpret_cast<const char*>(c_nombre) : "";

        int anio = sqlite3_column_int(stmtSelect, 3);

        const unsigned char* c_portada = sqlite3_column_text(stmtSelect, 4);
        std::string portada = c_portada ? reinterpret_cast<const char*>(c_portada) : "";

        Album alb(id,ruta,nombre,portada,anio,true);

        albums.push_back(alb);
    }
    sqlite3_finalize(stmtSelect);
    return albums;
}
