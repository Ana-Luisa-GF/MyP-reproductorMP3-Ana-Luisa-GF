#ifndef CANCIONES_DAO_H
#define CANCIONES_DAO_H

#include "Cancion.h"
#include <sqlite3.h>
#include <filesystem>
#include <vector>
#include "Album.h"

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
    int agregarAlbun(std::filesystem::path ruta, std::string nombre,std::string portada);
    int agregarPerformer(std::string nombre);
    std::vector<Cancion> getCanciones();
    bool modificarColumna(const std::string &columna, const int &id, const std::string &nuevoValor);
    Album getAlbum(const int &id);
    bool modificarColumna_album(const int &id, const std::string &columna,const int &anio, const std::string &nuevoValor);
    std::vector<Album> getAlbums();
};
#endif