#include "RepControlador.h"
#include "Cancion.h"
#include "mainwindow.h"

RepControlador::RepControlador(Minero& m, CancionesDAO& daoRef) :
    minero(m),
    dao(daoRef),
    vista(nullptr){}


void RepControlador::setVista(MainWindow* w) {
    vista = w;
}

void RepControlador::minar(std::string ruta){
    minero.ValidarDirectorio(ruta);
        minero.recorrerArchivos();
}

std::vector<Cancion> RepControlador::getCanciones(){
    return dao.getCanciones();
}

bool RepControlador::modificarGenero(const int &id, const std::string &nuevoGenero){
    return dao.modificarColumna("genre",id,nuevoGenero);
}

bool RepControlador::modificarAnio(const int &id, const std::string &nuevoAnio){
    return dao.modificarColumna("year",id,nuevoAnio);
}

bool RepControlador::modificarTitulo(const int &id, const std::string &nuevoTitulo){
    return dao.modificarColumna("title",id,nuevoTitulo);
}

bool RepControlador::modificarTrack(const int &id, const std::string &nuevoTitulo){
    return dao.modificarColumna("track",id,nuevoTitulo);
}

Album RepControlador::getAlbum(const int &id){
    return dao.getAlbum(id);
}

bool RepControlador::mofificarAlbum(const int &id, const std::string &columna,const int &anio, const std::string &nuevoValor){
    return  dao.modificarColumna_album(id, columna, anio, nuevoValor);
}

std::vector<Album> RepControlador::getAlbums(){
    return dao.getAlbums();
}

Performer RepControlador::getPerformer(const int &id){
    return dao.getPerofrmer(id);
}

std::vector<Person> RepControlador::getPersonasGrupo(const int &idGrupo){
    return dao.getPersonasGrupo(idGrupo);
}

std::vector<Performer> RepControlador::getPerformers(){
    return dao.getPerformers();
}

bool RepControlador::modificarColumna_performer(const int &id, const std::string &columna,const int &id_type, const std::string &nombre){
    return dao.modificarColumna_performer(id,columna,id_type,nombre);
}

int RepControlador::agregarPerson(const std::string &nombre){
    return dao.agregarPerson(nombre);
}
bool RepControlador::modificarColumna_person(const int &id, const std::string &columna,const std::string &nuevoValor){
    return dao.modificarColumna_person(id,columna,nuevoValor);
}
Person RepControlador::getPerson(const std::string &nombre){
    return dao.getPerson(nombre);
}

std::vector<Person> RepControlador::getPersons(){
    return dao.getPersons();
}

int RepControlador::agregarGrupo(const std::string &nombre){
    return dao.agregarGrupo(nombre);
}

bool RepControlador::modificarColumna_goup(const int &id, const std::string &columna,const std::string &nuevoValor){
    return dao.modificarColumna_goup(id,columna,nuevoValor);
}

Group RepControlador::getGroup(const std::string &nombre){
    return dao.getGroup(nombre);
}

bool RepControlador::definirTipoPerformer(const int &id, const int &nuevoTipo){
    return dao.modificarColumna_performer(id,"id_type",nuevoTipo,"");
}

bool RepControlador::agregarPersonAGrupo(const int &id_person, const int &id_group){
    return dao.agregarPersonAGrupo(id_person,id_group);
}
bool RepControlador::eliminarPersonDeGrupo(const int &id_person, const int &id_group){
    return dao.eliminarPersonDeGrupo(id_person,id_group);
}