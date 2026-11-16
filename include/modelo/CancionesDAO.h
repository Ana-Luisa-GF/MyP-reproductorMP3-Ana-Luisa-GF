#ifndef CANCIONES_DAO_H
#define CANCIONES_DAO_H

#include "estructuras_compartidas/Cancion.h"
#include <sqlite3.h>
#include <filesystem>
#include <vector>
#include "estructuras_compartidas/Album.h"
#include "estructuras_compartidas/Performer.h"
#include "estructuras_compartidas/Person.h"
#include "estructuras_compartidas/Group.h"
#include "modelo/TraductorBusquedas.h"

/**
 * @class CancionesDAO
 * @brief Clase encargada de gestionar las consultas en la base de datos SQLite
 * del reproductor.
 */
class CancionesDAO{
 
private:
    bool usarRutaTests;  ///< Indica si se va a ejecutar la clase para pruebas unitarias.
    sqlite3* baseDeDatos;  ///< Puntero a la conexión de SQLite.


    
public:

    /**
     * @brief Constructor por defecto. Configura el DAO para no usarse para pruebas unitarias.
     */
    CancionesDAO() {usarRutaTests = false;}

    /**
     * @brief Constructor con parametro.
     * @param esTest true para indicar que se usara la clase para pruebas, false en otro caso.
     */
    CancionesDAO(bool esTest) { usarRutaTests = esTest;}

    /**
     * @brief Destructor. Cierra la conexión con SQLite.
     */
    ~CancionesDAO();

    /**
     * @brief Obtiene el puntero a la conexión de SQLite.
     * @return puntero a la conexión de SQLite.
     */
    sqlite3* getConexion() { return baseDeDatos; }


    /**
     * @brief Crea la ruta para alojar la base si no existe, inicializa el archivo de la
     * base de datos, y crea las tablas necesarias si no existen.
     */
    void crearBaseDeDatos();

    /**
     * @brief Inserta una lista de canciones en la base de datos.
     * @param listCanciones Vector con los objetos Cancion a insertar.
     */
    void agregarCanciones(const std::vector<Cancion>& listCanciones);

    /**
     * @brief Recupera todas las canciones registradas en la base de datos.
     * @return Vector con todos los objetos Cancion encontrados.
     */
    std::vector<Cancion> getCanciones();

    /**
     * @brief Actualiza el valor de una columna específica de una canción.
     * @param columna Nombre de la columna a modificar.
     * @param id Identificador único de la canción.
     * @param nuevoValor Nuevo dato a asignar.
     * @return true si la actualización fue exitosa, false en otro caso.
     */
    bool modificarColumna(const std::string &columna, const int &id, const std::string &nuevoValor);


    /**
     * @brief Registra un nuevo álbum en la base de datos.
     * @param ruta Ruta del álbum en el sistema de archivos.
     * @param nombre Título del álbum.
     * @param portada Ruta o referencia a la imagen de portada.
     * @return id asignado al nuevo álbum insertado (o el id existente si ya estaba registrado).
     */
    int agregarAlbun(const std::filesystem::path &ruta, const std::string &nombre, const std::string &portada);

    /**
     * @brief Obtiene la información de un álbum.
     * @param id Identificador único del álbum.
     * @return Objeto Album con la información.
     */
    Album getAlbum(const int &id);

    /**
     * @brief Actualiza el valor de una columna específica de un álbum.
     * @param id Identificador único del álbum.
     * @param columna Nombre de la columna a actualizar.
     * @param anio Nuevo año de lanzamiento (si es el valor a modificar).
     * @param nuevoValor Nuevo valor para la columna especificada (si no es el año).
     * @return true si la actualización fue exitosa, false en otro caso.
     */
    bool modificarColumna_album(const int &id, const std::string &columna,const int &anio, const std::string &nuevoValor);

    /**
     * @brief Recupera todos los álbumes almacenados en la base de datos.
     * @return Vector con los álbums encontrados.
     */
    std::vector<Album> getAlbums();


    /**
     * @brief Registra un nuevo intérprete (persona o grupo) en la base de datos.
     * @param nombre Nombre artístico o de la banda.
     * @param tipo Identificador del tipo de intérprete (persona o grupo).
     * @param ruta_imagen Ruta al archivo de imagen del intérprete.
     * @return id generado para el intérprete.
     */
    int agregarPerformer(const std::string &nombre, const int &tipo, const std::string &ruta_imagen);

