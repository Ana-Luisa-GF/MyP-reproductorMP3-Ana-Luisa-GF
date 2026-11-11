#include "RepControlador.h"
#include "Cancion.h"
#include "mainwindow.h"

RepControlador::RepControlador(Minero& m, CancionesDAO& daoRef) :
    minero(m),
    dao(daoRef),
    vista(nullptr){}


void RepControlador::setVista(MainWindow* w) {
    vista = w;
}

void RepControlador::minar(std::string ruta){
    minero.ValidarDirectorio(ruta);
        minero.recorrerArchivos();
}

std::vector<Cancion> RepControlador::getCanciones(){
    return dao.getCanciones();
}

bool RepControlador::modificarGenero(const int &id, const std::string &nuevoGenero){
    return dao.modificarColumna("genre",id,nuevoGenero);
}

bool RepControlador::modificarAnio(const int &id, const std::string &nuevoAnio){
    return dao.modificarColumna("year",id,nuevoAnio);
}

bool RepControlador::modificarTitulo(const int &id, const std::string &nuevoTitulo){
    return dao.modificarColumna("title",id,nuevoTitulo);
}

bool RepControlador::modificarTrack(const int &id, const std::string &nuevoTitulo){
    return dao.modificarColumna("track",id,nuevoTitulo);
}

Album RepControlador::getAlbum(const int &id){
    return dao.getAlbum(id);
}

bool RepControlador::mofificarAlbum(const int &id, const std::string &columna,const int &anio, const std::string &nuevoValor){
    return  dao.modificarColumna_album(id, columna, anio, nuevoValor);
}
