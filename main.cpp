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

    RepControlador controlador(minero); 

    std::string ruta;
    std::cout<<"El progrma hace las cosas hasta aqui"<<endl;

    MainWindow w(&controlador);
    w.show();

    return a.exec();

    /*
    if(argc >= 2 ){
        std::cout<<"Hay un directorio"<<endl;
        ruta = argv[1];

        minero.ValidarDirectorio(ruta);
        minero.recorrerArchivos();
    }

  return 0;   */
}

/*
#include <QApplication>
#include "mainwindow.h"
#include "CancionesDAO.h"
#include "Minero.h"
#include "RepControlador.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    // 1. Inicializamos la base de datos / modelo
    CancionesDAO dao(false); 
    Minero minero;

    // 2. Creamos el Controlador pasándole el modelo o dependencias necesarias
    RepControlador controlador(minero); 

    // 3. Creamos la Vista (MainWindow) inyectándole el controlador
    MainWindow w(&controlador);
    w.show();

    return a.exec();
    */