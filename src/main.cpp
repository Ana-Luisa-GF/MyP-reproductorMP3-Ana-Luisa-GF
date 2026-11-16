#include <QApplication>
#include "modelo/CancionesDAO.h"
#include "modelo/Minero.h"
#include <string>
#include <iostream>
#include "vista/mainwindow.h"
#include "controlador/RepControlador.h"
#include "modelo/TraductorBusquedas.h"
#include "modelo/ReproductorAudio.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    CancionesDAO cancionesDAO;
    cancionesDAO.crearBaseDeDatos();
    Minero minero(cancionesDAO);

    TraductorBusquedas trad;
    ReproductorAudio repAudio;

    RepControlador controlador(minero,cancionesDAO,trad,repAudio);

    MainWindow w(&controlador);
    controlador.setVista(&w);
    w.cargarListaCanciones(controlador.getCanciones());
    w.show();

    return a.exec();
}