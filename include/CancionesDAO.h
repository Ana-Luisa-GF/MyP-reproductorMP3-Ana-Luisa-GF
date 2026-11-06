#ifndef CANCIONES_DAO_H
#define CANCIONES_DAO_H

#include "Cancion.h"
#include <sqlite3.h>
#include <filesystem>
#include <vector>

class CancionesDAO{
 
private:
    bool usarRutaTests; 
    sqlite3* baseDeDatos;

    
public:
    CancionesDAO() {usarRutaTests = false;}
    CancionesDAO(bool esTest) { usarRutaTests = esTest;}
    ~CancionesDAO();

    void crearBaseDeDatos();
    void agregarCanciones(const std::vector<Cancion>& listCanciones);
    int agregarAlbun(std::filesystem::path ruta, std::string nombre);
    int agregarPerformer(std::string nombre);

};
#endif