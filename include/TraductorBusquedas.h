#ifndef TRADUCTORBUSQUEDAS_H
#define TRADUCTORBUSQUEDAS_H
#include <string>
#include <vector>

class TraductorBusquedas
{
public:
    std::string performer;
    bool bPerformer;

    std::string cancion;
    bool bCancion;

    std::string anio_c;
    bool bAnio_c;

    std::string genero;
    bool bGenero;

    std::string album;
    bool bAlbum;

    std::string anio_a;
    bool bAnio_a;

    int grupos;
    bool bGrupos;

    int personas;
    bool bPersonas;

    std::vector<std::string> tokens;

    TraductorBusquedas();
    void getTokens(const std::string &busqueda);
    bool analizarToken(const std::string &token);
    bool determinarTokens();
    void limpiar();
};

#endif // TRADUCTORBUSQUEDAS_H
