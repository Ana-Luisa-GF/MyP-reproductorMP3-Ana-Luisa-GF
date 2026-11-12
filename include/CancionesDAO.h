#ifndef CANCIONES_DAO_H
#define CANCIONES_DAO_H

#include "Cancion.h"
#include <sqlite3.h>
#include <filesystem>
#include <vector>
#include "Album.h"
#include "Performer.h"
#include "Person.h"
#include "Group.h"

class CancionesDAO{
 
private:
    bool usarRutaTests; 
    sqlite3* baseDeDatos;


    
public:
    CancionesDAO() {usarRutaTests = false;}
    CancionesDAO(bool esTest) { usarRutaTests = esTest;}
    ~CancionesDAO();
    sqlite3* getConexion() { return baseDeDatos; }

    void crearBaseDeDatos();
    void agregarCanciones(const std::vector<Cancion>& listCanciones);
    int agregarAlbun(const std::filesystem::path &ruta, const std::string &nombre, const std::string &portada);
    int agregarPerformer(const std::string &nombre, const int &tipo, const std::string &ruta_imagen);

    std::vector<Cancion> getCanciones();
    bool modificarColumna(const std::string &columna, const int &id, const std::string &nuevoValor);

    Album getAlbum(const int &id);
    bool modificarColumna_album(const int &id, const std::string &columna,const int &anio, const std::string &nuevoValor);
    std::vector<Album> getAlbums();

    Performer getPerofrmer(const int &id);
    bool modificarColumna_performer(const int &id, const std::string &columna,const int &id_type, const std::string &nombre);
    std::vector<Performer> getPerformers();

    int agregarPerson(const std::string &nombre);
    bool modificarColumna_person(const int &id, const std::string &columna,const std::string &nuevoValor);
    Person getPerson(const std::string &nombre);

    int agregarGrupo(const std::string &nombre);
    bool modificarColumna_goup(const int &id, const std::string &columna,const std::string &nuevoValor);
    Group getGroup(const std::string &nombre);
};
#endif