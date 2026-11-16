#ifndef TRADUCTORBUSQUEDAS_H
#define TRADUCTORBUSQUEDAS_H
#include <string>
#include <vector>

/**
 * @class TraductorBusquedas
 * @brief Clase encargada de analizar, descomponer la cadena de texto de búsqueda en 
 * criterios estructurados. 
 */
class TraductorBusquedas
{
public:
    std::string performer; ///< Valor solicitado del tipo performer.
    bool bPerformer; ///< true si se busca el valor, false si se excluye. 

    std::string cancion; ///< Valor solicitado del tipo canción (titulo)
    bool bCancion; ///< true si se busca el valor, false si se excluye. 

    std::string anio_c; ///< Año de la canción solicitado.    
    bool bAnio_c; ///< true si se busca el valor, false si se excluye. 

    std::string genero; ///< Valor solicitado del tipo género.
    bool bGenero; ///< true si se busca el valor, false si se excluye. 

    std::string album; ///< Valor solicitado del tipo albúm (titulo).
    bool bAlbum; ///< true si se busca el valor, false si se excluye. 

    std::string anio_a; ///< Año del álbum solicitado.
    bool bAnio_a; ///< true si se busca el valor, false si se excluye. 

    int grupos;  ///< Indicador numérico (de estado) para búsqueda por grupos.
    bool bGrupos; ///< true si se busca un grupo, false si se excluyen. 

    int personas; ///< Indicador numérico (de estado) para búsqueda por personas.
    bool bPersonas; ///< true si se busca una persona, false si se excluyen. 

    std::vector<std::string> tokens; ///< Lista de segmentos individuales obtenidos al dividir la cadena de búsqueda por comas.

    /**
     * @brief Constructor por defecto.
     */
    TraductorBusquedas();

    /**
     * @brief Divide la cadena de búsqueda general en tokens individuales utilizando las comas como delimitador.
     * @param busqueda Cadena completa introducida por el usuario en la interfaz.
     */
    void getTokens(const std::string &busqueda);

    /**
     * @brief Analiza un token individual, identifica el comando, extrae su valor, valida tipos de datos (como años) y configura su bandera.
     * @param token Cadena de texto que representa una petición.
     * @return true si el token fue reconocido y validado exitosamente, false en otro caso.
     */
    bool analizarToken(const std::string &token);

    /**
     * @brief Itera sobre la lista de tokens almacenados y procesa cada uno.
     * @return true si todos los tokens fueron procesados correctamente, false si alguno está mal formulado.
     */
    bool determinarTokens();

    /**
     * @brief Limpia todos los campos del traductor a su estado inicial por defecto.
     */
    void limpiar();
};

#endif // TRADUCTORBUSQUEDAS_H
