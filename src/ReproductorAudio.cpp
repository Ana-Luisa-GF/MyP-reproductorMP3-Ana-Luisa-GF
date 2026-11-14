#include "ReproductorAudio.h"

ReproductorAudio::ReproductorAudio() {
    player = new QMediaPlayer;
    audioOutput = new QAudioOutput;
    player->setAudioOutput(audioOutput);
    audioOutput->setVolume(0.5);
}

ReproductorAudio::~ReproductorAudio() {
    delete player;
    delete audioOutput;
}

void ReproductorAudio::asignarCancion(const QString &ruta_cancion){
    if(player)
    player->setSource(QUrl::fromLocalFile(ruta_cancion));
}

void ReproductorAudio::repreoducirCancion(){
    if(player && player->hasAudio())
        player->play();
}

void ReproductorAudio::pararCancion(){
    if(player && player->hasAudio())
        player->pause();
}

