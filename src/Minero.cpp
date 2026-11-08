#include "Minero.h"
#include <cstring>
#include <iostream>
#include <taglib/fileref.h>
#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/attachedpictureframe.h>
#include <fstream>
#include <filesystem>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <pwd.h>

using namespace std::filesystem;


Minero:: Minero(const CancionesDAO& cancionesDAO){
    this->cancionesDAO = cancionesDAO;
}

bool Minero::ValidarDirectorio(const string& rEntrada){
    path directorio = rEntrada;

    if(!exists(directorio)){
        fprintf(stderr,"Error la ruta proportcionada no es un directorio: %s\n", strerror(errno));
        return false;
    }
    if(!is_directory(directorio)){
        fprintf(stderr,"Error la ruta proportcionada no es un directorio: %s\n", strerror(errno));
        return false;
    }
    entrada = directorio;
    return true;
}

vector<Cancion> Minero::getArchivosMP3(){
    vector<Cancion> archivosMp3;

    try{
        for(const auto& elem  : filesystem::recursive_directory_iterator(entrada)){
            if(filesystem::is_regular_file(elem.status()) && elem.path().extension()==".mp3")
                archivosMp3.push_back(leerEtiquetas(elem.path()));
        }
    }catch (const filesystem::filesystem_error& e) {
        fprintf(stderr, "No se pudieron leer correctamente todos los archivos: %s\n", e.what());

    }

    return archivosMp3;
}



Cancion Minero::leerEtiquetas(const path& archivoMp3){
    TagLib::FileRef f(archivoMp3.string().c_str());

    if(f.isNull() || !f.tag()){
        cerr << "No se pudo leer el archivo o no contiene etiquetas" << endl;
        return Cancion(archivoMp3, "", "", "", "", 0, 0, "");
    }

    TagLib::Tag *tag = f.tag();
    path rutaPortada = "";

    TagLib::MPEG::File archivoMpeg(archivoMp3.string().c_str());
    if (archivoMpeg.isValid()) {
        TagLib::ID3v2::Tag *tagId3v2 = archivoMpeg.ID3v2Tag(false);
        if (tagId3v2) {
            TagLib::ID3v2::FrameList listaMarcos = tagId3v2->frameList("APIC");

            for (auto* marco : listaMarcos) {
                auto* apicFrame = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame*>(marco);
                if (!apicFrame) continue;

                if (apicFrame->type() == TagLib::ID3v2::AttachedPictureFrame::FrontCover) {
                    TagLib::ByteVector datosImagen = apicFrame->picture();
                    rutaPortada = guardarImagen(datosImagen, archivoMp3);
                    break;
                }
            }
        }
    }

    Cancion c(archivoMp3, tag->title().toCString(true), tag->artist().toCString(true), tag->genre().toCString(true),
              tag->album().toCString(true), tag->track(), tag->year(), rutaPortada);
    return c;
}

path Minero::guardarImagen(const TagLib::ByteVector& datosImagen, path rutaCancion) {
    path rutaCompletaImagen;
    if (datosImagen.isEmpty()) return rutaCompletaImagen;

    const char *homedir = getenv("HOME");
    if (!homedir)
        homedir = getpwuid(getuid())->pw_dir;

    path rutaDirectorioImg = homedir;
    rutaDirectorioImg /= ".local/share/proyec_reproductorMP3/portadas";

    if (!exists(rutaDirectorioImg)) {
        if (mkdir(rutaDirectorioImg.c_str(), 0755) != 0) {
            fprintf(stderr, "Fallo al crear el directorio de portadas: %s\n", strerror(errno));
            return rutaCompletaImagen;
        }
    }

    std::string nombreArchivo = rutaCancion.stem().string() + ".jpg";
    rutaCompletaImagen = rutaDirectorioImg / nombreArchivo;

    std::ofstream archivoSalida(rutaCompletaImagen, std::ios::out | std::ios::binary);
    if (archivoSalida.is_open()) {
        archivoSalida.write(datosImagen.data(), datosImagen.size());
        archivoSalida.close();
    }
    return rutaCompletaImagen;
}


void Minero::recorrerArchivos(){
    archivosMP3 = getArchivosMP3();
    std::cout<<"La lista de archivos tiene " << archivosMP3.size() << " archivos"<<endl;
    cancionesDAO.agregarCanciones(archivosMP3);
}
