#include "RepControlador.h"

RepControlador::RepControlador(Minero& m) : minero(m) {} 

void RepControlador::minar(std::string ruta){
    minero.ValidarDirectorio(ruta);
        minero.recorrerArchivos();
}
