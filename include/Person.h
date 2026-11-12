#ifndef PERSON_H
#define PERSON_H
#include <string>

class Person{

public:

    int id;
    std::string stage_name;
    std::string real_name;
    std::string birth_date;
    std::string death_date;
    bool person_valida=false;

    Person(const int& id,const std::string& stage_name, const std::string& real_name, const std::string& birth_date,
              const std::string& death_date, const bool &person_valida){
        this->id = id;
        this->stage_name = stage_name;
        this->birth_date = birth_date;
        this->death_date = death_date;
        this->person_valida=person_valida;
    }

    Person(){}
};
#endif // PERSON_H
