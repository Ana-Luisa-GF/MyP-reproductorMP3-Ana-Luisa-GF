#ifndef ALBUM_H
#define ALBUM_H
#include <string>
#include <filesystem>

class Album{

public:

    int id;
    std::filesystem::path ruta;
    std::string nombre;
    std::string portada;
    int anio;
    bool album_valido;

    Album(const int& id,std::filesystem::path ruta, const std::string& titulo, const std::string& portada,
          const int& anio, const bool& album_valido){
        this->id = id;
        this->nombre = titulo;
        this->ruta = ruta;
        this->anio = (anio == 0)? 2026 : anio;
        this->portada = portada;
        this->album_valido=album_valido;
    }

    Album(){}
};

#endif // ALBUM_H
