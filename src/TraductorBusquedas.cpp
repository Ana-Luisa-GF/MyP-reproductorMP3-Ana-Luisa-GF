#include "TraductorBusquedas.h"
#include <sstream>
#include <QString>
#include <iostream>

TraductorBusquedas::TraductorBusquedas() {}

void TraductorBusquedas::getTokens(const std::string &busqueda){
    std::vector<std::string> tokens;
    std::stringstream ss(busqueda);
    std::string segmento;
    while (std::getline(ss, segmento, ',')) {
        tokens.push_back(segmento);
    }
    this->tokens = tokens;
}

bool TraductorBusquedas::analizarToken(const std::string &token){
    std::stringstream ss(token);
    std::string primer_comando="";
    std::string resto_linea = "";
    ss >> primer_comando;

    std::getline(ss, resto_linea);

    if (!resto_linea.empty() && resto_linea[0] == ' ') {
        resto_linea.erase(0, 1);
    }

    if(primer_comando == "personas"){
        personas = 1;
        bPersonas = true;
        return true;
    }
    if(primer_comando == "-personas"){
        personas = 1;
        bPersonas = false;
        return true;
    }
    if(primer_comando == "grupos"){
        grupos = 1;
        bGrupos = true;
        return true;
    }
    if(primer_comando == "-grupos"){
        grupos = 1;
        bGrupos = false;
        return true;
    }
    if(primer_comando == "performer:"){
        performer = resto_linea;
        bPerformer = true;
        return true;
    }
    if(primer_comando == "-performer:"){
        performer = resto_linea;
        bPerformer = false;
        return true;
    }
    if(primer_comando == "canción:"){
        cancion = resto_linea;
        bCancion = true;
        return true;
    }
    if(primer_comando == "-canción:"){
        cancion = resto_linea;
        bCancion = false;
        return true;
    }
    if(primer_comando == "añoCancion:"){
        anio_c = resto_linea;
        bAnio_c = true;
        QString anio = QString::fromStdString(anio_c);
        bool ok = false;
        anio.toInt(&ok);
        return ok;
    }
    if(primer_comando =="-añoCancion:"){
        anio_c = resto_linea;
        bAnio_c = false;
        QString anio = QString::fromStdString(anio_c);
        bool ok = false;
        anio.toInt(&ok);
        return ok;
    }
    if(primer_comando == "genero:"){
        genero = resto_linea;
        bGenero = true;
        return true;
    }
    if(primer_comando == "-genero:"){
        genero = resto_linea;
        bGenero = false;
        return true;
    }
    if(primer_comando == "album:"){
        album = resto_linea;
        bAlbum = true;
        return true;
    }
    if(primer_comando == "-album:"){
        album = resto_linea;
        bAlbum = false;
        return true;
    }
    if(primer_comando == "añoAlbum:"){
        anio_a = resto_linea;
        bAnio_a = true;
        QString anio = QString::fromStdString(anio_a);
        bool ok = false;
        anio.toInt(&ok);
        return ok;
    }
    if(primer_comando == "-añoAlbum:"){
        anio_a = resto_linea;
        bAnio_a = false;
        QString anio = QString::fromStdString(anio_a);
        bool ok = false;
        anio.toInt(&ok);
        return ok;
    }
    return false;
}

bool TraductorBusquedas::determinarTokens(){
    for(std::string token : tokens){
        if(!analizarToken(token))
            return false;
    }
    return true;
}

void TraductorBusquedas::limpiar(){
    performer.clear();
    bPerformer=false;

    cancion.clear();
    bCancion=false;

    anio_c.clear();
    bAnio_c=false;

    genero.clear();
    bGenero=false;

    album.clear();
    bAlbum=false;

    anio_a.clear();
    bAnio_a=false;

    grupos=0;
    bGrupos=false;

    personas=0;
    bPersonas=false;
    tokens.clear();
}