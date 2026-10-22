#include "Minero.h"
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <taglib/fileref.h>
#include <taglib/tag.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pwd.h>
#include <fstream>
#include <sstream>


using namespace std;

void Minero::crearBaseDeDatos(){
    const char *homedir;

    if ((homedir = getenv("HOME")) == NULL) {
        homedir = getpwuid(getuid())->pw_dir;
    }

    path ruta_bd = homedir;
    ruta_bd /= ".local/share/proyec_reproductorMP3";

    if(!exists(ruta_bd) || !is_directory(ruta_bd))
        if (mkdir(ruta_bd.c_str(), 0755) != 0){
            fprintf(stderr,"Fallo al crear el directorio para la base de datos: %s\n", strerror(errno));
            exit(1);
        } 
        
    path archivoBD = ruta_bd / "rolas.bd";
    
    if(exists(archivoBD))
        return;


    sqlite3* baseDeDatos;
    int codigo_error = sqlite3_open(archivoBD.c_str(), &baseDeDatos);

    if (codigo_error != SQLITE_OK){ 
        fprintf(stderr,"Error al abrir la base de datos:: %s\n",  sqlite3_errmsg(baseDeDatos)); 
        exit(1);
    }

    ifstream archivoSql("rolas.sql");

    if (!archivoSql.is_open()) {
        fprintf(stderr, "No se pudo abrir el archivo rolas.sql para inicializar las tablas.\n");
        sqlite3_close(baseDeDatos);
        exit(1);
    }

    std::stringstream buffer;
    buffer << archivoSql.rdbuf();
    string consultaSql = buffer.str();
    archivoSql.close();

   char* mensajeError = nullptr;
    int mensaje = sqlite3_exec(baseDeDatos, consultaSql.c_str(), 0, 0, &mensajeError);

    if (mensaje != SQLITE_OK) {
        fprintf(stderr, "Error al crear las tablas: %s\n", mensajeError);
        sqlite3_free(mensajeError);
        sqlite3_close(baseDeDatos);
        exit(1);
    }

    sqlite3_close(baseDeDatos);
}

void Minero::ValidarDirectorio(string rEntrada){
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