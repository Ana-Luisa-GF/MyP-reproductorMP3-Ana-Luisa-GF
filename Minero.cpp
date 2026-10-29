#include "Minero.h"
#include <cstring>
#include <iostream>
#include <taglib/fileref.h>
//#include <cstdlib>
//#include <taglib/tag.h>
//#include <unistd.h>
//#include <sys/stat.h>
//#include <sys/types.h>
//#include <pwd.h>
//#include <fstream>
//#include <sstream>
//#include <ctime>



Minero:: Minero(const CancionesDAO& cancionesDAO){
    this->cancionesDAO = cancionesDAO;
}

void Minero::ValidarDirectorio(const string& rEntrada){
    path directorio = rEntrada;

    if(!exists(directorio)){
        fprintf(stderr,"Error la ruta proportcionada no es un directorio: %s\n", strerror(errno));
        exit(1);
    }
    if(!is_directory(directorio)){
        fprintf(stderr,"Error la ruta proportcionada no es un directorio: %s\n", strerror(errno));
        exit(1);
    }
    entrada = directorio;
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

    if(!f.isNull()&&f.tag()){ 
        TagLib::Tag *tag=f.tag();
        
        Cancion c (archivoMp3,  tag->title().toCString(true), tag->artist().toCString(true), 
                tag->genre().toCString(true), tag->album().toCString(true), tag->track(), tag->year());

        cout<< "Artista: "<< tag->artist().toCString(true) <<endl;
        cout<< "Titulo: "<< tag->title().toCString(true) <<endl;
        cout<< "Álbum: "<< tag->album().toCString(true) <<endl;
        cout<< "Género: "<< tag->genre().toCString(true) <<endl;
        cout<< "Año: "<< tag->year()<<endl;
        cout<< "Track:"<< tag->track()<<endl;

        return c;

    }else{
        cerr<<"No se pudo leer el archivo o no contiene etiquetas"<<endl;
    }

    Cancion c (archivoMp3, "", "", "", "", 0, 0);
    return c;
}

void Minero::recorrerArchivos(){
    archivosMP3 = getArchivosMP3();
    std::cout<<"La lista de archivos tiene " << archivosMP3.size() << " archivos"<<endl;
    cancionesDAO.agregarCanciones(archivosMP3);
}
