#ifndef MINERO_H
#define MINERO_H

#include "Cancion.h"
#include "CancionesDAO.h"
#include <string>
#include <vector>
#include <filesystem>



using namespace std::filesystem;
using namespace std;

/**
 * @brief Clase encargada de recorrer el directorio de entrada y leer los archivos MP3.
 */
class Minero{

private:

    CancionesDAO cancionesDAO;
    path entrada; 
    vector<Cancion> archivosMP3;

public:
    
    Minero(const CancionesDAO& cancionesDAO);
    void ValidarDirectorio(const string& rEntrada);
    vector<Cancion> getArchivosMP3();
    Cancion leerEtiquetas(const path& archivoMp3);
    void recorrerArchivos();
};
#endif