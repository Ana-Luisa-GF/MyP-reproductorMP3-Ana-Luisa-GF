#include <gtest/gtest.h>
#include "CancionesDAO.h"
#include "Minero.h"
#include "Cancion.h"

namespace fileSys = std::filesystem;

TEST(Minero_Test, ValidacionDeDirectorio) {
    CancionesDAO cancionesDAO(true);
    Minero minero(cancionesDAO);

    EXPECT_DEATH(minero.ValidarDirectorio("/ruta/que//no/existe_145"), ".*");
    EXPECT_DEATH(minero.ValidarDirectorio("rolas.sql"), ".*");
    minero.ValidarDirectorio("Musica_tests");
}


TEST(Minero_Test, Leer_Etiquetas){
    
    CancionesDAO cancionesDAO(true);
    Minero minero(cancionesDAO);

    fileSys::path ruta = "Musica_tests/demo.mp3";
    Cancion c1(ruta,"T","A","Jazz","L",3,1950);
    Cancion c2 = minero.leerEtiquetas(ruta);

    EXPECT_EQ(c1.ruta.string(), c2.ruta.string());
    EXPECT_EQ(c1.titulo, c2.titulo);
    EXPECT_EQ(c1.artista, c2.artista);
    EXPECT_EQ(c1.genero, c2.genero);
    EXPECT_EQ(c1.album, c2.album);
    EXPECT_EQ(c1.track, c2.track);
    EXPECT_EQ(c1.anio, c2.anio);

}

TEST(Minero_Test, Capturar_archivos){
    
    CancionesDAO cancionesDAO(true);
    Minero minero(cancionesDAO);

    fileSys::path ruta = "Musica_tests";
    minero.ValidarDirectorio(ruta);
    vector<Cancion> canciones = minero.getArchivosMP3();
    EXPECT_EQ(canciones.size(), 9);
}
