#ifndef CANCION_H
#define CANCION_H

#include <string>
#include <filesystem>

/**
 * @struct Cancion
 * @brief Estructura que guarda los datos de una canción en la base de datos.
 */
struct Cancion{

public:

    int id = -1;  ///< Identificador único de la canción en la base de datos. 
    std::filesystem::path ruta; ///< Rruta del archivo mp3 de la canción.
    std::string titulo; ///< Nombre de la canción.
    std::string artista; ///< Artista de la canción.
    std::string genero; ///< Género de la canción.
    std::string album; ///< Álbum de la canción.
    int track; ///< Track de la canción.
    int anio; ///< Año de creación de la canción.
    std::filesystem::path portada; ///Ruta de la imagen de portada de la canión.
    int id_album; ///< Identificador único del álbum de la canción.
    int id_performer; ///< Identificador único del interprete de la canción.
    std::filesystem::path portada_artista; ///< Ruta de la imagen del interprete de la canción.


    /**
     * @brief Constructor parametrizado de la estructura Cancion.
     * @param ruta Rruta del archivo mp3 de la canción.
     * @param titulo Titulo de la canción.
     * @param artista Artista de la canción.
     * @param genero Género de la canción.
     * @param album Álbum de la canción.
     * @param track Track de la canción.
     * @param anio Año de creación de la canción.
     * @param portada Ruta de la portada del álbum.
     */
    Cancion(const std::filesystem::path& ruta, const std::string& titulo, const std::string& artista, 
            const std::string& genero, const std::string& album, const int& track, const int& anio,
            const std::filesystem::path& portada){

        this->ruta = ruta;
        this->track = track;
        this->artista = (artista.empty())? "Unknown" : artista;
        this->titulo = (titulo.empty())? ruta.stem().string() : titulo;
        this->album = (album.empty())? ruta.parent_path().string() : album;
        this->genero = (genero.empty())? "Unknown" : genero;
        this->anio = (anio == 0)? 2026 : anio;
        this->portada = portada;

    }

    /**
     * @brief Constructor por defecto.
     */
    Cancion(){};

    /**
     * @brief Asigna el id de la canción.
     * @param n id de la canción.
     */
    void setId(int n){id = n;}

    /**
     * @brief Asigna el id del álbum.
     * @param n id del álbum.
     */
    void setIdAlbum(int n){id_album = n;}

    /**
     * @brief Asigna el id del interprete.
     * @param n id del interprete.
     */
    void setIdPerformer(int n){id_performer = n;}

    /**
     * @brief Asigna ruta de la imágen de la portada del artista.
     * @param portada_artista ruta de la imágen de la portada del artista.
     */
    void setPortadaArtist(const std::filesystem::path &portada_artista){this->portada_artista = portada_artista;}
};
#endif