    /**
     * @brief Obtiene los datos de un intérprete por su id.
     * @param id Identificador único del intérprete.
     * @return Performer encontrado.
     */
    Performer getPerofrmer(const int &id);

    /**
     * @brief Actualiza el valor de una columna específica de un intérprete.
     * @param id Identificador único del intérprete.
     * @param columna Nombre de la columna a editar.
     * @param id_type Identificador del tipo de intérprete (si es el valor a modificar).
     * @param nombre Nuevo valor o nombre a asignar (si no es el tipo de interprete).
     * @return true si la actualización fue exitosa, false en otro caso.
     */
    bool modificarColumna_performer(const int &id, const std::string &columna,const int &id_type, const std::string &nombre);

    /**
     * @brief Recupera todos los interpretes almacenados en la base de datos.
     * @return Vector con los interpretes encontrados.
     */
    std::vector<Performer> getPerformers();

    /**
     * @brief Registra una nueva persona en la base de datos.
     * @param nombre Nombre artístico de la persona.
     * @return id asignado al nuevo registro.
     */
    int agregarPerson(const std::string &nombre);

    /**
     * @brief Actualiza el valor de una columna específica de una persona.
     * @param id Identificador único de la persona.
     * @param columna Nombre de la columna a modificar.
     * @param nuevoValor Nuevo dato a asignar.
     * @return true si la actualización fue exitosa, false en otro caso.
     */
    bool modificarColumna_person(const int &id, const std::string &columna,const std::string &nuevoValor);

    /**
     * @brief Busca y recupera una persona por su nombre.
     * @param nombre Nombre de la persona a buscar.
     * @return Objeto Person con los datos encontrados.
     */
    Person getPerson(const std::string &nombre);

    /**
     * @brief Obtiene todas las personas registradas en la base de datos.
     * @return Vector de objetos Person.
     */
    std::vector<Person> getPersons();


    /**
     * @brief Registra un nuevo grupo.
     * @param nombre Nombre del grupo.
     * @return id asignado al nuevo grupo.
     */
    int agregarGrupo(const std::string &nombre);

    /**
     * @brief Actualiza el valor de una columna específica de un grupo.
     * @param id Identificador único del grupo.
     * @param columna Nombre de la columna a modificar.
     * @param nuevoValor Nuevo dato a asignar.
     * @return true si la actualización fue exitosa, false en otro caso.
     */
    bool modificarColumna_goup(const int &id, const std::string &columna,const std::string &nuevoValor);

    /**
     * @brief Obtiene la lista de integrantes (personas) que pertenecen a un grupo específico.
     * @param idGrupo Identificador único del grupo.
     * @return Vector de objetos Person asociados al grupo.
     */
    std::vector<Person> getPersonasGrupo(const int &idGrupo);

    /**
     * @brief Busca y recupera un grupo por su nombre.
     * @param nombre Nombre del grupo.
     * @return Objeto Group con la información.
     */
    Group getGroup(const std::string &nombre);

    /**
     * @brief Vincula una persona como integrante de un grupo.
     * @param id_person Identificador de la persona.
     * @param id_group Identificador del grupo.
     * @return true si se creó la relación, false si falló.
     */
    bool agregarPersonAGrupo(const int &id_person, const int &id_group);

    /**
     * @brief Elimina una persona de un grupo.
     * @param id_person Identificador de la persona.
     * @param id_group Identificador del grupo.
     * @return true si se eliminó el vínculo correctamente, false en otro caso.
     */
    bool eliminarPersonDeGrupo(const int &id_person, const int &id_group);


    /**
     * @brief Consulta canciones aplicando filtros de búsqueda.
     * @param filtros Objeto TraductorBusquedas con las especificaciones de búsqueda.
     * @return Vector de objetos Cancion que coinciden con los filtros.
     */
    std::vector<Cancion> buscarCancionesFiltros(const TraductorBusquedas& filtros);

    /**
     * @brief Consulta interpretes aplicando filtros de búsqueda.
     * @param filtros Objeto TraductorBusquedas con las especificaciones de búsqueda.
     * @return Vector de objetos Performer que coinciden con los filtros.
     */
    std::vector<Performer> buscarPerformersFiltros(const TraductorBusquedas& filtros);

    /**
     * @brief Consulta álbumes aplicando filtros de búsqueda.
     * @param filtros Objeto TraductorBusquedas con las especificaciones de búsqueda.
     * @return Vector de objetos Album que coinciden con los filtros.
     */
    std::vector<Album> buscarAlbumsFiltros(const TraductorBusquedas& filtros);
};
#endif