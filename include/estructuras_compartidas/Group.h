#ifndef GROUP_H
#define GROUP_H
#include "Person.h"
#include <vector>
#include <string>

/**
 * @struct Grupo
 * @brief Estructura que guarda los datos de un grupo en la base de datos.
 */
struct Group{

public:

    int id; ///< Identificador único del grupo en la base de datos.
    std::string name; ///< Nombre del grupo.
    std::string start_date; ///< Fecha de creación del grupo.
    std::string end_date; ///< Fecha de disolución del grupo.
    std::vector<Person> integrantes; ///< Vector con los integrantes del grupo.
    bool grupo_valido=false; ///< Booleano que indica si la estructura tiene información valida de un grupo.

    /**
     * @brief Constructor parametrizado de la estructura Grupo.
    * @param id Identificador único del grupo en la base de datos.
    * @param name Nombre del grupo.
    * @param start_date Fecha de creación del grupo.
    * @param end_date Fecha de disolución del grupo.
    * @param grupo_validoBooleano que indica si la estructura tiene información valida de un grupo.
    */
    Group(const int& id,const std::string& name, const std::string& start_date, const std::string& end_date,
          const bool &grupo_valido){
        this->id = id;
        this->name = name;
        this->start_date = start_date;
        this->end_date = end_date;
        this->grupo_valido=grupo_valido;
    }

    /**
     * @brief Constructor por defecto.
     */
    Group(){}

    /**
     * @brief Asigna el vector con los integrantes del grupo.
     * @param integrantes Vector con los integrantes del grupo.
     */
    void setIntegrantes(std::vector<Person> integrantes){ this->integrantes = integrantes;}

    /**
     * @brief Devuelve el vector con los integrantes del grupo.
     * @return Vector con los integrantes del grupo.
     */
    std::vector<Person> getIntegrantes(){return integrantes;}
};
#endif // GROUP_H
