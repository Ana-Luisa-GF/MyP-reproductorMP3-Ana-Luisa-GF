#include <QApplication>
#include "CancionesDAO.h"
#include "Minero.h"
#include <string>
#include <iostream>
#include "mainwindow.h"
#include "RepControlador.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);

    CancionesDAO cancionesDAO;
    cancionesDAO.crearBaseDeDatos();
    Minero minero(cancionesDAO);

    RepControlador controlador(minero,cancionesDAO);

    std::string ruta;
    std::cout<<"El progrma hace las cosas hasta aqui"<<endl;

    MainWindow w(&controlador);
    w.cargarListaCanciones(controlador.getCanciones());
    w.show();

    return a.exec();
}