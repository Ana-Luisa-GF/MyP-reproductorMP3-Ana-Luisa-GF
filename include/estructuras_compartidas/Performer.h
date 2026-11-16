#ifndef PERFORMER_H
#define PERFORMER_H

#include <string>

/**
 * @struct Performer
 * @brief Estructura que guarda los datos de un interprete en la base de datos.
 */
struct Performer{

public:

    int id; ///< Identificador único del interprete en la base de datos.
    int id_type; ///< Identificador del tipo de interprete (persona o grupo).
    std::string nombre; ///< Nombre del interprete.
    std::string ruta_imagen; ///< Ruta de la imagen del interprete de la canción.
    bool performer_valido=false; ///< Booleano que indica si la estructura tiene información valida de un interprete.

    /**
     * @brief Constructor parametrizado de la estructura Performer.
    * @param id Identificador único del interprete en la base de datos.
    * @param id_type Identificador del tipo de interprete (persona o grupo).
    * @param nombre Nombre del interprete.
    * @param ruta_imagen Ruta de la imagen del interprete de la canción.
    * @param performer_valido Booleano que indica si la estructura tiene información valida de un interprete.
    */
    Performer(const int& id, const int& id_type, const std::string& nombre, const std::string& ruta_imagen,
            const bool &performer_valido){
        this->id = id;
        this->id_type = id_type;
        this->nombre = nombre;
        this->ruta_imagen = ruta_imagen;
        this->performer_valido=performer_valido;
    }

    /**
     * @brief Constructor por defecto.
     */
    Performer(){}
};

#endif // PERFORMER_H
