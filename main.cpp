#include "CancionesDAO.h"
#include "Minero.h"
#include <string>
#include <iostream>

int main(int argc, char* argv[]) {
    CancionesDAO cancionesDAO;
    cancionesDAO.crearBaseDeDatos();
    Minero minero(cancionesDAO);


    std::string ruta;
    std::cout<<"El progrma hace las cosas hasta aqui"<<endl;


    if(argc >= 2 ){
        std::cout<<"Hay un directorio"<<endl;
        ruta = argv[1];

        minero.ValidarDirectorio(ruta);
        minero.recorrerArchivos();
    }

  return 0;
}