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

        mensajeError = nullptr;
        consultaSql = "ALTER TABLE performers ADD COLUMN portada TEXT;";
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
            

        performer = agregarPerformer(cancion.artista,2,cancion.portada_artista);
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

int CancionesDAO::agregarPerformer(const std::string &nombre, const int &tipo, const std::string &ruta_imagen){

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


    string peticion2 = "INSERT OR IGNORE INTO performers (id_type, name, portada) VALUES (?, ?, ?);";
    sqlite3_stmt* stmtInsert = nullptr;

    if (sqlite3_prepare_v2(baseDeDatos, peticion2.c_str(), -1, &stmtInsert, nullptr) != SQLITE_OK)
        return -1;
    

    sqlite3_bind_int(stmtInsert, 1, 2);
    sqlite3_bind_text(stmtInsert, 2, nombre.c_str(), -1, SQLITE_STATIC);
    sqlite3_bind_text(stmtInsert, 3, ruta_imagen.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmtInsert) != SQLITE_DONE){
        fprintf(stderr, "Error al insertar performer: %s\n", sqlite3_errmsg(baseDeDatos));
        sqlite3_finalize(stmtInsert);
        return -1;
    }

    int id_performer = static_cast<int>(sqlite3_last_insert_rowid(baseDeDatos));
    
    sqlite3_finalize(stmtInsert);

    return id_performer;
}

int CancionesDAO::agregarAlbun(const std::filesystem::path &ruta, const std::string &nombre, const std::string &portada){

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
        c.setIdPerformer(id_performer);

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




Performer CancionesDAO::getPerofrmer(const int &id){
    Performer performer;
    string peticion = "SELECT id_performer, id_type, name, portada FROM performers WHERE id_performer = ?";
    sqlite3_stmt* stmtSelect = nullptr;
    if(sqlite3_prepare_v2(baseDeDatos, peticion.c_str(), -1, &stmtSelect, nullptr) != SQLITE_OK){
        return performer;
    }

    sqlite3_bind_int(stmtSelect, 1, id);

    if(sqlite3_step(stmtSelect) == SQLITE_ROW){
        int id = sqlite3_column_int(stmtSelect, 0);

        int id_type = sqlite3_column_int(stmtSelect, 1);

        const unsigned char* namePtr = sqlite3_column_text(stmtSelect, 2);
        std::string nombre = namePtr ? reinterpret_cast<const char*>(namePtr) : "";

        const unsigned char* portadaPtr = sqlite3_column_text(stmtSelect, 3);
        std::string portada = portadaPtr ? reinterpret_cast<const char*>(portadaPtr) : "";

        sqlite3_finalize(stmtSelect);
        return Performer(id,id_type,nombre,portada,true);
    }
    sqlite3_finalize(stmtSelect);
    return performer;
}


bool CancionesDAO::modificarColumna_performer(const int &id, const std::string &columna,const int &id_type, const std::string &nombre){
    if (columna != "name" && columna != "id_type")
        return false;

    std::string peticion = "UPDATE performers SET " + columna + " = ? WHERE id_performer = ?";

    sqlite3_stmt* stmtSelect = nullptr;
    if(sqlite3_prepare_v2(baseDeDatos, peticion.c_str(), -1, &stmtSelect, nullptr) != SQLITE_OK)
        return false;

    if(columna == "name")
        sqlite3_bind_text(stmtSelect, 1, nombre.c_str(), -1, SQLITE_TRANSIENT);
    else
        sqlite3_bind_int(stmtSelect, 1, id_type);

    sqlite3_bind_int(stmtSelect, 2, id);

    bool exito = (sqlite3_step(stmtSelect) == SQLITE_DONE);

    sqlite3_finalize(stmtSelect);
    return exito;
}

std::vector<Performer> CancionesDAO::getPerformers(){
    vector<Performer> performers;
    std::string peticion = "SELECT id_performer, id_type, name, portada FROM performers;";

    sqlite3_stmt* stmtSelect = nullptr;
    if(sqlite3_prepare_v2(baseDeDatos, peticion.c_str(), -1, &stmtSelect, nullptr) != SQLITE_OK)
        return performers;


    while(sqlite3_step(stmtSelect) == SQLITE_ROW){

        int id = sqlite3_column_int(stmtSelect, 0);
        int id_type = sqlite3_column_int(stmtSelect, 1);

        const unsigned char* c_nombre = sqlite3_column_text(stmtSelect, 2);
        std::string nombre = c_nombre ? reinterpret_cast<const char*>(c_nombre) : "";

        const unsigned char* c_portada = sqlite3_column_text(stmtSelect, 3);
        std::string portada = c_portada ? reinterpret_cast<const char*>(c_portada) : "";

        Performer per(id, id_type, nombre, portada, true);

        performers.push_back(per);
    }
    sqlite3_finalize(stmtSelect);
    return performers;
}

int CancionesDAO::agregarPerson(const std::string &nombre){
    string peticion1 = "SELECT * FROM persons WHERE stage_name = ?";
    sqlite3_stmt* stmtSelect = nullptr;
    if(sqlite3_prepare_v2(baseDeDatos, peticion1.c_str(), -1, &stmtSelect, nullptr) != SQLITE_OK)
        return -1;

    sqlite3_bind_text(stmtSelect, 1, nombre.c_str(), -1, SQLITE_TRANSIENT);
    if(sqlite3_step(stmtSelect) == SQLITE_ROW){
        int id_person = sqlite3_column_int(stmtSelect, 0);
        sqlite3_finalize(stmtSelect);
        return id_person;
    }

    sqlite3_finalize(stmtSelect);


    string peticion2 = "INSERT OR IGNORE INTO persons (stage_name) VALUES (?);";
    sqlite3_stmt* stmtInsert = nullptr;

    if (sqlite3_prepare_v2(baseDeDatos, peticion2.c_str(), -1, &stmtInsert, nullptr) != SQLITE_OK)
        return -1;

    sqlite3_bind_text(stmtInsert, 1, nombre.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmtInsert) != SQLITE_DONE){
        fprintf(stderr, "Error al insertar person: %s\n", sqlite3_errmsg(baseDeDatos));
        sqlite3_finalize(stmtInsert);
        return -1;
    }

    int id_person = static_cast<int>(sqlite3_last_insert_rowid(baseDeDatos));

    sqlite3_finalize(stmtInsert);

    return id_person;
}

