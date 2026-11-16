#ifndef PERSON_H
#define PERSON_H
#include <string>

/**
 * @struct Person
 * @brief Estructura que guarda los datos de una persona en la base de datos.
 */
struct Person{

public:

    int id; ///< Identificador único de la persona en la base de datos.
    std::string stage_name; ///< Nombre artistico de la persona.
    std::string real_name; ///< Nombre real de la persona.
    std::string birth_date; ///< Fecha de nacimiento de la persona.
    std::string death_date; ///< Fecha de muerte de la persona.
    bool person_valida=false; ///< Booleano que indica si la estructura tiene información valida de una persona.

    /**
    * @brief Constructor parametrizado de la estructura Performer.
    * @param id Identificador único de la persona en la base de datos.
    * @param stage_name Nombre artistico de la persona.
    * @param real_name Nombre real de la persona.
    * @param birth_date Fecha de nacimiento de la persona.
    * @param death_date Fecha de muerte de la persona.
    * @param person_valida Booleano que indica si la estructura tiene información valida de una persona.
    */
    Person(const int& id,const std::string& stage_name, const std::string& real_name, const std::string& birth_date,
              const std::string& death_date, const bool &person_valida){
        this->id = id;
        this->stage_name = stage_name;
        this->real_name = real_name;
        this->birth_date = birth_date;
        this->death_date = death_date;
        this->person_valida=person_valida;
    }


    /**
     * @brief Constructor por defecto.
     */
    Person(){}
};
#endif // PERSON_H
