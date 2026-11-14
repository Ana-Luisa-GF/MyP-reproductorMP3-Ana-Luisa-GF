#include <QApplication>
#include "CancionesDAO.h"
#include "Minero.h"
#include <string>
#include <iostream>
#include "mainwindow.h"
#include "RepControlador.h"
#include "TraductorBusquedas.h"
#include "ReproductorAudio.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    CancionesDAO cancionesDAO;
    cancionesDAO.crearBaseDeDatos();
    Minero minero(cancionesDAO);

    TraductorBusquedas trad;
    ReproductorAudio repAudio;


    RepControlador controlador(minero,cancionesDAO,trad,repAudio);

    std::string ruta;
    std::cout<<"El progrma hace las cosas hasta aqui"<<endl;

    MainWindow w(&controlador);
    controlador.setVista(&w);
    w.cargarListaCanciones(controlador.getCanciones());
    w.show();

    return a.exec();
}