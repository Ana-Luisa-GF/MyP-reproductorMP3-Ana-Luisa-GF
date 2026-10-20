#include "Minero.h"
#include <cstring>
#include <iostream>
#include <taglib/fileref.h>
#include <taglib/tag.h>

using namespace std;

void Minero::ValidarDirectorio(string rEntrada){
    path directorio = rEntrada;

    if(!exists(directorio)){
        fprintf(stderr,"Error la ruta proportcionada no es un directorio: %s\n", strerror(errno));
        return;
    }
    if(!is_directory(directorio)){
        fprintf(stderr,"Error la ruta proportcionada no es un directorio: %s\n", strerror(errno));
        return;
    }

    entrada = directorio;
}

vector<path> Minero::getArchivosMP3(){
    vector<path> archivosMp3;

    try{
        for(const auto& elem  : filesystem::recursive_directory_iterator(entrada)){
            if(filesystem::is_regular_file(elem.status()) && elem.path().extension()==".mp3")
                archivosMp3.push_back(elem.path());
        }
    }catch (const filesystem::filesystem_error& e) {
        fprintf(stderr, "No se pudieron leer correctamente todos los archivos: %s\n", e.what());
    }

    return archivosMp3;
}

void Minero::leerEtiquetas(path archivoMp3){
    TagLib::FileRef f(archivoMp3.string().c_str());

    if(!f.isNull()&&f.tag()){
        TagLib::Tag *tag=f.tag();

        cout<< "Artista: "<< tag->artist().toCString(true) <<endl;
        cout<< "Titulo: "<< tag->title().toCString(true) <<endl;
        cout<< "Álbum: "<< tag->album().toCString(true) <<endl;
        cout<< "Género: "<< tag->genre().toCString(true) <<endl;
        cout<< "Año: "<< tag->year()<<endl;
    }else{
        cerr<<"No se pudo leer el archivo o contiene etiquetas"<<endl;
    }
}

void Minero::recorrerArchivos(){
    archivosMP3 = getArchivosMP3();
    std::cout<<"La lista de archivos tiene " << archivosMP3.size() << " archivos"<<endl;
    for(path arch : archivosMP3)
        leerEtiquetas(arch);
}

int main(int argc, char* argv[]){
    Minero minero;
    string ruta;
    std::cout<<"El progrma hace las cosas hasta aqui"<<endl;

    if(argc >= 2 ){
        std::cout<<"Hay un directorio"<<endl;
        ruta = argv[1];

        minero.ValidarDirectorio(ruta);
        minero.recorrerArchivos();
    }

  return 0;
}