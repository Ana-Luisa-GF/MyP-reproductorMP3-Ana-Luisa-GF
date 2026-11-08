#ifndef REP_CONTROLADOR_H
#define REP_CONTROLADOR_H
#include "Minero.h"
#include "CancionesDAO.h"

class MainWindow;
class RepControlador{
    Minero& minero;
    CancionesDAO& dao;
    MainWindow* vista;

public:
    void setVista(MainWindow* w);
    RepControlador(Minero& m, CancionesDAO& daoRef);
    void minar(std::string ruta);
    std::vector<Cancion> getCanciones();

};
#endif