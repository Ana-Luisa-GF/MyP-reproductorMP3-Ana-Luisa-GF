#ifndef REP_CONTROLADOR_H
#define REP_CONTROLADOR_H
#include "Minero.h"

class RepControlador{
    Minero& minero;
public:
    RepControlador(Minero& m);
    void minar(std::string ruta);
};
#endif