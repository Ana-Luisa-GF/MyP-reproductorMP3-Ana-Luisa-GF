#ifndef CANCIONES_DAO_H
#define CANCIONES_DAO_H

#include "Cancion.h"
#include <sqlite3.h>
#include <filesystem>
#include <vector>

class CancionesDAO{
 
private:
    sqlite3* baseDeDatos;
    
public:
    ~CancionesDAO();
    void crearBaseDeDatos();
    void agregarCanciones(const std::vector<Cancion>& listCanciones);
    int agregarAlbun(std::filesystem::path ruta, std::string nombre);
    int agregarPerformer(std::string nombre);

};
#endif