#ifndef REPRODUCTORAUDIO_H
#define REPRODUCTORAUDIO_H
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QUrl>
#include <functional>

class ReproductorAudio
{
public:
    ReproductorAudio();
    ~ReproductorAudio();
    void asignarCancion(const QString &ruta_cancion);
    void repreoducirCancion();
    void pararCancion();

private:
    QMediaPlayer *player;
    QAudioOutput *audioOutput;


};

#endif // REPRODUCTORAUDIO_H
