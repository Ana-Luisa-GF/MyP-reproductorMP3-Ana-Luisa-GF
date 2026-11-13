#ifndef GROUP_H
#define GROUP_H
#include "Person.h"
#include <vector>
#include <string>

class Group{

public:

    int id;
    std::string name;
    std::string start_date;
    std::string end_date;
    std::vector<Person> integrantes;
    bool grupo_valido=false;

    Group(const int& id,const std::string& name, const std::string& start_date, const std::string& end_date,
          const bool &grupo_valido){
        this->id = id;
        this->name = name;
        this->start_date = start_date;
        this->end_date = end_date;
        this->grupo_valido=grupo_valido;
    }

    Group(){}

    void setIntegrantes(std::vector<Person> integrantes){ this->integrantes = integrantes;}
    std::vector<Person> getIntegrantes(){return integrantes;}
};
#endif // GROUP_H
