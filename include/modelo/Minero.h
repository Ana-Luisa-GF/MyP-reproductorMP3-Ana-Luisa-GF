#ifndef MINERO_H
#define MINERO_H

#include "estructuras_compartidas/Cancion.h"
#include "modelo/CancionesDAO.h"
#include <string>
#include <vector>
#include <filesystem>
#include <taglib/fileref.h>

using namespace std::filesystem;
using namespace std;

/**
 * @class Minero
 * @brief Clase encargada de leer directorios, filtrar archivos MP3 y extraer sus datos.
 * 
 * Utiliza la librería TagLib para leer la información interna de los archivos mp3
 * y obtiene las rutas de las imágenes de portada asociadas.
 */
class Minero{

private:

CancionesDAO cancionesDAO; ///< Instancia del DAO para interactuar con la base de datos.
std::filesystem::path entrada; ///< Ruta del directorio que se va a mianr.
vector<Cancion> archivosMP3; ///< Lista interna con la información de los archivos MP3 encontrados.

public:

    /**
     * @brief Constructor de la clase Minero.
     * @param cancionesDAO Referencia al DAO para comunicarse con la base de datos.
     */
    Minero(const CancionesDAO& cancionesDAO);

    /**
     * @brief Valida que la ruta de entreda dada sea valida.
     * @return true si la ruta es valida, false en otro caso.
     */
    bool ValidarDirectorio(const string& rEntrada);

    /**
     * @brief Aplica Minero::leerEtiquetas a cada archivo mp3 del directorio, crea el objeto 
     Cancion de cada archivo, y la inserta en la lista de Canciones.
     * @return Vector con los objetos Cancion de cada archivo mp3.
     */
    vector<Cancion> getArchivosMP3();

    /**
     * @brief Lee las etiquetas ID3 y extrae las carátulasl del archivo MP3.
     * @param archivoMp3 Ruta del archivo mp3 a procesar.
     * @return Objeto Cancion con todos los datos y la ruta de su portada y su interprete.
     */
    Cancion leerEtiquetas(const path& archivoMp3);

    /**
     * @brief llama a Minero::getArchivosMP3 para obtener el vector con los objetos Cancion
     * y ocupa CancionesDAO::agregarCanciones para insertarlas en la base de datos.
     */
    void recorrerArchivos();

    /**
     * @brief Guarda los bytes de la imagen de la carátula o interprete la ruta de la base de datos. 
     * @param datosImagen Vector de bytes de la imagen obtenidos con TagLib.
     * @param rutaCancion Ruta del archivo de audio.
     * @param tipo_imagen Tipo de la imagen (de portada o interprete).
     * @return Ruta completa donde se guardó la imagen.
     */
    path guardarImagen(const TagLib::ByteVector& datosImagen, const path &rutaCancion, const std::string &tipo_imagen);
};
#endif