bool CancionesDAO::modificarColumna_person(const int &id, const std::string &columna,const std::string &nuevoValor){
    if (columna != "stage_name" && columna != "real_name" && columna != "birth_date" && columna != "death_date")
        return false;

    std::string peticion = "UPDATE persons SET " + columna + " = ? WHERE id_person = ?";

    sqlite3_stmt* stmtSelect = nullptr;
    if(sqlite3_prepare_v2(baseDeDatos, peticion.c_str(), -1, &stmtSelect, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_text(stmtSelect, 1, nuevoValor.c_str(), -1, SQLITE_TRANSIENT);

    sqlite3_bind_int(stmtSelect, 2, id);

    bool exito = (sqlite3_step(stmtSelect) == SQLITE_DONE);

    sqlite3_finalize(stmtSelect);
    return exito;
}

Person CancionesDAO::getPerson(const std::string &nombre){
    Person person;
    string peticion = "SELECT id_person, stage_name, real_name, birth_date, death_date FROM persons WHERE stage_name = ?";
    sqlite3_stmt* stmtSelect = nullptr;
    if(sqlite3_prepare_v2(baseDeDatos, peticion.c_str(), -1, &stmtSelect, nullptr) != SQLITE_OK){
        return person;
    }

    sqlite3_bind_text(stmtSelect, 1, nombre.c_str(), -1, SQLITE_TRANSIENT);

    if(sqlite3_step(stmtSelect) == SQLITE_ROW){

        int id_person = sqlite3_column_int(stmtSelect, 0);

        const unsigned char* stage_namePtr = sqlite3_column_text(stmtSelect, 1);
        std::string stage_name = stage_namePtr ? reinterpret_cast<const char*>(stage_namePtr) : "";

        const unsigned char* real_namePtr = sqlite3_column_text(stmtSelect, 2);
        std::string real_name = real_namePtr ? reinterpret_cast<const char*>(real_namePtr) : "";

        const unsigned char* birth_datePtr = sqlite3_column_text(stmtSelect, 3);
        std::string birth_date = birth_datePtr ? reinterpret_cast<const char*>(birth_datePtr) : "";

        const unsigned char* death_datePtr = sqlite3_column_text(stmtSelect, 4);
        std::string death_date = death_datePtr ? reinterpret_cast<const char*>(death_datePtr) : "";

        sqlite3_finalize(stmtSelect);
        return Person(id_person, stage_name, real_name, birth_date, death_date, true);
    }
    sqlite3_finalize(stmtSelect);
    return person;
}


int CancionesDAO::agregarGrupo(const std::string &nombre){
    string peticion1 = "SELECT * FROM groups WHERE name = ?";
    sqlite3_stmt* stmtSelect = nullptr;
    if(sqlite3_prepare_v2(baseDeDatos, peticion1.c_str(), -1, &stmtSelect, nullptr) != SQLITE_OK)
        return -1;

    sqlite3_bind_text(stmtSelect, 1, nombre.c_str(), -1, SQLITE_TRANSIENT);
    if(sqlite3_step(stmtSelect) == SQLITE_ROW){
        int id_group = sqlite3_column_int(stmtSelect, 0);
        sqlite3_finalize(stmtSelect);
        return id_group;
    }

    sqlite3_finalize(stmtSelect);


    string peticion2 = "INSERT OR IGNORE INTO groups (name) VALUES (?);";
    sqlite3_stmt* stmtInsert = nullptr;

    if (sqlite3_prepare_v2(baseDeDatos, peticion2.c_str(), -1, &stmtInsert, nullptr) != SQLITE_OK)
        return -1;

    sqlite3_bind_text(stmtInsert, 1, nombre.c_str(), -1, SQLITE_STATIC);

    if (sqlite3_step(stmtInsert) != SQLITE_DONE){
        fprintf(stderr, "Error al insertar grupo: %s\n", sqlite3_errmsg(baseDeDatos));
        sqlite3_finalize(stmtInsert);
        return -1;
    }

    int id_group = static_cast<int>(sqlite3_last_insert_rowid(baseDeDatos));

    sqlite3_finalize(stmtInsert);

    return id_group;
}

bool CancionesDAO::modificarColumna_goup(const int &id, const std::string &columna,const std::string &nuevoValor){
    if (columna != "name" && columna != "start_date" && columna != "end_date")
        return false;

    std::string peticion = "UPDATE groups SET " + columna + " = ? WHERE id_group = ?";

    sqlite3_stmt* stmtSelect = nullptr;
    if(sqlite3_prepare_v2(baseDeDatos, peticion.c_str(), -1, &stmtSelect, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_text(stmtSelect, 1, nuevoValor.c_str(), -1, SQLITE_TRANSIENT);

    sqlite3_bind_int(stmtSelect, 2, id);

    bool exito = (sqlite3_step(stmtSelect) == SQLITE_DONE);

    sqlite3_finalize(stmtSelect);
    return exito;
}

Group CancionesDAO::getGroup(const std::string &nombre){
    Group grupo;
    string peticion = "SELECT id_group, name, start_date, end_date FROM groups WHERE name = ?";
    sqlite3_stmt* stmtSelect = nullptr;
    if(sqlite3_prepare_v2(baseDeDatos, peticion.c_str(), -1, &stmtSelect, nullptr) != SQLITE_OK){
        return grupo;
    }

    sqlite3_bind_text(stmtSelect, 1, nombre.c_str(), -1, SQLITE_TRANSIENT);

    if(sqlite3_step(stmtSelect) == SQLITE_ROW){

        int id_group = sqlite3_column_int(stmtSelect, 0);

        const unsigned char* namePtr = sqlite3_column_text(stmtSelect, 1);
        std::string name = namePtr ? reinterpret_cast<const char*>(namePtr) : "";

        const unsigned char* start_datePtr = sqlite3_column_text(stmtSelect, 2);
        std::string start_date = start_datePtr ? reinterpret_cast<const char*>(start_datePtr) : "";

        const unsigned char* end_datePtr = sqlite3_column_text(stmtSelect, 3);
        std::string end_date = end_datePtr ? reinterpret_cast<const char*>(end_datePtr) : "";

        sqlite3_finalize(stmtSelect);

        Group g(id_group,name,start_date,end_date,true);
        g.setIntegrantes(getPersonasGrupo(grupo.id));
        return g;
    }
    sqlite3_finalize(stmtSelect);

    return grupo;
}

bool CancionesDAO::agregarPersonAGrupo(const int &id_person, const int &id_group) {
    std::string peticion = "INSERT OR IGNORE INTO in_group (id_person, id_group) VALUES (?, ?);";
    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(baseDeDatos, peticion.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_int(stmt, 1, id_person);
    sqlite3_bind_int(stmt, 2, id_group);

    bool exito = (sqlite3_step(stmt) == SQLITE_DONE);
    sqlite3_finalize(stmt);

    return exito;
}

std::vector<Person> CancionesDAO::getPersonasGrupo(const int &idGrupo) {
    std::vector<Person> personasDelGrupo;

    std::string peticion = "SELECT p.id_person, p.stage_name, p.real_name, p.birth_date, p.death_date "
                           "FROM persons p "
                           "JOIN in_group ig ON p.id_person = ig.id_person "
                           "JOIN groups g ON ig.id_group = g.id_group "
                           "WHERE g.id_group = ?";

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(baseDeDatos, peticion.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return personasDelGrupo;
    }

    sqlite3_bind_int(stmt, 1, idGrupo);

    while (sqlite3_step(stmt) == SQLITE_ROW) {
        int id_person = sqlite3_column_int(stmt, 0);

        const unsigned char* stage_namePtr = sqlite3_column_text(stmt, 1);
        std::string stage_name = stage_namePtr ? reinterpret_cast<const char*>(stage_namePtr) : "";

        const unsigned char* real_namePtr = sqlite3_column_text(stmt, 2);
        std::string real_name = real_namePtr ? reinterpret_cast<const char*>(real_namePtr) : "";

        const unsigned char* birth_datePtr = sqlite3_column_text(stmt, 3);
        std::string birth_date = birth_datePtr ? reinterpret_cast<const char*>(birth_datePtr) : "";

        const unsigned char* death_datePtr = sqlite3_column_text(stmt, 4);
        std::string death_date = death_datePtr ? reinterpret_cast<const char*>(death_datePtr) : "";

        personasDelGrupo.push_back(Person(id_person, stage_name, real_name, birth_date, death_date, true));
    }

    sqlite3_finalize(stmt);
    return personasDelGrupo;
}


bool CancionesDAO::eliminarPersonDeGrupo(const int &id_person, const int &id_group) {
    std::string peticion = "DELETE FROM in_group WHERE id_person = ? AND id_group = ?;";
    sqlite3_stmt* stmt = nullptr;

    if (sqlite3_prepare_v2(baseDeDatos, peticion.c_str(), -1, &stmt, nullptr) != SQLITE_OK)
        return false;

    sqlite3_bind_int(stmt, 1, id_person);
    sqlite3_bind_int(stmt, 2, id_group);

    bool exito = (sqlite3_step(stmt) == SQLITE_DONE);

    sqlite3_finalize(stmt);
    return exito;
}

std::vector<Person> CancionesDAO::getPersons(){
    vector<Person> artistas;
    std::string peticion = "SELECT id_person, stage_name, real_name, birth_date, death_date FROM persons;";

    sqlite3_stmt* stmtSelect = nullptr;
    if(sqlite3_prepare_v2(baseDeDatos, peticion.c_str(), -1, &stmtSelect, nullptr) != SQLITE_OK)
        return artistas;


    while(sqlite3_step(stmtSelect) == SQLITE_ROW){

        int id_perso = sqlite3_column_int(stmtSelect, 0);

        const unsigned char* c_stage_name = sqlite3_column_text(stmtSelect, 1);
        std::string stage_name = c_stage_name ? reinterpret_cast<const char*>(c_stage_name) : "";

        const unsigned char* c_real_name = sqlite3_column_text(stmtSelect, 2);
        std::string real_name = c_real_name ? reinterpret_cast<const char*>(c_real_name) : "";

        const unsigned char* c_birth_date = sqlite3_column_text(stmtSelect, 3);
        std::string birth_date = c_birth_date ? reinterpret_cast<const char*>(c_birth_date) : "";

        const unsigned char* c_death_date = sqlite3_column_text(stmtSelect, 4);
        std::string death_date = c_death_date ? reinterpret_cast<const char*>(c_death_date) : "";

        Person artist(id_perso,stage_name,real_name,birth_date,death_date, true);

        artistas.push_back(artist);
    }
    sqlite3_finalize(stmtSelect);
    return artistas;
}

std::vector<Cancion> CancionesDAO::buscarCancionesFiltros(const TraductorBusquedas& filtros){
    std::vector<Cancion> resultado;

    std::vector<std::string> valores;
    std::vector<bool> esAnio;
    std::string peticion = "SELECT r.path, r.title, r.track, r.year, r.genre, r.portada, p.name, a.name, r.id_rola, a.id_album, r.id_performer "
                      "FROM rolas r "
                      "JOIN performers p ON r.id_performer = p.id_performer "
                      "JOIN albums a ON r.id_album = a.id_album "
                      "WHERE 1=1";

    if(!filtros.album.empty()){
        if(filtros.bAlbum)
            peticion += " AND a.name LIKE ?";
        else
            peticion += " AND a.name NOT LIKE ?";
        valores.push_back("%" + filtros.album + "%");
        esAnio.push_back(false);
    }
    if(!filtros.performer.empty()){
        if(filtros.bPerformer)
            peticion += " AND p.name LIKE ?";
        else
            peticion += " AND p.name NOT LIKE ?";
        valores.push_back("%" + filtros.performer + "%");
        esAnio.push_back(false);
    }
    if(!filtros.cancion.empty()){
        if(filtros.bCancion)
            peticion += " AND r.title LIKE ?";
        else
            peticion += " AND r.title NOT LIKE ?";
        valores.push_back("%" + filtros.cancion + "%");
        esAnio.push_back(false);
    }
    if(!filtros.genero.empty()){
        if(filtros.bGenero)
            peticion += " AND r.genre LIKE ?";
        else
            peticion += " AND r.genre NOT LIKE ?";
        valores.push_back("%" + filtros.genero + "%");
        esAnio.push_back(false);
    }
    if(!filtros.anio_c.empty()){
        if(filtros.bAnio_c)
            peticion += " AND r.year = ?";
        else
            peticion += " AND r.year != ?";
        valores.push_back(filtros.anio_c);
        esAnio.push_back(true);
    }

    sqlite3_stmt* stmt = nullptr;
    if (sqlite3_prepare_v2(baseDeDatos, peticion.c_str(), -1, &stmt, nullptr) != SQLITE_OK) {
        return resultado;
    }

    for (size_t i = 0; i < valores.size(); ++i) {
        int indiceParametro = i + 1;

        if (esAnio[i])
            sqlite3_bind_int(stmt, indiceParametro, std::stoi(valores[i]));
        else
            sqlite3_bind_text(stmt, indiceParametro, valores[i].c_str(), -1, SQLITE_TRANSIENT);
    }

    while(sqlite3_step(stmt) == SQLITE_ROW){

        const unsigned char* c_ruta = sqlite3_column_text(stmt, 0);
        std::string ruta = c_ruta ? reinterpret_cast<const char*>(c_ruta) : "";

        const unsigned char* c_titulo = sqlite3_column_text(stmt, 1);
        std::string titulo = c_titulo ? reinterpret_cast<const char*>(c_titulo) : "";

        int track = sqlite3_column_int(stmt, 2);
        int anio = sqlite3_column_int(stmt, 3);

        const unsigned char* c_genero = sqlite3_column_text(stmt, 4);
        std::string genero = c_genero ? reinterpret_cast<const char*>(c_genero) : "";

        const unsigned char* c_portada = sqlite3_column_text(stmt, 5);
        std::string portada = c_portada ? reinterpret_cast<const char*>(c_portada) : "";

        const unsigned char* c_artista = sqlite3_column_text(stmt, 6);
        std::string artista = c_artista ? reinterpret_cast<const char*>(c_artista) : "Unknown";

        const unsigned char* c_album = sqlite3_column_text(stmt, 7);
        std::string album = c_album ? reinterpret_cast<const char*>(c_album) : "Unknown";

        int id = sqlite3_column_int(stmt, 8);
        int id_album = sqlite3_column_int(stmt, 9);
        int id_performer = sqlite3_column_int(stmt, 10);

        Cancion c(ruta,titulo,artista,genero,album,track,anio,portada);
        c.setId(id);
        c.setIdAlbum(id_album);
        c.setIdPerformer(id_performer);

        resultado.push_back(c);
    }
    sqlite3_finalize(stmt);
    return resultado;
}

std::vector<Performer> CancionesDAO::buscarPerformersFiltros(const TraductorBusquedas& filtros){
    vector<Performer> performers;
    std::string peticion = "SELECT id_performer, id_type, name, portada FROM performers "
                           "WHERE 1=1";

    if(filtros.personas == 1 && filtros.bPersonas){
        if(filtros.grupos == 1 && filtros.bGrupos)
            peticion += " AND id_type IN (0, 1)";
        else
            peticion += " AND id_type = 0";
    }else{
        if(filtros.grupos == 1 && filtros.bGrupos)
            peticion += " AND id_type = 1";
    }

    if(filtros.personas == 1 && !filtros.bPersonas){
        if(filtros.grupos == 1 && !filtros.bGrupos)
            peticion += " AND id_type NOT IN (0, 1)";
        else
            peticion += " AND id_type != 0";
    }else{
        if(filtros.grupos == 1 && !filtros.bGrupos)
            peticion += " AND id_type != 1";
    }

    std::string valor;

    if(!filtros.performer.empty()){
        if(filtros.bPerformer)
            peticion += " AND name LIKE ?";
        else
            peticion += " AND name NOT LIKE ?";
         valor = "%"+filtros.performer+"%";
    }

    sqlite3_stmt* stmtSelect = nullptr;
    if(sqlite3_prepare_v2(baseDeDatos, peticion.c_str(), -1, &stmtSelect, nullptr) != SQLITE_OK)
        return performers;

    if(!valor.empty())
        sqlite3_bind_text(stmtSelect, 1, valor.c_str(), -1, SQLITE_TRANSIENT);

    while(sqlite3_step(stmtSelect) == SQLITE_ROW){

        int id = sqlite3_column_int(stmtSelect, 0);
        int id_type = sqlite3_column_int(stmtSelect, 1);

        const unsigned char* c_nombre = sqlite3_column_text(stmtSelect, 2);
        std::string nombre = c_nombre ? reinterpret_cast<const char*>(c_nombre) : "";

        const unsigned char* c_portada = sqlite3_column_text(stmtSelect, 3);
        std::string portada = c_portada ? reinterpret_cast<const char*>(c_portada) : "";

        Performer per(id, id_type, nombre, portada, true);

        performers.push_back(per);
    }
    sqlite3_finalize(stmtSelect);
    return performers;
}

std::vector<Album> CancionesDAO::buscarAlbumsFiltros(const TraductorBusquedas& filtros){
    vector<Album> albums;
    std::string peticion = "SELECT id_album, path, name, year, portada FROM albums WHERE 1=1";

    std::string nombre_Album;
    bool hayAnio = false;
    bool hayNombre = false;
    int argumentos =1;

    if(!filtros.album.empty()){
        if(filtros.bAlbum)
            peticion += " AND name LIKE ?";
        else
            peticion += " AND name NOT LIKE ?";
        nombre_Album = "%" + filtros.album + "%";
        hayNombre = true;
    }

    if(!filtros.anio_a.empty()){
        if(filtros.bAnio_a)
            peticion += " AND year = ?";
        else
            peticion += " AND year != ?";
        hayAnio = true;
    }

    sqlite3_stmt* stmtSelect = nullptr;
    if(sqlite3_prepare_v2(baseDeDatos, peticion.c_str(), -1, &stmtSelect, nullptr) != SQLITE_OK)
        return albums;

    if (hayNombre){
        sqlite3_bind_text(stmtSelect, argumentos, nombre_Album.c_str(), -1, SQLITE_TRANSIENT);
        argumentos ++;
    }

    if(hayAnio){
        sqlite3_bind_int(stmtSelect, argumentos, std::stoi(filtros.anio_a));
        argumentos++;
    }

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
