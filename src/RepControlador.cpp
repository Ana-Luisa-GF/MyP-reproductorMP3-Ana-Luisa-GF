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
