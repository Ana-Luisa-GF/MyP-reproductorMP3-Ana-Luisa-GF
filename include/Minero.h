#ifndef MINERO_H
#define MINERO_H

#include "Cancion.h"
#include "CancionesDAO.h"
#include <string>
#include <vector>
#include <filesystem>
#include <taglib/fileref.h>

using namespace std::filesystem;
using namespace std;

/**
 * @brief Clase encargada de recorrer el directorio de entrada y leer los archivos MP3.
 */
class Minero{

private:

    CancionesDAO cancionesDAO;
    std::filesystem::path entrada;
    vector<Cancion> archivosMP3;

public:
    
    Minero(const CancionesDAO& cancionesDAO);
    bool ValidarDirectorio(const string& rEntrada);
    vector<Cancion> getArchivosMP3();
    Cancion leerEtiquetas(const path& archivoMp3);
    void recorrerArchivos();
    path guardarImagen(const TagLib::ByteVector& datosImagen, const path &rutaCancion, const std::string &tipo_imagen);
};
#endif