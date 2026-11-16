#ifndef REP_CONTROLADOR_H
#define REP_CONTROLADOR_H
#include "modelo/Minero.h"
#include "modelo/CancionesDAO.h"
#include "estructuras_compartidas/Album.h"
#include "estructuras_compartidas/Performer.h"
#include "estructuras_compartidas/Group.h"
#include "modelo/TraductorBusquedas.h"
#include "modelo/ReproductorAudio.h"

class MainWindow;

/**
 * @class RepControlador
 * @brief Clase controlador que comunica a la vista y el modelo.
 */
class RepControlador{

    ReproductorAudio &repAudio; ///< Referencia a la clase que reproduce el audio.
    TraductorBusquedas &trad; ///< Referencia al traductor de consultas de búsqueda.
    Minero& minero; ///< Referencia al minero de archivos.
    CancionesDAO& dao; ///< Referencia al DAO de la base de datos.
    MainWindow* vista; ///< Puntero a la interfaz gráfica.


public:

    /**
     * @brief Asigna la instancia de la interfaz gráfica.
     * @param w Puntero a la ventana principal (MainWindow).
     */
    void setVista(MainWindow* w);

    /**
     * @brief Constructor del controlador.
     * @param m Referencia al Minero de archivos.
     * @param daoRef Referencia al DAO de la base de datos.
     * @param tr Referencia al Traductor de búsquedas.
     * @param repA Referencia al Reproductor de audio.
     */
    RepControlador(Minero& m, CancionesDAO& daoRef, TraductorBusquedas &tr, ReproductorAudio &repA);

    /**
     * @brief Inicia el proceso de minado de archivos en una ruta dada.
     * @note Llama internamente a Minero::ValidarDirectorio y Minero::recorrerArchivos.
     * @param ruta Directorio del sistema de archivos a escanear.
     */

    void minar(std::string ruta);

    /**
     * @brief Recupera todas las canciones de la base de datos.
     * @note Llama a CancionesDAO::getCanciones.
     * @return Vector con los objetos Cancion.
     */
    std::vector<Cancion> getCanciones();

    /**
     * @brief Actualiza el género de una canción.
     * @note Llama a CancionesDAO::modificarColumna.
     */
    bool modificarGenero(const int &id, const std::string &nuevoGenero);

    /**
     * @brief Actualiza el Año de una canción.
     * @note Llama a CancionesDAO::modificarColumna.
     */
    bool modificarAnio(const int &id, const std::string &nuevoAnio);

    /**
     * @brief Actualiza el titulo de una canción.
     * @note Llama a CancionesDAO::modificarColumna.
     */
    bool modificarTitulo(const int &id, const std::string &nuevoTitulo);

    /**
     * @brief Actualiza el Track de una canción.
     * @note Llama a CancionesDAO::modificarColumna.
     */
    bool modificarTrack(const int &id, const std::string &nuevoTrack);

    /**
     * @brief Modifica atributos específicos de un álbum.
     * @note Llama a CancionesDAO::modificarColumna_album.
     */
    bool mofificarAlbum(const int &id, const std::string &columna,const int &anio, const std::string &nuevoValor);
    
    /**
     * @brief Obtiene la lista completa de álbumes.
     * @note Llama a CancionesDAO::getAlbums.
     */
    std::vector<Album> getAlbums();
    
    /**
     * @brief Obtiene la información de un álbum por su ID.
     * @note Llama a CancionesDAO::getAlbum.
     */
    Album getAlbum(const int &id);

    /**
     * @brief Agrega un nuevo intérprete.
     * @note Llama a CancionesDAO::agregarPerformer.
     */
    int agregarPerformer(const std::string &nombre, const int &tipo);

    /**
     * @brief Obtiene un intérprete por su id.
     * @note Llama a CancionesDAO::getPerofrmer.
     */
    Performer getPerformer(const int &id);

    /**
     * @brief Obtiene todos los intérpretes registrados.
     * @note Llama a CancionesDAO::getPerformers.
     */
    std::vector<Performer> getPerformers();

    /**
     * @brief Modifica una columna de un intérprete.
     * @note Llama a CancionesDAO::modificarColumna_performer.
     */
    bool modificarColumna_performer(const int &id, const std::string &columna,const int &id_type, const std::string &nombre);
    
    /**
     * @brief Define o cambia el tipo de un intérprete.
     * @note Llama a CancionesDAO::modificarColumna_performer.
     */
    bool definirTipoPerformer(const int &id, const int &nuevoTipo);


    /**
     * @brief Agrega una nueva persona.
     * @note Llama a CancionesDAO::agregarPerson.
     */
    int agregarPerson(const std::string &nombre);

    /**
     * @brief Modifica una columna de una persona.
     * @note Llama a CancionesDAO::modificarColumna_person.
     */
    bool modificarColumna_person(const int &id, const std::string &columna,const std::string &nuevoValor);
    
    /**
     * @brief Busca una persona por su nombre.
     * @note Llama a CancionesDAO::getPerson.
     */
    Person getPerson(const std::string &nombre);

    /**
     * @brief Obtiene el listado de todas las personas.
     * @note Llama a CancionesDAO::getPersons.
     */
    std::vector<Person> getPersons();

    /**
     * @brief Agrega un nuevo grupo.
     * @note Llama a CancionesDAO::agregarGrupo.
     */
    int agregarGrupo(const std::string &nombre);
    
    /**
     * @brief Modifica una columna de un grupo.
     * @note Llama a CancionesDAO::modificarColumna_goup.
     */
    bool modificarColumna_goup(const int &id, const std::string &columna,const std::string &nuevoValor);
    
    /**
     * @brief Busca un grupo por su nombre.
     * @note Llama a CancionesDAO::getGroup.
     */
    Group getGroup(const std::string &nombre);

    /**
     * @brief Asocia una persona a un grupo.
     * @note Llama a CancionesDAO::agregarPersonAGrupo.
     */
    bool agregarPersonAGrupo(const int &id_person, const int &id_group);

    /**
     * @brief Elimina a una persona de un grupo.
     * @note Llama a CancionesDAO::eliminarPersonDeGrupo.
     */
    bool eliminarPersonDeGrupo(const int &id_person, const int &id_group);

    /**
     * @brief Obtiene las personas que integran un grupo.
     * @note Llama a CancionesDAO::getPersonasGrupo.
     */
    std::vector<Person> getPersonasGrupo(const int &idGrupo);

    /**
     * @brief Procesa una cadena de búsqueda aplicando filtros y manda a llamar a la vista para mostrar el resultado.
     * @param busqueda Texto introducido por el usuario para parsear mediante el TraductorBusquedas y consultar al DAO.
     */
    void busqueda(const std::string &busqueda);

    /**
     * @brief Carga una pista en el reproductor de audio.
     * @note Llama a ReproductorAudio::asignarCancion.
     */
    void pasarCancion(QString ruta_cancion);

    /**
     * @brief Reproduce la pista actual.
     * @note Llama a ReproductorAudio::repreoducirCancion.
     */
    void play();

    /**
     * @brief Pausa la reproducción actual.
     * @note Llama a ReproductorAudio::pararCancion.
     */
    void pause();
};
#endif

