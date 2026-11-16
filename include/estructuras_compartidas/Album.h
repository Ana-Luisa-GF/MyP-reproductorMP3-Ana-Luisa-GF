#ifndef ALBUM_H
#define ALBUM_H
#include <string>
#include <filesystem>

/**
 * @struct Album
 * @brief Estructura que guarda los datos de un album en la base de datos.
 */
struct Album{

public:

    int id; ///< Identificador único del álbum en la base de datos. 
    std::filesystem::path ruta; ///<Rruta del directorio que representa al álbum.
    std::string nombre; ///< Nombre del álbum.
    std::string portada; ///< Ruta de la imagen de la portada del album.
    int anio; ///< Año de creación del album. 
    bool album_valido; ///< Booleano que indica si la estructura tiene información valida de un álbum.

    /**
     * @brief Constructor parametrizado de la estructura Album.
     * @param id id del álbum.
     * @param ruta Ruta de directorio del álbum.
     * @param titulo Titulo del álbum.
     * @param portada Ruta de la portada del álbum.
     * @param anio Año del album,
     * @param album_valido Booleano de validación del álbum.
     */
    Album(const int& id,std::filesystem::path ruta, const std::string& titulo, const std::string& portada,
          const int& anio, const bool& album_valido){
        this->id = id;
        this->nombre = titulo;
        this->ruta = ruta;
        this->anio = (anio == 0)? 2026 : anio;
        this->portada = portada;
        this->album_valido=album_valido;
    }

    /**
     * @brief Constructor por defecto.
     */
    Album(){}
};

#endif // ALBUM_H
