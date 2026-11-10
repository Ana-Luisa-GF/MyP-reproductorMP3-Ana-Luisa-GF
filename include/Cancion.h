#ifndef CANCION_H
#define CANCION_H

#include <string>
#include <filesystem>

struct Cancion{

public:

    int id = -1;
    std::filesystem::path ruta;
    std::string titulo;
    std::string artista;
    std::string genero;
    std::string album;
    int track;
    int anio;
    std::filesystem::path portada;

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

    Cancion(){};

    void setId(int n){id = n;}

};
#endif

