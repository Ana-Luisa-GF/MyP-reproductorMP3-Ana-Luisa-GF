#ifndef PERFORMER_H
#define PERFORMER_H

#include <string>

class Performer{

public:

    int id;
    int id_type;
    std::string nombre;
    std::string ruta_imagen;
    bool performer_valido=false;

    Performer(const int& id, const int& id_type, const std::string& nombre, const std::string& ruta_imagen,
            const bool &performer_valido){
        this->id = id;
        this->id_type = id_type;
        this->nombre = nombre;
        this->ruta_imagen = ruta_imagen;
        this->performer_valido=performer_valido;
    }

    Performer(){}
};

#endif // PERFORMER_H
