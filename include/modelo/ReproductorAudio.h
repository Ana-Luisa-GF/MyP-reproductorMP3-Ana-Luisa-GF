#ifndef REPRODUCTORAUDIO_H
#define REPRODUCTORAUDIO_H
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QUrl>
#include <functional>

/**
 * @class ReproductorAudio
 * @brief Clase encargada de gestionar la reproducción de los archivos mp3.
 */
class ReproductorAudio
{
public:

    /**
     * @brief Constructor por defecto. Inicializa las instancias internas de QMediaPlayer y QAudioOutput, 
     * vincula la salida de audio al reproductor y establece un volumen inicial predeterminado.
     */
    ReproductorAudio();

    /**
     * @brief Destructor. Libera la memoria de las instancias internas de 
     * QMediaPlayer y QAudioOutput.
     */
    ~ReproductorAudio();

    /**
     * @brief Carga un archivo de audio mp3.
     * @param ruta_cancion Ruta en el sistema de archivos de la pista.
     */
    void asignarCancion(const QString &ruta_cancion);

    /**
     * @brief Inicia o reanuda la reproducción de la pista actual.
     */
    void repreoducirCancion();

    /**
     * @brief Pausa la reproducción de la pista actual.
     */
    void pararCancion();

private:
    QMediaPlayer *player; ///< Puntero al motor de reproducción.
    QAudioOutput *audioOutput; ///< Puntero al controlador de salida y volumen de audio.


};

#endif // REPRODUCTORAUDIO_H
