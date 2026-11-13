#ifndef REP_CONTROLADOR_H
#define REP_CONTROLADOR_H
#include "Minero.h"
#include "CancionesDAO.h"
#include "Album.h"
#include "Performer.h"
#include "Group.h"
#include "TraductorBusquedas.h"

class MainWindow;
class RepControlador{

    TraductorBusquedas &trad;
    Minero& minero;
    CancionesDAO& dao;
    MainWindow* vista;


public:
    void setVista(MainWindow* w);
    RepControlador(Minero& m, CancionesDAO& daoRef, TraductorBusquedas &tr);

    void minar(std::string ruta);
    std::vector<Cancion> getCanciones();
    bool modificarGenero(const int &id, const std::string &nuevoGenero);
    bool modificarAnio(const int &id, const std::string &nuevoAnio);
    bool modificarTitulo(const int &id, const std::string &nuevoTitulo);
    bool modificarTrack(const int &id, const std::string &nuevoTitulo);

    bool mofificarAlbum(const int &id, const std::string &columna,const int &anio, const std::string &nuevoValor);
    std::vector<Album> getAlbums();
    Album getAlbum(const int &id);

    int agregarPerformer(const std::string &nombre, const int &tipo);
    Performer getPerformer(const int &id);
    std::vector<Performer> getPerformers();
    bool modificarColumna_performer(const int &id, const std::string &columna,const int &id_type, const std::string &nombre);
    bool definirTipoPerformer(const int &id, const int &nuevoTipo);

    int agregarPerson(const std::string &nombre);
    bool modificarColumna_person(const int &id, const std::string &columna,const std::string &nuevoValor);
    Person getPerson(const std::string &nombre);
    std::vector<Person> getPersons();

    int agregarGrupo(const std::string &nombre);
    bool modificarColumna_goup(const int &id, const std::string &columna,const std::string &nuevoValor);
    Group getGroup(const std::string &nombre);

    bool agregarPersonAGrupo(const int &id_person, const int &id_group);
    bool eliminarPersonDeGrupo(const int &id_person, const int &id_group);
    std::vector<Person> getPersonasGrupo(const int &idGrupo);

    void busqueda(const std::string &busqueda);
};
#endif

