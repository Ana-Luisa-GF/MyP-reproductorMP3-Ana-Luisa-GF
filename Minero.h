#ifndef MINERO_H
#define MINERO_H

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

    path entrada; 
    vector<path> archivosMP3;

public:
    
    void ValidarDirectorio(string rEntrada);
    vector<path> getArchivosMP3();
    void leerEtiquetas(path archivoMp3);
    void recorrerArchivos();
};
#endif